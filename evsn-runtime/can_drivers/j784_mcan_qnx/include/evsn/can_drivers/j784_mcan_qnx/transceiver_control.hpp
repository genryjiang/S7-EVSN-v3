#pragma once

#include "evsn/can_drivers/j784_mcan_qnx/config.hpp"
#include "evsn/can_drivers/j784_mcan_qnx/j784_board_config.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace evsn::can_drivers::j784_mcan_qnx {

inline constexpr const char *kJ784EvmBoardRevision = "PROC141E5";
inline constexpr std::size_t kMaxJ784TransceiverActionsPerInstance = 4U;

enum class J784TransceiverBackend : std::uint8_t {
  none = 0U,
  wkup_gpio_mmio = 1U,
  tca6424_i2c = 2U,
};

enum class J784TransceiverSignal : std::uint8_t {
  no_standby_gpio = 0U,
  transceiver_standby = 1U,
  can_uart_mux0 = 2U,
  can_uart_mux1 = 3U,
  can_uart_mux2 = 4U,
};

struct J784WkupGpioAction {
  McanHardwareRegion region{};
  std::uint32_t gpio_line{0U};
  std::uint32_t bank{0U};
  std::uint32_t bit{0U};
  std::uint32_t direction_offset{0U};
  std::uint32_t set_offset{0U};
  std::uint32_t clear_offset{0U};
  bool output_high{false};
};

struct J784Tca6424Action {
  std::uint8_t i2c_address{0U};
  std::uint8_t port{0U};
  std::uint8_t pin{0U};
  std::uint8_t output_register{0U};
  std::uint8_t config_register{0U};
  bool output_high{false};
};

struct J784TransceiverAction {
  J784TransceiverBackend backend{J784TransceiverBackend::none};
  J784TransceiverSignal signal{J784TransceiverSignal::no_standby_gpio};
  const char *description{nullptr};
  J784WkupGpioAction wkup_gpio{};
  J784Tca6424Action tca6424{};
  bool writes_hardware{false};
  bool requires_explicit_enable{false};
  const char *explicit_enable_note{nullptr};
};

struct J784TransceiverActionPlan {
  J784McanPhysicalInstance instance{J784McanPhysicalInstance::mcu_mcan0};
  const char *instance_name{nullptr};
  const char *board_revision{nullptr};
  const char *board_connector_name{nullptr};
  const char *linux_reference_device_tree_node_name{nullptr};
  const char *ethernet_firmware_conflict_note{nullptr};
  std::array<J784TransceiverAction, kMaxJ784TransceiverActionsPerInstance>
      actions{};
  std::size_t action_count{0U};
  bool has_actions_requiring_explicit_enable{false};
};

struct J784TransceiverRuntimeOptions {
  bool apply_writes{false};
  bool allow_main_mcan4_ethfw_conflict_override{false};
  const char *main_mcan4_tca6424_i2c_device_path{nullptr};
};

[[nodiscard]] const char *
j784_transceiver_backend_name(J784TransceiverBackend backend) noexcept;
[[nodiscard]] const char *
j784_transceiver_signal_name(J784TransceiverSignal signal) noexcept;
[[nodiscard]] McanStatus
make_j784_transceiver_action_plan(J784McanPhysicalInstance instance,
                                  J784TransceiverActionPlan &plan) noexcept;
[[nodiscard]] McanStatus
validate_j784_transceiver_action(const J784TransceiverAction &action) noexcept;
[[nodiscard]] McanStatus validate_j784_transceiver_action_plan(
    const J784TransceiverActionPlan &plan) noexcept;
[[nodiscard]] McanStatus validate_j784_transceiver_runtime_options(
    const J784TransceiverActionPlan &plan,
    const J784TransceiverRuntimeOptions &options) noexcept;
[[nodiscard]] int
format_j784_transceiver_action(const J784TransceiverAction &action,
                               char *buffer, std::size_t buffer_size) noexcept;

} // namespace evsn::can_drivers::j784_mcan_qnx
