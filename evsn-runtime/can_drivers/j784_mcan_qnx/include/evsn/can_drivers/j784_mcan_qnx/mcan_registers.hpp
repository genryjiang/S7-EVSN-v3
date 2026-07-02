#pragma once

#include "evsn/can_drivers/j784_mcan_qnx/mcan_core.hpp"

#include <cstdint>

namespace evsn::can_drivers::j784_mcan_qnx {

enum class McanCfgRegisterOffset : std::uint32_t {
  crel = 0x00U,
  dbtp = 0x0CU,
  test = 0x10U,
  cccr = 0x18U,
  nbtp = 0x1CU,
  ir = 0x50U,
  ie = 0x54U,
  ile = 0x5CU,
  gfc = 0x80U,
  sidfc = 0x84U,
  xidfc = 0x88U,
  xidam = 0x90U,
  rxf0c = 0xA0U,
  rxf0s = 0xA4U,
  rxf0a = 0xA8U,
  rxbc = 0xACU,
  rxf1c = 0xB0U,
  rxf1s = 0xB4U,
  rxf1a = 0xB8U,
  rxesc = 0xBCU,
  txbc = 0xC0U,
  txfqs = 0xC4U,
  txesc = 0xC8U,
  txbar = 0xD0U,
  txbto = 0xD8U,
  txefc = 0xF0U,
};

enum class McanSsRegisterOffset : std::uint32_t {
  pid = 0x00U,
  ctrl = 0x04U,
  stat = 0x08U,
  ics = 0x0CU,
  irs = 0x10U,
  iecs = 0x14U,
  ie = 0x18U,
  eoi = 0x20U,
};

struct McanRegisterProgramming {
  std::uint32_t cccr_config{0U};
  std::uint32_t cccr_run{0U};
  std::uint32_t test{0U};
  std::uint32_t nominal_bit_timing{0U};
  std::uint32_t data_bit_timing{0U};
  std::uint32_t interrupt_enable{0U};
  std::uint32_t interrupt_line_enable{0U};
  std::uint32_t global_filter_config{0U};
  std::uint32_t extended_id_mask{0U};
};

[[nodiscard]] constexpr std::uint32_t
offset_value(McanCfgRegisterOffset offset) noexcept {
  return static_cast<std::uint32_t>(offset);
}

[[nodiscard]] constexpr std::uint32_t
offset_value(McanSsRegisterOffset offset) noexcept {
  return static_cast<std::uint32_t>(offset);
}

[[nodiscard]] std::uint32_t mcan_rx_tx_error_interrupt_mask() noexcept;
[[nodiscard]] McanStatus encode_nominal_bit_timing_register(
    const McanBitTiming &timing, std::uint32_t &encoded) noexcept;
[[nodiscard]] McanStatus encode_data_bit_timing_register(
    const McanBitTiming &timing, std::uint32_t &encoded) noexcept;
[[nodiscard]] McanStatus make_mcan_register_programming(
    const McanCoreConfig &config, McanRegisterProgramming &programming)
    noexcept;

} // namespace evsn::can_drivers::j784_mcan_qnx
