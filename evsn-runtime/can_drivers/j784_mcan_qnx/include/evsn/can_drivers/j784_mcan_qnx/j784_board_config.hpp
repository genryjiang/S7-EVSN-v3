#pragma once

#include "evsn/can_drivers/j784_mcan_qnx/config.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace evsn::can_drivers::j784_mcan_qnx {

enum class J784McanPhysicalInstance : std::uint8_t {
  mcu_mcan0 = 0U,
  mcu_mcan1 = 1U,
  main_mcan4 = 2U,
  main_mcan16 = 3U,
};

struct J784McanTiReferenceEvidence {
  const char *ti_boot_device_tree_label{nullptr};
  const char *ti_boot_pinmux_state_name{nullptr};
  const char *ti_boot_transceiver_node_name{nullptr};
  const char *ti_boot_standby_control_description{nullptr};
  std::uint32_t gic_spi_line0{0U};
  std::uint32_t gic_spi_line1{0U};
  std::uint32_t mcan_clock_select_register_offset{0U};
  bool ti_boot_device_tree_status_is_okay{false};
};

struct J784McanBoardPortEvidence {
  const char *board_revision{nullptr};
  const char *linux_reference_device_tree_node_name{nullptr};
  const char *linux_reference_device_tree_status{nullptr};
  const char *linux_reference_can_interface_name{nullptr};
  const char *board_connector_name{nullptr};
  const char *ti_pdk_external_loopback_reference{nullptr};
  const char *ethernet_firmware_overlay_conflict_note{nullptr};
  bool linux_reference_device_tree_marks_enabled{false};
  bool ethernet_firmware_overlay_disables_path{false};
};

struct J784McanBoardInstance {
  J784McanPhysicalInstance physical_instance{
      J784McanPhysicalInstance::mcu_mcan0};
  const char *name{nullptr};
  std::uint8_t default_endpoint_instance_id{0U};
  McanHardwareInstanceConfig hardware{};
  J784McanTiReferenceEvidence ti_reference_evidence{};
  J784McanBoardPortEvidence board_port_evidence{};
  bool pdk_loopback_example_instance{false};
  bool qnx_bsp_startup_reference_observed{false};
};

inline constexpr std::size_t kJ784McanProductionScopeInstanceCount = 4U;

struct J784McanProductionScopePolicy {
  std::array<bool, kJ784McanProductionScopeInstanceCount> descoped_instances{};
  std::array<const char *, kJ784McanProductionScopeInstanceCount>
      descope_reasons{};
  bool main_mcan4_ethfw_mux_conflict_resolved{false};
};

[[nodiscard]] std::size_t j784_mcan_board_instance_count() noexcept;
[[nodiscard]] std::array<J784McanPhysicalInstance,
                         kJ784McanProductionScopeInstanceCount>
j784_mcan_production_scope_instances() noexcept;
[[nodiscard]] const J784McanBoardInstance *
j784_mcan_board_instance_at(std::size_t index) noexcept;
[[nodiscard]] const J784McanBoardInstance *
find_j784_mcan_board_instance(J784McanPhysicalInstance instance) noexcept;
[[nodiscard]] const J784McanBoardInstance *
find_j784_mcan_board_instance_by_endpoint(
    std::uint8_t endpoint_instance_id) noexcept;
[[nodiscard]] McanStatus
apply_j784_mcan_board_mapping(McanControllerConfig &controller,
                              J784McanPhysicalInstance instance) noexcept;
[[nodiscard]] J784McanProductionScopePolicy
j784_mcan_default_production_scope_policy() noexcept;
[[nodiscard]] McanStatus make_j784_mcan_production_resource_manager_config(
    McanResourceManagerConfig &config) noexcept;
[[nodiscard]] McanStatus validate_j784_mcan_production_ownership_scope(
    const McanResourceManagerConfig &config,
    const J784McanProductionScopePolicy &policy) noexcept;

} // namespace evsn::can_drivers::j784_mcan_qnx
