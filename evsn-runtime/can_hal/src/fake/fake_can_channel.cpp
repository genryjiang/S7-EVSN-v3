#include "evsn/can_hal/fake_can_backend.hpp"

#include "common/backend_status.hpp"

namespace evsn::can_hal {
namespace {

constexpr CanCapabilities fake_capabilities() noexcept {
    return CanCapabilities{
        true,
        true,
        true,
        true,
        true,
        true,
        true,
        true,
        true,
        true};
}

CanResult forced_result(
    const CanStatus status,
    const CanOperation operation,
    const int native_error) noexcept {
    if (status == CanStatus::ok) {
        return ok_result(operation);
    }
    return make_result(status, operation, native_error);
}

}  // namespace

CanResult CanChannel::open(const CanOpenConfig& config) noexcept {
    const auto forced = forced_result(fake_open_status_, CanOperation::open, fake_native_error_);
    if (!forced.ok()) {
        ++diagnostics_.open_failures;
        record_failure(diagnostics_, forced);
        return forced;
    }

    config_ = config;
    capabilities_ = fake_capabilities();
    open_ = true;
    ++diagnostics_.opens;
    return ok_result(CanOperation::open);
}

CanResult CanChannel::read(CanFrame& frame) noexcept {
    if (!open_) {
        const auto result = make_result(CanStatus::not_open, CanOperation::read);
        record_failure(diagnostics_, result);
        return result;
    }

    const auto forced = forced_result(fake_read_status_, CanOperation::read, fake_native_error_);
    if (!forced.ok()) {
        record_failure(diagnostics_, forced);
        return forced;
    }

    if (fake_rx_count_ == 0U) {
        const auto result = make_result(CanStatus::no_frame, CanOperation::read);
        record_failure(diagnostics_, result);
        return result;
    }

    frame = fake_rx_queue_[fake_rx_head_];
    fake_rx_head_ = (fake_rx_head_ + 1U) % kFakeQueueCapacity;
    --fake_rx_count_;
    ++diagnostics_.rx_frames;
    return ok_result(CanOperation::read);
}

CanResult CanChannel::write(const CanFrame& frame) noexcept {
    if (!open_) {
        const auto result = make_result(CanStatus::not_open, CanOperation::write);
        record_failure(diagnostics_, result);
        return result;
    }

    const auto validation = validate_frame(frame, capabilities_);
    if (!validation.ok()) {
        record_failure(diagnostics_, validation);
        return validation;
    }

    const auto forced = forced_result(fake_write_status_, CanOperation::write, fake_native_error_);
    if (!forced.ok()) {
        record_failure(diagnostics_, forced);
        return forced;
    }

    if (fake_tx_count_ >= kFakeQueueCapacity) {
        const auto result = make_result(CanStatus::tx_full, CanOperation::write);
        record_failure(diagnostics_, result);
        return result;
    }

    fake_tx_log_[fake_tx_count_] = frame;
    ++fake_tx_count_;
    ++diagnostics_.tx_frames;
    return ok_result(CanOperation::write);
}

CanResult CanChannel::close() noexcept {
    if (!open_) {
        return ok_result(CanOperation::close);
    }

    open_ = false;
    ++diagnostics_.closes;
    return ok_result(CanOperation::close);
}

FakeCanBackendControl::FakeCanBackendControl(CanChannel& channel) noexcept
    : channel_{&channel} {
}

CanResult FakeCanBackendControl::clear() noexcept {
    channel_->fake_rx_queue_ = {};
    channel_->fake_tx_log_ = {};
    channel_->fake_rx_head_ = 0U;
    channel_->fake_rx_count_ = 0U;
    channel_->fake_tx_count_ = 0U;
    channel_->fake_open_status_ = CanStatus::ok;
    channel_->fake_read_status_ = CanStatus::ok;
    channel_->fake_write_status_ = CanStatus::ok;
    channel_->fake_native_error_ = 0;
    return ok_result(CanOperation::configure);
}

CanResult FakeCanBackendControl::inject_rx(const CanFrame& frame) noexcept {
    const auto validation = validate_frame(frame, fake_capabilities());
    if (!validation.ok()) {
        return validation;
    }
    if (channel_->fake_rx_count_ >= CanChannel::kFakeQueueCapacity) {
        return make_result(CanStatus::tx_full, CanOperation::configure);
    }

    const auto insert_index =
        (channel_->fake_rx_head_ + channel_->fake_rx_count_) % CanChannel::kFakeQueueCapacity;
    channel_->fake_rx_queue_[insert_index] = frame;
    ++channel_->fake_rx_count_;
    return ok_result(CanOperation::configure);
}

CanResult FakeCanBackendControl::transmitted_frame(
    const std::size_t index,
    CanFrame& frame) const noexcept {
    if (index >= channel_->fake_tx_count_) {
        return make_result(CanStatus::invalid_argument, CanOperation::read);
    }
    frame = channel_->fake_tx_log_[index];
    return ok_result(CanOperation::read);
}

std::size_t FakeCanBackendControl::transmitted_count() const noexcept {
    return channel_->fake_tx_count_;
}

void FakeCanBackendControl::force_open_status(
    const CanStatus status,
    const int native_error) noexcept {
    channel_->fake_open_status_ = status;
    channel_->fake_native_error_ = native_error;
}

void FakeCanBackendControl::force_read_status(
    const CanStatus status,
    const int native_error) noexcept {
    channel_->fake_read_status_ = status;
    channel_->fake_native_error_ = native_error;
}

void FakeCanBackendControl::force_write_status(
    const CanStatus status,
    const int native_error) noexcept {
    channel_->fake_write_status_ = status;
    channel_->fake_native_error_ = native_error;
}

}  // namespace evsn::can_hal
