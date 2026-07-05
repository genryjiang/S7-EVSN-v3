#pragma once

#include "evsn/can_drivers/j784_mcan_qnx/mcan_core.hpp"

#include <array>
#include <cstdint>

namespace evsn::can_drivers::j784_mcan_qnx {

struct McanResourceManagerRuntimeOptions {
  bool frame_io_enabled{false};
  bool internal_loopback{false};
  bool interrupts_enabled{false};
  bool complete_tx_immediately{false};
};

struct McanResourceManagerRuntime {
  McanResourceManagerConfig config{};
  McanResourceManagerRuntimeOptions options{};
  std::array<McanCore, kMaxControllers> cores{};
  std::size_t controller_count{0U};
  bool initialized{false};
};

[[nodiscard]] McanStatus initialize_resource_manager_runtime(
    McanResourceManagerRuntime &runtime,
    const McanResourceManagerConfig &config,
    const McanResourceManagerRuntimeOptions &options) noexcept;
[[nodiscard]] McanStatus resource_manager_transmit_frame(
    McanResourceManagerRuntime &runtime, EndpointKind endpoint_kind,
    std::uint8_t instance_id, std::uint8_t queue_id,
    const McanFrameTransfer &frame) noexcept;
[[nodiscard]] McanStatus resource_manager_receive_frame(
    McanResourceManagerRuntime &runtime, EndpointKind endpoint_kind,
    std::uint8_t instance_id, std::uint8_t queue_id,
    McanFrameTransfer &frame) noexcept;
[[nodiscard]] McanStatus resource_manager_query_capabilities(
    const McanResourceManagerRuntime &runtime, EndpointKind endpoint_kind,
    std::uint8_t instance_id,
    McanControllerCapabilities &capabilities) noexcept;
[[nodiscard]] McanStatus resource_manager_query_diagnostics(
    const McanResourceManagerRuntime &runtime, EndpointKind endpoint_kind,
    std::uint8_t instance_id,
    McanDiagnosticsSnapshot &diagnostics) noexcept;
[[nodiscard]] McanStatus resource_manager_rx_ready_count(
    const McanResourceManagerRuntime &runtime, EndpointKind endpoint_kind,
    std::uint8_t instance_id, std::uint8_t queue_id,
    std::size_t &ready_count) noexcept;
[[nodiscard]] McanStatus resource_manager_tx_room_count(
    const McanResourceManagerRuntime &runtime, EndpointKind endpoint_kind,
    std::uint8_t instance_id, std::uint8_t queue_id,
    std::size_t &room_count) noexcept;
[[nodiscard]] int resource_manager_status_to_errno(McanStatus status) noexcept;

} // namespace evsn::can_drivers::j784_mcan_qnx
