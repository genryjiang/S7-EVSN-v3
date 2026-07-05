#pragma once

#include "evsn/can_drivers/j784_mcan_qnx/devctl_abi.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace evsn::can_drivers::j784_mcan_qnx {

constexpr auto kQnxCanCompatMaxIrqs = std::size_t{3U};
constexpr auto kQnxCanCompatClassicMailboxCount = std::uint16_t{64U};
constexpr auto kQnxCanCompatFdMailboxCount = std::uint16_t{28U};

struct QnxCanCompatRawTiming {
  bool valid{false};
  std::uint16_t presdiv{0U};
  std::uint8_t propseg{0U};
  std::uint8_t pseg1{0U};
  std::uint8_t pseg2{0U};
  std::uint8_t rjw{0U};
  McanControllerTimingOverride timing{};
};

struct QnxCanCompatStartupConfig {
  std::uint8_t can_unit{0U};
  std::array<std::uint32_t, kQnxCanCompatMaxIrqs> irqs{};
  std::size_t irq_count{0U};
  std::uint32_t nominal_bitrate{50'000U};
  std::uint32_t data_bitrate{50'000U};
  std::uint16_t mailbox_count{0U};
  QnxCanCompatRawTiming nominal_raw_timing{};
  QnxCanCompatRawTiming data_raw_timing{};
  bool fd_mode{false};
  bool raw_mode{false};
  bool internal_loopback{false};
  bool listen_only{false};
  bool iso_fd{false};
  bool tdc_enabled{false};
  std::uint16_t tdc_offset{0U};
  bool single_message_buffer{false};
  bool io_privilege_requested{false};
  bool extended_mid_default{false};
};

struct QnxCanCompatCanMessage {
  std::array<std::uint8_t, 8U> data{};
  std::uint8_t length{0U};
  std::uint32_t message_id{0U};
  std::uint32_t timestamp{0U};
  bool extended_id{false};
  bool remote_frame{false};
};

[[nodiscard]] bool qnx_can_compat_startup_args_present(
    int argc, const char *const argv[]) noexcept;
[[nodiscard]] McanStatus parse_qnx_can_compat_startup(
    int argc, const char *const argv[],
    QnxCanCompatStartupConfig &config) noexcept;
[[nodiscard]] McanStatus qnx_can_compat_make_resource_manager_config(
    const QnxCanCompatStartupConfig &startup,
    McanResourceManagerConfig &config) noexcept;
[[nodiscard]] McanStatus make_qnx_can_compat_endpoint_path(
    EndpointKind kind, std::uint8_t can_unit, std::uint8_t mailbox_id,
    McanEndpointPath &path) noexcept;
[[nodiscard]] McanStatus make_qnx_can_compat_namespace_path(
    std::uint8_t can_unit, McanEndpointPath &path) noexcept;
[[nodiscard]] McanStatus qnx_can_compat_message_from_frame(
    const McanFrameTransfer &frame,
    QnxCanCompatCanMessage &message) noexcept;
[[nodiscard]] McanStatus qnx_can_compat_frame_from_message(
    const QnxCanCompatCanMessage &message,
    McanFrameTransfer &frame) noexcept;
[[nodiscard]] int
qnx_can_compat_raw_tx_status_to_errno(McanStatus status) noexcept;

} // namespace evsn::can_drivers::j784_mcan_qnx
