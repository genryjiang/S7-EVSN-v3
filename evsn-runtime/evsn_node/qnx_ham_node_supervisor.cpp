#include "node_supervisor.hpp"

#ifndef __QNXNTO__
#error "qnx_ham_node_supervisor.cpp must only be built for QNX targets"
#endif

#include <ha/ham.h>

#include <cerrno>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

std::string errno_message(const std::string& action) {
    return action + ": " + std::strerror(errno);
}

long parse_long_environment(const char* environment_name, const char* value) {
    errno = 0;
    char* end = nullptr;
    const long parsed = std::strtol(value, &end, 10);
    if (errno == ERANGE || end == value || end == nullptr || *end != '\0') {
        throw std::runtime_error(std::string{environment_name} + " must be an integer");
    }
    return parsed;
}

std::chrono::milliseconds configured_heartbeat_period() {
    const char* value = std::getenv("EVSN_HAM_HEARTBEAT_MS");
    if (value == nullptr || *value == '\0') {
        return std::chrono::milliseconds{1000};
    }

    const long millis = parse_long_environment("EVSN_HAM_HEARTBEAT_MS", value);
    if (millis <= 0) {
        throw std::runtime_error("EVSN_HAM_HEARTBEAT_MS must be a positive integer");
    }
    const auto period_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::milliseconds{millis}).count();
    if (period_ns < static_cast<long long>(HAMHBEATMIN)) {
        throw std::runtime_error("EVSN_HAM_HEARTBEAT_MS must be at least QNX HAMHBEATMIN");
    }
    return std::chrono::milliseconds{millis};
}

int configured_threshold(const char* environment_name, int default_value) {
    const char* value = std::getenv(environment_name);
    if (value == nullptr || *value == '\0') {
        return default_value;
    }

    const long threshold = parse_long_environment(environment_name, value);
    if (threshold < 0) {
        throw std::runtime_error(std::string{environment_name} + " must be zero or positive");
    }
    if (threshold > std::numeric_limits<int>::max()) {
        throw std::runtime_error(std::string{environment_name} + " exceeds int range");
    }
    return static_cast<int>(threshold);
}

std::string quote_if_needed(const std::string& path) {
    if (path.find_first_of(" \t\"\\") == std::string::npos) {
        return path;
    }

    std::string quoted{"\""};
    for (char character : path) {
        if (character == '"' || character == '\\') {
            quoted.push_back('\\');
        }
        quoted.push_back(character);
    }
    quoted.push_back('"');
    return quoted;
}

std::string current_executable_path() {
    std::ifstream exefile{"/proc/self/exefile"};
    if (!exefile) {
        throw std::runtime_error("Unable to open /proc/self/exefile for HAM restart command");
    }
    std::string path;
    std::getline(exefile, path);
    if (path.empty()) {
        throw std::runtime_error("Unable to read /proc/self/exefile for HAM restart command");
    }
    return path;
}

std::string configured_restart_command() {
    const char* override = std::getenv("EVSN_HAM_RESTART_COMMAND");
    if (override != nullptr && *override != '\0') {
        return override;
    }
    return quote_if_needed(current_executable_path());
}

std::string configured_high_command() {
    const char* command = std::getenv("EVSN_HAM_HEARTBEAT_HIGH_COMMAND");
    if (command == nullptr) {
        return {};
    }
    return command;
}

uint64_t heartbeat_period_ns(std::chrono::milliseconds period) {
    return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(period).count());
}

void require_action(ham_action_t* action, const std::string& description) {
    if (action == nullptr) {
        throw std::runtime_error(errno_message(description));
    }
}

ham_condition_t* require_condition(
    ham_entity_t* entity,
    int type,
    const char* name,
    unsigned flags) {
    ham_condition_t* condition = ham_condition(entity, type, name, flags);
    if (condition == nullptr) {
        throw std::runtime_error(errno_message(std::string{"ham_condition "} + name));
    }
    return condition;
}

class HamNodeSupervisor final : public NodeSupervisor {
public:
    explicit HamNodeSupervisor(std::string node_name) {
        config_.entity_name = make_supervision_entity_name(node_name);
        config_.heartbeat_period = configured_heartbeat_period();
        config_.missed_heartbeat_low = configured_threshold("EVSN_HAM_MISSED_HEARTBEAT_LOW", 3);
        config_.missed_heartbeat_high = configured_threshold("EVSN_HAM_MISSED_HEARTBEAT_HIGH", 5);
        if (config_.missed_heartbeat_low > config_.missed_heartbeat_high) {
            throw std::runtime_error(
                "EVSN_HAM_MISSED_HEARTBEAT_LOW must be less than or equal to "
                "EVSN_HAM_MISSED_HEARTBEAT_HIGH");
        }
        config_.restart_command = configured_restart_command();
        config_.heartbeat_high_command = configured_high_command();

        entity_ = ham_attach_self(
            config_.entity_name.c_str(),
            heartbeat_period_ns(config_.heartbeat_period),
            config_.missed_heartbeat_low,
            config_.missed_heartbeat_high,
            0);
        if (entity_ == nullptr) {
            throw std::runtime_error(errno_message("ham_attach_self " + config_.entity_name));
        }

        try {
            configure_death_restart();
            configure_heartbeat_actions();
        } catch (...) {
            shutdown();
            throw;
        }
    }

    ~HamNodeSupervisor() override {
        shutdown();
    }

    const SupervisionConfig& config() const override {
        return config_;
    }

    void heartbeat() override {
        ham_heartbeat();
    }

    void shutdown() override {
        if (entity_ == nullptr) {
            return;
        }
        if (ham_detach_self(entity_, 0) == -1) {
            // Destructors call shutdown(), so detach failure is made terminal
            // for this backend instance rather than throwing during cleanup.
            entity_ = nullptr;
            return;
        }
        entity_ = nullptr;
    }

private:
    void configure_death_restart() {
        ham_condition_t* death = require_condition(entity_, CONDDEATH, "death", HCONDNOWAIT);
        require_action(
            ham_action_log(
                death,
                "death_log",
                "EVSNNode process death detected; restarting",
                1,
                1,
                0),
            "ham_action_log death_log");
        require_action(
            ham_action_restart(
                death,
                "restart",
                config_.restart_command.c_str(),
                HACTIONBREAKONFAIL),
            "ham_action_restart restart");
    }

    void configure_heartbeat_actions() {
        ham_condition_t* low = require_condition(
            entity_,
            CONDHBEATMISSEDLOW,
            "heartbeat_missed_low",
            HCONDNOWAIT);
        require_action(
            ham_action_log(
                low,
                "heartbeat_low_log",
                "EVSNNode missed low heartbeat threshold",
                1,
                1,
                0),
            "ham_action_log heartbeat_low_log");

        ham_condition_t* high = require_condition(
            entity_,
            CONDHBEATMISSEDHIGH,
            "heartbeat_missed_high",
            HCONDNOWAIT);
        require_action(
            ham_action_log(
                high,
                "heartbeat_high_log",
                "EVSNNode missed high heartbeat threshold",
                1,
                1,
                0),
            "ham_action_log heartbeat_high_log");

        if (!config_.heartbeat_high_command.empty()) {
            require_action(
                ham_action_execute(
                    high,
                    "heartbeat_high_execute",
                    config_.heartbeat_high_command.c_str(),
                    HACTIONBREAKONFAIL),
                "ham_action_execute heartbeat_high_execute");
        }
    }

    SupervisionConfig config_{};
    ham_entity_t* entity_{nullptr};
};

}

std::unique_ptr<NodeSupervisor> create_node_supervisor(const std::string& node_name) {
    return std::make_unique<HamNodeSupervisor>(node_name);
}
