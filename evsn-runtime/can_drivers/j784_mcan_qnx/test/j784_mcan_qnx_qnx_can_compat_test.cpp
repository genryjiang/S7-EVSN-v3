#include "evsn/can_drivers/j784_mcan_qnx/qnx_can_compat.hpp"

#include <cerrno>

#include <gtest/gtest.h>

namespace {

using evsn::can_drivers::j784_mcan_qnx::EndpointKind;
using evsn::can_drivers::j784_mcan_qnx::McanFrameFlag;
using evsn::can_drivers::j784_mcan_qnx::McanFrameTransfer;
using evsn::can_drivers::j784_mcan_qnx::McanMode;
using evsn::can_drivers::j784_mcan_qnx::McanResourceManagerConfig;
using evsn::can_drivers::j784_mcan_qnx::McanStatus;
using evsn::can_drivers::j784_mcan_qnx::QnxCanCompatCanMessage;
using evsn::can_drivers::j784_mcan_qnx::QnxCanCompatStartupConfig;

McanStatus parse_args(const char *const *args, const int argc,
                      QnxCanCompatStartupConfig &config) {
  return evsn::can_drivers::j784_mcan_qnx::parse_qnx_can_compat_startup(
      argc, args, config);
}

McanFrameTransfer classic_frame() {
  auto frame = McanFrameTransfer{};
  frame.can_id = 0x123U;
  frame.dlc = 8U;
  frame.length = 8U;
  for (auto index = std::uint8_t{0U}; index < frame.length; ++index) {
    frame.payload[index] = static_cast<std::uint8_t>(0xA0U + index);
  }
  return frame;
}

} // namespace

TEST(J784McanQnxCanCompatTest,
     BuildsClassicQnxCanEndpointPathsWithoutPrivateCtlAlias) {
  auto path = evsn::can_drivers::j784_mcan_qnx::McanEndpointPath{};

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::make_qnx_can_compat_endpoint_path(
                EndpointKind::rx, 1U, 0U, path),
            McanStatus::ok);
  EXPECT_STREQ(path.value.data(), "/dev/can1/rx0");

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::make_qnx_can_compat_endpoint_path(
                EndpointKind::tx, 2U, 3U, path),
            McanStatus::ok);
  EXPECT_STREQ(path.value.data(), "/dev/can2/tx3");

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::make_qnx_can_compat_endpoint_path(
                EndpointKind::ctl, 0U, 0U, path),
            McanStatus::unsupported_mode);

  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::make_qnx_can_compat_namespace_path(
          1U, path),
      McanStatus::ok);
  EXPECT_STREQ(path.value.data(), "/dev/can1");
}

TEST(J784McanQnxCanCompatTest, ParsesNxpStyleClassicRawLoopbackStartup) {
  const char *args[] = {"-R", "-t", "-b", "500K", "-n", "16", "can1,42,43"};
  auto config = QnxCanCompatStartupConfig{};

  ASSERT_EQ(parse_args(args, 7, config), McanStatus::ok);
  EXPECT_EQ(config.can_unit, 1U);
  EXPECT_EQ(config.irq_count, 2U);
  EXPECT_EQ(config.irqs[0], 42U);
  EXPECT_EQ(config.irqs[1], 43U);
  EXPECT_TRUE(config.raw_mode);
  EXPECT_TRUE(config.internal_loopback);
  EXPECT_FALSE(config.fd_mode);
  EXPECT_EQ(config.nominal_bitrate, 500'000U);
  EXPECT_EQ(config.mailbox_count, 16U);
}

TEST(J784McanQnxCanCompatTest,
     CarriesFirstPositionalIrqIntoControllerHardwareConfig) {
  const char *args[] = {"can1,42,43", "-R"};
  auto startup = QnxCanCompatStartupConfig{};
  ASSERT_EQ(parse_args(args, 2, startup), McanStatus::ok);

  auto config = McanResourceManagerConfig{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::
                qnx_can_compat_make_resource_manager_config(startup, config),
            McanStatus::ok);

  EXPECT_TRUE(config.controllers[0].qnx_irq_routing_evidence_confirmed);
  EXPECT_EQ(config.controllers[0].hardware.qnx_logical_irq, 42U);
}

TEST(J784McanQnxCanCompatTest,
     KeepsSdkDefaultIrqNonAuthorizingWhenStartupOmitsIrq) {
  const char *args[] = {"can1", "-R"};
  auto startup = QnxCanCompatStartupConfig{};
  ASSERT_EQ(parse_args(args, 2, startup), McanStatus::ok);

  auto config = McanResourceManagerConfig{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::
                qnx_can_compat_make_resource_manager_config(startup, config),
            McanStatus::ok);

  EXPECT_FALSE(config.controllers[0].qnx_irq_routing_evidence_confirmed);
  EXPECT_EQ(config.controllers[0].hardware.qnx_logical_irq, 0U);
}

TEST(J784McanQnxCanCompatTest, ParsesFdOptionsAndReducesDefaultMailboxCount) {
  const char *args[] = {"-f", "-R", "-b", "500K", "-d", "2M", "can0"};
  auto config = QnxCanCompatStartupConfig{};

  EXPECT_EQ(parse_args(args, 7, config), McanStatus::invalid_bitrate);

  const char *valid_args[] = {"-f", "-R", "-b", "500K", "-d", "5M", "can0"};
  ASSERT_EQ(parse_args(valid_args, 7, config), McanStatus::ok);
  EXPECT_TRUE(config.fd_mode);
  EXPECT_EQ(config.data_bitrate, 5'000'000U);
  EXPECT_EQ(config.mailbox_count,
            evsn::can_drivers::j784_mcan_qnx::kQnxCanCompatFdMailboxCount);

  const char *default_data_args[] = {"-f", "-R", "-b", "500K", "can0"};
  ASSERT_EQ(parse_args(default_data_args, 5, config), McanStatus::ok);
  EXPECT_TRUE(config.fd_mode);
  EXPECT_EQ(config.nominal_bitrate, 500'000U);
  EXPECT_EQ(config.data_bitrate, 50'000U);
}

TEST(J784McanQnxCanCompatTest, RejectsFdMailboxCountsAboveFdLayoutLimit) {
  const char *args[] = {"-f", "-n", "64", "can0"};
  auto config = QnxCanCompatStartupConfig{};

  EXPECT_EQ(parse_args(args, 4, config), McanStatus::invalid_queue_capacity);
}

TEST(J784McanQnxCanCompatTest, ValidatesAndCarriesRawTimingOverrides) {
  const char *args[] = {"-B", "0x9,0x6,0x5,0x1,0x0", "can0"};
  auto startup = QnxCanCompatStartupConfig{};
  ASSERT_EQ(parse_args(args, 3, startup), McanStatus::ok);
  EXPECT_TRUE(startup.nominal_raw_timing.valid);
  EXPECT_EQ(startup.nominal_bitrate, 500'000U);

  auto config = McanResourceManagerConfig{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::
                qnx_can_compat_make_resource_manager_config(startup, config),
            McanStatus::ok);
  EXPECT_TRUE(config.controllers[0].nominal_timing_override.valid);
  EXPECT_EQ(config.controllers[0].nominal_timing_override.prescaler, 10U);
  EXPECT_EQ(
      config.controllers[0].nominal_timing_override.time_segment_before_sample,
      13U);
  EXPECT_EQ(
      config.controllers[0].nominal_timing_override.time_segment_after_sample,
      2U);
}

TEST(J784McanQnxCanCompatTest,
     ConvertsClassicStandardMessagesThroughQnxMidLayout) {
  const auto frame = classic_frame();
  auto message = QnxCanCompatCanMessage{};

  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::qnx_can_compat_message_from_frame(
                frame, message),
            McanStatus::ok);
  EXPECT_EQ(message.message_id, 0x123U << 18U);
  EXPECT_FALSE(message.extended_id);
  EXPECT_EQ(message.length, 8U);

  auto decoded = McanFrameTransfer{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::qnx_can_compat_frame_from_message(
                message, decoded),
            McanStatus::ok);
  EXPECT_EQ(decoded.can_id, frame.can_id);
  EXPECT_EQ(decoded.length, frame.length);
  EXPECT_EQ(decoded.payload[7], frame.payload[7]);
}

TEST(J784McanQnxCanCompatTest, ConvertsExtendedAndRemoteClassicMessages) {
  auto frame = classic_frame();
  frame.flags =
      evsn::can_drivers::j784_mcan_qnx::flag_value(McanFrameFlag::extended_id);
  frame.can_id = 0x1ABCDEU;
  auto message = QnxCanCompatCanMessage{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::qnx_can_compat_message_from_frame(
                frame, message),
            McanStatus::ok);
  EXPECT_TRUE(message.extended_id);
  EXPECT_EQ(message.message_id, frame.can_id);

  frame = classic_frame();
  frame.flags =
      evsn::can_drivers::j784_mcan_qnx::flag_value(McanFrameFlag::remote);
  frame.length = 0U;
  frame.dlc = 8U;
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::qnx_can_compat_message_from_frame(
                frame, message),
            McanStatus::ok);
  EXPECT_TRUE(message.remote_frame);
  EXPECT_EQ(message.length, 8U);
}

TEST(J784McanQnxCanCompatTest,
     RejectsFdBrsAndOversizedFramesForClassicQnxRawAbi) {
  auto frame = classic_frame();
  frame.flags =
      evsn::can_drivers::j784_mcan_qnx::flag_value(McanFrameFlag::fd_frame);
  frame.dlc = 9U;
  frame.length = 12U;
  auto message = QnxCanCompatCanMessage{};

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::qnx_can_compat_message_from_frame(
                frame, message),
            McanStatus::unsupported_mode);

  message = QnxCanCompatCanMessage{};
  message.length = 9U;
  auto decoded = McanFrameTransfer{};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::qnx_can_compat_frame_from_message(
                message, decoded),
            McanStatus::invalid_argument);
}

TEST(J784McanQnxCanCompatTest, BuildsResourceManagerConfigFromStartupOptions) {
  const char *args[] = {"-f", "-R", "-t", "-b", "500K",
                        "-d", "5M", "-n", "16", "can1"};
  auto startup = QnxCanCompatStartupConfig{};
  ASSERT_EQ(parse_args(args, 10, startup), McanStatus::ok);

  auto config = McanResourceManagerConfig{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::
                qnx_can_compat_make_resource_manager_config(startup, config),
            McanStatus::ok);
  EXPECT_EQ(config.controller_count, 1U);
  EXPECT_EQ(config.controllers[0].instance_id, 1U);
  EXPECT_EQ(config.controllers[0].mode, McanMode::fd);
  EXPECT_TRUE(config.controllers[0].brs_enabled);
  EXPECT_EQ(config.controllers[0].rx_queue_capacity, 16U);
  EXPECT_EQ(config.controllers[0].tx_queue_capacity, 16U);
}

TEST(J784McanQnxCanCompatTest,
     BuildsFdResourceManagerConfigWithDocumentedDefaultDataBitrate) {
  const char *args[] = {"-f", "-R", "-b", "500K", "can1"};
  auto startup = QnxCanCompatStartupConfig{};
  ASSERT_EQ(parse_args(args, 5, startup), McanStatus::ok);

  auto config = McanResourceManagerConfig{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::
                qnx_can_compat_make_resource_manager_config(startup, config),
            McanStatus::ok);
  EXPECT_EQ(config.controllers[0].instance_id, 1U);
  EXPECT_EQ(config.controllers[0].mode, McanMode::fd);
  EXPECT_EQ(config.controllers[0].arbitration_bitrate, 500'000U);
  EXPECT_EQ(config.controllers[0].data_bitrate, 50'000U);
}

TEST(J784McanQnxCanCompatTest, MapsRawTxFullToDocumentedQnxErrno) {
  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::qnx_can_compat_raw_tx_status_to_errno(
          McanStatus::tx_queue_full),
      EAGAIN);
  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::qnx_can_compat_raw_tx_status_to_errno(
          McanStatus::invalid_queue),
      EINVAL);
}
