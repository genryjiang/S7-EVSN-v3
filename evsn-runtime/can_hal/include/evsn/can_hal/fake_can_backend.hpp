#pragma once

#include "evsn/can_hal/can_channel.hpp"

#include <cstddef>

namespace evsn::can_hal {

class FakeCanBackendControl {
public:
    explicit FakeCanBackendControl(CanChannel& channel) noexcept;

    [[nodiscard]] CanResult clear() noexcept;
    [[nodiscard]] CanResult inject_rx(const CanFrame& frame) noexcept;
    [[nodiscard]] CanResult transmitted_frame(std::size_t index, CanFrame& frame) const noexcept;
    [[nodiscard]] std::size_t transmitted_count() const noexcept;
    void force_open_status(CanStatus status, int native_error = 0) noexcept;
    void force_read_status(CanStatus status, int native_error = 0) noexcept;
    void force_write_status(CanStatus status, int native_error = 0) noexcept;

private:
    CanChannel* channel_;
};

}  // namespace evsn::can_hal
