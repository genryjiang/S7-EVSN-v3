#include "evsn/can_drivers/j784_mcan_qnx/mcan_registers.hpp"

#include <gtest/gtest.h>

namespace {

using evsn::can_drivers::j784_mcan_qnx::McanControllerConfig;
using evsn::can_drivers::j784_mcan_qnx::McanCoreConfig;
using evsn::can_drivers::j784_mcan_qnx::McanCfgRegisterOffset;
using evsn::can_drivers::j784_mcan_qnx::McanMode;
using evsn::can_drivers::j784_mcan_qnx::McanRegisterProgramming;
using evsn::can_drivers::j784_mcan_qnx::McanSsRegisterOffset;
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

} // namespace

TEST(J784McanQnxRegistersTest, KeepsSdkBackedOffsetsStable) {
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::offset_value(
                McanCfgRegisterOffset::cccr),
            0x18U);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::offset_value(
                McanCfgRegisterOffset::nbtp),
            0x1CU);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::offset_value(
                McanCfgRegisterOffset::rxf0c),
            0xA0U);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::offset_value(
                McanCfgRegisterOffset::txbar),
            0xD0U);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::offset_value(
                McanSsRegisterOffset::ie),
            0x18U);
}

TEST(J784McanQnxRegistersTest, EncodesBoschNominalAndDataBitTimingFields) {
  auto encoded = std::uint32_t{0U};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                encode_nominal_bit_timing_register(
                    evsn::can_drivers::j784_mcan_qnx::
                        make_classic_500k_timing(),
                    encoded),
            McanStatus::ok);
  EXPECT_EQ(encoded, 0x00090C01U);

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::
                encode_data_bit_timing_register(
                    evsn::can_drivers::j784_mcan_qnx::make_fd_2m_timing(),
                    encoded),
            McanStatus::ok);
  EXPECT_EQ(encoded, 0x00030610U);
}

TEST(J784McanQnxRegistersTest, BuildsConfigAndRunRegisterPrograms) {
  auto programming = McanRegisterProgramming{};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::make_mcan_register_programming(
                loopback_config(), programming),
            McanStatus::ok);

  EXPECT_EQ(programming.cccr_config & 0x3U, 0x3U);
  EXPECT_EQ(programming.cccr_config & (1U << 7U), 1U << 7U);
  EXPECT_EQ(programming.cccr_config & (1U << 8U), 1U << 8U);
  EXPECT_EQ(programming.cccr_config & (1U << 9U), 1U << 9U);
  EXPECT_EQ(programming.cccr_run & 0x3U, 0U);
  EXPECT_EQ(programming.test, 1U << 4U);
  EXPECT_NE(programming.interrupt_enable, 0U);
  EXPECT_EQ(programming.interrupt_line_enable, 1U);
  EXPECT_EQ(programming.extended_id_mask, 0x1FFFFFFFU);
}

TEST(J784McanQnxRegistersTest, RejectsDataTimingThatDoesNotFitDataRegister) {
  auto config = loopback_config();
  config.data_timing.prescaler = 33U;
  config.data_timing.source_clock_hz =
      config.data_timing.bitrate * config.data_timing.prescaler *
      (1U + config.data_timing.time_segment_before_sample +
       config.data_timing.time_segment_after_sample);

  auto programming = McanRegisterProgramming{};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::make_mcan_register_programming(
                config, programming),
            McanStatus::invalid_bitrate);
}
