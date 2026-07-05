#include "evsn/can_drivers/j784_mcan_qnx/j784_board_config.hpp"

#include <array>

namespace evsn::can_drivers::j784_mcan_qnx {
namespace {

constexpr McanHardwareRegion region(const std::uint64_t base,
                                    const std::uint32_t size) noexcept {
  return McanHardwareRegion{base, size};
}

constexpr McanHardwareInstanceConfig
hardware(const std::uint64_t subsystem_base,
         const std::uint64_t controller_base,
         const std::uint64_t message_ram_base,
         const std::uint32_t tisci_device_id) noexcept {
  return McanHardwareInstanceConfig{
      region(subsystem_base, 0x100U), region(controller_base, 0x200U),
      region(message_ram_base, 0x8000U), tisci_device_id, 0U};
}

constexpr J784McanTiReferenceEvidence
ti_reference_evidence(const char *const ti_boot_device_tree_label,
                      const char *const ti_boot_pinmux_state_name,
                      const char *const ti_boot_transceiver_node_name,
                      const char *const ti_boot_standby_control_description,
                      const std::uint32_t gic_spi_line0,
                      const std::uint32_t gic_spi_line1,
                      const std::uint32_t mcan_clock_select_register_offset,
                      const bool ti_boot_device_tree_status_is_okay) noexcept {
  return J784McanTiReferenceEvidence{ti_boot_device_tree_label,
                                     ti_boot_pinmux_state_name,
                                     ti_boot_transceiver_node_name,
                                     ti_boot_standby_control_description,
                                     gic_spi_line0,
                                     gic_spi_line1,
                                     mcan_clock_select_register_offset,
                                     ti_boot_device_tree_status_is_okay};
}

constexpr J784McanBoardPortEvidence board_port_evidence(
    const char *const linux_reference_device_tree_node_name,
    const char *const linux_reference_device_tree_status,
    const char *const linux_reference_can_interface_name,
    const char *const board_connector_name,
    const char *const ti_pdk_external_loopback_reference,
    const char *const ethernet_firmware_overlay_conflict_note,
    const bool linux_reference_device_tree_marks_enabled,
    const bool ethernet_firmware_overlay_disables_path) noexcept {
  return J784McanBoardPortEvidence{"PROC141E5",
                                   linux_reference_device_tree_node_name,
                                   linux_reference_device_tree_status,
                                   linux_reference_can_interface_name,
                                   board_connector_name,
                                   ti_pdk_external_loopback_reference,
                                   ethernet_firmware_overlay_conflict_note,
                                   linux_reference_device_tree_marks_enabled,
                                   ethernet_firmware_overlay_disables_path};
}

constexpr auto kInstances = std::array<J784McanBoardInstance, 4U>{
    J784McanBoardInstance{
        J784McanPhysicalInstance::mcu_mcan0, "MCU_MCAN0", 0U,
        hardware(0x40520000ULL, 0x40528000ULL, 0x40500000ULL, 263U),
        ti_reference_evidence("&mcu_mcan0", "mcu_mcan0_pins_default",
                              "transceiver1: can-phy0 ti,tcan1042",
                              "wkup_gpio0 69 GPIO_ACTIVE_HIGH standby", 864U,
                              865U, 0x8020U, true),
        board_port_evidence("can@40528000", "okay", "mcu_mcan0", "J42",
                            "PDK external loopback pair MCU_MCAN0/MCU_MCAN1 "
                            "on J42/J43",
                            nullptr, true, false),
        true, false},
    J784McanBoardInstance{
        J784McanPhysicalInstance::mcu_mcan1, "MCU_MCAN1", 1U,
        hardware(0x40560000ULL, 0x40568000ULL, 0x40540000ULL, 264U),
        ti_reference_evidence("&mcu_mcan1", "mcu_mcan1_pins_default",
                              "transceiver2: can-phy1 ti,tcan1042",
                              "wkup_gpio0 2 GPIO_ACTIVE_HIGH standby", 867U,
                              868U, 0x8024U, true),
        board_port_evidence("can@40568000", "okay", "mcu_mcan1", "J43",
                            "PDK external loopback pair MCU_MCAN0/MCU_MCAN1 "
                            "on J42/J43",
                            nullptr, true, false),
        true, false},
    J784McanBoardInstance{
        J784McanPhysicalInstance::main_mcan4, "MAIN_MCAN4", 2U,
        hardware(0x2740000ULL, 0x2741000ULL, 0x2748000ULL, 249U),
        ti_reference_evidence(nullptr, nullptr, nullptr, nullptr, 168U, 169U,
                              0x8490U, false),
        board_port_evidence("can@2741000", "okay", "main_mcan4", "J45",
                            "PDK external loopback pair MAIN_MCAN4/"
                            "MAIN_MCAN16 on J45/J46",
                            "k3-j784s4-evm-ethfw.dtbo disables MAIN_MCAN4, "
                            "transceiver3, and mux1",
                            true, true),
        true, false},
    J784McanBoardInstance{
        J784McanPhysicalInstance::main_mcan16, "MAIN_MCAN16", 3U,
        hardware(0x26A0000ULL, 0x26A1000ULL, 0x26A8000ULL, 261U),
        ti_reference_evidence("&main_mcan16", "main_mcan16_pins_default",
                              "transceiver3: can-phy2 ti,tcan1042",
                              "standby pin grounded by default", 816U, 817U,
                              0x84C0U, true),
        board_port_evidence("can@26a1000", "okay", "main_mcan16", "J46",
                            "PDK external loopback pair MAIN_MCAN4/"
                            "MAIN_MCAN16 on J45/J46",
                            nullptr, true, false),
        true, false},
};

constexpr auto kProductionScopeInstances =
    std::array<J784McanPhysicalInstance, kJ784McanProductionScopeInstanceCount>{
        J784McanPhysicalInstance::mcu_mcan0,
        J784McanPhysicalInstance::mcu_mcan1,
        J784McanPhysicalInstance::main_mcan4,
        J784McanPhysicalInstance::main_mcan16};

[[nodiscard]] bool is_empty(const char *const value) noexcept {
  return value == nullptr || value[0] == '\0';
}

[[nodiscard]] bool region_matches(const McanHardwareRegion &lhs,
                                  const McanHardwareRegion &rhs) noexcept {
  return lhs.base_address == rhs.base_address &&
         lhs.size_bytes == rhs.size_bytes;
}

[[nodiscard]] bool controller_matches_board_instance(
    const McanControllerConfig &controller,
    const J784McanBoardInstance &instance) noexcept {
  return controller.instance_id == instance.default_endpoint_instance_id &&
         region_matches(controller.hardware.subsystem,
                        instance.hardware.subsystem) &&
         region_matches(controller.hardware.controller,
                        instance.hardware.controller) &&
         region_matches(controller.hardware.message_ram,
                        instance.hardware.message_ram) &&
         controller.hardware.tisci_device_id ==
             instance.hardware.tisci_device_id;
}

[[nodiscard]] const McanControllerConfig *find_controller_for_board_instance(
    const McanResourceManagerConfig &config,
    const J784McanBoardInstance &instance) noexcept {
  for (auto index = std::size_t{0U}; index < config.controller_count; ++index) {
    if (config.controllers[index].instance_id ==
        instance.default_endpoint_instance_id) {
      return &config.controllers[index];
    }
  }
  return nullptr;
}

void initialize_production_controller(
    McanControllerConfig &controller) noexcept {
  controller = McanControllerConfig{};
  controller.rx_queue_count = 1U;
  controller.tx_queue_count = 1U;
  controller.rx_queue_capacity = 16U;
  controller.tx_queue_capacity = 16U;
  controller.mode = McanMode::fd;
  controller.brs_enabled = true;
  controller.arbitration_bitrate_valid = true;
  controller.arbitration_bitrate = 500'000U;
  controller.data_bitrate_valid = true;
  controller.data_bitrate = 2'000'000U;
}

} // namespace

std::size_t j784_mcan_board_instance_count() noexcept {
  return kInstances.size();
}

std::array<J784McanPhysicalInstance, kJ784McanProductionScopeInstanceCount>
j784_mcan_production_scope_instances() noexcept {
  return kProductionScopeInstances;
}

const J784McanBoardInstance *
j784_mcan_board_instance_at(const std::size_t index) noexcept {
  if (index >= kInstances.size()) {
    return nullptr;
  }
  return &kInstances[index];
}

const J784McanBoardInstance *find_j784_mcan_board_instance(
    const J784McanPhysicalInstance instance) noexcept {
  for (const auto &candidate : kInstances) {
    if (candidate.physical_instance == instance) {
      return &candidate;
    }
  }
  return nullptr;
}

const J784McanBoardInstance *find_j784_mcan_board_instance_by_endpoint(
    const std::uint8_t endpoint_instance_id) noexcept {
  for (const auto &candidate : kInstances) {
    if (candidate.default_endpoint_instance_id == endpoint_instance_id) {
      return &candidate;
    }
  }
  return nullptr;
}

McanStatus apply_j784_mcan_board_mapping(
    McanControllerConfig &controller,
    const J784McanPhysicalInstance instance) noexcept {
  const auto *const mapping = find_j784_mcan_board_instance(instance);
  if (mapping == nullptr) {
    return McanStatus::invalid_hardware_mapping;
  }
  controller.instance_id = mapping->default_endpoint_instance_id;
  controller.hardware = mapping->hardware;
  controller.board_mapping_evidence_confirmed = true;
  return validate_controller_hardware_mapping(controller);
}

J784McanProductionScopePolicy
j784_mcan_default_production_scope_policy() noexcept {
  return J784McanProductionScopePolicy{};
}

McanStatus make_j784_mcan_production_resource_manager_config(
    McanResourceManagerConfig &config) noexcept {
  config = McanResourceManagerConfig{};
  config.controller_count = kProductionScopeInstances.size();
  for (auto index = std::size_t{0U}; index < kProductionScopeInstances.size();
       ++index) {
    initialize_production_controller(config.controllers[index]);
    const auto status = apply_j784_mcan_board_mapping(
        config.controllers[index], kProductionScopeInstances[index]);
    if (!status_ok(status)) {
      config = McanResourceManagerConfig{};
      return status;
    }
  }
  return validate_resource_manager_skeleton(config);
}

McanStatus validate_j784_mcan_production_ownership_scope(
    const McanResourceManagerConfig &config,
    const J784McanProductionScopePolicy &policy) noexcept {
  const auto skeleton_status = validate_resource_manager_skeleton(config);
  if (!status_ok(skeleton_status)) {
    return skeleton_status;
  }
  for (auto scope_index = std::size_t{0U};
       scope_index < kProductionScopeInstances.size(); ++scope_index) {
    const auto instance = kProductionScopeInstances[scope_index];
    const auto *const board_instance = find_j784_mcan_board_instance(instance);
    if (board_instance == nullptr ||
        !board_instance->board_port_evidence
             .linux_reference_device_tree_marks_enabled) {
      return McanStatus::invalid_hardware_mapping;
    }
    if (policy.descoped_instances[scope_index]) {
      if (is_empty(policy.descope_reasons[scope_index])) {
        return McanStatus::invalid_argument;
      }
      continue;
    }
    const auto *const controller =
        find_controller_for_board_instance(config, *board_instance);
    if (controller == nullptr) {
      return McanStatus::missing_hardware_evidence;
    }
    if (!controller_matches_board_instance(*controller, *board_instance)) {
      return McanStatus::invalid_hardware_mapping;
    }
    if (instance == J784McanPhysicalInstance::main_mcan4 &&
        board_instance->board_port_evidence
            .ethernet_firmware_overlay_disables_path &&
        !policy.main_mcan4_ethfw_mux_conflict_resolved) {
      return McanStatus::missing_hardware_evidence;
    }
    const auto controller_status =
        validate_controller_for_interrupt_start(*controller);
    if (!status_ok(controller_status)) {
      return controller_status;
    }
  }
  return McanStatus::ok;
}

} // namespace evsn::can_drivers::j784_mcan_qnx
