#ifndef _QNX_SOURCE
#define _QNX_SOURCE
#endif

#include "evsn/can_hal/can_channel.hpp"

#include "common/backend_status.hpp"

#include <cerrno>
#include <cstdint>
#include <cstring>
#include <devctl.h>
#include <fcntl.h>
#include <sys/can_dcmd.h>
#include <unistd.h>

namespace evsn::can_hal {
namespace {

constexpr auto kQnxInvalidFd = -1;
constexpr auto kQnxStandardMidShift = std::uint32_t{18U};

constexpr CanCapabilities qnx_devctl_capabilities() noexcept {
    return CanCapabilities{
        true,
        false,
        true,
        true,
        false,
        false,
        false,
        false,
        false,
        false};
}

CanResult qnx_result(
    const int native_error,
    const CanOperation operation,
    const CanStatus eagain_status = CanStatus::io_error) noexcept {
    if (native_error == EOK) {
        return ok_result(operation);
    }
    if (native_error == EAGAIN) {
        return make_result(eagain_status, operation, native_error);
    }
    return make_result(status_from_errno(native_error), operation, native_error);
}

void zero_can_msg(struct can_msg& message) noexcept {
    std::memset(&message, 0, sizeof(message));
}

CanResult portable_from_qnx(const struct can_msg& message, CanFrame& frame) noexcept {
    auto length = static_cast<std::uint8_t>(message.len);
    if (length > kClassicPayloadCapacity) {
        return make_result(CanStatus::unsupported, CanOperation::read);
    }

    frame = CanFrame{};
    frame.id_format = message.ext.is_extended_mid != 0U
        ? CanIdFormat::extended_29_bit
        : CanIdFormat::standard_11_bit;
    frame.id = frame.id_format == CanIdFormat::extended_29_bit
        ? (message.mid & kMaxExtendedCanId)
        : ((message.mid >> kQnxStandardMidShift) & kMaxStandardCanId);
    frame.kind = message.ext.is_remote_frame != 0U ? CanFrameKind::remote : CanFrameKind::data;
    frame.frame_format = CanFrameFormat::classic;
    frame.dlc = length;
    frame.length = frame.kind == CanFrameKind::remote ? 0U : length;
    frame.timestamp.valid = false;
    frame.timestamp.monotonic_ns = 0U;

    for (auto index = std::uint8_t{0U}; index < length; ++index) {
        frame.payload[index] = message.dat[index];
    }

    return validate_frame(frame, qnx_devctl_capabilities());
}

void qnx_from_portable(const CanFrame& frame, struct can_msg& message) noexcept {
    zero_can_msg(message);
    message.mid = frame.id_format == CanIdFormat::extended_29_bit
        ? (frame.id & kMaxExtendedCanId)
        : ((frame.id & kMaxStandardCanId) << kQnxStandardMidShift);
    message.ext.is_extended_mid = frame.id_format == CanIdFormat::extended_29_bit ? 1U : 0U;
    message.ext.is_remote_frame = frame.kind == CanFrameKind::remote ? 1U : 0U;
    message.len = frame.kind == CanFrameKind::remote ? frame.dlc : frame.length;

    for (auto index = std::uint8_t{0U}; index < message.len; ++index) {
        message.dat[index] = frame.payload[index];
    }
}

CanResult close_fd(int& fd, CanDiagnostics& diagnostics) noexcept {
    if (fd == kQnxInvalidFd) {
        return ok_result(CanOperation::close);
    }

    const auto close_result = ::close(fd);
    fd = kQnxInvalidFd;
    if (close_result == 0) {
        return ok_result(CanOperation::close);
    }

    const auto result = make_result(status_from_errno(errno), CanOperation::close, errno);
    record_failure(diagnostics, result);
    return result;
}

}  // namespace

CanResult CanChannel::open(const CanOpenConfig& config) noexcept {
    if (open_) {
        (void)close();
    }

    capabilities_ = qnx_devctl_capabilities();
    if (path_is_empty(config.rx_path) || path_is_empty(config.tx_path)) {
        const auto result = make_result(CanStatus::invalid_argument, CanOperation::open);
        ++diagnostics_.open_failures;
        record_failure(diagnostics_, result);
        return result;
    }
    if (config.mode == CanMode::fd || config.arbitration_bitrate_valid ||
        config.data_bitrate_valid || config.latency_limit_valid) {
        const auto result = make_result(CanStatus::unsupported, CanOperation::open);
        ++diagnostics_.open_failures;
        record_failure(diagnostics_, result);
        return result;
    }

    rx_fd_ = ::open(config.rx_path.data(), O_RDONLY);
    if (rx_fd_ == kQnxInvalidFd) {
        const auto result = make_result(status_from_errno(errno), CanOperation::open, errno);
        ++diagnostics_.open_failures;
        record_failure(diagnostics_, result);
        return result;
    }

    tx_fd_ = ::open(config.tx_path.data(), O_WRONLY);
    if (tx_fd_ == kQnxInvalidFd) {
        const auto result = make_result(status_from_errno(errno), CanOperation::open, errno);
        ++diagnostics_.open_failures;
        record_failure(diagnostics_, result);
        (void)close_fd(rx_fd_, diagnostics_);
        return result;
    }

    config_ = config;
    open_ = true;
    ++diagnostics_.opens;
    return ok_result(CanOperation::open);
}

CanResult CanChannel::read(CanFrame& frame) noexcept {
    if (!open_ || rx_fd_ == kQnxInvalidFd) {
        const auto result = make_result(CanStatus::not_open, CanOperation::read);
        record_failure(diagnostics_, result);
        return result;
    }

    struct can_msg message {};
    zero_can_msg(message);
    const auto native = ::devctl(
        rx_fd_,
        CAN_DEVCTL_RX_FRAME_RAW_NOBLOCK,
        &message,
        sizeof(message),
        nullptr);
    const auto result = qnx_result(native, CanOperation::read, CanStatus::no_frame);
    if (!result.ok()) {
        record_failure(diagnostics_, result);
        return result;
    }

    const auto converted = portable_from_qnx(message, frame);
    if (!converted.ok()) {
        record_failure(diagnostics_, converted);
        return converted;
    }

    ++diagnostics_.rx_frames;
    return ok_result(CanOperation::read);
}

CanResult CanChannel::write(const CanFrame& frame) noexcept {
    if (!open_ || tx_fd_ == kQnxInvalidFd) {
        const auto result = make_result(CanStatus::not_open, CanOperation::write);
        record_failure(diagnostics_, result);
        return result;
    }

    const auto validation = validate_frame(frame, capabilities_);
    if (!validation.ok()) {
        record_failure(diagnostics_, validation);
        return validation;
    }

    struct can_msg message {};
    qnx_from_portable(frame, message);
    const auto native = ::devctl(
        tx_fd_,
        CAN_DEVCTL_TX_FRAME_RAW,
        &message,
        sizeof(message),
        nullptr);
    const auto result = qnx_result(native, CanOperation::write, CanStatus::tx_full);
    if (!result.ok()) {
        record_failure(diagnostics_, result);
        return result;
    }

    ++diagnostics_.tx_frames;
    return ok_result(CanOperation::write);
}

CanResult CanChannel::close() noexcept {
    auto final_result = ok_result(CanOperation::close);
    const auto rx_result = close_fd(rx_fd_, diagnostics_);
    const auto tx_result = close_fd(tx_fd_, diagnostics_);
    if (!rx_result.ok()) {
        final_result = rx_result;
    } else if (!tx_result.ok()) {
        final_result = tx_result;
    }

    if (open_) {
        ++diagnostics_.closes;
    }
    open_ = false;
    return final_result;
}

}  // namespace evsn::can_hal
