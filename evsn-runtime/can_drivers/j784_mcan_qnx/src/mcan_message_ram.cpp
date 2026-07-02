#include "evsn/can_drivers/j784_mcan_qnx/mcan_message_ram.hpp"

namespace evsn::can_drivers::j784_mcan_qnx {
namespace {

constexpr auto kStandardFilterWords = std::uint8_t{1U};
constexpr auto kExtendedFilterWords = std::uint8_t{2U};
constexpr auto kTxEventWords = std::uint8_t{2U};
constexpr auto kCanFd64ElementWords = std::uint8_t{18U};
constexpr auto kCanFd64ElementSizeCode = std::uint32_t{7U};
constexpr auto kMaxStandardId = std::uint32_t{0x7FFU};
constexpr auto kMaxExtendedId = std::uint32_t{0x1FFFFFFFU};
constexpr auto kTxRxElementPayloadWordOffset = std::size_t{2U};
constexpr auto kHeaderIdStandardShift = std::uint32_t{18U};
constexpr auto kHeaderRtr = std::uint32_t{1U << 29U};
constexpr auto kHeaderXtd = std::uint32_t{1U << 30U};
constexpr auto kHeaderEsi = std::uint32_t{1U << 31U};
constexpr auto kElementDlcShift = std::uint32_t{16U};
constexpr auto kElementBrs = std::uint32_t{1U << 20U};
constexpr auto kElementFdf = std::uint32_t{1U << 21U};

[[nodiscard]] bool section_is_empty(
    const McanMessageRamSection &section) noexcept {
  return section.count == 0U || section.element_words == 0U;
}

[[nodiscard]] std::uint32_t section_end_word(
    const McanMessageRamSection &section) noexcept {
  return static_cast<std::uint32_t>(section.start_word) +
         (static_cast<std::uint32_t>(section.count) * section.element_words);
}

[[nodiscard]] bool section_is_valid(const McanMessageRamSection &section,
                                    const std::uint32_t capacity_words,
                                    const std::uint8_t max_count,
                                    const std::uint8_t element_words) noexcept {
  if (section.count > max_count) {
    return false;
  }
  if (section_is_empty(section)) {
    return section.count == 0U && section.element_words == 0U;
  }
  if (section.element_words != element_words) {
    return false;
  }
  return section_end_word(section) <= capacity_words;
}

[[nodiscard]] bool sections_overlap(const McanMessageRamSection &lhs,
                                    const McanMessageRamSection &rhs) noexcept {
  if (section_is_empty(lhs) || section_is_empty(rhs)) {
    return false;
  }
  return lhs.start_word < section_end_word(rhs) &&
         rhs.start_word < section_end_word(lhs);
}

[[nodiscard]] bool layout_has_overlaps(
    const McanMessageRamLayout &layout) noexcept {
  const auto sections = std::array<McanMessageRamSection, 7U>{
      layout.standard_filters, layout.extended_filters, layout.tx_event_fifo,
      layout.tx_buffers,      layout.rx_fifo0,         layout.rx_fifo1,
      layout.rx_buffers};
  for (auto index = std::size_t{0U}; index < sections.size(); ++index) {
    for (auto other = index + 1U; other < sections.size(); ++other) {
      if (sections_overlap(sections[index], sections[other])) {
        return true;
      }
    }
  }
  return false;
}

[[nodiscard]] std::uint32_t start_field(
    const McanMessageRamSection &section) noexcept {
  return static_cast<std::uint32_t>(section.start_word) << 2U;
}

[[nodiscard]] std::uint32_t payload_word(
    const McanFrameTransfer &frame, const std::size_t word_index) noexcept {
  const auto payload_offset = word_index * 4U;
  auto word = std::uint32_t{0U};
  for (auto byte = std::size_t{0U}; byte < 4U; ++byte) {
    const auto frame_offset = payload_offset + byte;
    if (frame_offset < frame.length) {
      word |= static_cast<std::uint32_t>(frame.payload[frame_offset])
              << (byte * 8U);
    }
  }
  return word;
}

void payload_from_words(const McanMessageRamElement &element,
                        McanFrameTransfer &frame) noexcept {
  for (auto index = std::size_t{0U}; index < frame.length; ++index) {
    const auto word_index = kTxRxElementPayloadWordOffset + (index / 4U);
    const auto shift = (index % 4U) * 8U;
    frame.payload[index] =
        static_cast<std::uint8_t>((element.words[word_index] >> shift) & 0xFFU);
  }
}

[[nodiscard]] bool length_from_dlc(const std::uint8_t dlc,
                                   std::uint8_t &length) noexcept {
  constexpr auto lengths = std::array<std::uint8_t, 16U>{
      0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 12U, 16U, 20U, 24U, 32U, 48U, 64U};
  if (dlc >= lengths.size()) {
    return false;
  }
  length = lengths[dlc];
  return true;
}

[[nodiscard]] McanStatus decode_header_words(
    const McanMessageRamElement &element, McanFrameTransfer &frame) noexcept {
  const auto header0 = element.words[0];
  const auto header1 = element.words[1];
  const auto extended_id = (header0 & kHeaderXtd) != 0U;
  const auto remote = (header0 & kHeaderRtr) != 0U;
  const auto esi = (header0 & kHeaderEsi) != 0U;
  const auto fd_frame = (header1 & kElementFdf) != 0U;
  const auto brs = (header1 & kElementBrs) != 0U;
  frame.can_id = extended_id ? header0 & kMaxExtendedId
                             : (header0 >> kHeaderIdStandardShift) &
                                   kMaxStandardId;
  frame.flags = 0U;
  if (extended_id) {
    frame.flags |= flag_value(McanFrameFlag::extended_id);
  }
  if (remote) {
    frame.flags |= flag_value(McanFrameFlag::remote);
  }
  if (fd_frame) {
    frame.flags |= flag_value(McanFrameFlag::fd_frame);
  }
  if (brs) {
    frame.flags |= flag_value(McanFrameFlag::brs);
  }
  if (esi) {
    frame.flags |= flag_value(McanFrameFlag::esi);
  }
  frame.dlc = static_cast<std::uint8_t>((header1 >> kElementDlcShift) & 0xFU);
  if (remote) {
    frame.length = 0U;
    return McanStatus::ok;
  }
  if (fd_frame) {
    return length_from_dlc(frame.dlc, frame.length)
               ? McanStatus::ok
               : McanStatus::invalid_argument;
  }
  if (frame.dlc > 8U) {
    return McanStatus::invalid_argument;
  }
  frame.length = frame.dlc;
  return McanStatus::ok;
}

} // namespace

McanMessageRamLayout make_pdk_loopback_message_ram_layout() noexcept {
  auto layout = McanMessageRamLayout{};
  layout.standard_filters = McanMessageRamSection{0U, 1U, kStandardFilterWords};
  layout.extended_filters = McanMessageRamSection{48U, 1U, kExtendedFilterWords};
  layout.tx_event_fifo = McanMessageRamSection{100U, 5U, kTxEventWords};
  layout.tx_buffers = McanMessageRamSection{148U, 5U, kCanFd64ElementWords};
  layout.rx_fifo0 = McanMessageRamSection{548U, 5U, kCanFd64ElementWords};
  layout.rx_fifo1 = McanMessageRamSection{748U, 5U, kCanFd64ElementWords};
  layout.rx_buffers = McanMessageRamSection{948U, 0U, 0U};
  return layout;
}

McanStatus validate_message_ram_layout(
    const McanMessageRamLayout &layout,
    const std::uint32_t ram_word_capacity) noexcept {
  if (ram_word_capacity == 0U) {
    return McanStatus::invalid_hardware_mapping;
  }
  if (!section_is_valid(layout.standard_filters, ram_word_capacity,
                        kMcanMessageRamMaxStandardFilters,
                        kStandardFilterWords) ||
      !section_is_valid(layout.extended_filters, ram_word_capacity,
                        kMcanMessageRamMaxExtendedFilters,
                        kExtendedFilterWords) ||
      !section_is_valid(layout.tx_event_fifo, ram_word_capacity,
                        kMcanMessageRamMaxTxBuffers, kTxEventWords) ||
      !section_is_valid(layout.tx_buffers, ram_word_capacity,
                        kMcanMessageRamMaxTxBuffers, kCanFd64ElementWords) ||
      !section_is_valid(layout.rx_fifo0, ram_word_capacity,
                        kMcanMessageRamMaxRxFifoElements,
                        kCanFd64ElementWords) ||
      !section_is_valid(layout.rx_fifo1, ram_word_capacity,
                        kMcanMessageRamMaxRxFifoElements,
                        kCanFd64ElementWords) ||
      !section_is_valid(layout.rx_buffers, ram_word_capacity,
                        kMcanMessageRamMaxRxFifoElements,
                        kCanFd64ElementWords) ||
      layout_has_overlaps(layout)) {
    return McanStatus::invalid_hardware_mapping;
  }
  return McanStatus::ok;
}

McanStatus encode_message_ram_registers(
    const McanMessageRamLayout &layout,
    McanMessageRamRegisters &registers) noexcept {
  registers = McanMessageRamRegisters{};
  const auto status = validate_message_ram_layout(layout);
  if (!status_ok(status)) {
    return status;
  }
  registers.sidfc = start_field(layout.standard_filters) |
                    (static_cast<std::uint32_t>(layout.standard_filters.count)
                     << 16U);
  registers.xidfc = start_field(layout.extended_filters) |
                    (static_cast<std::uint32_t>(layout.extended_filters.count)
                     << 16U);
  registers.txbc = start_field(layout.tx_buffers) |
                   (static_cast<std::uint32_t>(layout.tx_buffers.count)
                    << 16U);
  registers.txefc = start_field(layout.tx_event_fifo) |
                    (static_cast<std::uint32_t>(layout.tx_event_fifo.count)
                     << 16U) |
                    (std::uint32_t{3U} << 24U);
  registers.rxf0c = start_field(layout.rx_fifo0) |
                    (static_cast<std::uint32_t>(layout.rx_fifo0.count)
                     << 16U) |
                    (std::uint32_t{3U} << 24U);
  registers.rxf1c = start_field(layout.rx_fifo1) |
                    (static_cast<std::uint32_t>(layout.rx_fifo1.count)
                     << 16U) |
                    (std::uint32_t{3U} << 24U);
  registers.rxbc = start_field(layout.rx_buffers);
  registers.rxesc = kCanFd64ElementSizeCode |
                    (kCanFd64ElementSizeCode << 4U) |
                    (kCanFd64ElementSizeCode << 8U);
  registers.txesc = kCanFd64ElementSizeCode;
  return McanStatus::ok;
}

McanStatus encode_tx_message_ram_element(
    const McanFrameTransfer &frame,
    const McanControllerCapabilities &capabilities,
    McanMessageRamElement &element) noexcept {
  element = McanMessageRamElement{};
  const auto status = validate_frame_transfer(frame, capabilities);
  if (!status_ok(status)) {
    return status;
  }
  const auto extended_id = has_flag(frame.flags, McanFrameFlag::extended_id);
  const auto remote = has_flag(frame.flags, McanFrameFlag::remote);
  const auto fd_frame = has_flag(frame.flags, McanFrameFlag::fd_frame);
  const auto brs = has_flag(frame.flags, McanFrameFlag::brs);
  const auto esi = has_flag(frame.flags, McanFrameFlag::esi);

  element.words[0] =
      extended_id ? frame.can_id : frame.can_id << kHeaderIdStandardShift;
  if (remote) {
    element.words[0] |= kHeaderRtr;
  }
  if (extended_id) {
    element.words[0] |= kHeaderXtd;
  }
  if (esi) {
    element.words[0] |= kHeaderEsi;
  }
  element.words[1] = static_cast<std::uint32_t>(frame.dlc)
                     << kElementDlcShift;
  if (brs) {
    element.words[1] |= kElementBrs;
  }
  if (fd_frame) {
    element.words[1] |= kElementFdf;
  }
  for (auto index = std::size_t{0U}; index < 16U; ++index) {
    element.words[kTxRxElementPayloadWordOffset + index] =
        payload_word(frame, index);
  }
  return McanStatus::ok;
}

McanStatus decode_rx_message_ram_element(
    const McanMessageRamElement &element,
    const McanControllerCapabilities &capabilities,
    McanFrameTransfer &frame) noexcept {
  frame = McanFrameTransfer{};
  auto status = decode_header_words(element, frame);
  if (!status_ok(status)) {
    return status;
  }
  payload_from_words(element, frame);
  status = validate_frame_transfer(frame, capabilities);
  return status;
}

} // namespace evsn::can_drivers::j784_mcan_qnx
