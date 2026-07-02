#include "evsn/can_drivers/j784_mcan_qnx/j784_board_config.hpp"

#include <array>

namespace evsn::can_drivers::j784_mcan_qnx {
namespace {

constexpr McanHardwareRegion region(const std::uint64_t base,
                                    const std::uint32_t size) noexcept {
  return McanHardwareRegion{base, size};
}

constexpr McanHardwareInstanceConfig hardware(
    const std::uint64_t subsystem_base, const std::uint64_t controller_base,
    const std::uint64_t message_ram_base,
    const std::uint32_t tisci_device_id) noexcept {
  return McanHardwareInstanceConfig{region(subsystem_base, 0x100U),
                                    region(controller_base, 0x200U),
                                    region(message_ram_base, 0x8000U),
                                    tisci_device_id};
}

constexpr auto kInstances = std::array<J784McanBoardInstance, 4U>{
    J784McanBoardInstance{J784McanPhysicalInstance::mcu_mcan0, "MCU_MCAN0",
                          0U,
                          hardware(0x40520000ULL, 0x40528000ULL,
                                   0x40500000ULL, 263U),
                          true, false},
    J784McanBoardInstance{J784McanPhysicalInstance::mcu_mcan1, "MCU_MCAN1",
                          1U,
                          hardware(0x40560000ULL, 0x40568000ULL,
                                   0x40540000ULL, 264U),
                          true, false},
    J784McanBoardInstance{J784McanPhysicalInstance::main_mcan4, "MAIN_MCAN4",
                          2U,
                          hardware(0x2740000ULL, 0x2741000ULL,
                                   0x2748000ULL, 249U),
                          true, false},
    J784McanBoardInstance{J784McanPhysicalInstance::main_mcan16, "MAIN_MCAN16",
                          3U,
                          hardware(0x26A0000ULL, 0x26A1000ULL,
                                   0x26A8000ULL, 261U),
                          true, false},
};

} // namespace

std::size_t j784_mcan_board_instance_count() noexcept {
  return kInstances.size();
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

} // namespace evsn::can_drivers::j784_mcan_qnx
