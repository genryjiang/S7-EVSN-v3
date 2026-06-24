#include "evsn/can_hal/can_channel.hpp"

#include <array>
#include <cstdint>
#include <cstdio>
#include <time.h>

namespace {

using evsn::can_hal::CanFrame;
using evsn::can_hal::CanFrameFormat;
using evsn::can_hal::CanFrameKind;
using evsn::can_hal::CanIdFormat;
using evsn::can_hal::CanOperation;
using evsn::can_hal::CanResult;
using evsn::can_hal::CanStatus;

constexpr auto kDefaultRxPath = "/dev/can0/rx0";
constexpr auto kDefaultTxPath = "/dev/can0/tx0";
constexpr auto kPollCount = 100U;
constexpr auto kPollSleepNs = 10000000L;

[[nodiscard]] const char* status_name(const CanStatus status) noexcept {
    switch (status) {
        case CanStatus::ok:
            return "ok";
        case CanStatus::no_frame:
            return "no_frame";
        case CanStatus::tx_full:
            return "tx_full";
        case CanStatus::invalid_argument:
            return "invalid_argument";
        case CanStatus::invalid_frame:
            return "invalid_frame";
        case CanStatus::unsupported:
            return "unsupported";
        case CanStatus::not_open:
            return "not_open";
        case CanStatus::permission_denied:
            return "permission_denied";
        case CanStatus::io_error:
            return "io_error";
        case CanStatus::configuration_error:
            return "configuration_error";
    }

    return "unknown";
}

[[nodiscard]] const char* operation_name(const CanOperation operation) noexcept {
    switch (operation) {
        case CanOperation::none:
            return "none";
        case CanOperation::open:
            return "open";
        case CanOperation::read:
            return "read";
        case CanOperation::write:
            return "write";
        case CanOperation::close:
            return "close";
        case CanOperation::validate:
            return "validate";
        case CanOperation::configure:
            return "configure";
    }

    return "unknown";
}

void print_result(const char* label, const CanResult result) noexcept {
    std::printf(
        "%s: status=%s operation=%s native=%d\n",
        label,
        status_name(result.status),
        operation_name(result.operation),
        result.native_error);
}

void print_frame(const char* label, const CanFrame& frame) noexcept {
    std::printf(
        "%s: id=0x%08x id_format=%s length=%u payload=",
        label,
        static_cast<unsigned int>(frame.id),
        frame.id_format == CanIdFormat::extended_29_bit ? "extended" : "standard",
        static_cast<unsigned int>(frame.length));

    for (auto index = std::uint8_t{0U}; index < frame.length; ++index) {
        std::printf("%02x", static_cast<unsigned int>(frame.payload[index]));
    }
    std::printf("\n");
}

[[nodiscard]] bool set_paths(
    evsn::can_hal::CanOpenConfig& config,
    const char* rx_path,
    const char* tx_path) noexcept {
    const auto rx_result = evsn::can_hal::set_rx_path(config, rx_path);
    if (!rx_result.ok()) {
        print_result("set_rx_path", rx_result);
        return false;
    }

    const auto tx_result = evsn::can_hal::set_tx_path(config, tx_path);
    if (!tx_result.ok()) {
        print_result("set_tx_path", tx_result);
        return false;
    }

    return true;
}

[[nodiscard]] CanFrame make_frame(
    const std::uint32_t id,
    const CanIdFormat id_format,
    const std::array<std::uint8_t, 8U>& payload,
    const std::uint8_t length) noexcept {
    auto frame = CanFrame{};
    frame.id = id;
    frame.id_format = id_format;
    frame.frame_format = CanFrameFormat::classic;
    frame.kind = CanFrameKind::data;
    frame.dlc = length;
    frame.length = length;

    for (auto index = std::uint8_t{0U}; index < length; ++index) {
        frame.payload[index] = payload[index];
    }

    return frame;
}

[[nodiscard]] bool frames_match(const CanFrame& actual, const CanFrame& expected) noexcept {
    if (actual.id != expected.id || actual.id_format != expected.id_format ||
        actual.frame_format != expected.frame_format || actual.kind != expected.kind ||
        actual.dlc != expected.dlc || actual.length != expected.length) {
        return false;
    }

    for (auto index = std::uint8_t{0U}; index < expected.length; ++index) {
        if (actual.payload[index] != expected.payload[index]) {
            return false;
        }
    }

    return true;
}

[[nodiscard]] bool drain_rx(evsn::can_hal::CanChannel& channel) noexcept {
    for (auto poll = 0U; poll < 16U; ++poll) {
        auto frame = CanFrame{};
        const auto result = channel.read(frame);
        if (result.status == CanStatus::no_frame) {
            return true;
        }
        if (!result.ok()) {
            print_result("drain_read", result);
            return false;
        }
        print_frame("drained", frame);
    }

    return true;
}

[[nodiscard]] bool wait_for_frame(
    evsn::can_hal::CanChannel& channel,
    const CanFrame& expected) noexcept {
    const auto poll_sleep = timespec{0, kPollSleepNs};
    for (auto poll = 0U; poll < kPollCount; ++poll) {
        auto frame = CanFrame{};
        const auto result = channel.read(frame);
        if (result.ok()) {
            print_frame("received", frame);
            if (frames_match(frame, expected)) {
                return true;
            }
        } else if (result.status != CanStatus::no_frame) {
            print_result("read", result);
            return false;
        }

        static_cast<void>(::nanosleep(&poll_sleep, nullptr));
    }

    std::printf("timed out waiting for expected frame\n");
    print_frame("expected", expected);
    return false;
}

[[nodiscard]] bool write_and_expect(
    evsn::can_hal::CanChannel& channel,
    const CanFrame& frame) noexcept {
    print_frame("transmit", frame);
    const auto write_result = channel.write(frame);
    if (!write_result.ok()) {
        print_result("write", write_result);
        return false;
    }

    return wait_for_frame(channel, frame);
}

void print_diagnostics(const evsn::can_hal::CanDiagnostics& diagnostics) noexcept {
    std::printf(
        "diagnostics: opens=%llu closes=%llu rx=%llu tx=%llu no_frame=%llu tx_full=%llu "
        "invalid=%llu unsupported=%llu open_failures=%llu io_errors=%llu\n",
        static_cast<unsigned long long>(diagnostics.opens),
        static_cast<unsigned long long>(diagnostics.closes),
        static_cast<unsigned long long>(diagnostics.rx_frames),
        static_cast<unsigned long long>(diagnostics.tx_frames),
        static_cast<unsigned long long>(diagnostics.no_frame_polls),
        static_cast<unsigned long long>(diagnostics.tx_full_events),
        static_cast<unsigned long long>(diagnostics.invalid_frame_rejects),
        static_cast<unsigned long long>(diagnostics.unsupported_feature_rejects),
        static_cast<unsigned long long>(diagnostics.open_failures),
        static_cast<unsigned long long>(diagnostics.io_errors));
}

}  // namespace

int main(const int argc, char* argv[]) {
    if (argc != 1 && argc != 3) {
        std::printf("usage: %s [rx_path tx_path]\n", argv[0]);
        return 2;
    }

    const char* const rx_path = argc == 3 ? argv[1] : kDefaultRxPath;
    const char* const tx_path = argc == 3 ? argv[2] : kDefaultTxPath;

    auto config = evsn::can_hal::CanOpenConfig{};
    config.mode = evsn::can_hal::CanMode::classic;
    if (!set_paths(config, rx_path, tx_path)) {
        return 2;
    }

    auto channel = evsn::can_hal::CanChannel{};
    const auto open_result = channel.open(config);
    if (!open_result.ok()) {
        print_result("open", open_result);
        print_diagnostics(channel.diagnostics());
        return 1;
    }

    const auto capabilities = channel.capabilities();
    std::printf(
        "opened rx=%s tx=%s backend=%s classic=%u fd=%u extended=%u remote=%u\n",
        rx_path,
        tx_path,
        EVSN_CAN_BACKEND_NAME,
        capabilities.classic_frame ? 1U : 0U,
        capabilities.fd_frame ? 1U : 0U,
        capabilities.extended_id ? 1U : 0U,
        capabilities.remote_frame ? 1U : 0U);

    if (!drain_rx(channel)) {
        static_cast<void>(channel.close());
        print_diagnostics(channel.diagnostics());
        return 1;
    }

    const auto standard_frame = make_frame(
        0x124U,
        CanIdFormat::standard_11_bit,
        std::array<std::uint8_t, 8U>{0x11U, 0x22U, 0x33U, 0x44U, 0x55U, 0x66U, 0x77U, 0x88U},
        8U);
    const auto extended_frame = make_frame(
        0x1234U,
        CanIdFormat::extended_29_bit,
        std::array<std::uint8_t, 8U>{0xAAU, 0xBBU, 0xCCU, 0xDDU, 0x00U, 0x00U, 0x00U, 0x00U},
        4U);

    if (!write_and_expect(channel, standard_frame) ||
        !write_and_expect(channel, extended_frame)) {
        static_cast<void>(channel.close());
        print_diagnostics(channel.diagnostics());
        return 1;
    }

    const auto close_result = channel.close();
    if (!close_result.ok()) {
        print_result("close", close_result);
        print_diagnostics(channel.diagnostics());
        return 1;
    }

    print_diagnostics(channel.diagnostics());
    std::printf("qnx_can_hal_vcan_smoke: passed\n");
    return 0;
}
