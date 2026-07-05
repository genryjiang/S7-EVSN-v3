#include "evsn/can_drivers/j784_mcan_qnx/config.hpp"

#include <limits>

namespace evsn::can_drivers::j784_mcan_qnx {
namespace {

constexpr auto kMcanConfigMinimumTimeQuanta = std::uint16_t{8U};
constexpr auto kMcanConfigMaximumTimeQuanta = std::uint16_t{80U};
constexpr auto kMcanConfigPrescalerMax = std::uint16_t{0x03FFU};
constexpr auto kMcanConfigTimingSegmentMax = std::uint8_t{0x7FU};

[[nodiscard]] bool
queue_capacity_is_valid(const std::uint16_t capacity) noexcept {
  return capacity != 0U && capacity <= kMaxQueueCapacity;
}

[[nodiscard]] bool
arbitration_bitrate_is_valid(const McanControllerConfig &config) noexcept {
  if (!config.arbitration_bitrate_valid) {
    return false;
  }
  return config.arbitration_bitrate >= kMinArbitrationBitrate &&
         config.arbitration_bitrate <= kMaxArbitrationBitrate;
}

[[nodiscard]] bool
data_bitrate_is_valid(const McanControllerConfig &config) noexcept {
  if (!config.data_bitrate_valid) {
    return false;
  }
  return config.data_bitrate >= kMinArbitrationBitrate &&
         config.data_bitrate <= kMaxDataBitrate;
}

[[nodiscard]] bool timing_override_shape_is_valid(
    const McanControllerTimingOverride &timing) noexcept {
  if (!timing.valid) {
    return true;
  }
  if (timing.source_clock_hz == 0U || timing.bitrate == 0U ||
      timing.prescaler == 0U || timing.time_segment_before_sample == 0U ||
      timing.time_segment_after_sample == 0U || timing.sync_jump_width == 0U) {
    return false;
  }
  if (timing.prescaler > kMcanConfigPrescalerMax ||
      timing.time_segment_before_sample > kMcanConfigTimingSegmentMax ||
      timing.time_segment_after_sample > kMcanConfigTimingSegmentMax ||
      timing.sync_jump_width > kMcanConfigTimingSegmentMax ||
      timing.sync_jump_width > timing.time_segment_after_sample) {
    return false;
  }
  const auto time_quanta =
      static_cast<std::uint16_t>(1U + timing.time_segment_before_sample +
                                 timing.time_segment_after_sample);
  if (time_quanta < kMcanConfigMinimumTimeQuanta ||
      time_quanta > kMcanConfigMaximumTimeQuanta) {
    return false;
  }
  const auto expected_clock = static_cast<std::uint64_t>(timing.bitrate) *
                              timing.prescaler * time_quanta;
  return expected_clock == timing.source_clock_hz;
}

[[nodiscard]] bool
hardware_evidence_is_complete(const McanControllerConfig &config) noexcept {
  return config.qnx_direct_ownership_confirmed &&
         config.board_mapping_evidence_confirmed &&
         config.target_startup_evidence_confirmed &&
         config.transceiver_control_evidence_confirmed &&
         config.qnx_irq_routing_evidence_confirmed &&
         config.external_can_bench_evidence_confirmed;
}

[[nodiscard]] bool
interrupt_evidence_is_complete(const McanControllerConfig &config) noexcept {
  return hardware_evidence_is_complete(config) &&
         config.hardware.qnx_logical_irq != 0U;
}

[[nodiscard]] bool region_overflows(const McanHardwareRegion &region) noexcept {
  return region.base_address >
         std::numeric_limits<std::uint64_t>::max() -
             static_cast<std::uint64_t>(region.size_bytes);
}

[[nodiscard]] bool regions_overlap(const McanHardwareRegion &lhs,
                                   const McanHardwareRegion &rhs) noexcept {
  const auto lhs_end =
      lhs.base_address + static_cast<std::uint64_t>(lhs.size_bytes);
  const auto rhs_end =
      rhs.base_address + static_cast<std::uint64_t>(rhs.size_bytes);
  return lhs.base_address < rhs_end && rhs.base_address < lhs_end;
}

[[nodiscard]] McanStatus append_char(McanEndpointPath &path,
                                     std::size_t &offset,
                                     const char value) noexcept {
  if (offset + 1U >= path.value.size()) {
    path.value.fill('\0');
    return McanStatus::path_too_long;
  }
  path.value[offset] = value;
  ++offset;
  path.value[offset] = '\0';
  return McanStatus::ok;
}

[[nodiscard]] McanStatus append_literal(McanEndpointPath &path,
                                        std::size_t &offset,
                                        const char *literal) noexcept {
  if (literal == nullptr) {
    return McanStatus::invalid_argument;
  }
  auto index = std::size_t{0U};
  while (literal[index] != '\0') {
    const auto status = append_char(path, offset, literal[index]);
    if (!status_ok(status)) {
      return status;
    }
    ++index;
  }
  return McanStatus::ok;
}

[[nodiscard]] McanStatus append_digit(McanEndpointPath &path,
                                      std::size_t &offset,
                                      const std::uint8_t value) noexcept {
  if (value > 9U) {
    return McanStatus::invalid_argument;
  }
  return append_char(path, offset, static_cast<char>('0' + value));
}

[[nodiscard]] bool
duplicate_controller_exists(const McanResourceManagerConfig &config,
                            const std::size_t current_index) noexcept {
  for (auto index = std::size_t{0U}; index < current_index; ++index) {
    if (config.controllers[index].instance_id ==
        config.controllers[current_index].instance_id) {
      return true;
    }
  }
  return false;
}

} // namespace

bool status_ok(const McanStatus status) noexcept {
  return status == McanStatus::ok;
}

const char *status_name(const McanStatus status) noexcept {
  switch (status) {
  case McanStatus::ok:
    return "ok";
  case McanStatus::invalid_argument:
    return "invalid_argument";
  case McanStatus::path_too_long:
    return "path_too_long";
  case McanStatus::invalid_instance:
    return "invalid_instance";
  case McanStatus::invalid_queue:
    return "invalid_queue";
  case McanStatus::duplicate_instance:
    return "duplicate_instance";
  case McanStatus::too_many_controllers:
    return "too_many_controllers";
  case McanStatus::invalid_queue_capacity:
    return "invalid_queue_capacity";
  case McanStatus::invalid_bitrate:
    return "invalid_bitrate";
  case McanStatus::unsupported_mode:
    return "unsupported_mode";
  case McanStatus::missing_hardware_evidence:
    return "missing_hardware_evidence";
  case McanStatus::invalid_state:
    return "invalid_state";
  case McanStatus::rx_queue_full:
    return "rx_queue_full";
  case McanStatus::tx_queue_full:
    return "tx_queue_full";
  case McanStatus::no_frame:
    return "no_frame";
  case McanStatus::invalid_hardware_mapping:
    return "invalid_hardware_mapping";
  case McanStatus::hardware_access_unavailable:
    return "hardware_access_unavailable";
  }
  return "unknown";
}

McanStatus make_endpoint_path(const EndpointKind kind,
                              const std::uint8_t instance_id,
                              const std::uint8_t queue_id,
                              McanEndpointPath &path) noexcept {
  path.value.fill('\0');
  if (instance_id >= kMaxControllers) {
    return McanStatus::invalid_instance;
  }
  if (queue_id >= kMaxQueuesPerController) {
    return McanStatus::invalid_queue;
  }

  auto offset = std::size_t{0U};
  auto status = append_literal(path, offset, "/dev/evsn/mcan");
  if (!status_ok(status)) {
    return status;
  }
  status = append_digit(path, offset, instance_id);
  if (!status_ok(status)) {
    return status;
  }
  status = append_char(path, offset, '/');
  if (!status_ok(status)) {
    return status;
  }
  if (kind == EndpointKind::ctl) {
    if (queue_id != 0U) {
      return McanStatus::invalid_queue;
    }
    return append_literal(path, offset, "ctl");
  }
  status = append_literal(path, offset, kind == EndpointKind::rx ? "rx" : "tx");
  if (!status_ok(status)) {
    return status;
  }
  return append_digit(path, offset, queue_id);
}

McanStatus
validate_controller_skeleton(const McanControllerConfig &config) noexcept {
  if (config.instance_id >= kMaxControllers) {
    return McanStatus::invalid_instance;
  }
  if (config.rx_queue_count == 0U ||
      config.rx_queue_count > kMaxQueuesPerController ||
      config.tx_queue_count == 0U ||
      config.tx_queue_count > kMaxQueuesPerController) {
    return McanStatus::invalid_queue;
  }
  if (!queue_capacity_is_valid(config.rx_queue_capacity) ||
      !queue_capacity_is_valid(config.tx_queue_capacity)) {
    return McanStatus::invalid_queue_capacity;
  }
  if (!arbitration_bitrate_is_valid(config)) {
    return McanStatus::invalid_bitrate;
  }
  if (!timing_override_shape_is_valid(config.nominal_timing_override) ||
      (config.nominal_timing_override.valid &&
       config.nominal_timing_override.bitrate != config.arbitration_bitrate)) {
    return McanStatus::invalid_bitrate;
  }
  if (config.mode == McanMode::classic) {
    if (config.brs_enabled || config.data_bitrate_valid ||
        config.data_timing_override.valid) {
      return McanStatus::unsupported_mode;
    }
    return McanStatus::ok;
  }
  if (config.mode != McanMode::fd) {
    return McanStatus::unsupported_mode;
  }
  if (!data_bitrate_is_valid(config)) {
    return McanStatus::invalid_bitrate;
  }
  if (!timing_override_shape_is_valid(config.data_timing_override) ||
      (config.data_timing_override.valid &&
       config.data_timing_override.bitrate != config.data_bitrate)) {
    return McanStatus::invalid_bitrate;
  }
  return McanStatus::ok;
}

McanStatus
validate_hardware_region(const McanHardwareRegion &region,
                         const std::uint32_t minimum_size_bytes) noexcept {
  if (region.base_address == 0U || region.size_bytes < minimum_size_bytes ||
      (region.base_address & 0x3ULL) != 0U ||
      (region.size_bytes & 0x3U) != 0U || region_overflows(region)) {
    return McanStatus::invalid_hardware_mapping;
  }
  return McanStatus::ok;
}

McanStatus validate_controller_hardware_mapping(
    const McanControllerConfig &config) noexcept {
  auto status = validate_hardware_region(config.hardware.subsystem, 0x100U);
  if (!status_ok(status)) {
    return status;
  }
  status = validate_hardware_region(config.hardware.controller, 0x200U);
  if (!status_ok(status)) {
    return status;
  }
  status = validate_hardware_region(config.hardware.message_ram, 0x800U);
  if (!status_ok(status)) {
    return status;
  }
  if (config.hardware.tisci_device_id == 0U ||
      regions_overlap(config.hardware.subsystem, config.hardware.controller) ||
      regions_overlap(config.hardware.subsystem, config.hardware.message_ram) ||
      regions_overlap(config.hardware.controller,
                      config.hardware.message_ram)) {
    return McanStatus::invalid_hardware_mapping;
  }
  return McanStatus::ok;
}

McanStatus validate_controller_for_hardware_start(
    const McanControllerConfig &config) noexcept {
  const auto skeleton_status = validate_controller_skeleton(config);
  if (!status_ok(skeleton_status)) {
    return skeleton_status;
  }
  if (!hardware_evidence_is_complete(config)) {
    return McanStatus::missing_hardware_evidence;
  }
  const auto mapping_status = validate_controller_hardware_mapping(config);
  if (!status_ok(mapping_status)) {
    return mapping_status;
  }
  return McanStatus::ok;
}

McanStatus validate_controller_for_interrupt_start(
    const McanControllerConfig &config) noexcept {
  const auto skeleton_status = validate_controller_skeleton(config);
  if (!status_ok(skeleton_status)) {
    return skeleton_status;
  }
  if (!interrupt_evidence_is_complete(config)) {
    return McanStatus::missing_hardware_evidence;
  }
  const auto mapping_status = validate_controller_hardware_mapping(config);
  if (!status_ok(mapping_status)) {
    return mapping_status;
  }
  return McanStatus::ok;
}

McanStatus validate_resource_manager_skeleton(
    const McanResourceManagerConfig &config) noexcept {
  if (config.controller_count == 0U) {
    return McanStatus::invalid_argument;
  }
  if (config.controller_count > config.controllers.size()) {
    return McanStatus::too_many_controllers;
  }
  for (auto index = std::size_t{0U}; index < config.controller_count; ++index) {
    const auto status = validate_controller_skeleton(config.controllers[index]);
    if (!status_ok(status)) {
      return status;
    }
    if (duplicate_controller_exists(config, index)) {
      return McanStatus::duplicate_instance;
    }
  }
  return McanStatus::ok;
}

McanStatus validate_resource_manager_hardware_start(
    const McanResourceManagerConfig &config) noexcept {
  const auto skeleton_status = validate_resource_manager_skeleton(config);
  if (!status_ok(skeleton_status)) {
    return skeleton_status;
  }
  for (auto index = std::size_t{0U}; index < config.controller_count; ++index) {
    const auto status =
        validate_controller_for_hardware_start(config.controllers[index]);
    if (!status_ok(status)) {
      return status;
    }
  }
  return McanStatus::ok;
}

McanStatus validate_resource_manager_interrupt_start(
    const McanResourceManagerConfig &config) noexcept {
  const auto skeleton_status = validate_resource_manager_skeleton(config);
  if (!status_ok(skeleton_status)) {
    return skeleton_status;
  }
  for (auto index = std::size_t{0U}; index < config.controller_count; ++index) {
    const auto status =
        validate_controller_for_interrupt_start(config.controllers[index]);
    if (!status_ok(status)) {
      return status;
    }
  }
  return McanStatus::ok;
}

} // namespace evsn::can_drivers::j784_mcan_qnx
