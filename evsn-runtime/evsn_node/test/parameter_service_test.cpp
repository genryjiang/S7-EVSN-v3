#include "parameter_service.hpp"

#include <atomic>
#include <chrono>
#include <fstream>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>

#include <unistd.h>

#include <dds/dds.hpp>
#include <gtest/gtest.h>
#include <rti/request/Requester.hpp>

namespace {
constexpr auto kNodeName = "parameter_test_node";
constexpr auto kServiceName = "EVSNParameterService";
constexpr auto kStateTopic = "/evsn/parameters/state";
constexpr auto kEventTopic = "/evsn/parameters/event";

std::string test_file_path(const std::string& suffix) {
    return "/tmp/evsn_parameter_service_" + std::to_string(getpid()) + "_" + suffix + ".toml";
}

void write_file(const std::string& path, const std::string& body) {
    auto file = std::ofstream{path};
    file << body;
}

std::string valid_parameter_file() {
    const auto path = test_file_path("valid");
    write_file(
        path,
        R"(schema_version = 1
node = "parameter_test_node"
fallback = "use declared safe defaults"

[[parameters]]
name = "enabled"
type = "bool"
default = true
mutable = true
unit = "flag"
description = "Controls the test flag"
fallback = "safe default is enabled"

[[parameters]]
name = "sample_period_ms"
type = "int64"
default = 20
mutable = true
min = 1
max = 100
unit = "ms"
description = "Polling period"
fallback = "safe default period"

[[parameters]]
name = "node_label"
type = "string"
default = "test"
mutable = false
unit = "text"
description = "Human-readable label"
fallback = "safe default label"
)");
    return path;
}

EVSNParameterValue int_value(std::int64_t value) {
    auto parameter_value = EVSNParameterValue{};
    parameter_value.value_type(EVSNParameterType::EVSN_PARAM_INT64);
    parameter_value.int_value(value);
    return parameter_value;
}

EVSNParameterRequest make_request(
    EVSNParameterOperation operation,
    const std::string& name = {},
    const EVSNParameterValue& value = {}) {
    auto request = EVSNParameterRequest{};
    request.node_name(kNodeName);
    request.parameter_name(name);
    request.requester("parameter_service_test");
    request.source("ctest");
    request.operation(operation);
    request.value(value);
    return request;
}

EVSNParameterReply send_request(
    rti::request::Requester<EVSNParameterRequest, EVSNParameterReply>& requester,
    const EVSNParameterRequest& request) {
    const auto request_id = requester.send_request(request);
    const auto has_reply = requester.wait_for_replies(
        1,
        dds::core::Duration::from_secs(5),
        request_id);
    if (!has_reply) {
        throw std::runtime_error("timed out waiting for parameter reply");
    }

    auto replies = requester.take_replies(request_id);
    for (const auto& reply : replies) {
        if (reply.info().valid()) {
            return reply.data();
        }
    }
    throw std::runtime_error("parameter reply contained no valid sample");
}

dds::sub::qos::DataReaderQos reliable_reader_qos(bool transient_local) {
    auto qos = dds::core::QosProvider::Default().datareader_qos();
    qos << dds::core::policy::Reliability::Reliable();
    if (transient_local) {
        qos << dds::core::policy::Durability::TransientLocal()
            << dds::core::policy::History::KeepLast(1);
    } else {
        qos << dds::core::policy::History::KeepAll();
    }
    return qos;
}

void construct_parameter_service(
    dds::domain::DomainParticipant& participant,
    const std::string& parameter_file) {
    auto service = EVSNParameterService{participant, kNodeName, parameter_file};
    (void)service;
}

template<class T>
std::optional<T> take_matching(
    dds::sub::DataReader<T>& reader,
    const std::string& parameter_name,
    std::chrono::milliseconds timeout) {
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    while (std::chrono::steady_clock::now() < deadline) {
        auto samples = reader.take();
        for (const auto& sample : samples) {
            if (sample.info().valid() && sample.data().node_name() == kNodeName &&
                sample.data().parameter_name() == parameter_name) {
                return sample.data();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds{20});
    }
    return std::nullopt;
}

std::optional<EVSNParameterEvent> take_rejection_event(
    dds::sub::DataReader<EVSNParameterEvent>& reader,
    const std::string& parameter_name,
    std::chrono::milliseconds timeout) {
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    while (std::chrono::steady_clock::now() < deadline) {
        auto samples = reader.take();
        for (const auto& sample : samples) {
            if (sample.info().valid() && sample.data().node_name() == kNodeName &&
                sample.data().parameter_name() == parameter_name && !sample.data().accepted()) {
                return sample.data();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds{20});
    }
    return std::nullopt;
}

class ServicePoller {
public:
    explicit ServicePoller(EVSNParameterService& service)
        : poller_{[this, &service]() {
              while (running_) {
                  service.poll();
                  std::this_thread::sleep_for(std::chrono::milliseconds{5});
              }
          }} {
    }

    ~ServicePoller() {
        running_ = false;
        if (poller_.joinable()) {
            poller_.join();
        }
    }

    ServicePoller(const ServicePoller&) = delete;
    ServicePoller& operator=(const ServicePoller&) = delete;

private:
    std::atomic<bool> running_{true};
    std::thread poller_;
};
}

TEST(EVSNParameterServiceTest, LoadsValidTomlDefaults) {
    auto participant = dds::domain::DomainParticipant{91};
    auto service = EVSNParameterService{participant, kNodeName, valid_parameter_file()};

    EXPECT_EQ(service.parameter_count(), 3U);
    EXPECT_EQ(service.parameter_value_as_string("sample_period_ms"), "20");
    EXPECT_EQ(service.parameter_value_as_string("enabled"), "true");
}

TEST(EVSNParameterServiceTest, RejectsInvalidTomlDefault) {
    const auto path = test_file_path("invalid");
    write_file(
        path,
        R"(schema_version = 1
node = "parameter_test_node"

[[parameters]]
name = "bad_period"
type = "int64"
default = 200
mutable = true
min = 1
max = 100
unit = "ms"
description = "Invalid default"
fallback = "fail closed"
)");
    auto participant = dds::domain::DomainParticipant{92};
    EXPECT_THROW(construct_parameter_service(participant, path), std::invalid_argument);
}

TEST(EVSNParameterServiceTest, HandlesRequestReplySuccessRejectionAndDescribe) {
    auto service_participant = dds::domain::DomainParticipant{93};
    auto client_participant = dds::domain::DomainParticipant{93};
    auto service = EVSNParameterService{service_participant, kNodeName, valid_parameter_file()};
    auto poller = ServicePoller{service};
    auto subscriber = dds::sub::Subscriber{client_participant};
    auto event_topic = dds::topic::Topic<EVSNParameterEvent>{client_participant, kEventTopic};
    auto event_reader = dds::sub::DataReader<EVSNParameterEvent>{
        subscriber,
        event_topic,
        reliable_reader_qos(false)};
    auto requester = rti::request::Requester<EVSNParameterRequest, EVSNParameterReply>{
        service_participant,
        kServiceName};
    std::this_thread::sleep_for(std::chrono::milliseconds{300});

    const auto set_reply = send_request(
        requester,
        make_request(
            EVSNParameterOperation::EVSN_PARAM_SET,
            "sample_period_ms",
            int_value(25)));
    ASSERT_TRUE(set_reply.accepted());
    ASSERT_EQ(set_reply.states().size(), 1U);
    EXPECT_EQ(set_reply.states()[0].value().int_value(), 25);

    auto event = std::optional<EVSNParameterEvent>{};
    for (auto attempt = 0; attempt < 5 && !event.has_value(); ++attempt) {
        const auto rejected_reply = send_request(
            requester,
            make_request(
                EVSNParameterOperation::EVSN_PARAM_SET,
                "sample_period_ms",
                int_value(1000)));
        EXPECT_FALSE(rejected_reply.accepted());
        EXPECT_NE(rejected_reply.reason().find("maximum"), std::string::npos);
        event = take_rejection_event(
            event_reader,
            "sample_period_ms",
            std::chrono::seconds{1});
    }
    ASSERT_TRUE(event.has_value());
    EXPECT_FALSE(event->accepted());

    const auto describe_reply = send_request(
        requester,
        make_request(EVSNParameterOperation::EVSN_PARAM_DESCRIBE, "sample_period_ms"));
    ASSERT_TRUE(describe_reply.accepted());
    ASSERT_EQ(describe_reply.descriptors().size(), 1U);
    EXPECT_EQ(describe_reply.descriptors()[0].unit(), "ms");
}

TEST(EVSNParameterServiceTest, LateJoinerGetsCurrentState) {
    auto service_participant = dds::domain::DomainParticipant{94};
    auto service = EVSNParameterService{service_participant, kNodeName, valid_parameter_file()};
    auto client_participant = dds::domain::DomainParticipant{94};
    auto subscriber = dds::sub::Subscriber{client_participant};
    auto topic = dds::topic::Topic<EVSNParameterState>{client_participant, kStateTopic};
    auto reader = dds::sub::DataReader<EVSNParameterState>{
        subscriber,
        topic,
        reliable_reader_qos(true)};

    const auto state = take_matching(reader, "sample_period_ms", std::chrono::seconds{5});
    ASSERT_TRUE(state.has_value());
    EXPECT_EQ(state->value().int_value(), 20);
}
