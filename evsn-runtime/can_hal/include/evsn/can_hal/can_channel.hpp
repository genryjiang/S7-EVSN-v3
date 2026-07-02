#pragma once

#include "evsn/can_hal/can_frame.hpp"
#include "evsn/can_hal/can_result.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace evsn::can_hal {

constexpr auto kCanPathCapacity = std::size_t{96U};

enum class CanMode : std::uint8_t {
    auto_detect = 0U,
    classic = 1U,
    fd = 2U,
};

struct CanOpenConfig {
    std::array<char, kCanPathCapacity> rx_path{};
    std::array<char, kCanPathCapacity> tx_path{};
    CanMode mode{CanMode::auto_detect};
    bool arbitration_bitrate_valid{false};
    std::uint32_t arbitration_bitrate{0U};
    bool data_bitrate_valid{false};
    std::uint32_t data_bitrate{0U};
    bool latency_limit_valid{false};
    std::uint32_t latency_limit_ms{0U};
};

struct CanCapabilities {
    bool classic_frame{false};
    bool fd_frame{false};
    bool extended_id{false};
    bool remote_frame{false};
    bool brs{false};
    bool esi{false};
    bool fdf_flag{false};
    bool timestamp{false};
    bool latency_limit{false};
    bool runtime_bitrate_change{false};
};

struct CanDiagnostics {
    std::uint64_t opens{0U};
    std::uint64_t closes{0U};
    std::uint64_t rx_frames{0U};
    std::uint64_t tx_frames{0U};
    std::uint64_t no_frame_polls{0U};
    std::uint64_t tx_full_events{0U};
    std::uint64_t invalid_frame_rejects{0U};
    std::uint64_t unsupported_feature_rejects{0U};
    std::uint64_t open_failures{0U};
    std::uint64_t io_errors{0U};
    CanResult last_backend_failure{};
};

[[nodiscard]] bool path_is_empty(const std::array<char, kCanPathCapacity>& path) noexcept;
[[nodiscard]] CanResult set_rx_path(CanOpenConfig& config, const char* path) noexcept;
[[nodiscard]] CanResult set_tx_path(CanOpenConfig& config, const char* path) noexcept;
[[nodiscard]] CanResult validate_frame(
    const CanFrame& frame,
    const CanCapabilities& capabilities) noexcept;

class FakeCanBackendControl;

class CanChannel {
public:
    CanChannel() noexcept;
    ~CanChannel() noexcept;

    CanChannel(const CanChannel&) = delete;
    CanChannel& operator=(const CanChannel&) = delete;
    CanChannel(CanChannel&&) = delete;
    CanChannel& operator=(CanChannel&&) = delete;

    [[nodiscard]] CanResult open(const CanOpenConfig& config) noexcept;
    [[nodiscard]] CanResult read(CanFrame& frame) noexcept;
    [[nodiscard]] CanResult write(const CanFrame& frame) noexcept;
    [[nodiscard]] CanResult close() noexcept;
    [[nodiscard]] CanCapabilities capabilities() const noexcept;
    [[nodiscard]] CanDiagnostics diagnostics() const noexcept;

private:
    friend class FakeCanBackendControl;

    static constexpr auto kFakeQueueCapacity = std::size_t{16U};

    CanOpenConfig config_{};
    CanCapabilities capabilities_{};
    CanDiagnostics diagnostics_{};
    bool open_{false};
    int rx_fd_{-1};
    int tx_fd_{-1};

    std::array<CanFrame, kFakeQueueCapacity> fake_rx_queue_{};
    std::array<CanFrame, kFakeQueueCapacity> fake_tx_log_{};
    std::size_t fake_rx_head_{0U};
    std::size_t fake_rx_count_{0U};
    std::size_t fake_tx_count_{0U};
    CanStatus fake_open_status_{CanStatus::ok};
    CanStatus fake_read_status_{CanStatus::ok};
    CanStatus fake_write_status_{CanStatus::ok};
    int fake_native_error_{0};
};

}  // namespace evsn::can_hal
