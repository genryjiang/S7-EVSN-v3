#include "evsn/can_drivers/j784_mcan_qnx/config.hpp"
#include "evsn/can_drivers/j784_mcan_qnx/j784_board_config.hpp"

#include <gtest/gtest.h>

namespace {

using evsn::can_drivers::j784_mcan_qnx::EndpointKind;
using evsn::can_drivers::j784_mcan_qnx::J784McanPhysicalInstance;
using evsn::can_drivers::j784_mcan_qnx::McanControllerConfig;
using evsn::can_drivers::j784_mcan_qnx::McanEndpointPath;
using evsn::can_drivers::j784_mcan_qnx::McanMode;
using evsn::can_drivers::j784_mcan_qnx::McanResourceManagerConfig;
using evsn::can_drivers::j784_mcan_qnx::McanStatus;

McanControllerConfig valid_classic_controller(const std::uint8_t instance_id) {
  auto config = McanControllerConfig{};
  config.instance_id = instance_id;
  config.rx_queue_count = 2U;
  config.tx_queue_count = 2U;
  config.rx_queue_capacity = 16U;
  config.tx_queue_capacity = 16U;
  config.mode = McanMode::classic;
  config.arbitration_bitrate_valid = true;
  config.arbitration_bitrate = 500'000U;
  return config;
}

McanControllerConfig valid_fd_controller(const std::uint8_t instance_id) {
  auto config = valid_classic_controller(instance_id);
  config.mode = McanMode::fd;
  config.brs_enabled = true;
  config.data_bitrate_valid = true;
  config.data_bitrate = 2'000'000U;
  return config;
}

void mark_hardware_evidence_complete(McanControllerConfig &config) {
  config.qnx_direct_ownership_confirmed = true;
  config.board_mapping_evidence_confirmed = true;
  config.target_startup_evidence_confirmed = true;
  config.bench_evidence_confirmed = true;
}

} // namespace

TEST(J784McanQnxConfigTest, BuildsBoundedEndpointPaths) {
  auto path = McanEndpointPath{};

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::make_endpoint_path(
                EndpointKind::rx, 2U, 1U, path),
            McanStatus::ok);
  EXPECT_STREQ(path.value.data(), "/dev/evsn/mcan2/rx1");

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::make_endpoint_path(
                EndpointKind::tx, 7U, 3U, path),
            McanStatus::ok);
  EXPECT_STREQ(path.value.data(), "/dev/evsn/mcan7/tx3");

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::make_endpoint_path(
                EndpointKind::ctl, 0U, 0U, path),
            McanStatus::ok);
  EXPECT_STREQ(path.value.data(), "/dev/evsn/mcan0/ctl");
}

TEST(J784McanQnxConfigTest,
     RejectsEndpointValuesOutsideBoundedInstanceAndQueueRange) {
  auto path = McanEndpointPath{};

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::make_endpoint_path(
                EndpointKind::rx, 8U, 0U, path),
            McanStatus::invalid_instance);
  EXPECT_EQ(path.value[0], '\0');

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::make_endpoint_path(
                EndpointKind::tx, 0U, 4U, path),
            McanStatus::invalid_queue);
  EXPECT_EQ(path.value[0], '\0');

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::make_endpoint_path(
                EndpointKind::ctl, 0U, 1U, path),
            McanStatus::invalid_queue);
}

TEST(J784McanQnxConfigTest,
     ValidatesSkeletonWithoutPretendingHardwareEvidenceExists) {
  const auto config = valid_fd_controller(1U);

  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::validate_controller_skeleton(config),
      McanStatus::ok);
  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::validate_controller_for_hardware_start(
          config),
      McanStatus::missing_hardware_evidence);
}

TEST(J784McanQnxConfigTest,
     AllowsHardwareStartOnlyAfterAllEvidenceFlagsArePresent) {
  auto config = valid_fd_controller(1U);
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::apply_j784_mcan_board_mapping(
                config, J784McanPhysicalInstance::mcu_mcan1),
            McanStatus::ok);
  mark_hardware_evidence_complete(config);

  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::validate_controller_for_hardware_start(
          config),
      McanStatus::ok);
}

TEST(J784McanQnxConfigTest,
     RejectsHardwareStartWhenEvidenceFlagsExistButMappingIsInvalid) {
  auto config = valid_fd_controller(1U);
  mark_hardware_evidence_complete(config);

  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::validate_controller_for_hardware_start(
          config),
      McanStatus::invalid_hardware_mapping);
}

TEST(J784McanQnxConfigTest, RejectsInvalidQueueShapesAndBitrates) {
  auto config = valid_classic_controller(0U);
  config.rx_queue_count = 0U;
  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::validate_controller_skeleton(config),
      McanStatus::invalid_queue);

  config = valid_classic_controller(0U);
  config.rx_queue_capacity = 3U;
  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::validate_controller_skeleton(config),
      McanStatus::invalid_queue_capacity);

  config = valid_classic_controller(0U);
  config.arbitration_bitrate = 0U;
  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::validate_controller_skeleton(config),
      McanStatus::invalid_bitrate);
}

TEST(J784McanQnxConfigTest, KeepsClassicAndFdModeRequirementsExplicit) {
  auto config = valid_classic_controller(0U);
  config.data_bitrate_valid = true;
  config.data_bitrate = 2'000'000U;
  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::validate_controller_skeleton(config),
      McanStatus::unsupported_mode);

  config = valid_fd_controller(0U);
  config.data_bitrate = 250'000U;
  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::validate_controller_skeleton(config),
      McanStatus::invalid_bitrate);

  config = valid_fd_controller(0U);
  config.mode = static_cast<McanMode>(99U);
  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::validate_controller_skeleton(config),
      McanStatus::unsupported_mode);
}

TEST(J784McanQnxConfigTest, RejectsDuplicateControllerInstances) {
  auto config = McanResourceManagerConfig{};
  config.controller_count = 2U;
  config.controllers[0] = valid_classic_controller(2U);
  config.controllers[1] = valid_fd_controller(2U);

  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::validate_resource_manager_skeleton(
          config),
      McanStatus::duplicate_instance);
}

TEST(J784McanQnxConfigTest,
     ValidatesResourceManagerHardwareStartEvidencePerController) {
  auto config = McanResourceManagerConfig{};
  config.controller_count = 2U;
  config.controllers[0] = valid_classic_controller(0U);
  config.controllers[1] = valid_fd_controller(1U);
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::apply_j784_mcan_board_mapping(
                config.controllers[0], J784McanPhysicalInstance::mcu_mcan0),
            McanStatus::ok);
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::apply_j784_mcan_board_mapping(
                config.controllers[1], J784McanPhysicalInstance::mcu_mcan1),
            McanStatus::ok);
  mark_hardware_evidence_complete(config.controllers[0]);

  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::validate_resource_manager_skeleton(
          config),
      McanStatus::ok);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                validate_resource_manager_hardware_start(config),
            McanStatus::missing_hardware_evidence);

  mark_hardware_evidence_complete(config.controllers[1]);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                validate_resource_manager_hardware_start(config),
            McanStatus::ok);
}
