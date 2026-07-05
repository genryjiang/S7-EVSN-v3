#include "evsn/can_drivers/j784_mcan_qnx/transceiver_control.hpp"

#include <cstdio>

namespace evsn::can_drivers::j784_mcan_qnx {
namespace {

inline constexpr auto kWkupGpio0Region =
    McanHardwareRegion{0x42110000ULL, 0x200U};
inline constexpr auto kWkupGpioBankStride = std::uint32_t{0x28U};
inline constexpr auto kWkupGpioDirectionBase = std::uint32_t{0x10U};
inline constexpr auto kWkupGpioSetBase = std::uint32_t{0x18U};
inline constexpr auto kWkupGpioClearBase = std::uint32_t{0x1CU};
inline constexpr auto kGpioPinsPerBank = std::uint32_t{16U};
inline constexpr auto kTca6424Address = std::uint8_t{0x22U};
inline constexpr auto kTca6424OutputBase = std::uint8_t{0x04U};
inline constexpr auto kTca6424ConfigBase = std::uint8_t{0x0CU};

[[nodiscard]] constexpr std::uint32_t
gpio_bank_for_line(const std::uint32_t line) noexcept {
  return line / kGpioPinsPerBank;
}

[[nodiscard]] constexpr std::uint32_t
gpio_bit_for_line(const std::uint32_t line) noexcept {
  return line % kGpioPinsPerBank;
}

[[nodiscard]] constexpr std::uint32_t
gpio_bank_offset(const std::uint32_t base, const std::uint32_t bank) noexcept {
  return base + (bank * kWkupGpioBankStride);
}

[[nodiscard]] constexpr J784WkupGpioAction
wkup_gpio_action(const std::uint32_t line, const bool output_high) noexcept {
  const auto bank = gpio_bank_for_line(line);
  const auto bit = gpio_bit_for_line(line);
  return J784WkupGpioAction{kWkupGpio0Region,
                            line,
                            bank,
                            bit,
                            gpio_bank_offset(kWkupGpioDirectionBase, bank),
                            gpio_bank_offset(kWkupGpioSetBase, bank),
                            gpio_bank_offset(kWkupGpioClearBase, bank),
                            output_high};
}

[[nodiscard]] constexpr J784Tca6424Action
tca6424_action(const std::uint8_t port, const std::uint8_t pin,
               const bool output_high) noexcept {
  return J784Tca6424Action{kTca6424Address,
                           port,
                           pin,
                           static_cast<std::uint8_t>(kTca6424OutputBase + port),
                           static_cast<std::uint8_t>(kTca6424ConfigBase + port),
                           output_high};
}

[[nodiscard]] constexpr J784TransceiverAction
wkup_standby_action(const std::uint32_t line,
                    const char *const description) noexcept {
  return J784TransceiverAction{J784TransceiverBackend::wkup_gpio_mmio,
                               J784TransceiverSignal::transceiver_standby,
                               description,
                               wkup_gpio_action(line, false),
                               J784Tca6424Action{},
                               true,
                               false,
                               nullptr};
}

[[nodiscard]] constexpr J784TransceiverAction
tca6424_main4_action(const J784TransceiverSignal signal,
                     const char *const description, const std::uint8_t port,
                     const std::uint8_t pin, const bool output_high) noexcept {
  return J784TransceiverAction{
      J784TransceiverBackend::tca6424_i2c,
      signal,
      description,
      J784WkupGpioAction{},
      tca6424_action(port, pin, output_high),
      true,
      true,
      "MAIN_MCAN4 is disabled by k3-j784s4-evm-ethfw.dtbo unless the ethfw "
      "overlay path is removed or ownership is otherwise confirmed"};
}

[[nodiscard]] constexpr J784TransceiverAction
main16_no_standby_action() noexcept {
  return J784TransceiverAction{
      J784TransceiverBackend::none,
      J784TransceiverSignal::no_standby_gpio,
      "MAIN_MCAN16 TCAN1042 standby is grounded/enabled on PROC141E5",
      J784WkupGpioAction{},
      J784Tca6424Action{},
      false,
      false,
      nullptr};
}

[[nodiscard]] bool is_empty(const char *const value) noexcept {
  return value == nullptr || value[0] == '\0';
}

[[nodiscard]] McanStatus
append_action(J784TransceiverActionPlan &plan,
              const J784TransceiverAction &action) noexcept {
  if (plan.action_count >= plan.actions.size()) {
    return McanStatus::invalid_queue_capacity;
  }
  plan.actions[plan.action_count] = action;
  ++plan.action_count;
  if (action.requires_explicit_enable) {
    plan.has_actions_requiring_explicit_enable = true;
  }
  return McanStatus::ok;
}

} // namespace

const char *
j784_transceiver_backend_name(const J784TransceiverBackend backend) noexcept {
  switch (backend) {
  case J784TransceiverBackend::none:
    return "none";
  case J784TransceiverBackend::wkup_gpio_mmio:
    return "wkup_gpio_mmio";
  case J784TransceiverBackend::tca6424_i2c:
    return "tca6424_i2c";
  }
  return "unknown";
}

const char *
j784_transceiver_signal_name(const J784TransceiverSignal signal) noexcept {
  switch (signal) {
  case J784TransceiverSignal::no_standby_gpio:
    return "no_standby_gpio";
  case J784TransceiverSignal::transceiver_standby:
    return "transceiver_standby";
  case J784TransceiverSignal::can_uart_mux0:
    return "can_uart_mux0";
  case J784TransceiverSignal::can_uart_mux1:
    return "can_uart_mux1";
  case J784TransceiverSignal::can_uart_mux2:
    return "can_uart_mux2";
  }
  return "unknown";
}

McanStatus
make_j784_transceiver_action_plan(const J784McanPhysicalInstance instance,
                                  J784TransceiverActionPlan &plan) noexcept {
  plan = J784TransceiverActionPlan{};
  plan.instance = instance;
  plan.board_revision = kJ784EvmBoardRevision;

  const auto *const board_instance = find_j784_mcan_board_instance(instance);
  if (board_instance == nullptr) {
    return McanStatus::invalid_hardware_mapping;
  }
  plan.instance_name = board_instance->name;
  plan.board_connector_name =
      board_instance->board_port_evidence.board_connector_name;
  plan.linux_reference_device_tree_node_name =
      board_instance->board_port_evidence.linux_reference_device_tree_node_name;
  plan.ethernet_firmware_conflict_note =
      board_instance->board_port_evidence
          .ethernet_firmware_overlay_conflict_note;
  plan.has_actions_requiring_explicit_enable =
      board_instance->board_port_evidence
          .ethernet_firmware_overlay_disables_path;

  auto status = McanStatus::ok;
  switch (instance) {
  case J784McanPhysicalInstance::mcu_mcan0:
    status = append_action(
        plan, wkup_standby_action(
                  69U, "drive WKUP_GPIO0_69 low to release MCU_MCAN0 standby"));
    break;
  case J784McanPhysicalInstance::mcu_mcan1:
    status = append_action(
        plan, wkup_standby_action(
                  2U, "drive WKUP_GPIO0_2 low to release MCU_MCAN1 standby"));
    break;
  case J784McanPhysicalInstance::main_mcan4:
    status = append_action(
        plan, tca6424_main4_action(
                  J784TransceiverSignal::transceiver_standby,
                  "drive TCA6424 port0 pin7 low to release MAIN_MCAN4 "
                  "standby",
                  0U, 7U, false));
    if (status_ok(status)) {
      status = append_action(
          plan, tca6424_main4_action(
                    J784TransceiverSignal::can_uart_mux0,
                    "drive TCA6424 port1 pin5 high for MAIN_MCAN4 mux path", 1U,
                    5U, true));
    }
    if (status_ok(status)) {
      status = append_action(
          plan, tca6424_main4_action(
                    J784TransceiverSignal::can_uart_mux1,
                    "drive TCA6424 port1 pin6 high for MAIN_MCAN4 mux path", 1U,
                    6U, true));
    }
    if (status_ok(status)) {
      status = append_action(
          plan, tca6424_main4_action(
                    J784TransceiverSignal::can_uart_mux2,
                    "drive TCA6424 port1 pin7 high for MAIN_MCAN4 mux path", 1U,
                    7U, true));
    }
    break;
  case J784McanPhysicalInstance::main_mcan16:
    status = append_action(plan, main16_no_standby_action());
    break;
  }

  if (!status_ok(status)) {
    return status;
  }
  return validate_j784_transceiver_action_plan(plan);
}

McanStatus
validate_j784_transceiver_action(const J784TransceiverAction &action) noexcept {
  if (action.description == nullptr) {
    return McanStatus::invalid_argument;
  }
  switch (action.backend) {
  case J784TransceiverBackend::none:
    return action.writes_hardware ? McanStatus::invalid_argument
                                  : McanStatus::ok;
  case J784TransceiverBackend::wkup_gpio_mmio:
    if (!action.writes_hardware ||
        action.wkup_gpio.region.base_address != kWkupGpio0Region.base_address ||
        action.wkup_gpio.region.size_bytes < kWkupGpio0Region.size_bytes ||
        action.wkup_gpio.bank !=
            gpio_bank_for_line(action.wkup_gpio.gpio_line) ||
        action.wkup_gpio.bit != gpio_bit_for_line(action.wkup_gpio.gpio_line) ||
        action.wkup_gpio.direction_offset !=
            gpio_bank_offset(kWkupGpioDirectionBase, action.wkup_gpio.bank) ||
        action.wkup_gpio.set_offset !=
            gpio_bank_offset(kWkupGpioSetBase, action.wkup_gpio.bank) ||
        action.wkup_gpio.clear_offset !=
            gpio_bank_offset(kWkupGpioClearBase, action.wkup_gpio.bank)) {
      return McanStatus::invalid_argument;
    }
    return McanStatus::ok;
  case J784TransceiverBackend::tca6424_i2c:
    if (!action.writes_hardware ||
        action.tca6424.i2c_address != kTca6424Address ||
        action.tca6424.port > 2U || action.tca6424.pin > 7U ||
        action.tca6424.output_register !=
            static_cast<std::uint8_t>(kTca6424OutputBase +
                                      action.tca6424.port) ||
        action.tca6424.config_register !=
            static_cast<std::uint8_t>(kTca6424ConfigBase +
                                      action.tca6424.port)) {
      return McanStatus::invalid_argument;
    }
    return McanStatus::ok;
  }
  return McanStatus::invalid_argument;
}

McanStatus validate_j784_transceiver_action_plan(
    const J784TransceiverActionPlan &plan) noexcept {
  if (plan.instance_name == nullptr || plan.board_revision == nullptr ||
      plan.board_connector_name == nullptr ||
      plan.linux_reference_device_tree_node_name == nullptr ||
      plan.action_count == 0U ||
      plan.action_count > kMaxJ784TransceiverActionsPerInstance) {
    return McanStatus::invalid_argument;
  }
  for (std::size_t index = 0U; index < plan.action_count; ++index) {
    const auto status = validate_j784_transceiver_action(plan.actions[index]);
    if (!status_ok(status)) {
      return status;
    }
  }
  return McanStatus::ok;
}

McanStatus validate_j784_transceiver_runtime_options(
    const J784TransceiverActionPlan &plan,
    const J784TransceiverRuntimeOptions &options) noexcept {
  const auto plan_status = validate_j784_transceiver_action_plan(plan);
  if (!status_ok(plan_status) || !options.apply_writes) {
    return plan_status;
  }

  for (std::size_t index = 0U; index < plan.action_count; ++index) {
    const auto &action = plan.actions[index];
    if (action.requires_explicit_enable &&
        !options.allow_main_mcan4_ethfw_conflict_override) {
      return McanStatus::missing_hardware_evidence;
    }
    if (action.backend == J784TransceiverBackend::tca6424_i2c &&
        is_empty(options.main_mcan4_tca6424_i2c_device_path)) {
      return McanStatus::hardware_access_unavailable;
    }
  }
  return McanStatus::ok;
}

int format_j784_transceiver_action(const J784TransceiverAction &action,
                                   char *const buffer,
                                   const std::size_t buffer_size) noexcept {
  if (buffer == nullptr || buffer_size == 0U) {
    return -1;
  }

  switch (action.backend) {
  case J784TransceiverBackend::none:
    return std::snprintf(buffer, buffer_size,
                         "backend=none signal=%s writes=no description=\"%s\"",
                         j784_transceiver_signal_name(action.signal),
                         action.description == nullptr ? ""
                                                       : action.description);
  case J784TransceiverBackend::wkup_gpio_mmio:
    return std::snprintf(
        buffer, buffer_size,
        "backend=wkup_gpio_mmio signal=%s wkup_gpio_line=%u "
        "wkup_gpio_bank=%u wkup_gpio_bit=%u "
        "direction_register_offset=0x%03x set_register_offset=0x%03x "
        "clear_register_offset=0x%03x output_level=%s "
        "requires_explicit_enable=%s "
        "description=\"%s\"",
        j784_transceiver_signal_name(action.signal),
        static_cast<unsigned int>(action.wkup_gpio.gpio_line),
        static_cast<unsigned int>(action.wkup_gpio.bank),
        static_cast<unsigned int>(action.wkup_gpio.bit),
        static_cast<unsigned int>(action.wkup_gpio.direction_offset),
        static_cast<unsigned int>(action.wkup_gpio.set_offset),
        static_cast<unsigned int>(action.wkup_gpio.clear_offset),
        action.wkup_gpio.output_high ? "high" : "low",
        action.requires_explicit_enable ? "yes" : "no",
        action.description == nullptr ? "" : action.description);
  case J784TransceiverBackend::tca6424_i2c:
    return std::snprintf(
        buffer, buffer_size,
        "backend=tca6424_i2c signal=%s i2c_address=0x%02x "
        "expander_port=%u expander_pin=%u output_register=0x%02x "
        "configuration_register=0x%02x output_level=%s "
        "requires_explicit_enable=%s description=\"%s\"",
        j784_transceiver_signal_name(action.signal),
        static_cast<unsigned int>(action.tca6424.i2c_address),
        static_cast<unsigned int>(action.tca6424.port),
        static_cast<unsigned int>(action.tca6424.pin),
        static_cast<unsigned int>(action.tca6424.output_register),
        static_cast<unsigned int>(action.tca6424.config_register),
        action.tca6424.output_high ? "high" : "low",
        action.requires_explicit_enable ? "yes" : "no",
        action.description == nullptr ? "" : action.description);
  }
  return -1;
}

} // namespace evsn::can_drivers::j784_mcan_qnx
