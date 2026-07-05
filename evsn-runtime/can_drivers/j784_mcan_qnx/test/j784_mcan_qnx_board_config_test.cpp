#include "evsn/can_drivers/j784_mcan_qnx/j784_board_config.hpp"

#include <gtest/gtest.h>

#include <array>

namespace {

using evsn::can_drivers::j784_mcan_qnx::J784McanPhysicalInstance;
using evsn::can_drivers::j784_mcan_qnx::McanControllerConfig;
using evsn::can_drivers::j784_mcan_qnx::McanMode;
using evsn::can_drivers::j784_mcan_qnx::McanResourceManagerConfig;
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

void mark_production_evidence_complete(McanControllerConfig &config,
                                       const std::uint32_t logical_irq) {
  config.qnx_direct_ownership_confirmed = true;
  config.board_mapping_evidence_confirmed = true;
  config.target_startup_evidence_confirmed = true;
  config.transceiver_control_evidence_confirmed = true;
  config.qnx_irq_routing_evidence_confirmed = true;
  config.external_can_bench_evidence_confirmed = true;
  config.hardware.qnx_logical_irq = logical_irq;
}

void mark_all_production_evidence_complete(McanResourceManagerConfig &config) {
  for (auto index = std::size_t{0U}; index < config.controller_count; ++index) {
    mark_production_evidence_complete(config.controllers[index],
                                      800U + static_cast<std::uint32_t>(index));
  }
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
  EXPECT_EQ(mcu0->hardware.qnx_logical_irq, 0U);
  EXPECT_EQ(mcu0->ti_reference_evidence.gic_spi_line0, 864U);
  EXPECT_EQ(mcu0->ti_reference_evidence.gic_spi_line1, 865U);
  EXPECT_EQ(mcu0->ti_reference_evidence.mcan_clock_select_register_offset,
            0x8020U);
  EXPECT_TRUE(mcu0->ti_reference_evidence.ti_boot_device_tree_status_is_okay);
  EXPECT_STREQ(mcu0->ti_reference_evidence.ti_boot_device_tree_label,
               "&mcu_mcan0");
  EXPECT_STREQ(mcu0->ti_reference_evidence.ti_boot_pinmux_state_name,
               "mcu_mcan0_pins_default");
  EXPECT_STREQ(mcu0->ti_reference_evidence.ti_boot_transceiver_node_name,
               "transceiver1: can-phy0 ti,tcan1042");
  EXPECT_STREQ(mcu0->ti_reference_evidence.ti_boot_standby_control_description,
               "wkup_gpio0 69 GPIO_ACTIVE_HIGH standby");
  EXPECT_STREQ(mcu0->board_port_evidence.board_revision, "PROC141E5");
  EXPECT_STREQ(mcu0->board_port_evidence.linux_reference_device_tree_node_name,
               "can@40528000");
  EXPECT_STREQ(mcu0->board_port_evidence.linux_reference_can_interface_name,
               "mcu_mcan0");
  EXPECT_STREQ(mcu0->board_port_evidence.board_connector_name, "J42");
  EXPECT_TRUE(
      mcu0->board_port_evidence.linux_reference_device_tree_marks_enabled);
  EXPECT_FALSE(
      mcu0->board_port_evidence.ethernet_firmware_overlay_disables_path);

  const auto *const mcu1 =
      evsn::can_drivers::j784_mcan_qnx::find_j784_mcan_board_instance(
          J784McanPhysicalInstance::mcu_mcan1);
  ASSERT_NE(mcu1, nullptr);
  EXPECT_STREQ(mcu1->name, "MCU_MCAN1");
  EXPECT_EQ(mcu1->default_endpoint_instance_id, 1U);
  EXPECT_EQ(mcu1->hardware.message_ram.base_address, 0x40540000ULL);
  EXPECT_EQ(mcu1->hardware.tisci_device_id, 264U);
  EXPECT_EQ(mcu1->hardware.qnx_logical_irq, 0U);
  EXPECT_EQ(mcu1->ti_reference_evidence.gic_spi_line0, 867U);
  EXPECT_EQ(mcu1->ti_reference_evidence.gic_spi_line1, 868U);
  EXPECT_EQ(mcu1->ti_reference_evidence.mcan_clock_select_register_offset,
            0x8024U);
  EXPECT_TRUE(mcu1->ti_reference_evidence.ti_boot_device_tree_status_is_okay);
  EXPECT_STREQ(mcu1->ti_reference_evidence.ti_boot_device_tree_label,
               "&mcu_mcan1");
  EXPECT_STREQ(mcu1->ti_reference_evidence.ti_boot_pinmux_state_name,
               "mcu_mcan1_pins_default");
  EXPECT_STREQ(mcu1->ti_reference_evidence.ti_boot_transceiver_node_name,
               "transceiver2: can-phy1 ti,tcan1042");
  EXPECT_STREQ(mcu1->ti_reference_evidence.ti_boot_standby_control_description,
               "wkup_gpio0 2 GPIO_ACTIVE_HIGH standby");
  EXPECT_STREQ(mcu1->board_port_evidence.board_revision, "PROC141E5");
  EXPECT_STREQ(mcu1->board_port_evidence.linux_reference_device_tree_node_name,
               "can@40568000");
  EXPECT_STREQ(mcu1->board_port_evidence.linux_reference_can_interface_name,
               "mcu_mcan1");
  EXPECT_STREQ(mcu1->board_port_evidence.board_connector_name, "J43");
  EXPECT_TRUE(
      mcu1->board_port_evidence.linux_reference_device_tree_marks_enabled);
  EXPECT_FALSE(
      mcu1->board_port_evidence.ethernet_firmware_overlay_disables_path);

  const auto *const main16 =
      evsn::can_drivers::j784_mcan_qnx::find_j784_mcan_board_instance(
          J784McanPhysicalInstance::main_mcan16);
  ASSERT_NE(main16, nullptr);
  EXPECT_STREQ(main16->name, "MAIN_MCAN16");
  EXPECT_EQ(main16->default_endpoint_instance_id, 3U);
  EXPECT_EQ(main16->hardware.message_ram.base_address, 0x26A8000ULL);
  EXPECT_EQ(main16->hardware.tisci_device_id, 261U);
  EXPECT_EQ(main16->hardware.qnx_logical_irq, 0U);
  EXPECT_EQ(main16->ti_reference_evidence.gic_spi_line0, 816U);
  EXPECT_EQ(main16->ti_reference_evidence.gic_spi_line1, 817U);
  EXPECT_EQ(main16->ti_reference_evidence.mcan_clock_select_register_offset,
            0x84C0U);
  EXPECT_TRUE(main16->ti_reference_evidence.ti_boot_device_tree_status_is_okay);
  EXPECT_STREQ(main16->ti_reference_evidence.ti_boot_device_tree_label,
               "&main_mcan16");
  EXPECT_STREQ(main16->ti_reference_evidence.ti_boot_pinmux_state_name,
               "main_mcan16_pins_default");
  EXPECT_STREQ(main16->ti_reference_evidence.ti_boot_transceiver_node_name,
               "transceiver3: can-phy2 ti,tcan1042");
  EXPECT_STREQ(
      main16->ti_reference_evidence.ti_boot_standby_control_description,
      "standby pin grounded by default");
  EXPECT_STREQ(main16->board_port_evidence.board_revision, "PROC141E5");
  EXPECT_STREQ(
      main16->board_port_evidence.linux_reference_device_tree_node_name,
      "can@26a1000");
  EXPECT_STREQ(main16->board_port_evidence.linux_reference_can_interface_name,
               "main_mcan16");
  EXPECT_STREQ(main16->board_port_evidence.board_connector_name, "J46");
  EXPECT_TRUE(
      main16->board_port_evidence.linux_reference_device_tree_marks_enabled);
  EXPECT_FALSE(
      main16->board_port_evidence.ethernet_firmware_overlay_disables_path);

  const auto *const main4 =
      evsn::can_drivers::j784_mcan_qnx::find_j784_mcan_board_instance(
          J784McanPhysicalInstance::main_mcan4);
  ASSERT_NE(main4, nullptr);
  EXPECT_EQ(main4->hardware.qnx_logical_irq, 0U);
  EXPECT_EQ(main4->ti_reference_evidence.gic_spi_line0, 168U);
  EXPECT_EQ(main4->ti_reference_evidence.gic_spi_line1, 169U);
  EXPECT_EQ(main4->ti_reference_evidence.mcan_clock_select_register_offset,
            0x8490U);
  EXPECT_FALSE(main4->ti_reference_evidence.ti_boot_device_tree_status_is_okay);
  EXPECT_EQ(main4->ti_reference_evidence.ti_boot_device_tree_label, nullptr);
  EXPECT_EQ(main4->ti_reference_evidence.ti_boot_pinmux_state_name, nullptr);
  EXPECT_EQ(main4->ti_reference_evidence.ti_boot_transceiver_node_name,
            nullptr);
  EXPECT_EQ(main4->ti_reference_evidence.ti_boot_standby_control_description,
            nullptr);
  EXPECT_STREQ(main4->board_port_evidence.board_revision, "PROC141E5");
  EXPECT_STREQ(main4->board_port_evidence.linux_reference_device_tree_node_name,
               "can@2741000");
  EXPECT_STREQ(main4->board_port_evidence.linux_reference_can_interface_name,
               "main_mcan4");
  EXPECT_STREQ(main4->board_port_evidence.board_connector_name, "J45");
  EXPECT_TRUE(
      main4->board_port_evidence.linux_reference_device_tree_marks_enabled);
  EXPECT_TRUE(
      main4->board_port_evidence.ethernet_firmware_overlay_disables_path);
  ASSERT_NE(main4->board_port_evidence.ethernet_firmware_overlay_conflict_note,
            nullptr);
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
  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::validate_controller_hardware_mapping(
          config),
      McanStatus::ok);
  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::validate_controller_for_hardware_start(
          config),
      McanStatus::missing_hardware_evidence);
}

TEST(J784McanQnxBoardConfigTest, FindsMappingsByEndpointInstance) {
  const auto *const mapping = evsn::can_drivers::j784_mcan_qnx::
      find_j784_mcan_board_instance_by_endpoint(1U);
  ASSERT_NE(mapping, nullptr);
  EXPECT_EQ(mapping->physical_instance, J784McanPhysicalInstance::mcu_mcan1);

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                find_j784_mcan_board_instance_by_endpoint(7U),
            nullptr);
}

TEST(J784McanQnxBoardConfigTest,
     ProductionScopeMatchesLinuxSdkEnabledCanPaths) {
  const auto production_scope =
      evsn::can_drivers::j784_mcan_qnx::j784_mcan_production_scope_instances();

  ASSERT_EQ(production_scope.size(), 4U);
  EXPECT_EQ(production_scope[0], J784McanPhysicalInstance::mcu_mcan0);
  EXPECT_EQ(production_scope[1], J784McanPhysicalInstance::mcu_mcan1);
  EXPECT_EQ(production_scope[2], J784McanPhysicalInstance::main_mcan4);
  EXPECT_EQ(production_scope[3], J784McanPhysicalInstance::main_mcan16);

  const std::array<const char *, 4U> expected_connectors{"J42", "J43", "J45",
                                                         "J46"};
  for (auto index = std::size_t{0U}; index < production_scope.size(); ++index) {
    const auto *const instance =
        evsn::can_drivers::j784_mcan_qnx::find_j784_mcan_board_instance(
            production_scope[index]);
    ASSERT_NE(instance, nullptr);
    EXPECT_TRUE(instance->board_port_evidence
                    .linux_reference_device_tree_marks_enabled);
    EXPECT_STREQ(instance->board_port_evidence.board_revision, "PROC141E5");
    EXPECT_STREQ(instance->board_port_evidence.board_connector_name,
                 expected_connectors[index]);
  }
}

TEST(J784McanQnxBoardConfigTest,
     BuildsProductionResourceManagerConfigForAllLinuxSdkEnabledPaths) {
  auto config = McanResourceManagerConfig{};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                make_j784_mcan_production_resource_manager_config(config),
            McanStatus::ok);

  ASSERT_EQ(config.controller_count, 4U);
  for (auto index = std::size_t{0U}; index < config.controller_count; ++index) {
    EXPECT_EQ(config.controllers[index].instance_id, index);
    EXPECT_TRUE(config.controllers[index].board_mapping_evidence_confirmed);
    EXPECT_EQ(
        evsn::can_drivers::j784_mcan_qnx::validate_controller_hardware_mapping(
            config.controllers[index]),
        McanStatus::ok);
  }
}

TEST(J784McanQnxBoardConfigTest,
     ProductionScopeFailsClosedUntilAllQnxOwnershipEvidenceExists) {
  auto config = McanResourceManagerConfig{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::
                make_j784_mcan_production_resource_manager_config(config),
            McanStatus::ok);
  const auto policy = evsn::can_drivers::j784_mcan_qnx::
      j784_mcan_default_production_scope_policy();

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                validate_j784_mcan_production_ownership_scope(config, policy),
            McanStatus::missing_hardware_evidence);

  mark_all_production_evidence_complete(config);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                validate_j784_mcan_production_ownership_scope(config, policy),
            McanStatus::missing_hardware_evidence);

  auto resolved_policy = policy;
  resolved_policy.main_mcan4_ethfw_mux_conflict_resolved = true;
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                validate_j784_mcan_production_ownership_scope(config,
                                                              resolved_policy),
            McanStatus::ok);
}

TEST(J784McanQnxBoardConfigTest,
     ProductionScopeRejectsMissingRequiredLinuxSdkPath) {
  auto config = McanResourceManagerConfig{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::
                make_j784_mcan_production_resource_manager_config(config),
            McanStatus::ok);
  mark_all_production_evidence_complete(config);
  config.controller_count = 3U;

  auto policy = evsn::can_drivers::j784_mcan_qnx::
      j784_mcan_default_production_scope_policy();
  policy.main_mcan4_ethfw_mux_conflict_resolved = true;
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                validate_j784_mcan_production_ownership_scope(config, policy),
            McanStatus::missing_hardware_evidence);
}

TEST(J784McanQnxBoardConfigTest,
     ProductionScopeAllowsExplicitDescopedPathOnlyWithReason) {
  auto config = McanResourceManagerConfig{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::
                make_j784_mcan_production_resource_manager_config(config),
            McanStatus::ok);
  config.controllers[2] = config.controllers[3];
  config.controller_count = 3U;
  mark_all_production_evidence_complete(config);

  auto policy = evsn::can_drivers::j784_mcan_qnx::
      j784_mcan_default_production_scope_policy();
  policy.descoped_instances[2] = true;
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                validate_j784_mcan_production_ownership_scope(config, policy),
            McanStatus::invalid_argument);

  policy.descope_reasons[2] = "board/product decision excludes MAIN_MCAN4";
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                validate_j784_mcan_production_ownership_scope(config, policy),
            McanStatus::ok);
}
