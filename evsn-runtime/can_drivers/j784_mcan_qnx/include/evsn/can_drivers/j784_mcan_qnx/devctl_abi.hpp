#pragma once

#include "evsn/can_drivers/j784_mcan_qnx/config.hpp"

#include <array>
#include <cstdint>

namespace evsn::can_drivers::j784_mcan_qnx {

constexpr auto kDevctlAbiVersion = std::uint32_t{1U};
constexpr auto kMaxFramePayload = std::uint8_t{64U};
constexpr auto kMcanFrameTransferStructSize = std::uint32_t{96U};
constexpr auto kMcanControllerCapabilitiesStructSize = std::uint32_t{20U};
constexpr auto kMcanDiagnosticsSnapshotStructSize = std::uint32_t{72U};

enum class McanDevctlCommand : std::uint8_t {
  query_capabilities = 1U,
  configure_bitrate = 2U,
  query_diagnostics = 3U,
  set_loopback = 4U,
  recover_bus = 5U,
};

enum class McanFrameFlag : std::uint32_t {
  extended_id = 1U << 0U,
  fd_frame = 1U << 1U,
  brs = 1U << 2U,
  esi = 1U << 3U,
  remote = 1U << 4U,
  timestamp_valid = 1U << 5U,
};

enum class McanCapabilityFlag : std::uint32_t {
  classic_frame = 1U << 0U,
  fd_frame = 1U << 1U,
  extended_id = 1U << 2U,
  brs = 1U << 3U,
  esi = 1U << 4U,
  hardware_timestamp = 1U << 5U,
  internal_loopback = 1U << 6U,
  bus_off_recovery = 1U << 7U,
};

struct McanFrameTransfer {
  std::uint32_t abi_version{kDevctlAbiVersion};
  std::uint32_t sizeof_struct{kMcanFrameTransferStructSize};
  std::uint32_t can_id{0U};
  std::uint32_t flags{0U};
  std::uint8_t dlc{0U};
  std::uint8_t length{0U};
  std::uint16_t reserved{0U};
  std::array<std::uint8_t, kMaxFramePayload> payload{};
  std::uint64_t monotonic_timestamp_ns{0U};
};

struct McanControllerCapabilities {
  std::uint32_t abi_version{kDevctlAbiVersion};
  std::uint32_t sizeof_struct{kMcanControllerCapabilitiesStructSize};
  std::uint32_t flags{0U};
  std::uint8_t max_rx_queues{kMaxQueuesPerController};
  std::uint8_t max_tx_queues{kMaxQueuesPerController};
  std::uint16_t max_rx_queue_capacity{kMaxQueueCapacity};
  std::uint16_t max_tx_queue_capacity{kMaxQueueCapacity};
  std::uint16_t reserved{0U};
};

struct McanDiagnosticsSnapshot {
  std::uint32_t abi_version{kDevctlAbiVersion};
  std::uint32_t sizeof_struct{kMcanDiagnosticsSnapshotStructSize};
  std::uint64_t rx_frames{0U};
  std::uint64_t tx_frames{0U};
  std::uint64_t rx_queue_full{0U};
  std::uint64_t tx_queue_full{0U};
  std::uint64_t invalid_frame_rejects{0U};
  std::uint64_t bus_off_events{0U};
  std::uint64_t interrupt_count{0U};
  std::uint32_t last_native_error{0U};
  std::uint32_t reserved{0U};
};

[[nodiscard]] constexpr std::uint32_t flag_value(McanFrameFlag flag) noexcept {
  return static_cast<std::uint32_t>(flag);
}

[[nodiscard]] constexpr std::uint32_t
capability_value(McanCapabilityFlag flag) noexcept {
  return static_cast<std::uint32_t>(flag);
}

[[nodiscard]] bool has_flag(std::uint32_t flags, McanFrameFlag flag) noexcept;
[[nodiscard]] bool has_capability(std::uint32_t flags,
                                  McanCapabilityFlag flag) noexcept;
[[nodiscard]] McanStatus validate_frame_transfer(
    const McanFrameTransfer &frame,
    const McanControllerCapabilities &capabilities) noexcept;

} // namespace evsn::can_drivers::j784_mcan_qnx
