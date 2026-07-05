#include "evsn/can_drivers/j784_mcan_qnx/resource_manager_runtime.hpp"

#include <cerrno>

#include <gtest/gtest.h>

namespace {

using evsn::can_drivers::j784_mcan_qnx::EndpointKind;
using evsn::can_drivers::j784_mcan_qnx::McanCapabilityFlag;
using evsn::can_drivers::j784_mcan_qnx::McanControllerConfig;
using evsn::can_drivers::j784_mcan_qnx::McanControllerCapabilities;
using evsn::can_drivers::j784_mcan_qnx::McanDevctlCommand;
using evsn::can_drivers::j784_mcan_qnx::McanDiagnosticsSnapshot;
using evsn::can_drivers::j784_mcan_qnx::McanFrameFlag;
using evsn::can_drivers::j784_mcan_qnx::McanFrameTransfer;
using evsn::can_drivers::j784_mcan_qnx::McanMode;
using evsn::can_drivers::j784_mcan_qnx::McanResourceManagerConfig;
using evsn::can_drivers::j784_mcan_qnx::McanResourceManagerRuntime;
using evsn::can_drivers::j784_mcan_qnx::McanResourceManagerRuntimeOptions;
using evsn::can_drivers::j784_mcan_qnx::McanStatus;

McanControllerConfig controller_config() {
  auto controller = McanControllerConfig{};
  controller.instance_id = 0U;
  controller.rx_queue_count = 1U;
  controller.tx_queue_count = 1U;
  controller.rx_queue_capacity = 4U;
  controller.tx_queue_capacity = 4U;
  controller.mode = McanMode::fd;
  controller.brs_enabled = true;
  controller.arbitration_bitrate_valid = true;
  controller.arbitration_bitrate = 500'000U;
  controller.data_bitrate_valid = true;
  controller.data_bitrate = 2'000'000U;
  return controller;
}

McanResourceManagerConfig runtime_config() {
  auto config = McanResourceManagerConfig{};
  config.controller_count = 1U;
  config.controllers[0] = controller_config();
  return config;
}

McanFrameTransfer fd_frame() {
  auto frame = McanFrameTransfer{};
  frame.can_id = 0x321U;
  frame.flags =
      evsn::can_drivers::j784_mcan_qnx::flag_value(McanFrameFlag::fd_frame) |
      evsn::can_drivers::j784_mcan_qnx::flag_value(McanFrameFlag::brs);
  frame.dlc = 9U;
  frame.length = 12U;
  frame.payload[0] = 0xC0U;
  frame.payload[11] = 0xDEU;
  return frame;
}

McanResourceManagerRuntime initialized_loopback_runtime() {
  auto runtime = McanResourceManagerRuntime{};
  auto options = McanResourceManagerRuntimeOptions{};
  options.frame_io_enabled = true;
  options.internal_loopback = true;
  options.interrupts_enabled = true;
  options.complete_tx_immediately = true;
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                initialize_resource_manager_runtime(runtime, runtime_config(),
                                                    options),
            McanStatus::ok);
  return runtime;
}

} // namespace

TEST(J784McanQnxResourceManagerRuntimeTest,
     KeepsFrameIoGatedUntilBackendModeEnablesIt) {
  auto runtime = McanResourceManagerRuntime{};
  auto options = McanResourceManagerRuntimeOptions{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::
                initialize_resource_manager_runtime(runtime, runtime_config(),
                                                    options),
            McanStatus::ok);

  const auto frame = fd_frame();
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                resource_manager_transmit_frame(
                    runtime, EndpointKind::tx, 0U, 0U, frame),
            McanStatus::hardware_access_unavailable);

  auto rx_frame = McanFrameTransfer{};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                resource_manager_receive_frame(runtime, EndpointKind::rx, 0U,
                                               0U, rx_frame),
            McanStatus::hardware_access_unavailable);
}

TEST(J784McanQnxResourceManagerRuntimeTest,
     DefinesStableQnxDevctlCommandNumbers) {
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                make_mcan_devctl_from_command(
                    McanDevctlCommand::query_capabilities,
                    evsn::can_drivers::j784_mcan_qnx::
                        kMcanControllerCapabilitiesStructSize),
            evsn::can_drivers::j784_mcan_qnx::
                kMcanDevctlQueryCapabilities);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                kMcanDevctlQueryCapabilities,
            0x40144D01U);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::kMcanDevctlQueryDiagnostics,
            0x40484D03U);
}

TEST(J784McanQnxResourceManagerRuntimeTest,
     InitializesBoundedControllerRuntimeAndCapabilities) {
  const auto runtime = initialized_loopback_runtime();

  auto capabilities = McanControllerCapabilities{};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                resource_manager_query_capabilities(
                    runtime, EndpointKind::ctl, 0U, capabilities),
            McanStatus::ok);
  EXPECT_TRUE(evsn::can_drivers::j784_mcan_qnx::has_capability(
      capabilities.flags, McanCapabilityFlag::classic_frame));
  EXPECT_TRUE(evsn::can_drivers::j784_mcan_qnx::has_capability(
      capabilities.flags, McanCapabilityFlag::fd_frame));
  EXPECT_TRUE(evsn::can_drivers::j784_mcan_qnx::has_capability(
      capabilities.flags, McanCapabilityFlag::internal_loopback));
  EXPECT_EQ(capabilities.max_rx_queue_capacity, 4U);
  EXPECT_EQ(capabilities.max_tx_queue_capacity, 4U);
}

TEST(J784McanQnxResourceManagerRuntimeTest,
     DerivesExactBitTimingForAcceptedArbitrationRates) {
  auto config = runtime_config();
  config.controllers[0].mode = McanMode::classic;
  config.controllers[0].brs_enabled = false;
  config.controllers[0].arbitration_bitrate = 250'000U;
  config.controllers[0].data_bitrate_valid = false;
  config.controllers[0].data_bitrate = 0U;

  auto runtime = McanResourceManagerRuntime{};
  auto options = McanResourceManagerRuntimeOptions{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::
                initialize_resource_manager_runtime(runtime, config, options),
            McanStatus::ok);

  EXPECT_EQ(runtime.cores[0].config.nominal_timing.source_clock_hz,
            80'000'000U);
  EXPECT_EQ(runtime.cores[0].config.nominal_timing.bitrate, 250'000U);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_bit_timing(
                runtime.cores[0].config.nominal_timing),
            McanStatus::ok);
}

TEST(J784McanQnxResourceManagerRuntimeTest,
     RejectsMultiQueueRuntimeUntilQueuesAreIsolated) {
  auto config = runtime_config();
  config.controllers[0].rx_queue_count = 2U;

  auto runtime = McanResourceManagerRuntime{};
  auto options = McanResourceManagerRuntimeOptions{};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                initialize_resource_manager_runtime(runtime, config, options),
            McanStatus::unsupported_mode);
}

TEST(J784McanQnxResourceManagerRuntimeTest,
     WritesTxAndReadsRxThroughInternalLoopback) {
  auto runtime = initialized_loopback_runtime();
  const auto tx_frame = fd_frame();

  auto ready_count = std::size_t{1U};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                resource_manager_rx_ready_count(
                    runtime, EndpointKind::rx, 0U, 0U, ready_count),
            McanStatus::ok);
  EXPECT_EQ(ready_count, 0U);

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                resource_manager_transmit_frame(
                    runtime, EndpointKind::tx, 0U, 0U, tx_frame),
            McanStatus::ok);

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                resource_manager_rx_ready_count(
                    runtime, EndpointKind::rx, 0U, 0U, ready_count),
            McanStatus::ok);
  EXPECT_EQ(ready_count, 1U);

  auto room_count = std::size_t{0U};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                resource_manager_tx_room_count(
                    runtime, EndpointKind::tx, 0U, 0U, room_count),
            McanStatus::ok);
  EXPECT_EQ(room_count, 4U);

  auto rx_frame = McanFrameTransfer{};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                resource_manager_receive_frame(runtime, EndpointKind::rx, 0U,
                                               0U, rx_frame),
            McanStatus::ok);
  EXPECT_EQ(rx_frame.can_id, tx_frame.can_id);
  EXPECT_EQ(rx_frame.flags, tx_frame.flags);
  EXPECT_EQ(rx_frame.length, tx_frame.length);
  EXPECT_EQ(rx_frame.payload[0], 0xC0U);
  EXPECT_EQ(rx_frame.payload[11], 0xDEU);

  auto diagnostics = McanDiagnosticsSnapshot{};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                resource_manager_query_diagnostics(
                    runtime, EndpointKind::ctl, 0U, diagnostics),
            McanStatus::ok);
  EXPECT_EQ(diagnostics.tx_frames, 1U);
  EXPECT_EQ(diagnostics.rx_frames, 1U);
  EXPECT_EQ(diagnostics.tx_queue_full, 0U);
}

TEST(J784McanQnxResourceManagerRuntimeTest,
     RejectsWrongEndpointDirectionAndInvalidQueues) {
  auto runtime = initialized_loopback_runtime();
  const auto frame = fd_frame();

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                resource_manager_transmit_frame(
                    runtime, EndpointKind::rx, 0U, 0U, frame),
            McanStatus::invalid_argument);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                resource_manager_transmit_frame(
                    runtime, EndpointKind::tx, 0U, 1U, frame),
            McanStatus::invalid_queue);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                resource_manager_transmit_frame(
                    runtime, EndpointKind::tx, 7U, 0U, frame),
            McanStatus::invalid_instance);

  auto rx_frame = McanFrameTransfer{};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                resource_manager_receive_frame(runtime, EndpointKind::tx, 0U,
                                               0U, rx_frame),
            McanStatus::invalid_argument);
}

TEST(J784McanQnxResourceManagerRuntimeTest, MapsRuntimeStatusesToErrnoValues) {
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                resource_manager_status_to_errno(McanStatus::ok),
            0);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                resource_manager_status_to_errno(McanStatus::no_frame),
            EAGAIN);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                resource_manager_status_to_errno(
                    McanStatus::unsupported_mode),
            ENOTSUP);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                resource_manager_status_to_errno(
                    McanStatus::invalid_hardware_mapping),
            ENODEV);
}
