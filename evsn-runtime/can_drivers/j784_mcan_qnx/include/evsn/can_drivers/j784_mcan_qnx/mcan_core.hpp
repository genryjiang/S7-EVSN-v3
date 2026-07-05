#pragma once

#include "evsn/can_drivers/j784_mcan_qnx/devctl_abi.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace evsn::can_drivers::j784_mcan_qnx {

constexpr auto kMcanCoreRegisterCount = std::size_t{8U};
constexpr auto kMcanCoreMinimumTimeQuanta = std::uint16_t{8U};
constexpr auto kMcanCoreMaximumTimeQuanta = std::uint16_t{80U};

enum class McanCoreState : std::uint8_t {
  reset = 0U,
  configured = 1U,
  started = 2U,
  stopped = 3U,
  faulted = 4U,
};

enum class McanCoreRegister : std::uint8_t {
  control = 0U,
  status = 1U,
  nominal_timing = 2U,
  data_timing = 3U,
  interrupt_enable = 4U,
  interrupt_status = 5U,
  rx_fifo_status = 6U,
  tx_fifo_status = 7U,
};

struct McanBitTiming {
  std::uint32_t source_clock_hz{0U};
  std::uint32_t bitrate{0U};
  std::uint16_t prescaler{0U};
  std::uint8_t time_segment_before_sample{0U};
  std::uint8_t time_segment_after_sample{0U};
  std::uint8_t sync_jump_width{0U};
};

struct McanCoreConfig {
  McanControllerConfig controller{};
  McanBitTiming nominal_timing{};
  McanBitTiming data_timing{};
  bool internal_loopback{false};
  bool interrupts_enabled{false};
};

struct McanCoreSnapshot {
  McanCoreState state{McanCoreState::reset};
  std::array<std::uint32_t, kMcanCoreRegisterCount> registers{};
  McanDiagnosticsSnapshot diagnostics{};
  std::size_t rx_queue_depth{0U};
  std::size_t tx_queue_depth{0U};
};

struct McanCore {
  McanCoreState state{McanCoreState::reset};
  McanCoreConfig config{};
  McanControllerCapabilities capabilities{};
  McanDiagnosticsSnapshot diagnostics{};
  std::array<std::uint32_t, kMcanCoreRegisterCount> registers{};
  std::array<McanFrameTransfer, kMaxQueueCapacity> rx_queue{};
  std::array<McanFrameTransfer, kMaxQueueCapacity> tx_queue{};
  std::size_t rx_head{0U};
  std::size_t rx_tail{0U};
  std::size_t rx_count{0U};
  std::size_t tx_head{0U};
  std::size_t tx_tail{0U};
  std::size_t tx_count{0U};
};

[[nodiscard]] McanBitTiming make_classic_500k_timing() noexcept;
[[nodiscard]] McanBitTiming make_fd_2m_timing() noexcept;
[[nodiscard]] McanStatus
validate_bit_timing(const McanBitTiming &timing) noexcept;
[[nodiscard]] McanStatus initialize_core(McanCore &core,
                                         const McanCoreConfig &config) noexcept;
[[nodiscard]] McanStatus start_core(McanCore &core) noexcept;
[[nodiscard]] McanStatus stop_core(McanCore &core) noexcept;
[[nodiscard]] McanStatus
inject_received_frame(McanCore &core, const McanFrameTransfer &frame) noexcept;
[[nodiscard]] McanStatus
transmit_frame(McanCore &core, const McanFrameTransfer &frame) noexcept;
[[nodiscard]] McanStatus complete_transmitted_frame(McanCore &core) noexcept;
[[nodiscard]] McanStatus receive_frame(McanCore &core,
                                       McanFrameTransfer &frame) noexcept;
[[nodiscard]] std::uint32_t read_core_register(const McanCore &core,
                                               McanCoreRegister reg) noexcept;
[[nodiscard]] McanCoreSnapshot snapshot_core(const McanCore &core) noexcept;

} // namespace evsn::can_drivers::j784_mcan_qnx
