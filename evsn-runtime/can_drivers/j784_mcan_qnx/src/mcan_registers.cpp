#include "evsn/can_drivers/j784_mcan_qnx/mcan_registers.hpp"

namespace evsn::can_drivers::j784_mcan_qnx {
namespace {

constexpr auto kCccrInit = std::uint32_t{1U << 0U};
constexpr auto kCccrConfigChangeEnable = std::uint32_t{1U << 1U};
constexpr auto kCccrTest = std::uint32_t{1U << 7U};
constexpr auto kCccrFdOperationEnable = std::uint32_t{1U << 8U};
constexpr auto kCccrBitRateSwitchEnable = std::uint32_t{1U << 9U};
constexpr auto kTestLoopback = std::uint32_t{1U << 4U};
constexpr auto kInterruptRxFifo0NewMessage = std::uint32_t{1U << 0U};
constexpr auto kInterruptRxFifo0Full = std::uint32_t{1U << 2U};
constexpr auto kInterruptRxFifo0MessageLost = std::uint32_t{1U << 3U};
constexpr auto kInterruptTxComplete = std::uint32_t{1U << 9U};
constexpr auto kInterruptMessageRamAccessFailure = std::uint32_t{1U << 17U};
constexpr auto kInterruptBitErrorCorrected = std::uint32_t{1U << 20U};
constexpr auto kInterruptBitErrorUncorrected = std::uint32_t{1U << 21U};
constexpr auto kInterruptErrorLoggingOverflow = std::uint32_t{1U << 22U};
constexpr auto kInterruptErrorPassive = std::uint32_t{1U << 23U};
constexpr auto kInterruptWarning = std::uint32_t{1U << 24U};
constexpr auto kInterruptBusOff = std::uint32_t{1U << 25U};
constexpr auto kInterruptProtocolArbitration = std::uint32_t{1U << 27U};
constexpr auto kInterruptProtocolData = std::uint32_t{1U << 28U};
constexpr auto kInterruptLine0Enable = std::uint32_t{1U << 0U};
constexpr auto kExtendedIdMaskAllBits = std::uint32_t{0x1FFFFFFFU};

[[nodiscard]] std::uint32_t mode_bits(const McanCoreConfig &config) noexcept {
  auto bits = std::uint32_t{0U};
  if (config.controller.mode == McanMode::fd) {
    bits |= kCccrFdOperationEnable;
  }
  if (config.controller.brs_enabled) {
    bits |= kCccrBitRateSwitchEnable;
  }
  if (config.internal_loopback) {
    bits |= kCccrTest;
  }
  return bits;
}

[[nodiscard]] bool actual_value_fits(const std::uint32_t value,
                                     const std::uint32_t max_actual) noexcept {
  return value >= 1U && value <= max_actual;
}

} // namespace

std::uint32_t mcan_rx_tx_error_interrupt_mask() noexcept {
  return kInterruptRxFifo0NewMessage | kInterruptRxFifo0Full |
         kInterruptRxFifo0MessageLost | kInterruptTxComplete |
         kInterruptMessageRamAccessFailure | kInterruptBitErrorCorrected |
         kInterruptBitErrorUncorrected | kInterruptErrorLoggingOverflow |
         kInterruptErrorPassive | kInterruptWarning | kInterruptBusOff |
         kInterruptProtocolArbitration | kInterruptProtocolData;
}

McanStatus encode_nominal_bit_timing_register(
    const McanBitTiming &timing, std::uint32_t &encoded) noexcept {
  encoded = 0U;
  const auto status = validate_bit_timing(timing);
  if (!status_ok(status)) {
    return status;
  }
  if (!actual_value_fits(timing.prescaler, 512U) ||
      !actual_value_fits(timing.time_segment_before_sample, 256U) ||
      !actual_value_fits(timing.time_segment_after_sample, 128U) ||
      !actual_value_fits(timing.sync_jump_width, 128U)) {
    return McanStatus::invalid_bitrate;
  }
  encoded =
      ((static_cast<std::uint32_t>(timing.time_segment_after_sample) - 1U)
       << 0U) |
      ((static_cast<std::uint32_t>(timing.time_segment_before_sample) - 1U)
       << 8U) |
      ((static_cast<std::uint32_t>(timing.prescaler) - 1U) << 16U) |
      ((static_cast<std::uint32_t>(timing.sync_jump_width) - 1U) << 25U);
  return McanStatus::ok;
}

McanStatus encode_data_bit_timing_register(
    const McanBitTiming &timing, std::uint32_t &encoded) noexcept {
  encoded = 0U;
  const auto status = validate_bit_timing(timing);
  if (!status_ok(status)) {
    return status;
  }
  if (!actual_value_fits(timing.prescaler, 32U) ||
      !actual_value_fits(timing.time_segment_before_sample, 32U) ||
      !actual_value_fits(timing.time_segment_after_sample, 16U) ||
      !actual_value_fits(timing.sync_jump_width, 16U)) {
    return McanStatus::invalid_bitrate;
  }
  encoded =
      ((static_cast<std::uint32_t>(timing.sync_jump_width) - 1U) << 0U) |
      ((static_cast<std::uint32_t>(timing.time_segment_after_sample) - 1U)
       << 4U) |
      ((static_cast<std::uint32_t>(timing.time_segment_before_sample) - 1U)
       << 8U) |
      ((static_cast<std::uint32_t>(timing.prescaler) - 1U) << 16U);
  return McanStatus::ok;
}

McanStatus make_mcan_register_programming(
    const McanCoreConfig &config,
    McanRegisterProgramming &programming) noexcept {
  programming = McanRegisterProgramming{};
  const auto skeleton_status = validate_controller_skeleton(config.controller);
  if (!status_ok(skeleton_status)) {
    return skeleton_status;
  }
  auto status =
      encode_nominal_bit_timing_register(config.nominal_timing,
                                         programming.nominal_bit_timing);
  if (!status_ok(status)) {
    return status;
  }
  if (config.nominal_timing.bitrate != config.controller.arbitration_bitrate) {
    return McanStatus::invalid_bitrate;
  }

  const auto mode = mode_bits(config);
  if (config.controller.mode == McanMode::fd) {
    status =
        encode_data_bit_timing_register(config.data_timing,
                                        programming.data_bit_timing);
    if (!status_ok(status)) {
      return status;
    }
    if (config.data_timing.bitrate != config.controller.data_bitrate) {
      return McanStatus::invalid_bitrate;
    }
  }

  programming.cccr_config = kCccrInit | kCccrConfigChangeEnable | mode;
  programming.cccr_run = mode;
  programming.test = config.internal_loopback ? kTestLoopback : 0U;
  programming.interrupt_enable =
      config.interrupts_enabled ? mcan_rx_tx_error_interrupt_mask() : 0U;
  programming.interrupt_line_enable =
      config.interrupts_enabled ? kInterruptLine0Enable : 0U;
  programming.global_filter_config = 0U;
  programming.extended_id_mask = kExtendedIdMaskAllBits;
  return McanStatus::ok;
}

} // namespace evsn::can_drivers::j784_mcan_qnx
