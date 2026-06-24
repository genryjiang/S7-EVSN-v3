#include "evsn_node.hpp"

#include <algorithm>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>

namespace {

constexpr auto smoke_domain_id = int32_t{215};
constexpr auto smoke_timer_period = std::chrono::milliseconds{100};
constexpr auto stall_duration = std::chrono::milliseconds{3500};
constexpr auto clean_tick_limit = int{25};
constexpr auto stall_tick_limit = int{6};
constexpr auto kill_tick_limit = int{3};
constexpr auto restarted_tick_limit = int{15};

struct smoke_config {
    std::string mode{"clean"};
    std::string prefix{"/tmp/qnx_evsn_node_ham_smoke"};
};

std::string marker_path(const std::string& prefix, const std::string& suffix) {
    return prefix + "." + suffix;
}

void write_marker(const std::string& path, const std::string& message) {
    auto file = std::ofstream{path};
    if (!file) {
        throw std::runtime_error("failed to open marker file: " + path);
    }
    file << message << '\n';
    if (!file) {
        throw std::runtime_error("failed to write marker file: " + path);
    }
}

bool valid_mode(const std::string& mode) {
    return mode == "clean" || mode == "stall" || mode == "kill" || mode == "restarted";
}

int tick_limit_for_mode(const std::string& mode) {
    if (mode == "stall") {
        return stall_tick_limit;
    }
    if (mode == "kill") {
        return kill_tick_limit;
    }
    if (mode == "restarted") {
        return restarted_tick_limit;
    }
    return clean_tick_limit;
}

class smoke_node final : public EVSNNode {
public:
    explicit smoke_node(smoke_config config) :
        EVSNNode{"qnx_evsn_node_ham_smoke", smoke_domain_id},
        config_{std::move(config)} {
        write_marker(marker_path(config_.prefix, "attached"), config_.mode);
        create_timer(smoke_timer_period, [this]() {
            on_timer();
        });
    }

    void write_timing_summary() const {
        auto file = std::ofstream{marker_path(config_.prefix, "timing")};
        if (!file) {
            throw std::runtime_error("failed to open timing marker");
        }
        const auto min_interval = interval_count_ == 0 ? int64_t{0} : min_interval_us_;
        file << "mode=" << config_.mode << '\n';
        file << "ticks=" << ticks_ << '\n';
        file << "interval_count=" << interval_count_ << '\n';
        file << "min_interval_us=" << min_interval << '\n';
        file << "max_interval_us=" << max_interval_us_ << '\n';
        if (!file) {
            throw std::runtime_error("failed to write timing marker");
        }
    }

private:
    void on_timer() {
        const auto now = std::chrono::steady_clock::now();
        ++ticks_;
        if (ticks_ == 1) {
            first_tick_ = now;
            last_tick_ = now;
            write_marker(marker_path(config_.prefix, "first_tick"), config_.mode);
        } else {
            record_interval(now);
        }
        if (config_.mode == "stall" && !stalled_ && ticks_ == 2) {
            stalled_ = true;
            write_marker(marker_path(config_.prefix, "stalling"), config_.mode);
            std::this_thread::sleep_for(stall_duration);
            write_marker(marker_path(config_.prefix, "resumed"), config_.mode);
            return;
        }
        if (config_.mode == "kill" && ticks_ >= tick_limit_for_mode(config_.mode)) {
            write_marker(marker_path(config_.prefix, "restarting"), config_.mode);
            std::raise(SIGKILL);
            return;
        }
        if (ticks_ >= tick_limit_for_mode(config_.mode)) {
            const auto suffix = config_.mode == "restarted" ? "restarted" : "alive";
            write_marker(marker_path(config_.prefix, suffix), config_.mode);
            std::raise(SIGTERM);
        }
    }

    void record_interval(std::chrono::steady_clock::time_point now) {
        const auto interval = static_cast<int64_t>(
            std::chrono::duration_cast<std::chrono::microseconds>(now - last_tick_).count());
        min_interval_us_ = std::min(min_interval_us_, interval);
        max_interval_us_ = std::max(max_interval_us_, interval);
        last_tick_ = now;
        ++interval_count_;
    }

    smoke_config config_;
    int ticks_{0};
    int interval_count_{0};
    int64_t min_interval_us_{std::numeric_limits<int64_t>::max()};
    int64_t max_interval_us_{0};
    bool stalled_{false};
    std::chrono::steady_clock::time_point first_tick_{};
    std::chrono::steady_clock::time_point last_tick_{};
};

smoke_config parse_config(int argc, char** argv) {
    auto config = smoke_config{};
    for (auto index = int{1}; index < argc; ++index) {
        const auto arg = std::string{argv[index]};
        if (arg == "--mode") {
            ++index;
            if (index >= argc) {
                throw std::runtime_error("--mode requires a value");
            }
            config.mode = argv[index];
        } else if (arg == "--prefix") {
            ++index;
            if (index >= argc) {
                throw std::runtime_error("--prefix requires a value");
            }
            config.prefix = argv[index];
        } else {
            throw std::runtime_error("unknown argument: " + arg);
        }
    }
    if (!valid_mode(config.mode)) {
        throw std::runtime_error("invalid mode: " + config.mode);
    }
    return config;
}

int run_smoke(const smoke_config& config) {
    {
        auto node = smoke_node{config};
        node.spin();
        node.write_timing_summary();
    }
    write_marker(marker_path(config.prefix, "detached"), config.mode);
    return 0;
}

int mark_file(int argc, char** argv) {
    if (argc != 4) {
        throw std::runtime_error("--mark requires <path> <message>");
    }
    write_marker(argv[2], argv[3]);
    return 0;
}

} // namespace

int main(int argc, char** argv) {
    try {
        if (argc > 1 && std::string{argv[1]} == "--mark") {
            return mark_file(argc, argv);
        }
        const auto config = parse_config(argc, argv);
        return run_smoke(config);
    } catch (const std::exception& error) {
        std::cerr << "qnx_evsn_node_ham_smoke: " << error.what() << '\n';
        return 2;
    }
}
