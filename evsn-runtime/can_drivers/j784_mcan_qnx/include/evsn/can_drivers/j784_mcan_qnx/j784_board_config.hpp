#pragma once

#include "evsn/can_drivers/j784_mcan_qnx/config.hpp"

#include <cstddef>
#include <cstdint>

namespace evsn::can_drivers::j784_mcan_qnx {

enum class J784McanPhysicalInstance : std::uint8_t {
  mcu_mcan0 = 0U,
  mcu_mcan1 = 1U,
  main_mcan4 = 2U,
  main_mcan16 = 3U,
};

struct J784McanBoardInstance {
  J784McanPhysicalInstance physical_instance{
      J784McanPhysicalInstance::mcu_mcan0};
  const char *name{nullptr};
  std::uint8_t default_endpoint_instance_id{0U};
  McanHardwareInstanceConfig hardware{};
  bool pdk_loopback_example_instance{false};
  bool qnx_bsp_startup_reference_observed{false};
};

[[nodiscard]] std::size_t j784_mcan_board_instance_count() noexcept;
[[nodiscard]] const J784McanBoardInstance *
j784_mcan_board_instance_at(std::size_t index) noexcept;
[[nodiscard]] const J784McanBoardInstance *
find_j784_mcan_board_instance(J784McanPhysicalInstance instance) noexcept;
[[nodiscard]] const J784McanBoardInstance *
find_j784_mcan_board_instance_by_endpoint(std::uint8_t endpoint_instance_id)
    noexcept;
[[nodiscard]] McanStatus
apply_j784_mcan_board_mapping(McanControllerConfig &controller,
                              J784McanPhysicalInstance instance) noexcept;

} // namespace evsn::can_drivers::j784_mcan_qnx
