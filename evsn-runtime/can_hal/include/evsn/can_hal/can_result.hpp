#pragma once

#include <cstdint>

namespace evsn::can_hal {

enum class CanStatus : std::uint8_t {
    ok = 0U,
    no_frame,
    tx_full,
    invalid_argument,
    invalid_frame,
    unsupported,
    not_open,
    permission_denied,
    io_error,
    configuration_error,
};

enum class CanOperation : std::uint8_t {
    none = 0U,
    open,
    read,
    write,
    close,
    validate,
    configure,
};

struct CanResult {
    CanStatus status{CanStatus::ok};
    CanOperation operation{CanOperation::none};
    int native_error{0};

    [[nodiscard]] constexpr bool ok() const noexcept {
        return status == CanStatus::ok;
    }
};

[[nodiscard]] constexpr CanResult ok_result(CanOperation operation = CanOperation::none) noexcept {
    return CanResult{CanStatus::ok, operation, 0};
}

[[nodiscard]] constexpr CanResult make_result(
    CanStatus status,
    CanOperation operation,
    int native_error = 0) noexcept {
    return CanResult{status, operation, native_error};
}

}  // namespace evsn::can_hal
