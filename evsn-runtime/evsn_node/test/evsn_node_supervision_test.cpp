#include "node_supervisor.hpp"
#include "evsn_node.hpp"
#include "custom_types.hpp"

#include <chrono>
#include <csignal>
#include <atomic>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

class EVSNNodeTestAccess {
public:
    static void replace_supervisor(EVSNNode& node, std::unique_ptr<NodeSupervisor> supervisor) {
        node.supervisor_ = std::move(supervisor);
        node.supervision_next_heartbeat_ = std::chrono::steady_clock::now();
    }
};

namespace {
class CountingSupervisor final : public NodeSupervisor {
public:
    explicit CountingSupervisor(std::vector<std::string>& events) : events_{events} {
        config_.entity_name = "test_supervisor";
        config_.heartbeat_period = std::chrono::milliseconds{0};
    }

    const SupervisionConfig& config() const override {
        return config_;
    }

    void heartbeat() override {
        events_.push_back("heartbeat");
    }

    void shutdown() override {
        events_.push_back("shutdown");
    }

private:
    SupervisionConfig config_{};
    std::vector<std::string>& events_;
};

class TimerOnlyNode final : public EVSNNode {
public:
    explicit TimerOnlyNode(std::vector<std::string>& events)
        : EVSNNode{"supervision_test_node", 41}, events_{events} {
        EVSNNodeTestAccess::replace_supervisor(
            *this,
            std::make_unique<CountingSupervisor>(events_));
        create_timer(std::chrono::milliseconds{1}, [this]() {
            events_.push_back("timer");
            ++ticks_;
            if (ticks_ == 2) {
                std::raise(SIGTERM);
            }
        });
    }

    ~TimerOnlyNode() override = default;

private:
    std::vector<std::string>& events_;
    int ticks_{0};
};

class ApiProbeNode final : public EVSNNode {
public:
    ApiProbeNode() : EVSNNode{"api_probe_node", 42} {
    }

    ~ApiProbeNode() override = default;

    using EVSNNode::create_timer;

    void create_invalid_publisher() {
        (void)create_publisher<EVSNParameterState>("bad/topic");
    }
};

class SubscriberNode final : public EVSNNode {
public:
    explicit SubscriberNode(std::atomic<bool>& received)
        : EVSNNode{"subscriber_test_node", 43}, received_{received} {
        create_subscriber<EVSNParameterState>(
            "/test/runtime_dispatch",
            [this](EVSNParameterState sample) {
                if (sample.node_name() == "pubsub_test_node" &&
                    sample.parameter_name() == "dispatch_probe") {
                    received_ = true;
                    std::raise(SIGTERM);
                }
            });
    }

    ~SubscriberNode() override = default;

private:
    std::atomic<bool>& received_;
};
}

TEST(EVSNNodeSupervisionTest, NoopSupervisorUsesSanitizedEntityNameAndDefaults) {
    const auto entity_name = make_supervision_entity_name("bad/node name");
    EXPECT_EQ(entity_name, "evsn_node_bad_node_name");

    const auto supervisor = create_node_supervisor("bad/node name");
    EXPECT_EQ(supervisor->config().entity_name, entity_name);
    EXPECT_EQ(supervisor->config().heartbeat_period, std::chrono::milliseconds{1000});
    EXPECT_EQ(supervisor->config().missed_heartbeat_low, 3);
    EXPECT_EQ(supervisor->config().missed_heartbeat_high, 5);
    supervisor->heartbeat();
    supervisor->shutdown();
}

TEST(EVSNNodeSupervisionTest, HeartbeatFollowsTimerProgress) {
    auto events = std::vector<std::string>{};
    {
        auto node = TimerOnlyNode{events};
        node.spin();
    }

    const auto expected = std::vector<std::string>{
        "timer",
        "heartbeat",
        "timer",
        "heartbeat",
        "shutdown",
    };
    EXPECT_EQ(events, expected);
}

TEST(EVSNNodeSupervisionTest, RejectsInvalidConstructionTimeApiInputs) {
    auto node = ApiProbeNode{};

    EXPECT_THROW(
        node.create_timer(std::chrono::milliseconds{0}, []() {}),
        std::invalid_argument);
    EXPECT_THROW(node.create_timer(std::chrono::milliseconds{1}, {}), std::invalid_argument);
    EXPECT_THROW(node.create_invalid_publisher(), std::invalid_argument);
}

TEST(EVSNNodeSupervisionTest, WaitSetDispatchesSubscriberCallbacks) {
    auto received = std::atomic<bool>{false};
    auto node = SubscriberNode{received};
    auto spin_thread = std::thread{[&node]() {
        node.spin();
    }};

    auto participant = dds::domain::DomainParticipant{43};
    auto publisher = dds::pub::Publisher{participant};
    auto topic = dds::topic::Topic<EVSNParameterState>{participant, "/test/runtime_dispatch"};
    auto writer = dds::pub::DataWriter<EVSNParameterState>{publisher, topic};
    auto sample = EVSNParameterState{};
    sample.node_name("pubsub_test_node");
    sample.parameter_name("dispatch_probe");
    sample.mutable_at_runtime(false);

    for (auto attempt = 0; attempt < 100 && !received.load(); ++attempt) {
        writer.write(sample);
        std::this_thread::sleep_for(std::chrono::milliseconds{20});
    }

    if (!received.load()) {
        std::raise(SIGTERM);
    }

    spin_thread.join();
    EXPECT_TRUE(received.load());
}
