#include "evsn/can_hal/can_channel.hpp"

#include <cstring>

namespace evsn::can_hal {
namespace {

constexpr auto kFdPayloadLengths = std::array<std::uint8_t, 16U>{
    0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 12U, 16U, 20U, 24U, 32U, 48U, 64U};

CanResult copy_path(std::array<char, kCanPathCapacity>& destination, const char* path) noexcept {
    if (path == nullptr || path[0] == '\0') {
        return make_result(CanStatus::invalid_argument, CanOperation::configure);
    }

    auto index = std::size_t{0U};
    for (; index < destination.size(); ++index) {
        destination[index] = path[index];
        if (path[index] == '\0') {
            return ok_result(CanOperation::configure);
        }
    }

    destination.fill('\0');
    return make_result(CanStatus::invalid_argument, CanOperation::configure);
}

}  // namespace

bool is_fd_payload_length(const std::uint8_t length) noexcept {
    for (const auto valid_length : kFdPayloadLengths) {
        if (length == valid_length) {
            return true;
        }
    }
    return false;
}

bool is_classic_payload_length(const std::uint8_t length) noexcept {
    return length <= kClassicPayloadCapacity;
}

bool can_id_is_valid(const std::uint32_t id, const CanIdFormat format) noexcept {
    if (format == CanIdFormat::standard_11_bit) {
        return id <= kMaxStandardCanId;
    }
    return id <= kMaxExtendedCanId;
}

bool dlc_from_length(const std::uint8_t length, std::uint8_t& dlc) noexcept {
    for (auto index = std::uint8_t{0U}; index < kFdPayloadLengths.size(); ++index) {
        if (kFdPayloadLengths[index] == length) {
            dlc = index;
            return true;
        }
    }
    return false;
}

bool length_from_dlc(const std::uint8_t dlc, std::uint8_t& length) noexcept {
    if (dlc >= kFdPayloadLengths.size()) {
        return false;
    }
    length = kFdPayloadLengths[dlc];
    return true;
}

bool path_is_empty(const std::array<char, kCanPathCapacity>& path) noexcept {
    return path[0] == '\0';
}

CanResult set_rx_path(CanOpenConfig& config, const char* path) noexcept {
    return copy_path(config.rx_path, path);
}

CanResult set_tx_path(CanOpenConfig& config, const char* path) noexcept {
    return copy_path(config.tx_path, path);
}

CanResult validate_frame(const CanFrame& frame, const CanCapabilities& capabilities) noexcept {
    if (!can_id_is_valid(frame.id, frame.id_format)) {
        return make_result(CanStatus::invalid_frame, CanOperation::validate);
    }
    if (frame.id_format == CanIdFormat::extended_29_bit && !capabilities.extended_id) {
        return make_result(CanStatus::unsupported, CanOperation::validate);
    }
    if ((frame.frame_format != CanFrameFormat::fd) &&
        (frame.fd_flags.brs || frame.fd_flags.esi || frame.fd_flags.fdf)) {
        return make_result(CanStatus::unsupported, CanOperation::validate);
    }

    auto expected_dlc = std::uint8_t{0U};
    if (frame.kind == CanFrameKind::remote) {
        if (!capabilities.remote_frame) {
            return make_result(CanStatus::unsupported, CanOperation::validate);
        }
        if (frame.frame_format != CanFrameFormat::classic || frame.length != 0U ||
            !is_classic_payload_length(frame.dlc)) {
            return make_result(CanStatus::invalid_frame, CanOperation::validate);
        }
        return ok_result(CanOperation::validate);
    }

    if (frame.frame_format == CanFrameFormat::classic) {
        if (!capabilities.classic_frame) {
            return make_result(CanStatus::unsupported, CanOperation::validate);
        }
        if (!is_classic_payload_length(frame.length) || frame.dlc != frame.length) {
            return make_result(CanStatus::invalid_frame, CanOperation::validate);
        }
        return ok_result(CanOperation::validate);
    }

    if (!capabilities.fd_frame) {
        return make_result(CanStatus::unsupported, CanOperation::validate);
    }
    if (!is_fd_payload_length(frame.length) || !dlc_from_length(frame.length, expected_dlc) ||
        frame.dlc != expected_dlc) {
        return make_result(CanStatus::invalid_frame, CanOperation::validate);
    }
    if ((frame.fd_flags.brs && !capabilities.brs) || (frame.fd_flags.esi && !capabilities.esi) ||
        (frame.fd_flags.fdf && !capabilities.fdf_flag)) {
        return make_result(CanStatus::unsupported, CanOperation::validate);
    }

    return ok_result(CanOperation::validate);
}

}  // namespace evsn::can_hal
