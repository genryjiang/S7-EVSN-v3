#include "evsn/can_hal/fake_can_backend.hpp"

#include <cstdint>

#include <gtest/gtest.h>

namespace {

evsn::can_hal::CanFrame make_frame(const std::uint32_t id, const std::uint8_t byte) {
    auto frame = evsn::can_hal::CanFrame{};
    frame.id = id;
    frame.id_format = evsn::can_hal::CanIdFormat::standard_11_bit;
    frame.frame_format = evsn::can_hal::CanFrameFormat::classic;
    frame.kind = evsn::can_hal::CanFrameKind::data;
    frame.dlc = 8U;
    frame.length = 8U;
    frame.payload[0] = byte;
    return frame;
}

evsn::can_hal::CanOpenConfig fake_config() {
    auto config = evsn::can_hal::CanOpenConfig{};
    (void)evsn::can_hal::set_rx_path(config, "fake-rx");
    (void)evsn::can_hal::set_tx_path(config, "fake-tx");
    return config;
}

}  // namespace

TEST(CanHalFakeBackendTest, OpensAndReportsCapabilities) {
    auto channel = evsn::can_hal::CanChannel{};
    const auto open_result = channel.open(fake_config());

    ASSERT_TRUE(open_result.ok());
    const auto caps = channel.capabilities();
    EXPECT_TRUE(caps.classic_frame);
    EXPECT_TRUE(caps.fd_frame);
    EXPECT_TRUE(caps.extended_id);
    EXPECT_TRUE(caps.remote_frame);
    EXPECT_EQ(channel.diagnostics().opens, 1U);
}

TEST(CanHalFakeBackendTest, InjectsRxAndRecordsTxDeterministically) {
    auto channel = evsn::can_hal::CanChannel{};
    auto control = evsn::can_hal::FakeCanBackendControl{channel};
    ASSERT_TRUE(channel.open(fake_config()).ok());
    ASSERT_TRUE(control.inject_rx(make_frame(0x120U, 0xA1U)).ok());

    auto received = evsn::can_hal::CanFrame{};
    ASSERT_TRUE(channel.read(received).ok());
    EXPECT_EQ(received.id, 0x120U);
    EXPECT_EQ(received.payload[0], 0xA1U);
    EXPECT_EQ(channel.diagnostics().rx_frames, 1U);

    ASSERT_TRUE(channel.write(make_frame(0x121U, 0xB2U)).ok());
    EXPECT_EQ(control.transmitted_count(), 1U);

    auto transmitted = evsn::can_hal::CanFrame{};
    ASSERT_TRUE(control.transmitted_frame(0U, transmitted).ok());
    EXPECT_EQ(transmitted.id, 0x121U);
    EXPECT_EQ(transmitted.payload[0], 0xB2U);
    EXPECT_EQ(channel.diagnostics().tx_frames, 1U);
}

TEST(CanHalFakeBackendTest, EmptyReceiveReturnsNoFrame) {
    auto channel = evsn::can_hal::CanChannel{};
    ASSERT_TRUE(channel.open(fake_config()).ok());

    auto frame = evsn::can_hal::CanFrame{};
    const auto result = channel.read(frame);

    EXPECT_EQ(result.status, evsn::can_hal::CanStatus::no_frame);
    EXPECT_EQ(channel.diagnostics().no_frame_polls, 1U);
}

TEST(CanHalFakeBackendTest, ForcedStatusesUpdateDiagnostics) {
    auto channel = evsn::can_hal::CanChannel{};
    auto control = evsn::can_hal::FakeCanBackendControl{channel};
    ASSERT_TRUE(channel.open(fake_config()).ok());

    control.force_write_status(evsn::can_hal::CanStatus::tx_full, 11);
    const auto result = channel.write(make_frame(0x122U, 0xC3U));

    EXPECT_EQ(result.status, evsn::can_hal::CanStatus::tx_full);
    EXPECT_EQ(result.native_error, 11);
    EXPECT_EQ(channel.diagnostics().tx_full_events, 1U);
}

TEST(CanHalFakeBackendTest, RejectsWriteBeforeOpen) {
    auto channel = evsn::can_hal::CanChannel{};

    const auto result = channel.write(make_frame(0x123U, 0xD4U));

    EXPECT_EQ(result.status, evsn::can_hal::CanStatus::not_open);
}

TEST(CanHalFakeBackendTest, RejectsInvalidTransmittedIndex) {
    auto channel = evsn::can_hal::CanChannel{};
    auto control = evsn::can_hal::FakeCanBackendControl{channel};
    ASSERT_TRUE(channel.open(fake_config()).ok());

    auto frame = evsn::can_hal::CanFrame{};
    const auto result = control.transmitted_frame(0U, frame);

    EXPECT_EQ(result.status, evsn::can_hal::CanStatus::invalid_argument);
}
