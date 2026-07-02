#include "evsn/can_drivers/j784_mcan_qnx/j784_board_config.hpp"

#include <gtest/gtest.h>

namespace {

using evsn::can_drivers::j784_mcan_qnx::J784McanPhysicalInstance;
using evsn::can_drivers::j784_mcan_qnx::McanControllerConfig;
using evsn::can_drivers::j784_mcan_qnx::McanMode;
using evsn::can_drivers::j784_mcan_qnx::McanStatus;

McanControllerConfig fd_controller() {
  auto config = McanControllerConfig{};
  config.mode = McanMode::fd;
  config.brs_enabled = true;
  config.arbitration_bitrate_valid = true;
  config.arbitration_bitrate = 500'000U;
  config.data_bitrate_valid = true;
  config.data_bitrate = 2'000'000U;
  return config;
}

} // namespace

TEST(J784McanQnxBoardConfigTest, ExposesSdkBackedDefaultInstances) {
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::j784_mcan_board_instance_count(),
            4U);

  const auto *const mcu0 =
      evsn::can_drivers::j784_mcan_qnx::find_j784_mcan_board_instance(
          J784McanPhysicalInstance::mcu_mcan0);
  ASSERT_NE(mcu0, nullptr);
  EXPECT_STREQ(mcu0->name, "MCU_MCAN0");
  EXPECT_EQ(mcu0->default_endpoint_instance_id, 0U);
  EXPECT_EQ(mcu0->hardware.message_ram.base_address, 0x40500000ULL);
  EXPECT_EQ(mcu0->hardware.subsystem.base_address, 0x40520000ULL);
  EXPECT_EQ(mcu0->hardware.controller.base_address, 0x40528000ULL);
  EXPECT_EQ(mcu0->hardware.tisci_device_id, 263U);

  const auto *const main16 =
      evsn::can_drivers::j784_mcan_qnx::find_j784_mcan_board_instance(
          J784McanPhysicalInstance::main_mcan16);
  ASSERT_NE(main16, nullptr);
  EXPECT_STREQ(main16->name, "MAIN_MCAN16");
  EXPECT_EQ(main16->default_endpoint_instance_id, 3U);
  EXPECT_EQ(main16->hardware.message_ram.base_address, 0x26A8000ULL);
  EXPECT_EQ(main16->hardware.tisci_device_id, 261U);
}

TEST(J784McanQnxBoardConfigTest,
     AppliesBoardMappingWithoutPretendingBenchEvidenceExists) {
  auto config = fd_controller();
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::apply_j784_mcan_board_mapping(
                config, J784McanPhysicalInstance::main_mcan4),
            McanStatus::ok);

  EXPECT_EQ(config.instance_id, 2U);
  EXPECT_TRUE(config.board_mapping_evidence_confirmed);
  EXPECT_EQ(config.hardware.controller.base_address, 0x2741000ULL);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                validate_controller_hardware_mapping(config),
            McanStatus::ok);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                validate_controller_for_hardware_start(config),
            McanStatus::missing_hardware_evidence);
}

TEST(J784McanQnxBoardConfigTest, FindsMappingsByEndpointInstance) {
  const auto *const mapping =
      evsn::can_drivers::j784_mcan_qnx::
          find_j784_mcan_board_instance_by_endpoint(1U);
  ASSERT_NE(mapping, nullptr);
  EXPECT_EQ(mapping->physical_instance, J784McanPhysicalInstance::mcu_mcan1);

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                find_j784_mcan_board_instance_by_endpoint(7U),
            nullptr);
}
