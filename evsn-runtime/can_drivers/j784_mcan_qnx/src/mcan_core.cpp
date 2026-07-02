#include "evsn/can_drivers/j784_mcan_qnx/mcan_core.hpp"

namespace evsn::can_drivers::j784_mcan_qnx {
namespace {

constexpr auto kControlInit = std::uint32_t{1U << 0U};
constexpr auto kControlStarted = std::uint32_t{1U << 1U};
constexpr auto kControlLoopback = std::uint32_t{1U << 2U};
constexpr auto kStatusConfigured = std::uint32_t{1U << 0U};
constexpr auto kStatusStarted = std::uint32_t{1U << 1U};
constexpr auto kInterruptRxReady = std::uint32_t{1U << 0U};
constexpr auto kInterruptTxComplete = std::uint32_t{1U << 1U};
constexpr auto kInterruptError = std::uint32_t{1U << 2U};
constexpr auto kPrescalerFieldMax = std::uint16_t{0x03FFU};
constexpr auto kTimingSegmentFieldMax = std::uint8_t{0x7FU};

[[nodiscard]] constexpr std::size_t
register_index(const McanCoreRegister reg) noexcept {
  return static_cast<std::size_t>(reg);
}

[[nodiscard]] std::uint16_t
time_quanta_per_bit(const McanBitTiming &timing) noexcept {
  return static_cast<std::uint16_t>(1U + timing.time_segment_before_sample +
                                    timing.time_segment_after_sample);
}

[[nodiscard]] std::uint32_t
encode_bit_timing(const McanBitTiming &timing) noexcept {
  return static_cast<std::uint32_t>(timing.prescaler) |
         (static_cast<std::uint32_t>(timing.time_segment_before_sample)
          << 10U) |
         (static_cast<std::uint32_t>(timing.time_segment_after_sample) << 17U) |
         (static_cast<std::uint32_t>(timing.sync_jump_width) << 24U);
}

[[nodiscard]] std::uint32_t
enabled_interrupts(const McanCoreConfig &config) noexcept {
  return config.interrupts_enabled
             ? kInterruptRxReady | kInterruptTxComplete | kInterruptError
             : 0U;
}

[[nodiscard]] McanControllerCapabilities
make_capabilities(const McanCoreConfig &config) noexcept {
  auto capabilities = McanControllerCapabilities{};
  capabilities.flags = capability_value(McanCapabilityFlag::classic_frame) |
                       capability_value(McanCapabilityFlag::extended_id);
  if (config.controller.mode == McanMode::fd) {
    capabilities.flags |= capability_value(McanCapabilityFlag::fd_frame) |
                          capability_value(McanCapabilityFlag::esi);
  }
  if (config.controller.brs_enabled) {
    capabilities.flags |= capability_value(McanCapabilityFlag::brs);
  }
  if (config.internal_loopback) {
    capabilities.flags |=
        capability_value(McanCapabilityFlag::internal_loopback);
  }
  capabilities.max_rx_queues = config.controller.rx_queue_count;
  capabilities.max_tx_queues = config.controller.tx_queue_count;
  capabilities.max_rx_queue_capacity = config.controller.rx_queue_capacity;
  capabilities.max_tx_queue_capacity = config.controller.tx_queue_capacity;
  return capabilities;
}

[[nodiscard]] bool rx_has_room(const McanCore &core) noexcept {
  return core.rx_count < core.config.controller.rx_queue_capacity;
}

[[nodiscard]] bool tx_has_room(const McanCore &core) noexcept {
  return core.tx_count < core.config.controller.tx_queue_capacity;
}

[[nodiscard]] std::size_t next_index(const std::size_t index,
                                     const std::size_t capacity) noexcept {
  return (index + 1U) == capacity ? 0U : index + 1U;
}

void refresh_fifo_registers(McanCore &core) noexcept {
  core.registers[register_index(McanCoreRegister::rx_fifo_status)] =
      static_cast<std::uint32_t>(core.rx_count);
  core.registers[register_index(McanCoreRegister::tx_fifo_status)] =
      static_cast<std::uint32_t>(core.tx_count);
}

void reset_runtime_state(McanCore &core) noexcept {
  core.registers.fill(0U);
  core.rx_queue.fill(McanFrameTransfer{});
  core.tx_queue.fill(McanFrameTransfer{});
  core.diagnostics = McanDiagnosticsSnapshot{};
  core.rx_head = 0U;
  core.rx_tail = 0U;
  core.rx_count = 0U;
  core.tx_head = 0U;
  core.tx_tail = 0U;
  core.tx_count = 0U;
}

void enqueue_rx(McanCore &core, const McanFrameTransfer &frame) noexcept {
  core.rx_queue[core.rx_head] = frame;
  core.rx_head =
      next_index(core.rx_head, core.config.controller.rx_queue_capacity);
  ++core.rx_count;
  ++core.diagnostics.rx_frames;
}

void enqueue_tx(McanCore &core, const McanFrameTransfer &frame) noexcept {
  core.tx_queue[core.tx_head] = frame;
  core.tx_head =
      next_index(core.tx_head, core.config.controller.tx_queue_capacity);
  ++core.tx_count;
  ++core.diagnostics.tx_frames;
}

[[nodiscard]] McanStatus ready_for_frame_io(const McanCore &core) noexcept {
  return core.state == McanCoreState::started ? McanStatus::ok
                                              : McanStatus::invalid_state;
}

[[nodiscard]] McanStatus
validate_runtime_frame(const McanCore &core,
                       const McanFrameTransfer &frame) noexcept {
  const auto status = validate_frame_transfer(frame, core.capabilities);
  if (!status_ok(status)) {
    return status;
  }
  return McanStatus::ok;
}

} // namespace

McanBitTiming make_classic_500k_timing() noexcept {
  return McanBitTiming{80'000'000U, 500'000U, 10U, 13U, 2U, 1U};
}

McanBitTiming make_fd_2m_timing() noexcept {
  return McanBitTiming{80'000'000U, 2'000'000U, 4U, 7U, 2U, 1U};
}

McanStatus validate_bit_timing(const McanBitTiming &timing) noexcept {
  if (timing.source_clock_hz == 0U || timing.bitrate == 0U ||
      timing.prescaler == 0U || timing.time_segment_before_sample == 0U ||
      timing.time_segment_after_sample == 0U || timing.sync_jump_width == 0U) {
    return McanStatus::invalid_bitrate;
  }
  if (timing.prescaler > kPrescalerFieldMax ||
      timing.time_segment_before_sample > kTimingSegmentFieldMax ||
      timing.time_segment_after_sample > kTimingSegmentFieldMax ||
      timing.sync_jump_width > kTimingSegmentFieldMax) {
    return McanStatus::invalid_bitrate;
  }
  if (timing.sync_jump_width > timing.time_segment_after_sample) {
    return McanStatus::invalid_bitrate;
  }
  const auto tq_per_bit = time_quanta_per_bit(timing);
  if (tq_per_bit < kMcanCoreMinimumTimeQuanta ||
      tq_per_bit > kMcanCoreMaximumTimeQuanta) {
    return McanStatus::invalid_bitrate;
  }
  const auto expected_clock = static_cast<std::uint64_t>(timing.bitrate) *
                              timing.prescaler * tq_per_bit;
  if (expected_clock != timing.source_clock_hz) {
    return McanStatus::invalid_bitrate;
  }
  return McanStatus::ok;
}

McanStatus initialize_core(McanCore &core,
                           const McanCoreConfig &config) noexcept {
  const auto config_status = validate_controller_skeleton(config.controller);
  if (!status_ok(config_status)) {
    return config_status;
  }
  auto timing_status = validate_bit_timing(config.nominal_timing);
  if (!status_ok(timing_status)) {
    return timing_status;
  }
  if (config.nominal_timing.bitrate != config.controller.arbitration_bitrate) {
    return McanStatus::invalid_bitrate;
  }
  if (config.controller.mode == McanMode::fd) {
    timing_status = validate_bit_timing(config.data_timing);
    if (!status_ok(timing_status)) {
      return timing_status;
    }
    if (config.data_timing.bitrate != config.controller.data_bitrate) {
      return McanStatus::invalid_bitrate;
    }
  }
  reset_runtime_state(core);
  core.config = config;
  core.capabilities = make_capabilities(config);
  core.registers[register_index(McanCoreRegister::control)] =
      kControlInit | (config.internal_loopback ? kControlLoopback : 0U);
  core.registers[register_index(McanCoreRegister::status)] = kStatusConfigured;
  core.registers[register_index(McanCoreRegister::nominal_timing)] =
      encode_bit_timing(config.nominal_timing);
  core.registers[register_index(McanCoreRegister::data_timing)] =
      config.controller.mode == McanMode::fd
          ? encode_bit_timing(config.data_timing)
          : 0U;
  core.registers[register_index(McanCoreRegister::interrupt_enable)] =
      enabled_interrupts(config);
  refresh_fifo_registers(core);
  core.state = McanCoreState::configured;
  return McanStatus::ok;
}

McanStatus start_core(McanCore &core) noexcept {
  if (core.state != McanCoreState::configured &&
      core.state != McanCoreState::stopped) {
    return McanStatus::invalid_state;
  }
  core.registers[register_index(McanCoreRegister::control)] &= ~kControlInit;
  core.registers[register_index(McanCoreRegister::control)] |= kControlStarted;
  core.registers[register_index(McanCoreRegister::status)] |= kStatusStarted;
  core.state = McanCoreState::started;
  return McanStatus::ok;
}

McanStatus stop_core(McanCore &core) noexcept {
  if (core.state != McanCoreState::started &&
      core.state != McanCoreState::configured) {
    return McanStatus::invalid_state;
  }
  core.registers[register_index(McanCoreRegister::control)] |= kControlInit;
  core.registers[register_index(McanCoreRegister::control)] &= ~kControlStarted;
  core.registers[register_index(McanCoreRegister::status)] &= ~kStatusStarted;
  core.state = McanCoreState::stopped;
  return McanStatus::ok;
}

McanStatus inject_received_frame(McanCore &core,
                                 const McanFrameTransfer &frame) noexcept {
  auto status = ready_for_frame_io(core);
  if (!status_ok(status)) {
    return status;
  }
  status = validate_runtime_frame(core, frame);
  if (!status_ok(status)) {
    ++core.diagnostics.invalid_frame_rejects;
    return status;
  }
  if (!rx_has_room(core)) {
    ++core.diagnostics.rx_queue_full;
    return McanStatus::rx_queue_full;
  }
  enqueue_rx(core, frame);
  core.registers[register_index(McanCoreRegister::interrupt_status)] |=
      kInterruptRxReady;
  refresh_fifo_registers(core);
  return McanStatus::ok;
}

McanStatus transmit_frame(McanCore &core,
                          const McanFrameTransfer &frame) noexcept {
  auto status = ready_for_frame_io(core);
  if (!status_ok(status)) {
    return status;
  }
  status = validate_runtime_frame(core, frame);
  if (!status_ok(status)) {
    ++core.diagnostics.invalid_frame_rejects;
    return status;
  }
  if (!tx_has_room(core)) {
    ++core.diagnostics.tx_queue_full;
    return McanStatus::tx_queue_full;
  }
  if (core.config.internal_loopback && !rx_has_room(core)) {
    ++core.diagnostics.rx_queue_full;
    return McanStatus::rx_queue_full;
  }
  enqueue_tx(core, frame);
  core.registers[register_index(McanCoreRegister::interrupt_status)] |=
      kInterruptTxComplete;
  if (core.config.internal_loopback) {
    enqueue_rx(core, frame);
    core.registers[register_index(McanCoreRegister::interrupt_status)] |=
        kInterruptRxReady;
  }
  refresh_fifo_registers(core);
  return McanStatus::ok;
}

McanStatus receive_frame(McanCore &core, McanFrameTransfer &frame) noexcept {
  const auto status = ready_for_frame_io(core);
  if (!status_ok(status)) {
    return status;
  }
  if (core.rx_count == 0U) {
    return McanStatus::no_frame;
  }
  frame = core.rx_queue[core.rx_tail];
  core.rx_queue[core.rx_tail] = McanFrameTransfer{};
  core.rx_tail =
      next_index(core.rx_tail, core.config.controller.rx_queue_capacity);
  --core.rx_count;
  refresh_fifo_registers(core);
  if (core.rx_count == 0U) {
    core.registers[register_index(McanCoreRegister::interrupt_status)] &=
        ~kInterruptRxReady;
  }
  return McanStatus::ok;
}

std::uint32_t read_core_register(const McanCore &core,
                                 const McanCoreRegister reg) noexcept {
  return core.registers[register_index(reg)];
}

McanCoreSnapshot snapshot_core(const McanCore &core) noexcept {
  return McanCoreSnapshot{core.state, core.registers, core.diagnostics,
                          core.rx_count, core.tx_count};
}

} // namespace evsn::can_drivers::j784_mcan_qnx
