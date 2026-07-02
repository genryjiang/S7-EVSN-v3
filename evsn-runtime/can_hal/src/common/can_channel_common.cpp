#include "evsn/can_hal/can_channel.hpp"

namespace evsn::can_hal {

CanChannel::CanChannel() noexcept = default;

CanChannel::~CanChannel() noexcept {
    (void)close();
}

CanCapabilities CanChannel::capabilities() const noexcept {
    return capabilities_;
}

CanDiagnostics CanChannel::diagnostics() const noexcept {
    return diagnostics_;
}

}  // namespace evsn::can_hal
