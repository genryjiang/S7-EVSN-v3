#include "evsn/can_drivers/j784_mcan_qnx/devctl_abi.hpp"

namespace evsn::can_drivers::j784_mcan_qnx {
namespace {

constexpr auto kMaxStandardCanId = std::uint32_t{0x7FFU};
constexpr auto kMaxExtendedCanId = std::uint32_t{0x1FFFFFFFU};
constexpr auto kClassicPayloadCapacity = std::uint8_t{8U};
constexpr auto kFdPayloadLengths = std::array<std::uint8_t, 16U>{
    0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 12U, 16U, 20U, 24U, 32U, 48U, 64U};
constexpr auto kKnownFrameFlags =
    flag_value(McanFrameFlag::extended_id) |
    flag_value(McanFrameFlag::fd_frame) | flag_value(McanFrameFlag::brs) |
    flag_value(McanFrameFlag::esi) | flag_value(McanFrameFlag::remote) |
    flag_value(McanFrameFlag::timestamp_valid);
constexpr auto kKnownCapabilityFlags =
    capability_value(McanCapabilityFlag::classic_frame) |
    capability_value(McanCapabilityFlag::fd_frame) |
    capability_value(McanCapabilityFlag::extended_id) |
    capability_value(McanCapabilityFlag::brs) |
    capability_value(McanCapabilityFlag::esi) |
    capability_value(McanCapabilityFlag::hardware_timestamp) |
    capability_value(McanCapabilityFlag::internal_loopback) |
    capability_value(McanCapabilityFlag::bus_off_recovery);

[[nodiscard]] bool length_from_dlc(const std::uint8_t dlc,
                                   std::uint8_t &length) noexcept {
  if (dlc >= kFdPayloadLengths.size()) {
    return false;
  }
  length = kFdPayloadLengths[dlc];
  return true;
}

[[nodiscard]] bool id_is_valid(const std::uint32_t id,
                               const bool extended_id) noexcept {
  if (extended_id) {
    return id <= kMaxExtendedCanId;
  }
  return id <= kMaxStandardCanId;
}

} // namespace

bool has_flag(const std::uint32_t flags, const McanFrameFlag flag) noexcept {
  return (flags & flag_value(flag)) != 0U;
}

bool has_capability(const std::uint32_t flags,
                    const McanCapabilityFlag flag) noexcept {
  return (flags & capability_value(flag)) != 0U;
}

McanStatus validate_frame_transfer(
    const McanFrameTransfer &frame,
    const McanControllerCapabilities &capabilities) noexcept {
  const auto extended_id = has_flag(frame.flags, McanFrameFlag::extended_id);
  const auto fd_frame = has_flag(frame.flags, McanFrameFlag::fd_frame);
  const auto brs = has_flag(frame.flags, McanFrameFlag::brs);
  const auto esi = has_flag(frame.flags, McanFrameFlag::esi);
  const auto remote = has_flag(frame.flags, McanFrameFlag::remote);
  const auto timestamp_valid =
      has_flag(frame.flags, McanFrameFlag::timestamp_valid);
  if (frame.abi_version != kDevctlAbiVersion ||
      frame.sizeof_struct != kMcanFrameTransferStructSize ||
      frame.reserved != 0U) {
    return McanStatus::invalid_argument;
  }
  if (capabilities.abi_version != kDevctlAbiVersion ||
      capabilities.sizeof_struct != kMcanControllerCapabilitiesStructSize ||
      capabilities.reserved != 0U ||
      (capabilities.flags & ~kKnownCapabilityFlags) != 0U) {
    return McanStatus::invalid_argument;
  }
  if ((frame.flags & ~kKnownFrameFlags) != 0U) {
    return McanStatus::invalid_argument;
  }
  if (timestamp_valid &&
      !has_capability(capabilities.flags,
                      McanCapabilityFlag::hardware_timestamp)) {
    return McanStatus::unsupported_mode;
  }
  if (!timestamp_valid && frame.monotonic_timestamp_ns != 0U) {
    return McanStatus::invalid_argument;
  }
  if (!id_is_valid(frame.can_id, extended_id)) {
    return McanStatus::invalid_argument;
  }
  if (extended_id &&
      !has_capability(capabilities.flags, McanCapabilityFlag::extended_id)) {
    return McanStatus::unsupported_mode;
  }
  if (remote) {
    if (fd_frame || brs || esi || frame.length != 0U ||
        frame.dlc > kClassicPayloadCapacity) {
      return McanStatus::invalid_argument;
    }
    return has_capability(capabilities.flags, McanCapabilityFlag::classic_frame)
               ? McanStatus::ok
               : McanStatus::unsupported_mode;
  }
  if (!fd_frame) {
    if (!has_capability(capabilities.flags,
                        McanCapabilityFlag::classic_frame)) {
      return McanStatus::unsupported_mode;
    }
    if (brs || esi || frame.dlc > kClassicPayloadCapacity ||
        frame.length != frame.dlc) {
      return McanStatus::invalid_argument;
    }
    return McanStatus::ok;
  }
  if (!has_capability(capabilities.flags, McanCapabilityFlag::fd_frame)) {
    return McanStatus::unsupported_mode;
  }
  if (brs && !has_capability(capabilities.flags, McanCapabilityFlag::brs)) {
    return McanStatus::unsupported_mode;
  }
  if (esi && !has_capability(capabilities.flags, McanCapabilityFlag::esi)) {
    return McanStatus::unsupported_mode;
  }

  auto expected_length = std::uint8_t{0U};
  if (!length_from_dlc(frame.dlc, expected_length) ||
      frame.length != expected_length) {
    return McanStatus::invalid_argument;
  }
  return McanStatus::ok;
}

} // namespace evsn::can_drivers::j784_mcan_qnx
