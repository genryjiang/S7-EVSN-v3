#include "evsn/can_drivers/j784_mcan_qnx/qnx_can_compat.hpp"

#include "evsn/can_drivers/j784_mcan_qnx/j784_board_config.hpp"

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <limits>

namespace evsn::can_drivers::j784_mcan_qnx {
namespace {

constexpr auto kQnxCanCompatSourceClockHz = std::uint32_t{80'000'000U};
constexpr auto kQnxCanStandardMidShift = std::uint32_t{18U};
constexpr auto kMaxStandardCanId = std::uint32_t{0x7FFU};
constexpr auto kMaxExtendedCanId = std::uint32_t{0x1FFFFFFFU};
constexpr auto kRawTimingSegmentMax = std::uint32_t{0x7U};
constexpr auto kRawTimingRjwMax = std::uint32_t{0x3U};
constexpr auto kRawTimingPresdivMax = std::uint32_t{0x03FEU};

[[nodiscard]] bool ascii_equal_ci(const char lhs, const char rhs) noexcept {
  auto lhs_norm = lhs;
  auto rhs_norm = rhs;
  if (lhs_norm >= 'a' && lhs_norm <= 'z') {
    lhs_norm = static_cast<char>(lhs_norm - 'a' + 'A');
  }
  if (rhs_norm >= 'a' && rhs_norm <= 'z') {
    rhs_norm = static_cast<char>(rhs_norm - 'a' + 'A');
  }
  return lhs_norm == rhs_norm;
}

[[nodiscard]] bool token_equal_ci(const char *const lhs,
                                  const char *const rhs) noexcept {
  if (lhs == nullptr || rhs == nullptr) {
    return false;
  }
  auto index = std::size_t{0U};
  while (lhs[index] != '\0' && rhs[index] != '\0') {
    if (!ascii_equal_ci(lhs[index], rhs[index])) {
      return false;
    }
    ++index;
  }
  return lhs[index] == rhs[index];
}

[[nodiscard]] bool parse_u32(const char *const token,
                             const std::uint32_t max_value,
                             std::uint32_t &value) noexcept {
  value = 0U;
  if (token == nullptr || token[0] == '\0') {
    return false;
  }
  char *end = nullptr;
  errno = 0;
  const auto parsed = std::strtoul(token, &end, 0);
  if (errno != 0 || end == token || *end != '\0' || parsed > max_value) {
    return false;
  }
  value = static_cast<std::uint32_t>(parsed);
  return true;
}

[[nodiscard]] bool parse_predefined_bitrate(const char *const token,
                                            const bool data_phase,
                                            std::uint32_t &bitrate) noexcept {
  bitrate = 0U;
  if (token_equal_ci(token, "50K")) {
    bitrate = 50'000U;
  } else if (token_equal_ci(token, "125K")) {
    bitrate = 125'000U;
  } else if (token_equal_ci(token, "250K")) {
    bitrate = 250'000U;
  } else if (token_equal_ci(token, "500K")) {
    bitrate = 500'000U;
  } else if (token_equal_ci(token, "1M")) {
    bitrate = 1'000'000U;
  } else if (data_phase && token_equal_ci(token, "5M")) {
    bitrate = 5'000'000U;
  }
  return bitrate != 0U;
}

[[nodiscard]] bool parse_raw_timing_field(const char *const begin,
                                          const char *const end,
                                          const std::uint32_t max_value,
                                          std::uint32_t &value) noexcept {
  value = 0U;
  if (begin == nullptr || end == nullptr || begin >= end) {
    return false;
  }
  char buffer[16]{};
  const auto length = static_cast<std::size_t>(end - begin);
  if (length >= sizeof(buffer)) {
    return false;
  }
  for (auto index = std::size_t{0U}; index < length; ++index) {
    buffer[index] = begin[index];
  }
  return parse_u32(buffer, max_value, value);
}

[[nodiscard]] bool parse_raw_timing(const char *const token,
                                    QnxCanCompatRawTiming &raw) noexcept {
  raw = QnxCanCompatRawTiming{};
  if (token == nullptr) {
    return false;
  }

  std::array<std::uint32_t, 5U> fields{};
  auto field_index = std::size_t{0U};
  const char *field_begin = token;
  const char *cursor = token;
  while (true) {
    if (*cursor == ',' || *cursor == '\0') {
      if (field_index >= fields.size()) {
        return false;
      }
      const auto field_max =
          field_index == 0U
              ? kRawTimingPresdivMax
              : (field_index == 4U ? kRawTimingRjwMax
                                   : kRawTimingSegmentMax);
      if (!parse_raw_timing_field(field_begin, cursor, field_max,
                                  fields[field_index])) {
        return false;
      }
      ++field_index;
      if (*cursor == '\0') {
        break;
      }
      field_begin = cursor + 1;
    }
    ++cursor;
  }
  if (field_index != fields.size() || fields[3] == 0U) {
    return false;
  }

  const auto prescaler = fields[0] + 1U;
  const auto before_sample = fields[1] + fields[2] + 2U;
  const auto after_sample = fields[3] + 1U;
  const auto sync_jump_width = fields[4] + 1U;
  const auto time_quanta = 1U + before_sample + after_sample;
  const auto denominator = prescaler * time_quanta;
  if (denominator == 0U ||
      (kQnxCanCompatSourceClockHz % denominator) != 0U ||
      sync_jump_width > after_sample) {
    return false;
  }

  raw.valid = true;
  raw.presdiv = static_cast<std::uint16_t>(fields[0]);
  raw.propseg = static_cast<std::uint8_t>(fields[1]);
  raw.pseg1 = static_cast<std::uint8_t>(fields[2]);
  raw.pseg2 = static_cast<std::uint8_t>(fields[3]);
  raw.rjw = static_cast<std::uint8_t>(fields[4]);
  raw.timing.valid = true;
  raw.timing.source_clock_hz = kQnxCanCompatSourceClockHz;
  raw.timing.bitrate = kQnxCanCompatSourceClockHz / denominator;
  raw.timing.prescaler = static_cast<std::uint16_t>(prescaler);
  raw.timing.time_segment_before_sample =
      static_cast<std::uint8_t>(before_sample);
  raw.timing.time_segment_after_sample =
      static_cast<std::uint8_t>(after_sample);
  raw.timing.sync_jump_width = static_cast<std::uint8_t>(sync_jump_width);
  return true;
}

[[nodiscard]] bool parse_can_num_with_irqs(
    const char *const token, QnxCanCompatStartupConfig &config) noexcept {
  if (token == nullptr || token[0] != 'c' || token[1] != 'a' ||
      token[2] != 'n' || token[3] < '0' || token[3] > '3') {
    return false;
  }
  config.can_unit = static_cast<std::uint8_t>(token[3] - '0');
  config.irq_count = 0U;
  auto cursor = &token[4];
  while (*cursor == ',') {
    if (config.irq_count >= config.irqs.size()) {
      return false;
    }
    ++cursor;
    const auto *const begin = cursor;
    while (*cursor != ',' && *cursor != '\0') {
      ++cursor;
    }
    auto irq = std::uint32_t{0U};
    if (!parse_raw_timing_field(begin, cursor,
                                std::numeric_limits<std::uint32_t>::max(),
                                irq)) {
      return false;
    }
    config.irqs[config.irq_count] = irq;
    ++config.irq_count;
  }
  return *cursor == '\0';
}

[[nodiscard]] bool token_is_number(const char *const token) noexcept {
  auto value = std::uint32_t{0U};
  return parse_u32(token, std::numeric_limits<std::uint32_t>::max(), value);
}

[[nodiscard]] McanStatus finalize_startup_config(
    QnxCanCompatStartupConfig &config, const bool mailbox_count_explicit,
    const bool data_option_used) noexcept {
  if (!config.fd_mode &&
      (data_option_used || config.data_raw_timing.valid || config.iso_fd ||
       config.tdc_enabled)) {
    return McanStatus::unsupported_mode;
  }
  const auto mailbox_limit = config.fd_mode ? kQnxCanCompatFdMailboxCount
                                            : kQnxCanCompatClassicMailboxCount;
  if (!mailbox_count_explicit) {
    config.mailbox_count = mailbox_limit;
  }
  if (config.mailbox_count == 0U || config.mailbox_count > mailbox_limit ||
      config.mailbox_count > kMaxQueueCapacity) {
    return McanStatus::invalid_queue_capacity;
  }
  return McanStatus::ok;
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
                                        const char *const literal) noexcept {
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

[[nodiscard]] McanControllerCapabilities classic_qnx_capabilities() noexcept {
  auto capabilities = McanControllerCapabilities{};
  capabilities.flags = capability_value(McanCapabilityFlag::classic_frame) |
                       capability_value(McanCapabilityFlag::extended_id);
  return capabilities;
}

} // namespace

bool qnx_can_compat_startup_args_present(
    const int argc, const char *const argv[]) noexcept {
  if (argc <= 0 || argv == nullptr || argv[0] == nullptr) {
    return false;
  }
  auto config = QnxCanCompatStartupConfig{};
  if (parse_can_num_with_irqs(argv[0], config)) {
    return true;
  }
  return argv[0][0] == '-' && argv[0][1] != '-';
}

McanStatus parse_qnx_can_compat_startup(
    const int argc, const char *const argv[],
    QnxCanCompatStartupConfig &config) noexcept {
  config = QnxCanCompatStartupConfig{};
  if (argc < 0 || (argc > 0 && argv == nullptr)) {
    return McanStatus::invalid_argument;
  }

  auto mailbox_count_explicit = false;
  auto data_option_used = false;
  auto positional_can_seen = false;
  auto nominal_predefined_seen = false;
  auto data_predefined_seen = false;

  for (auto index = int{0}; index < argc; ++index) {
    const auto *const arg = argv[index];
    if (arg == nullptr || arg[0] == '\0') {
      return McanStatus::invalid_argument;
    }
    if (arg[0] != '-') {
      if (positional_can_seen || !parse_can_num_with_irqs(arg, config)) {
        return McanStatus::invalid_argument;
      }
      positional_can_seen = true;
      continue;
    }
    if (arg[1] == '\0' || arg[2] != '\0') {
      return McanStatus::invalid_argument;
    }

    switch (arg[1]) {
    case 'b':
      if (index + 1 >= argc || nominal_predefined_seen ||
          config.nominal_raw_timing.valid ||
          !parse_predefined_bitrate(argv[index + 1], false,
                                    config.nominal_bitrate)) {
        return McanStatus::invalid_bitrate;
      }
      nominal_predefined_seen = true;
      ++index;
      break;
    case 'B':
      if (index + 1 >= argc || nominal_predefined_seen ||
          config.nominal_raw_timing.valid ||
          !parse_raw_timing(argv[index + 1], config.nominal_raw_timing)) {
        return McanStatus::invalid_bitrate;
      }
      config.nominal_bitrate = config.nominal_raw_timing.timing.bitrate;
      ++index;
      break;
    case 'c':
      config.tdc_enabled = true;
      if (index + 1 < argc && token_is_number(argv[index + 1])) {
        auto offset = std::uint32_t{0U};
        if (!parse_u32(argv[index + 1], 0xFFFFU, offset)) {
          return McanStatus::invalid_argument;
        }
        config.tdc_offset = static_cast<std::uint16_t>(offset);
        ++index;
      }
      break;
    case 'd':
      if (index + 1 >= argc || data_predefined_seen ||
          config.data_raw_timing.valid ||
          !parse_predefined_bitrate(argv[index + 1], true,
                                    config.data_bitrate)) {
        return McanStatus::invalid_bitrate;
      }
      data_predefined_seen = true;
      data_option_used = true;
      ++index;
      break;
    case 'D':
      if (index + 1 >= argc || data_predefined_seen ||
          config.data_raw_timing.valid ||
          !parse_raw_timing(argv[index + 1], config.data_raw_timing)) {
        return McanStatus::invalid_bitrate;
      }
      config.data_bitrate = config.data_raw_timing.timing.bitrate;
      data_option_used = true;
      ++index;
      break;
    case 'f':
      config.fd_mode = true;
      break;
    case 'h':
      config.single_message_buffer = true;
      break;
    case 'I':
      config.io_privilege_requested = true;
      break;
    case 'l':
      config.listen_only = true;
      break;
    case 'n': {
      if (index + 1 >= argc) {
        return McanStatus::invalid_argument;
      }
      auto count = std::uint32_t{0U};
      if (!parse_u32(argv[index + 1], kMaxQueueCapacity, count) ||
          count == 0U) {
        return McanStatus::invalid_queue_capacity;
      }
      config.mailbox_count = static_cast<std::uint16_t>(count);
      mailbox_count_explicit = true;
      ++index;
      break;
    }
    case 'o':
      config.iso_fd = true;
      break;
    case 'r': {
      if (index + 1 >= argc) {
        return McanStatus::invalid_argument;
      }
      auto with_irqs = QnxCanCompatStartupConfig{};
      char token[64]{};
      const auto prefix_len = std::strlen("can0,");
      if (std::strlen(argv[index + 1]) + prefix_len >= sizeof(token)) {
        return McanStatus::invalid_argument;
      }
      token[0] = 'c';
      token[1] = 'a';
      token[2] = 'n';
      token[3] = static_cast<char>('0' + config.can_unit);
      token[4] = ',';
      auto copy_index = std::size_t{0U};
      while (argv[index + 1][copy_index] != '\0') {
        token[5U + copy_index] = argv[index + 1][copy_index];
        ++copy_index;
      }
      if (!parse_can_num_with_irqs(token, with_irqs)) {
        return McanStatus::invalid_argument;
      }
      config.irq_count = with_irqs.irq_count;
      config.irqs = with_irqs.irqs;
      ++index;
      break;
    }
    case 'R':
      config.raw_mode = true;
      break;
    case 't':
      config.internal_loopback = true;
      break;
    case 'u': {
      if (index + 1 >= argc) {
        return McanStatus::invalid_argument;
      }
      auto unit = std::uint32_t{0U};
      if (!parse_u32(argv[index + 1], 3U, unit)) {
        return McanStatus::invalid_instance;
      }
      config.can_unit = static_cast<std::uint8_t>(unit);
      ++index;
      break;
    }
    case 'x':
      config.extended_mid_default = true;
      break;
    default:
      return McanStatus::unsupported_mode;
    }
  }

  return finalize_startup_config(config, mailbox_count_explicit,
                                 data_option_used);
}

McanStatus qnx_can_compat_make_resource_manager_config(
    const QnxCanCompatStartupConfig &startup,
    McanResourceManagerConfig &config) noexcept {
  config = McanResourceManagerConfig{};
  config.controller_count = 1U;
  auto &controller = config.controllers[0];
  controller.instance_id = startup.can_unit;
  controller.rx_queue_count = 1U;
  controller.tx_queue_count = 1U;
  controller.rx_queue_capacity = startup.mailbox_count;
  controller.tx_queue_capacity = startup.mailbox_count;
  controller.mode = startup.fd_mode ? McanMode::fd : McanMode::classic;
  controller.brs_enabled = startup.fd_mode;
  controller.arbitration_bitrate_valid = true;
  controller.arbitration_bitrate = startup.nominal_bitrate;
  controller.nominal_timing_override = startup.nominal_raw_timing.timing;
  if (startup.fd_mode) {
    controller.data_bitrate_valid = true;
    controller.data_bitrate = startup.data_bitrate;
    controller.data_timing_override = startup.data_raw_timing.timing;
  }
  const auto *const board_instance =
      find_j784_mcan_board_instance_by_endpoint(startup.can_unit);
  if (board_instance != nullptr) {
    controller.hardware = board_instance->hardware;
    controller.board_mapping_evidence_confirmed = true;
  }
  if (startup.irq_count > 0U && startup.irqs[0] != 0U) {
    controller.hardware.qnx_logical_irq = startup.irqs[0];
    controller.qnx_irq_routing_evidence_confirmed = true;
  }
  return validate_resource_manager_skeleton(config);
}

McanStatus make_qnx_can_compat_endpoint_path(
    const EndpointKind kind, const std::uint8_t can_unit,
    const std::uint8_t mailbox_id, McanEndpointPath &path) noexcept {
  path.value.fill('\0');
  if (kind == EndpointKind::ctl) {
    return McanStatus::unsupported_mode;
  }
  if (can_unit >= kMaxControllers) {
    return McanStatus::invalid_instance;
  }
  if (mailbox_id >= kMaxQueuesPerController) {
    return McanStatus::invalid_queue;
  }

  auto offset = std::size_t{0U};
  auto status = append_literal(path, offset, "/dev/can");
  if (!status_ok(status)) {
    return status;
  }
  status = append_digit(path, offset, can_unit);
  if (!status_ok(status)) {
    return status;
  }
  status = append_char(path, offset, '/');
  if (!status_ok(status)) {
    return status;
  }
  status = append_literal(path, offset, kind == EndpointKind::rx ? "rx" : "tx");
  if (!status_ok(status)) {
    return status;
  }
  return append_digit(path, offset, mailbox_id);
}

McanStatus make_qnx_can_compat_namespace_path(
    const std::uint8_t can_unit, McanEndpointPath &path) noexcept {
  path.value.fill('\0');
  if (can_unit >= kMaxControllers) {
    return McanStatus::invalid_instance;
  }

  auto offset = std::size_t{0U};
  auto status = append_literal(path, offset, "/dev/can");
  if (!status_ok(status)) {
    return status;
  }
  return append_digit(path, offset, can_unit);
}

McanStatus qnx_can_compat_message_from_frame(
    const McanFrameTransfer &frame,
    QnxCanCompatCanMessage &message) noexcept {
  message = QnxCanCompatCanMessage{};
  if (has_flag(frame.flags, McanFrameFlag::fd_frame) ||
      has_flag(frame.flags, McanFrameFlag::brs) ||
      has_flag(frame.flags, McanFrameFlag::esi) ||
      has_flag(frame.flags, McanFrameFlag::timestamp_valid) ||
      frame.length > message.data.size() || frame.dlc > message.data.size()) {
    return McanStatus::unsupported_mode;
  }
  const auto validation =
      validate_frame_transfer(frame, classic_qnx_capabilities());
  if (!status_ok(validation)) {
    return validation;
  }
  const auto extended = has_flag(frame.flags, McanFrameFlag::extended_id);
  const auto remote = has_flag(frame.flags, McanFrameFlag::remote);
  message.message_id = extended ? (frame.can_id & kMaxExtendedCanId)
                                : ((frame.can_id & kMaxStandardCanId)
                                   << kQnxCanStandardMidShift);
  message.length = remote ? frame.dlc : frame.length;
  message.extended_id = extended;
  message.remote_frame = remote;
  message.timestamp = 0U;
  for (auto index = std::size_t{0U}; index < message.length; ++index) {
    message.data[index] = frame.payload[index];
  }
  return McanStatus::ok;
}

McanStatus qnx_can_compat_frame_from_message(
    const QnxCanCompatCanMessage &message,
    McanFrameTransfer &frame) noexcept {
  frame = McanFrameTransfer{};
  if (message.length > message.data.size()) {
    return McanStatus::invalid_argument;
  }
  const auto can_id = message.extended_id
                          ? (message.message_id & kMaxExtendedCanId)
                          : ((message.message_id >> kQnxCanStandardMidShift) &
                             kMaxStandardCanId);
  if ((!message.extended_id && can_id > kMaxStandardCanId) ||
      (message.extended_id && can_id > kMaxExtendedCanId)) {
    return McanStatus::invalid_argument;
  }
  frame.can_id = can_id;
  frame.flags = message.extended_id ? flag_value(McanFrameFlag::extended_id)
                                    : 0U;
  if (message.remote_frame) {
    frame.flags |= flag_value(McanFrameFlag::remote);
  }
  frame.dlc = message.length;
  frame.length = message.remote_frame ? 0U : message.length;
  for (auto index = std::size_t{0U}; index < frame.length; ++index) {
    frame.payload[index] = message.data[index];
  }
  return validate_frame_transfer(frame, classic_qnx_capabilities());
}

int qnx_can_compat_raw_tx_status_to_errno(const McanStatus status) noexcept {
  switch (status) {
  case McanStatus::ok:
    return 0;
  case McanStatus::tx_queue_full:
  case McanStatus::no_frame:
    return EAGAIN;
  case McanStatus::unsupported_mode:
    return ENOTSUP;
  case McanStatus::hardware_access_unavailable:
    return EACCES;
  case McanStatus::invalid_hardware_mapping:
    return ENODEV;
  case McanStatus::invalid_state:
    return EIO;
  case McanStatus::path_too_long:
    return ENAMETOOLONG;
  case McanStatus::too_many_controllers:
  case McanStatus::rx_queue_full:
    return EOVERFLOW;
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
