#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <dds/dds.hpp>

class NodeSupervisor;
class EVSNParameterService;
class EVSNNodeTestAccess;

class EVSNNode {
public:
    // main loop
    void spin();
protected:
    template<typename T>
    class PublisherHandle {
    public:
        PublisherHandle() = default;

        template<typename U, typename std::enable_if_t<std::is_same_v<std::remove_cv_t<std::remove_reference_t<U>>, T>, int> = 0>
        void publish(U&& data) const {
            if (node_ == nullptr) {
                throw std::logic_error("Publisher handle is not initialised");
            }
            node_->publish<T>(data_writer_id_, std::forward<U>(data));
        }

        template<typename U, typename std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_reference_t<U>>, T>, int> = 0>
        void publish(U&&) const = delete;

    private:
        explicit PublisherHandle(EVSNNode* node, uint32_t data_writer_id) :
            node_{node}, data_writer_id_{data_writer_id} {
        }

        EVSNNode* node_{nullptr};
        uint32_t data_writer_id_{};
        friend class EVSNNode;
    };

    // =================================================================================================
    // CONSTRUCTOR
    // =================================================================================================
    explicit EVSNNode(
        const std::string& name,
        int32_t domain_id = 0,
        const std::string& parameter_file = {});
    virtual ~EVSNNode() = 0;

    EVSNNode(const EVSNNode&) = delete;
    EVSNNode& operator=(const EVSNNode&) = delete;
    EVSNNode(EVSNNode&&) = delete;           
    EVSNNode& operator=(EVSNNode&&) = delete;

    // =================================================================================================
    // PROTECTED MEMBER FUNCTIONS
    // =================================================================================================
    const std::string& get_name() const;
    std::size_t parameter_count() const;
    std::string parameter_value_as_string(const std::string& parameter_name) const;

    template<typename T>
    void create_subscriber(const std::string& topic_name, std::function<void(T)> callback);

    template<typename T>
    void create_subscriber(
        const std::string& topic_name,
        std::function<void(T)> callback,
        const dds::sub::qos::DataReaderQos& qos);

    template<typename T>
    PublisherHandle<T> create_publisher(const std::string& topic_name);

    template<typename T>
    PublisherHandle<T> create_publisher(
        const std::string& topic_name,
        const dds::pub::qos::DataWriterQos& qos);

    void create_timer(std::chrono::milliseconds millis, std::function<void()> callback);

private:
    struct Timer {
        std::chrono::milliseconds period;
        std::chrono::steady_clock::time_point trigger_time;
        std::function<void()> callback;
    };

    class SubscriptionBase {
    public:
        virtual ~SubscriptionBase() = default;

        virtual void attach(dds::core::cond::WaitSet& waitset) = 0;
        virtual void detach(dds::core::cond::WaitSet& waitset) noexcept = 0;
        virtual bool dispatch_if_active(const dds::core::cond::Condition& condition) = 0;
    };

    template<typename T>
    class Subscription final : public SubscriptionBase {
    public:
        Subscription(
            dds::sub::Subscriber subscriber,
            dds::topic::Topic<T> topic,
            dds::sub::qos::DataReaderQos qos,
            std::function<void(T)> callback) :
            reader_{std::move(subscriber), std::move(topic), std::move(qos)},
            condition_{reader_},
            callback_{std::move(callback)} {
            condition_.enabled_statuses(dds::core::status::StatusMask::data_available());
        }

        void attach(dds::core::cond::WaitSet& waitset) override {
            waitset += condition_;
            attached_ = true;
        }

        void detach(dds::core::cond::WaitSet& waitset) noexcept override {
            if (!attached_) {
                return;
            }
            try {
                waitset -= condition_;
            } catch (...) {
            }
            attached_ = false;
        }

        bool dispatch_if_active(const dds::core::cond::Condition& condition) override {
            if (condition != condition_) {
                return false;
            }
            drain();
            return true;
        }

    private:
        void drain() {
            dds::sub::LoanedSamples<T> samples = reader_.take();
            for (const auto& sample : samples) {
                if (sample.info().valid()) {
                    callback_(sample.data());
                }
            }
        }

        dds::sub::DataReader<T> reader_;
        dds::core::cond::StatusCondition condition_;
        std::function<void(T)> callback_;
        bool attached_{false};
    };

    static std::atomic<bool> running_;
    std::string name_;
    std::unique_ptr<NodeSupervisor> supervisor_;
    std::chrono::steady_clock::time_point supervision_next_heartbeat_{};
    dds::domain::DomainParticipant participant_;
    dds::pub::Publisher publisher_;
    dds::sub::Subscriber subscriber_;
    dds::core::cond::WaitSet waitset_;
    
    std::vector<dds::pub::AnyDataWriter> data_writers_{};
    std::vector<std::unique_ptr<SubscriptionBase>> subscriptions_{};
    std::vector<Timer> timers_{};
    std::unique_ptr<EVSNParameterService> parameter_service_{};
    
    void topic_name_validator(const std::string& name);
    void record_spin_progress(std::chrono::steady_clock::time_point now);
    void detach_subscriptions() noexcept;
    void initialise_timers(std::chrono::steady_clock::time_point now);
    bool run_due_timers(std::chrono::steady_clock::time_point now);
    bool dispatch_waitset(std::chrono::milliseconds timeout);
    std::chrono::milliseconds next_wait_timeout(std::chrono::steady_clock::time_point now) const;
    static std::string resolve_parameter_file(
        const std::string& node_name,
        const std::string& parameter_file);
    static void signal_handler(int signum);

    template<typename T, typename U>
    void publish(uint32_t data_writer_id, U&& data);

    friend class EVSNNodeTestAccess;
};


// =================================================================================================
// IMPLEMENTATION OF TEMPLATED FUNCTIONS
// =================================================================================================

template<typename T>
void EVSNNode::create_subscriber(const std::string& topic_name, std::function<void(T)> callback) {
    create_subscriber<T>(
        topic_name,
        std::move(callback),
        dds::core::QosProvider::Default().datareader_qos());
}

template<typename T>
void EVSNNode::create_subscriber(
    const std::string& topic_name,
    std::function<void(T)> callback,
    const dds::sub::qos::DataReaderQos& qos) {
    if (running_) {
        throw std::logic_error("Cannot create subscriber while node is running");
    }
    topic_name_validator(topic_name);
    dds::topic::Topic<T> topic{participant_, topic_name};
    auto subscription = std::make_unique<Subscription<T>>(
        subscriber_,
        std::move(topic),
        qos,
        std::move(callback));
    subscription->attach(waitset_);
    subscriptions_.push_back(std::move(subscription));
}

template<typename T>
EVSNNode::PublisherHandle<T> EVSNNode::create_publisher(const std::string& topic_name) {
    return create_publisher<T>(
        topic_name,
        dds::core::QosProvider::Default().datawriter_qos());
}

template<typename T>
EVSNNode::PublisherHandle<T> EVSNNode::create_publisher(
    const std::string& topic_name,
    const dds::pub::qos::DataWriterQos& qos) {
    if (running_) {
        throw std::logic_error("Cannot create publisher while node is running");
    }
    topic_name_validator(topic_name);
    dds::topic::Topic<T> topic{participant_, topic_name};
    dds::pub::DataWriter<T> writer{publisher_, topic, qos};
    data_writers_.push_back(writer);

    uint32_t data_writer_id = data_writers_.size() - 1;
    return PublisherHandle<T>{this, data_writer_id};
}

template<typename T, typename U>
void EVSNNode::publish(uint32_t data_writer_id, U&& data) {
    data_writers_.at(data_writer_id).template get<T>().write(std::forward<U>(data));
}
