#include "evsn/can_drivers/j784_mcan_qnx/resource_manager_runtime.hpp"

#include <cerrno>

namespace evsn::can_drivers::j784_mcan_qnx {
namespace {

constexpr auto kMcanRuntimeSourceClockHz = std::uint32_t{80'000'000U};
constexpr auto kMcanRuntimePrescalerMax = std::uint32_t{0x03FFU};
constexpr auto kMcanRuntimeTimingSegmentMax = std::uint32_t{0x7FU};

[[nodiscard]] bool endpoint_queue_valid(const McanControllerConfig &controller,
                                        const EndpointKind endpoint_kind,
                                        const std::uint8_t queue_id) noexcept {
  if (endpoint_kind == EndpointKind::rx) {
    return queue_id < controller.rx_queue_count;
  }
  if (endpoint_kind == EndpointKind::tx) {
    return queue_id < controller.tx_queue_count;
  }
  return queue_id == 0U;
}

[[nodiscard]] McanStatus find_controller_index(
    const McanResourceManagerRuntime &runtime, const std::uint8_t instance_id,
    std::size_t &controller_index) noexcept {
  if (!runtime.initialized) {
    return McanStatus::invalid_state;
  }
  for (auto index = std::size_t{0U}; index < runtime.controller_count;
       ++index) {
    if (runtime.config.controllers[index].instance_id == instance_id) {
      controller_index = index;
      return McanStatus::ok;
    }
  }
  return McanStatus::invalid_instance;
}

[[nodiscard]] McanStatus validate_endpoint(
    const McanResourceManagerRuntime &runtime, const EndpointKind endpoint_kind,
    const std::uint8_t instance_id, const std::uint8_t queue_id,
    std::size_t &controller_index) noexcept {
  auto status = find_controller_index(runtime, instance_id, controller_index);
  if (!status_ok(status)) {
    return status;
  }
  const auto &controller = runtime.config.controllers[controller_index];
  if (!endpoint_queue_valid(controller, endpoint_kind, queue_id)) {
    return McanStatus::invalid_queue;
  }
  return McanStatus::ok;
}

[[nodiscard]] McanStatus derive_exact_bit_timing(
    const std::uint32_t bitrate, McanBitTiming &timing) noexcept {
  timing = McanBitTiming{};
  if (bitrate == 0U) {
    return McanStatus::invalid_bitrate;
  }

  auto tq = static_cast<std::uint32_t>(kMcanCoreMaximumTimeQuanta);
  while (tq >= static_cast<std::uint32_t>(kMcanCoreMinimumTimeQuanta)) {
    const auto denominator = static_cast<std::uint64_t>(bitrate) * tq;
    if (denominator != 0U &&
        (static_cast<std::uint64_t>(kMcanRuntimeSourceClockHz) %
         denominator) == 0U) {
      const auto prescaler =
          static_cast<std::uint32_t>(kMcanRuntimeSourceClockHz) /
          static_cast<std::uint32_t>(denominator);
      const auto sample_quantum = (tq * 4U) / 5U;
      if (prescaler >= 1U && prescaler <= kMcanRuntimePrescalerMax &&
          sample_quantum > 1U && sample_quantum < tq) {
        const auto before_sample = sample_quantum - 1U;
        const auto after_sample = tq - sample_quantum;
        if (before_sample >= 1U &&
            before_sample <= kMcanRuntimeTimingSegmentMax &&
            after_sample >= 1U &&
            after_sample <= kMcanRuntimeTimingSegmentMax) {
          timing.source_clock_hz = kMcanRuntimeSourceClockHz;
          timing.bitrate = bitrate;
          timing.prescaler = static_cast<std::uint16_t>(prescaler);
          timing.time_segment_before_sample =
              static_cast<std::uint8_t>(before_sample);
          timing.time_segment_after_sample =
              static_cast<std::uint8_t>(after_sample);
          timing.sync_jump_width = 1U;
          return McanStatus::ok;
        }
      }
    }
    --tq;
  }
  return McanStatus::invalid_bitrate;
}

void copy_timing_override(const McanControllerTimingOverride &source,
                          McanBitTiming &target) noexcept {
  target.source_clock_hz = source.source_clock_hz;
  target.bitrate = source.bitrate;
  target.prescaler = source.prescaler;
  target.time_segment_before_sample = source.time_segment_before_sample;
  target.time_segment_after_sample = source.time_segment_after_sample;
  target.sync_jump_width = source.sync_jump_width;
}

[[nodiscard]] bool controller_queue_model_supported(
    const McanControllerConfig &controller) noexcept {
  return controller.rx_queue_count == 1U && controller.tx_queue_count == 1U;
}

} // namespace

McanStatus initialize_resource_manager_runtime(
    McanResourceManagerRuntime &runtime,
    const McanResourceManagerConfig &config,
    const McanResourceManagerRuntimeOptions &options) noexcept {
  const auto config_status = validate_resource_manager_skeleton(config);
  if (!status_ok(config_status)) {
    return config_status;
  }

  auto next_runtime = McanResourceManagerRuntime{};
  next_runtime.config = config;
  next_runtime.options = options;
  next_runtime.controller_count = config.controller_count;

  for (auto index = std::size_t{0U}; index < config.controller_count; ++index) {
    if (!controller_queue_model_supported(config.controllers[index])) {
      return McanStatus::unsupported_mode;
    }

    auto core_config = McanCoreConfig{};
    core_config.controller = config.controllers[index];
    auto status = McanStatus::ok;
    if (config.controllers[index].nominal_timing_override.valid) {
      copy_timing_override(config.controllers[index].nominal_timing_override,
                           core_config.nominal_timing);
    } else {
      status = derive_exact_bit_timing(
          config.controllers[index].arbitration_bitrate,
          core_config.nominal_timing);
    }
    if (!status_ok(status)) {
      return status;
    }
    if (config.controllers[index].mode == McanMode::fd) {
      if (config.controllers[index].data_timing_override.valid) {
        copy_timing_override(config.controllers[index].data_timing_override,
                             core_config.data_timing);
      } else {
        status = derive_exact_bit_timing(config.controllers[index].data_bitrate,
                                         core_config.data_timing);
      }
      if (!status_ok(status)) {
        return status;
      }
    }
    core_config.internal_loopback = options.internal_loopback;
    core_config.interrupts_enabled = options.interrupts_enabled;

    status = initialize_core(next_runtime.cores[index], core_config);
    if (!status_ok(status)) {
      return status;
    }
    status = start_core(next_runtime.cores[index]);
    if (!status_ok(status)) {
      return status;
    }
  }

  next_runtime.initialized = true;
  runtime = next_runtime;
  return McanStatus::ok;
}

McanStatus resource_manager_transmit_frame(
    McanResourceManagerRuntime &runtime, const EndpointKind endpoint_kind,
    const std::uint8_t instance_id, const std::uint8_t queue_id,
    const McanFrameTransfer &frame) noexcept {
  if (!runtime.options.frame_io_enabled) {
    return McanStatus::hardware_access_unavailable;
  }
  if (endpoint_kind != EndpointKind::tx) {
    return McanStatus::invalid_argument;
  }
  auto controller_index = std::size_t{0U};
  auto status = validate_endpoint(runtime, endpoint_kind, instance_id, queue_id,
                                  controller_index);
  if (!status_ok(status)) {
    return status;
  }
  status = transmit_frame(runtime.cores[controller_index], frame);
  if (!status_ok(status)) {
    return status;
  }
  if (runtime.options.complete_tx_immediately) {
    return complete_transmitted_frame(runtime.cores[controller_index]);
  }
  return McanStatus::ok;
}

McanStatus resource_manager_receive_frame(
    McanResourceManagerRuntime &runtime, const EndpointKind endpoint_kind,
    const std::uint8_t instance_id, const std::uint8_t queue_id,
    McanFrameTransfer &frame) noexcept {
  if (!runtime.options.frame_io_enabled) {
    return McanStatus::hardware_access_unavailable;
  }
  if (endpoint_kind != EndpointKind::rx) {
    return McanStatus::invalid_argument;
  }
  auto controller_index = std::size_t{0U};
  auto status = validate_endpoint(runtime, endpoint_kind, instance_id, queue_id,
                                  controller_index);
  if (!status_ok(status)) {
    return status;
  }
  return receive_frame(runtime.cores[controller_index], frame);
}

McanStatus resource_manager_query_capabilities(
    const McanResourceManagerRuntime &runtime,
    const EndpointKind endpoint_kind, const std::uint8_t instance_id,
    McanControllerCapabilities &capabilities) noexcept {
  if (endpoint_kind != EndpointKind::ctl) {
    return McanStatus::invalid_argument;
  }
  auto controller_index = std::size_t{0U};
  const auto status =
      validate_endpoint(runtime, endpoint_kind, instance_id, 0U,
                        controller_index);
  if (!status_ok(status)) {
    return status;
  }
  capabilities = runtime.cores[controller_index].capabilities;
  return McanStatus::ok;
}

McanStatus resource_manager_query_diagnostics(
    const McanResourceManagerRuntime &runtime,
    const EndpointKind endpoint_kind, const std::uint8_t instance_id,
    McanDiagnosticsSnapshot &diagnostics) noexcept {
  if (endpoint_kind != EndpointKind::ctl) {
    return McanStatus::invalid_argument;
  }
  auto controller_index = std::size_t{0U};
  const auto status =
      validate_endpoint(runtime, endpoint_kind, instance_id, 0U,
                        controller_index);
  if (!status_ok(status)) {
    return status;
  }
  diagnostics = runtime.cores[controller_index].diagnostics;
  return McanStatus::ok;
}

McanStatus resource_manager_rx_ready_count(
    const McanResourceManagerRuntime &runtime,
    const EndpointKind endpoint_kind, const std::uint8_t instance_id,
    const std::uint8_t queue_id, std::size_t &ready_count) noexcept {
  ready_count = 0U;
  if (!runtime.options.frame_io_enabled) {
    return McanStatus::hardware_access_unavailable;
  }
  if (endpoint_kind != EndpointKind::rx) {
    return McanStatus::invalid_argument;
  }
  auto controller_index = std::size_t{0U};
  const auto status = validate_endpoint(runtime, endpoint_kind, instance_id,
                                        queue_id, controller_index);
  if (!status_ok(status)) {
    return status;
  }
  ready_count = runtime.cores[controller_index].rx_count;
  return McanStatus::ok;
}

McanStatus resource_manager_tx_room_count(
    const McanResourceManagerRuntime &runtime,
    const EndpointKind endpoint_kind, const std::uint8_t instance_id,
    const std::uint8_t queue_id, std::size_t &room_count) noexcept {
  room_count = 0U;
  if (!runtime.options.frame_io_enabled) {
    return McanStatus::hardware_access_unavailable;
  }
  if (endpoint_kind != EndpointKind::tx) {
    return McanStatus::invalid_argument;
  }
  auto controller_index = std::size_t{0U};
  const auto status = validate_endpoint(runtime, endpoint_kind, instance_id,
                                        queue_id, controller_index);
  if (!status_ok(status)) {
    return status;
  }
  const auto &core = runtime.cores[controller_index];
  room_count = static_cast<std::size_t>(
      core.config.controller.tx_queue_capacity - core.tx_count);
  return McanStatus::ok;
}

int resource_manager_status_to_errno(const McanStatus status) noexcept {
  switch (status) {
  case McanStatus::ok:
    return 0;
  case McanStatus::path_too_long:
    return ENAMETOOLONG;
  case McanStatus::too_many_controllers:
  case McanStatus::rx_queue_full:
  case McanStatus::tx_queue_full:
    return EOVERFLOW;
  case McanStatus::no_frame:
    return EAGAIN;
  case McanStatus::unsupported_mode:
    return ENOTSUP;
  case McanStatus::invalid_hardware_mapping:
    return ENODEV;
  case McanStatus::hardware_access_unavailable:
    return EACCES;
  case McanStatus::invalid_state:
    return EIO;
  case McanStatus::invalid_argument:
  case McanStatus::invalid_instance:
  case McanStatus::invalid_queue:
  case McanStatus::duplicate_instance:
  case McanStatus::invalid_queue_capacity:
  case McanStatus::invalid_bitrate:
  case McanStatus::missing_hardware_evidence:
    return EINVAL;
  }
  return EINVAL;
}

} // namespace evsn::can_drivers::j784_mcan_qnx
