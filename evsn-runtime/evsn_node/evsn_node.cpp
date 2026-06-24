#include "node_supervisor.hpp"
#include "parameter_service.hpp"
#include "evsn_node.hpp"

#include <csignal>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>

std::atomic<bool> EVSNNode::running_{false};

namespace {
constexpr auto kParameterPollPeriod = std::chrono::milliseconds{20};
constexpr auto kMaximumWaitPeriod = std::chrono::milliseconds{100};

bool is_topic_segment_character(char character) {
    return (character >= 'a' && character <= 'z') ||
        (character >= '0' && character <= '9') ||
        character == '_';
}

bool is_valid_topic_name(const std::string& name) {
    if (name.size() < 2 || name.front() != '/' || name.back() == '/') {
        return false;
    }

    auto previous_was_separator = true;
    for (auto index = std::size_t{1}; index < name.size(); ++index) {
        const char character = name[index];
        if (character == '/') {
            if (previous_was_separator) {
                return false;
            }
            previous_was_separator = true;
            continue;
        }
        if (!is_topic_segment_character(character)) {
            return false;
        }
        previous_was_separator = false;
    }

    return !previous_was_separator;
}
}

// =================================================================================================
// CONSTRUCTOR AND DESTRUCTOR
// =================================================================================================
EVSNNode::EVSNNode(const std::string& name, int32_t domain_id, const std::string& parameter_file) :
    participant_{domain_id}, 
    publisher_{participant_}, 
    subscriber_{participant_} {
        if (name.empty()) {
            throw std::invalid_argument("Name cannot be empty");
        }
        name_ = name;
        supervisor_ = create_node_supervisor(name_);
        parameter_service_ = std::make_unique<EVSNParameterService>(
            participant_,
            name_,
            resolve_parameter_file(name_, parameter_file));
        timers_.push_back({
            kParameterPollPeriod,
            std::chrono::steady_clock::now(),
            [this]() {
                parameter_service_->poll();
            }});
    }

EVSNNode::~EVSNNode() {
    detach_subscriptions();
    if (supervisor_ != nullptr) {
        supervisor_->shutdown();
    }
}

// =================================================================================================
// PRIVATE HELPER FUNCTIONS
// =================================================================================================
void EVSNNode::topic_name_validator(const std::string& name) {
    if (!is_valid_topic_name(name)) {
        throw std::invalid_argument("Invalid topic name: " + name);
    }
}

void EVSNNode::signal_handler(int signum) {
    running_ = false;
}

std::string EVSNNode::resolve_parameter_file(
    const std::string& node_name,
    const std::string& parameter_file) {
    if (!parameter_file.empty()) {
        return parameter_file;
    }
    const auto explicit_file = std::getenv("EVSN_PARAM_FILE");
    if (explicit_file != nullptr && std::string{explicit_file}.size() > 0) {
        return explicit_file;
    }
    auto candidates = std::vector<std::string>{};
    const auto param_dir = std::getenv("EVSN_PARAM_DIR");
    if (param_dir != nullptr && std::string{param_dir}.size() > 0) {
        candidates.push_back(std::string{param_dir} + "/" + node_name + "_parameters.toml");
        candidates.push_back(std::string{param_dir} + "/" + node_name + "_param.toml");
    }
    candidates.push_back("param/" + node_name + "_parameters.toml");
    candidates.push_back("param/" + node_name + "_param.toml");
    candidates.push_back("../param/" + node_name + "_parameters.toml");
    candidates.push_back("../param/" + node_name + "_param.toml");
    for (const auto& candidate : candidates) {
        auto file = std::ifstream{candidate};
        if (file.good()) {
            return candidate;
        }
    }
    return {};
}

void EVSNNode::record_spin_progress(std::chrono::steady_clock::time_point now) {
    if (supervisor_ == nullptr) {
        return;
    }

    const auto heartbeat_period = supervisor_->config().heartbeat_period;
    if (heartbeat_period <= std::chrono::milliseconds{0} || now >= supervision_next_heartbeat_) {
        supervisor_->heartbeat();
        supervision_next_heartbeat_ = now + heartbeat_period;
    }
}

void EVSNNode::detach_subscriptions() noexcept {
    for (auto& subscription : subscriptions_) {
        if (subscription != nullptr) {
            subscription->detach(waitset_);
        }
    }
}

void EVSNNode::initialise_timers(std::chrono::steady_clock::time_point now) {
    supervision_next_heartbeat_ = now;
    for (Timer& timer : timers_) {
        timer.trigger_time = now + timer.period;
    }
}

bool EVSNNode::run_due_timers(std::chrono::steady_clock::time_point now) {
    auto made_progress = false;
    for (Timer& timer : timers_) {
        if (timer.trigger_time > now) {
            continue;
        }

        timer.callback();
        made_progress = true;

        const auto now_after = std::chrono::steady_clock::now();
        if (timer.trigger_time <= now_after) {
            const auto elapsed = now_after - timer.trigger_time;
            const auto periods_to_skip = (elapsed / timer.period) + 1;
            timer.trigger_time += timer.period * periods_to_skip;
        } else {
            timer.trigger_time += timer.period;
        }
    }
    return made_progress;
}

bool EVSNNode::dispatch_waitset(std::chrono::milliseconds timeout) {
    const auto triggered = waitset_.wait(dds::core::Duration(timeout));
    auto made_progress = false;
    for (const auto& condition : triggered) {
        for (auto& subscription : subscriptions_) {
            if (subscription != nullptr && subscription->dispatch_if_active(condition)) {
                made_progress = true;
                break;
            }
        }
    }
    return made_progress;
}

std::chrono::milliseconds EVSNNode::next_wait_timeout(
    std::chrono::steady_clock::time_point now) const {
    auto timeout = kMaximumWaitPeriod;
    for (const Timer& timer : timers_) {
        auto remaining = std::chrono::ceil<std::chrono::milliseconds>(timer.trigger_time - now);
        if (remaining < std::chrono::milliseconds{0}) {
            remaining = std::chrono::milliseconds{0};
        }
        if (remaining < timeout) {
            timeout = remaining;
        }
    }
    return timeout;
}

// =================================================================================================
// MEMBER FUNCTIONS
// =================================================================================================
const std::string& EVSNNode::get_name() const {
    return name_;
}

std::size_t EVSNNode::parameter_count() const {
    return parameter_service_->parameter_count();
}

std::string EVSNNode::parameter_value_as_string(const std::string& parameter_name) const {
    return parameter_service_->parameter_value_as_string(parameter_name);
}

void EVSNNode::create_timer(std::chrono::milliseconds millis, std::function<void()> callback) {
    if (running_) {
        throw std::logic_error("Cannot create timer while node is running");
    }
    if (millis <= std::chrono::milliseconds{0}) {
        throw std::invalid_argument("Timer period must be positive");
    }
    if (!callback) {
        throw std::invalid_argument("Timer callback must be callable");
    }
    timers_.push_back(Timer{millis, std::chrono::steady_clock::time_point{}, std::move(callback)});
}

void EVSNNode::spin() {
    running_ = true;
    if (std::signal(SIGINT, EVSNNode::signal_handler) == SIG_ERR ||
        std::signal(SIGTERM, EVSNNode::signal_handler) == SIG_ERR) {
        running_ = false;
        throw std::runtime_error("Failed to install EVSNNode signal handler");
    }

    initialise_timers(std::chrono::steady_clock::now());

    try {
        while (running_) {
            const auto now = std::chrono::steady_clock::now();
            const auto made_progress = run_due_timers(now);
            if (made_progress) {
                record_spin_progress(std::chrono::steady_clock::now());
            }

            if (!running_) {
                break;
            }

            const auto dds_progress = dispatch_waitset(next_wait_timeout(std::chrono::steady_clock::now()));
            if (dds_progress) {
                record_spin_progress(std::chrono::steady_clock::now());
            }
        }
    } catch (...) {
        running_ = false;
        throw;
    }
    running_ = false;
}
