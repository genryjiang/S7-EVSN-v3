#include "evsn/can_drivers/j784_mcan_qnx/transceiver_control.hpp"

#include <gtest/gtest.h>

#include <array>

namespace {

using evsn::can_drivers::j784_mcan_qnx::J784McanPhysicalInstance;
using evsn::can_drivers::j784_mcan_qnx::J784TransceiverActionPlan;
using evsn::can_drivers::j784_mcan_qnx::J784TransceiverBackend;
using evsn::can_drivers::j784_mcan_qnx::J784TransceiverRuntimeOptions;
using evsn::can_drivers::j784_mcan_qnx::J784TransceiverSignal;
using evsn::can_drivers::j784_mcan_qnx::McanStatus;

J784TransceiverActionPlan plan_for(const J784McanPhysicalInstance instance) {
  auto plan = J784TransceiverActionPlan{};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::make_j784_transceiver_action_plan(
                instance, plan),
            McanStatus::ok);
  return plan;
}

} // namespace

TEST(J784McanQnxTransceiverControlTest, McuMcan0UsesWkupGpio69) {
  const auto plan = plan_for(J784McanPhysicalInstance::mcu_mcan0);
  ASSERT_EQ(plan.action_count, 1U);
  EXPECT_STREQ(plan.instance_name, "MCU_MCAN0");
  EXPECT_STREQ(plan.board_revision, "PROC141E5");
  EXPECT_STREQ(plan.board_connector_name, "J42");
  EXPECT_FALSE(plan.has_actions_requiring_explicit_enable);

  const auto &action = plan.actions[0];
  EXPECT_EQ(action.backend, J784TransceiverBackend::wkup_gpio_mmio);
  EXPECT_EQ(action.signal, J784TransceiverSignal::transceiver_standby);
  EXPECT_TRUE(action.writes_hardware);
  EXPECT_FALSE(action.requires_explicit_enable);
  EXPECT_EQ(action.wkup_gpio.region.base_address, 0x42110000ULL);
  EXPECT_EQ(action.wkup_gpio.gpio_line, 69U);
  EXPECT_EQ(action.wkup_gpio.bank, 4U);
  EXPECT_EQ(action.wkup_gpio.bit, 5U);
  EXPECT_EQ(action.wkup_gpio.direction_offset, 0x0B0U);
  EXPECT_EQ(action.wkup_gpio.set_offset, 0x0B8U);
  EXPECT_EQ(action.wkup_gpio.clear_offset, 0x0BCU);
  EXPECT_FALSE(action.wkup_gpio.output_high);
}

TEST(J784McanQnxTransceiverControlTest, McuMcan1UsesWkupGpio2) {
  const auto plan = plan_for(J784McanPhysicalInstance::mcu_mcan1);
  ASSERT_EQ(plan.action_count, 1U);
  EXPECT_STREQ(plan.instance_name, "MCU_MCAN1");
  EXPECT_STREQ(plan.board_connector_name, "J43");
  EXPECT_FALSE(plan.has_actions_requiring_explicit_enable);

  const auto &action = plan.actions[0];
  EXPECT_EQ(action.backend, J784TransceiverBackend::wkup_gpio_mmio);
  EXPECT_EQ(action.wkup_gpio.region.base_address, 0x42110000ULL);
  EXPECT_EQ(action.wkup_gpio.gpio_line, 2U);
  EXPECT_EQ(action.wkup_gpio.bank, 0U);
  EXPECT_EQ(action.wkup_gpio.bit, 2U);
  EXPECT_EQ(action.wkup_gpio.direction_offset, 0x010U);
  EXPECT_EQ(action.wkup_gpio.set_offset, 0x018U);
  EXPECT_EQ(action.wkup_gpio.clear_offset, 0x01CU);
  EXPECT_FALSE(action.wkup_gpio.output_high);
}

TEST(J784McanQnxTransceiverControlTest,
     MainMcan4RequiresExplicitTca6424MuxAndStandbyEnable) {
  const auto plan = plan_for(J784McanPhysicalInstance::main_mcan4);
  ASSERT_EQ(plan.action_count, 4U);
  EXPECT_STREQ(plan.instance_name, "MAIN_MCAN4");
  EXPECT_STREQ(plan.board_connector_name, "J45");
  EXPECT_TRUE(plan.has_actions_requiring_explicit_enable);
  ASSERT_NE(plan.ethernet_firmware_conflict_note, nullptr);

  const std::array<J784TransceiverSignal, 4U> expected_signals{
      J784TransceiverSignal::transceiver_standby,
      J784TransceiverSignal::can_uart_mux0,
      J784TransceiverSignal::can_uart_mux1,
      J784TransceiverSignal::can_uart_mux2};
  const std::array<std::uint8_t, 4U> expected_ports{0U, 1U, 1U, 1U};
  const std::array<std::uint8_t, 4U> expected_pins{7U, 5U, 6U, 7U};
  const std::array<bool, 4U> expected_levels{false, true, true, true};

  for (std::size_t index = 0U; index < plan.action_count; ++index) {
    const auto &action = plan.actions[index];
    EXPECT_EQ(action.backend, J784TransceiverBackend::tca6424_i2c);
    EXPECT_EQ(action.signal, expected_signals[index]);
    EXPECT_TRUE(action.writes_hardware);
    EXPECT_TRUE(action.requires_explicit_enable);
    EXPECT_NE(action.explicit_enable_note, nullptr);
    EXPECT_EQ(action.tca6424.i2c_address, 0x22U);
    EXPECT_EQ(action.tca6424.port, expected_ports[index]);
    EXPECT_EQ(action.tca6424.pin, expected_pins[index]);
    EXPECT_EQ(action.tca6424.output_register,
              static_cast<std::uint8_t>(0x04U + expected_ports[index]));
    EXPECT_EQ(action.tca6424.config_register,
              static_cast<std::uint8_t>(0x0CU + expected_ports[index]));
    EXPECT_EQ(action.tca6424.output_high, expected_levels[index]);
  }
}

TEST(J784McanQnxTransceiverControlTest, MainMcan16IsDocumentedNoOp) {
  const auto plan = plan_for(J784McanPhysicalInstance::main_mcan16);
  ASSERT_EQ(plan.action_count, 1U);
  EXPECT_STREQ(plan.instance_name, "MAIN_MCAN16");
  EXPECT_STREQ(plan.board_connector_name, "J46");
  EXPECT_FALSE(plan.has_actions_requiring_explicit_enable);

  const auto &action = plan.actions[0];
  EXPECT_EQ(action.backend, J784TransceiverBackend::none);
  EXPECT_EQ(action.signal, J784TransceiverSignal::no_standby_gpio);
  EXPECT_FALSE(action.writes_hardware);
}

TEST(J784McanQnxTransceiverControlTest,
     ProbeFormattingIsDeterministicAndDoesNotRequireBackends) {
  const auto mcu0 = plan_for(J784McanPhysicalInstance::mcu_mcan0);
  auto buffer = std::array<char, 512U>{};
  const auto format_rc =
      evsn::can_drivers::j784_mcan_qnx::format_j784_transceiver_action(
          mcu0.actions[0], buffer.data(), buffer.size());
  ASSERT_GT(format_rc, 0);
  ASSERT_LT(static_cast<std::size_t>(format_rc), buffer.size());
  EXPECT_STREQ(
      buffer.data(),
      "backend=wkup_gpio_mmio signal=transceiver_standby "
      "wkup_gpio_line=69 wkup_gpio_bank=4 wkup_gpio_bit=5 "
      "direction_register_offset=0x0b0 set_register_offset=0x0b8 "
      "clear_register_offset=0x0bc output_level=low "
      "requires_explicit_enable=no description=\"drive WKUP_GPIO0_69 low to "
      "release MCU_MCAN0 standby\"");

  const auto main4 = plan_for(J784McanPhysicalInstance::main_mcan4);
  const auto probe_status = evsn::can_drivers::j784_mcan_qnx::
      validate_j784_transceiver_runtime_options(
          main4, J784TransceiverRuntimeOptions{false, false, nullptr});
  EXPECT_EQ(probe_status, McanStatus::ok);
}

TEST(J784McanQnxTransceiverControlTest,
     ApplyModeRequiresEthfwOverrideAndI2cEvidence) {
  const auto main4 = plan_for(J784McanPhysicalInstance::main_mcan4);

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                validate_j784_transceiver_runtime_options(
                    main4, J784TransceiverRuntimeOptions{true, false, nullptr}),
            McanStatus::missing_hardware_evidence);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                validate_j784_transceiver_runtime_options(
                    main4, J784TransceiverRuntimeOptions{true, true, nullptr}),
            McanStatus::hardware_access_unavailable);
  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::
          validate_j784_transceiver_runtime_options(
              main4, J784TransceiverRuntimeOptions{true, true, "/dev/i2c0"}),
      McanStatus::ok);
}
