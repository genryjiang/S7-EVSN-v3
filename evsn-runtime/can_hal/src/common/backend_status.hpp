#pragma once

#include "evsn/can_hal/can_channel.hpp"

#include <cerrno>

namespace evsn::can_hal {

inline CanStatus status_from_errno(const int native_error) noexcept {
    switch (native_error) {
        case EAGAIN:
            return CanStatus::no_frame;
        case EACCES:
        case EPERM:
            return CanStatus::permission_denied;
        case EINVAL:
            return CanStatus::invalid_argument;
        case ENOTTY:
        case ENOSYS:
            return CanStatus::unsupported;
        default:
            return CanStatus::io_error;
    }
}

inline void record_failure(CanDiagnostics& diagnostics, const CanResult result) noexcept {
    diagnostics.last_backend_failure = result;
    if (result.status == CanStatus::tx_full) {
        ++diagnostics.tx_full_events;
    } else if (result.status == CanStatus::no_frame) {
        ++diagnostics.no_frame_polls;
    } else if (result.status == CanStatus::invalid_frame) {
        ++diagnostics.invalid_frame_rejects;
    } else if (result.status == CanStatus::unsupported) {
        ++diagnostics.unsupported_feature_rejects;
    } else if (
        result.status == CanStatus::io_error ||
        result.status == CanStatus::permission_denied ||
        result.status == CanStatus::configuration_error) {
        ++diagnostics.io_errors;
    }
}

}  // namespace evsn::can_hal
