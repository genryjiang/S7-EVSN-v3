#include "evsn/can_drivers/j784_mcan_qnx/mcan_core.hpp"

#include <gtest/gtest.h>

namespace {

using evsn::can_drivers::j784_mcan_qnx::McanBitTiming;
using evsn::can_drivers::j784_mcan_qnx::McanControllerConfig;
using evsn::can_drivers::j784_mcan_qnx::McanCore;
using evsn::can_drivers::j784_mcan_qnx::McanCoreConfig;
using evsn::can_drivers::j784_mcan_qnx::McanCoreRegister;
using evsn::can_drivers::j784_mcan_qnx::McanCoreState;
using evsn::can_drivers::j784_mcan_qnx::McanFrameFlag;
using evsn::can_drivers::j784_mcan_qnx::McanFrameTransfer;
using evsn::can_drivers::j784_mcan_qnx::McanMode;
using evsn::can_drivers::j784_mcan_qnx::McanStatus;

McanControllerConfig fd_controller() {
  auto config = McanControllerConfig{};
  config.instance_id = 0U;
  config.rx_queue_count = 1U;
  config.tx_queue_count = 1U;
  config.rx_queue_capacity = 4U;
  config.tx_queue_capacity = 4U;
  config.mode = McanMode::fd;
  config.brs_enabled = true;
  config.arbitration_bitrate_valid = true;
  config.arbitration_bitrate = 500'000U;
  config.data_bitrate_valid = true;
  config.data_bitrate = 2'000'000U;
  return config;
}

McanCoreConfig loopback_config() {
  auto config = McanCoreConfig{};
  config.controller = fd_controller();
  config.nominal_timing =
      evsn::can_drivers::j784_mcan_qnx::make_classic_500k_timing();
  config.data_timing = evsn::can_drivers::j784_mcan_qnx::make_fd_2m_timing();
  config.internal_loopback = true;
  config.interrupts_enabled = true;
  return config;
}

McanFrameTransfer fd_frame() {
  auto frame = McanFrameTransfer{};
  frame.can_id = 0x123U;
  frame.flags =
      evsn::can_drivers::j784_mcan_qnx::flag_value(McanFrameFlag::fd_frame) |
      evsn::can_drivers::j784_mcan_qnx::flag_value(McanFrameFlag::brs);
  frame.dlc = 9U;
  frame.length = 12U;
  frame.payload[0] = 0xA5U;
  frame.payload[11] = 0x5AU;
  return frame;
}

} // namespace

TEST(J784McanQnxCoreTest, ValidatesBitTimingWithExactClockRelationship) {
  auto timing = evsn::can_drivers::j784_mcan_qnx::make_classic_500k_timing();
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_bit_timing(timing),
            McanStatus::ok);

  timing.sync_jump_width = 3U;
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_bit_timing(timing),
            McanStatus::invalid_bitrate);

  timing = evsn::can_drivers::j784_mcan_qnx::make_classic_500k_timing();
  timing.source_clock_hz = 79'000'000U;
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_bit_timing(timing),
            McanStatus::invalid_bitrate);

  timing = evsn::can_drivers::j784_mcan_qnx::make_classic_500k_timing();
  timing.prescaler = 0x0400U;
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_bit_timing(timing),
            McanStatus::invalid_bitrate);
}

TEST(J784McanQnxCoreTest, RejectsTimingThatDoesNotMatchControllerBitrates) {
  auto config = loopback_config();
  config.nominal_timing.bitrate = 250'000U;
  auto core = McanCore{};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::initialize_core(core, config),
            McanStatus::invalid_bitrate);

  config = loopback_config();
  config.data_timing.bitrate = 4'000'000U;
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::initialize_core(core, config),
            McanStatus::invalid_bitrate);
}

TEST(J784McanQnxCoreTest, InitializesFakeRegisterModelWithoutHardwareEvidence) {
  auto core = McanCore{};
  const auto config = loopback_config();

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::initialize_core(core, config),
            McanStatus::ok);

  const auto snapshot = evsn::can_drivers::j784_mcan_qnx::snapshot_core(core);
  EXPECT_EQ(snapshot.state, McanCoreState::configured);
  EXPECT_NE(evsn::can_drivers::j784_mcan_qnx::read_core_register(
                core, McanCoreRegister::control),
            0U);
  EXPECT_NE(evsn::can_drivers::j784_mcan_qnx::read_core_register(
                core, McanCoreRegister::interrupt_enable),
            0U);
  EXPECT_EQ(snapshot.rx_queue_depth, 0U);
  EXPECT_EQ(snapshot.tx_queue_depth, 0U);
}

TEST(J784McanQnxCoreTest, RejectsFrameIoBeforeStart) {
  auto core = McanCore{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::initialize_core(
                core, loopback_config()),
            McanStatus::ok);

  auto frame = fd_frame();
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::transmit_frame(core, frame),
            McanStatus::invalid_state);
}

TEST(J784McanQnxCoreTest, TransmitsAndReceivesThroughInternalLoopback) {
  auto core = McanCore{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::initialize_core(
                core, loopback_config()),
            McanStatus::ok);
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::start_core(core), McanStatus::ok);

  const auto frame = fd_frame();
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::transmit_frame(core, frame),
            McanStatus::ok);

  auto snapshot = evsn::can_drivers::j784_mcan_qnx::snapshot_core(core);
  EXPECT_EQ(snapshot.tx_queue_depth, 1U);
  EXPECT_EQ(snapshot.rx_queue_depth, 1U);
  EXPECT_EQ(snapshot.diagnostics.tx_frames, 1U);
  EXPECT_EQ(snapshot.diagnostics.rx_frames, 1U);

  auto received = McanFrameTransfer{};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::receive_frame(core, received),
            McanStatus::ok);
  EXPECT_EQ(received.can_id, frame.can_id);
  EXPECT_EQ(received.length, frame.length);
  EXPECT_EQ(received.payload[0], 0xA5U);
  EXPECT_EQ(received.payload[11], 0x5AU);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::receive_frame(core, received),
            McanStatus::no_frame);
}

TEST(J784McanQnxCoreTest, EnforcesConfiguredQueueCapacity) {
  auto core = McanCore{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::initialize_core(
                core, loopback_config()),
            McanStatus::ok);
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::start_core(core), McanStatus::ok);

  const auto frame = fd_frame();
  for (auto index = std::size_t{0U}; index < 4U; ++index) {
    EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::transmit_frame(core, frame),
              McanStatus::ok);
  }
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::transmit_frame(core, frame),
            McanStatus::tx_queue_full);

  const auto snapshot = evsn::can_drivers::j784_mcan_qnx::snapshot_core(core);
  EXPECT_EQ(snapshot.tx_queue_depth, 4U);
  EXPECT_EQ(snapshot.diagnostics.tx_queue_full, 1U);
}

TEST(J784McanQnxCoreTest, RejectsInvalidRuntimeFramesWithoutMutatingQueues) {
  auto core = McanCore{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::initialize_core(
                core, loopback_config()),
            McanStatus::ok);
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::start_core(core), McanStatus::ok);

  auto frame = fd_frame();
  frame.dlc = 13U;
  frame.length = 24U;
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::transmit_frame(core, frame),
            McanStatus::invalid_argument);

  const auto snapshot = evsn::can_drivers::j784_mcan_qnx::snapshot_core(core);
  EXPECT_EQ(snapshot.tx_queue_depth, 0U);
  EXPECT_EQ(snapshot.rx_queue_depth, 0U);
  EXPECT_EQ(snapshot.diagnostics.invalid_frame_rejects, 1U);
}

TEST(J784McanQnxCoreTest, CanInjectBoundedReceivedFramesForFakeMmioTests) {
  auto config = loopback_config();
  config.internal_loopback = false;
  auto core = McanCore{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::initialize_core(core, config),
            McanStatus::ok);
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::start_core(core), McanStatus::ok);

  const auto frame = fd_frame();
  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::inject_received_frame(core, frame),
      McanStatus::ok);

  auto received = McanFrameTransfer{};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::receive_frame(core, received),
            McanStatus::ok);
  EXPECT_EQ(received.can_id, frame.can_id);
}
