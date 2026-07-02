#include "evsn/can_hal/can_channel.hpp"

#include <array>
#include <cstdint>

#include <gtest/gtest.h>

namespace {

using evsn::can_hal::CanCapabilities;
using evsn::can_hal::CanFdFlags;
using evsn::can_hal::CanFrame;
using evsn::can_hal::CanFrameFormat;
using evsn::can_hal::CanFrameKind;
using evsn::can_hal::CanIdFormat;
using evsn::can_hal::CanStatus;

constexpr CanCapabilities all_capabilities() noexcept {
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

CanFrame classic_frame(const std::uint32_t id, const std::uint8_t length) {
    auto frame = CanFrame{};
    frame.id = id;
    frame.id_format = CanIdFormat::standard_11_bit;
    frame.frame_format = CanFrameFormat::classic;
    frame.kind = CanFrameKind::data;
    frame.dlc = length;
    frame.length = length;
    for (auto index = std::uint8_t{0U}; index < length; ++index) {
        frame.payload[index] = static_cast<std::uint8_t>(0xA0U + index);
    }
    return frame;
}

}  // namespace

TEST(CanHalValidationTest, ConvertsCanFdDlcLengths) {
    auto dlc = std::uint8_t{0U};
    auto length = std::uint8_t{0U};

    EXPECT_TRUE(evsn::can_hal::dlc_from_length(8U, dlc));
    EXPECT_EQ(dlc, 8U);
    EXPECT_TRUE(evsn::can_hal::dlc_from_length(12U, dlc));
    EXPECT_EQ(dlc, 9U);
    EXPECT_TRUE(evsn::can_hal::dlc_from_length(64U, dlc));
    EXPECT_EQ(dlc, 15U);
    EXPECT_FALSE(evsn::can_hal::dlc_from_length(9U, dlc));

    EXPECT_TRUE(evsn::can_hal::length_from_dlc(9U, length));
    EXPECT_EQ(length, 12U);
    EXPECT_FALSE(evsn::can_hal::length_from_dlc(16U, length));
}

TEST(CanHalValidationTest, AcceptsValidClassicStandardFrame) {
    const auto frame = classic_frame(0x321U, 8U);
    const auto result = evsn::can_hal::validate_frame(frame, all_capabilities());

    EXPECT_TRUE(result.ok());
}

TEST(CanHalValidationTest, RejectsOutOfRangeStandardIdentifier) {
    auto frame = classic_frame(0x800U, 8U);
    frame.id_format = CanIdFormat::standard_11_bit;
    const auto result = evsn::can_hal::validate_frame(frame, all_capabilities());

    EXPECT_EQ(result.status, CanStatus::invalid_frame);
}

TEST(CanHalValidationTest, RejectsOutOfRangeExtendedIdentifier) {
    auto frame = classic_frame(0x20000000U, 8U);
    frame.id_format = CanIdFormat::extended_29_bit;
    const auto result = evsn::can_hal::validate_frame(frame, all_capabilities());

    EXPECT_EQ(result.status, CanStatus::invalid_frame);
}

TEST(CanHalValidationTest, RejectsClassicLengthAboveEightBytes) {
    auto frame = classic_frame(0x123U, 8U);
    frame.dlc = 9U;
    frame.length = 9U;
    const auto result = evsn::can_hal::validate_frame(frame, all_capabilities());

    EXPECT_EQ(result.status, CanStatus::invalid_frame);
}

TEST(CanHalValidationTest, AcceptsCanFdDlcRepresentableLengths) {
    auto frame = classic_frame(0x123U, 8U);
    frame.frame_format = CanFrameFormat::fd;
    frame.dlc = 13U;
    frame.length = 32U;
    frame.fd_flags = CanFdFlags{true, true, true};
    const auto result = evsn::can_hal::validate_frame(frame, all_capabilities());

    EXPECT_TRUE(result.ok());
}

TEST(CanHalValidationTest, RejectsUnsupportedCanFdFrame) {
    auto caps = all_capabilities();
    caps.fd_frame = false;
    auto frame = classic_frame(0x123U, 8U);
    frame.frame_format = CanFrameFormat::fd;
    frame.dlc = 9U;
    frame.length = 12U;
    const auto result = evsn::can_hal::validate_frame(frame, caps);

    EXPECT_EQ(result.status, CanStatus::unsupported);
}

TEST(CanHalValidationTest, RemoteFramesAreClassicOnlyAndCarryNoPayload) {
    auto frame = classic_frame(0x123U, 0U);
    frame.kind = CanFrameKind::remote;
    frame.dlc = 8U;
    frame.length = 0U;
    EXPECT_TRUE(evsn::can_hal::validate_frame(frame, all_capabilities()).ok());

    frame.length = 1U;
    EXPECT_EQ(
        evsn::can_hal::validate_frame(frame, all_capabilities()).status,
        CanStatus::invalid_frame);
}

TEST(CanHalValidationTest, ClassicAndRemoteFramesRejectFdFlags) {
    auto classic = classic_frame(0x123U, 8U);
    classic.fd_flags.brs = true;
    EXPECT_EQ(
        evsn::can_hal::validate_frame(classic, all_capabilities()).status,
        CanStatus::unsupported);

    auto remote = classic_frame(0x124U, 0U);
    remote.kind = CanFrameKind::remote;
    remote.dlc = 8U;
    remote.length = 0U;
    remote.fd_flags.fdf = true;
    EXPECT_EQ(
        evsn::can_hal::validate_frame(remote, all_capabilities()).status,
        CanStatus::unsupported);
}

TEST(CanHalValidationTest, CopiesBoundedEndpointPaths) {
    auto config = evsn::can_hal::CanOpenConfig{};

    EXPECT_TRUE(evsn::can_hal::set_rx_path(config, "/dev/can0/rx0").ok());
    EXPECT_TRUE(evsn::can_hal::set_tx_path(config, "/dev/can0/tx0").ok());
    EXPECT_STREQ(config.rx_path.data(), "/dev/can0/rx0");
    EXPECT_STREQ(config.tx_path.data(), "/dev/can0/tx0");
    EXPECT_FALSE(evsn::can_hal::path_is_empty(config.rx_path));
    EXPECT_EQ(
        evsn::can_hal::set_rx_path(config, nullptr).status,
        CanStatus::invalid_argument);
}
