#pragma once

#include "evsn/can_drivers/j784_mcan_qnx/devctl_abi.hpp"

#include <array>
#include <cstdint>

namespace evsn::can_drivers::j784_mcan_qnx {

constexpr auto kMcanMessageRamBytes = std::uint32_t{0x8000U};
constexpr auto kMcanMessageRamWordBytes = std::uint32_t{4U};
constexpr auto kMcanMessageRamWords =
    kMcanMessageRamBytes / kMcanMessageRamWordBytes;
constexpr auto kMcanMessageRamElementWords = std::size_t{18U};
constexpr auto kMcanMessageRamMaxTxBuffers = std::uint8_t{32U};
constexpr auto kMcanMessageRamMaxRxFifoElements = std::uint8_t{64U};
constexpr auto kMcanMessageRamMaxStandardFilters = std::uint8_t{128U};
constexpr auto kMcanMessageRamMaxExtendedFilters = std::uint8_t{64U};

struct McanMessageRamSection {
  std::uint16_t start_word{0U};
  std::uint8_t count{0U};
  std::uint8_t element_words{0U};
};

struct McanMessageRamLayout {
  McanMessageRamSection standard_filters{};
  McanMessageRamSection extended_filters{};
  McanMessageRamSection tx_event_fifo{};
  McanMessageRamSection tx_buffers{};
  McanMessageRamSection rx_fifo0{};
  McanMessageRamSection rx_fifo1{};
  McanMessageRamSection rx_buffers{};
};

struct McanMessageRamRegisters {
  std::uint32_t sidfc{0U};
  std::uint32_t xidfc{0U};
  std::uint32_t txbc{0U};
  std::uint32_t txefc{0U};
  std::uint32_t rxf0c{0U};
  std::uint32_t rxf1c{0U};
  std::uint32_t rxbc{0U};
  std::uint32_t rxesc{0U};
  std::uint32_t txesc{0U};
};

struct McanMessageRamElement {
  std::array<std::uint32_t, kMcanMessageRamElementWords> words{};
};

[[nodiscard]] McanMessageRamLayout
make_pdk_loopback_message_ram_layout() noexcept;
[[nodiscard]] McanStatus validate_message_ram_layout(
    const McanMessageRamLayout &layout,
    std::uint32_t ram_word_capacity = kMcanMessageRamWords) noexcept;
[[nodiscard]] McanStatus encode_message_ram_registers(
    const McanMessageRamLayout &layout,
    McanMessageRamRegisters &registers) noexcept;
[[nodiscard]] McanStatus encode_tx_message_ram_element(
    const McanFrameTransfer &frame,
    const McanControllerCapabilities &capabilities,
    McanMessageRamElement &element) noexcept;
[[nodiscard]] McanStatus decode_rx_message_ram_element(
    const McanMessageRamElement &element,
    const McanControllerCapabilities &capabilities,
    McanFrameTransfer &frame) noexcept;

} // namespace evsn::can_drivers::j784_mcan_qnx
