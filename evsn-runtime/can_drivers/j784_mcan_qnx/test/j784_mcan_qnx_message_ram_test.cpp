#include "evsn/can_drivers/j784_mcan_qnx/mcan_message_ram.hpp"

#include <cstddef>
#include <cstdint>

#include <gtest/gtest.h>

namespace {

using evsn::can_drivers::j784_mcan_qnx::McanCapabilityFlag;
using evsn::can_drivers::j784_mcan_qnx::McanControllerCapabilities;
using evsn::can_drivers::j784_mcan_qnx::McanFrameFlag;
using evsn::can_drivers::j784_mcan_qnx::McanFrameTransfer;
using evsn::can_drivers::j784_mcan_qnx::McanMessageRamElement;
using evsn::can_drivers::j784_mcan_qnx::McanMessageRamRegisters;
using evsn::can_drivers::j784_mcan_qnx::McanStatus;

McanControllerCapabilities fd_capabilities() {
  auto capabilities = McanControllerCapabilities{};
  capabilities.flags = evsn::can_drivers::j784_mcan_qnx::capability_value(
                           McanCapabilityFlag::classic_frame) |
                       evsn::can_drivers::j784_mcan_qnx::capability_value(
                           McanCapabilityFlag::fd_frame) |
                       evsn::can_drivers::j784_mcan_qnx::capability_value(
                           McanCapabilityFlag::extended_id) |
                       evsn::can_drivers::j784_mcan_qnx::capability_value(
                           McanCapabilityFlag::brs) |
                       evsn::can_drivers::j784_mcan_qnx::capability_value(
                           McanCapabilityFlag::esi);
  return capabilities;
}

McanFrameTransfer fd_frame() {
  auto frame = McanFrameTransfer{};
  frame.can_id = 0x1ABCDEU;
  frame.flags =
      evsn::can_drivers::j784_mcan_qnx::flag_value(McanFrameFlag::extended_id) |
      evsn::can_drivers::j784_mcan_qnx::flag_value(McanFrameFlag::fd_frame) |
      evsn::can_drivers::j784_mcan_qnx::flag_value(McanFrameFlag::brs) |
      evsn::can_drivers::j784_mcan_qnx::flag_value(McanFrameFlag::esi);
  frame.dlc = 15U;
  frame.length = 64U;
  for (auto index = std::size_t{0U}; index < frame.length; ++index) {
    frame.payload[index] = static_cast<std::uint8_t>(index + 1U);
  }
  return frame;
}

} // namespace

TEST(J784McanQnxMessageRamTest, EncodesPdkLoopbackLayoutRegisters) {
  const auto layout =
      evsn::can_drivers::j784_mcan_qnx::make_pdk_loopback_message_ram_layout();
  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::validate_message_ram_layout(layout),
      McanStatus::ok);

  auto registers = McanMessageRamRegisters{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::encode_message_ram_registers(
                layout, registers),
            McanStatus::ok);
  EXPECT_EQ(registers.sidfc, (0U << 2U) | (1U << 16U));
  EXPECT_EQ(registers.xidfc, (48U << 2U) | (1U << 16U));
  EXPECT_EQ(registers.txbc, (148U << 2U) | (5U << 16U));
  EXPECT_EQ(registers.rxf0c, (548U << 2U) | (5U << 16U) | (3U << 24U));
  EXPECT_EQ(registers.rxf1c, (748U << 2U) | (5U << 16U) | (3U << 24U));
  EXPECT_EQ(registers.rxesc, 0x777U);
  EXPECT_EQ(registers.txesc, 0x7U);
}

TEST(J784McanQnxMessageRamTest, RejectsOverlappingOrOversizedLayouts) {
  auto layout =
      evsn::can_drivers::j784_mcan_qnx::make_pdk_loopback_message_ram_layout();
  layout.rx_fifo0.start_word = 150U;
  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::validate_message_ram_layout(layout),
      McanStatus::invalid_hardware_mapping);

  layout =
      evsn::can_drivers::j784_mcan_qnx::make_pdk_loopback_message_ram_layout();
  layout.tx_buffers.count = 33U;
  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::validate_message_ram_layout(layout),
      McanStatus::invalid_hardware_mapping);
}

TEST(J784McanQnxMessageRamTest, RejectsLayoutElementSizeMismatch) {
  auto layout =
      evsn::can_drivers::j784_mcan_qnx::make_pdk_loopback_message_ram_layout();
  layout.rx_fifo0.element_words = 17U;
  EXPECT_EQ(
      evsn::can_drivers::j784_mcan_qnx::validate_message_ram_layout(layout),
      McanStatus::invalid_hardware_mapping);

  layout =
      evsn::can_drivers::j784_mcan_qnx::make_pdk_loopback_message_ram_layout();
  layout.tx_buffers.element_words = 19U;
  auto registers = McanMessageRamRegisters{};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::encode_message_ram_registers(
                layout, registers),
            McanStatus::invalid_hardware_mapping);
}

TEST(J784McanQnxMessageRamTest, EncodesAndDecodesCanFdPayloadElements) {
  const auto frame = fd_frame();
  auto element = McanMessageRamElement{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::encode_tx_message_ram_element(
                frame, fd_capabilities(), element),
            McanStatus::ok);
  EXPECT_EQ(element.words[0] & 0x1FFFFFFFU, frame.can_id);
  EXPECT_NE(element.words[0] & (1U << 30U), 0U);
  EXPECT_NE(element.words[0] & (1U << 31U), 0U);
  EXPECT_NE(element.words[1] & (1U << 20U), 0U);
  EXPECT_NE(element.words[1] & (1U << 21U), 0U);
  EXPECT_EQ(element.words[2], 0x04030201U);
  EXPECT_EQ(element.words[17], 0x403F3E3DU);

  auto decoded = McanFrameTransfer{};
  ASSERT_EQ(evsn::can_drivers::j784_mcan_qnx::decode_rx_message_ram_element(
                element, fd_capabilities(), decoded),
            McanStatus::ok);
  EXPECT_EQ(decoded.can_id, frame.can_id);
  EXPECT_EQ(decoded.flags, frame.flags);
  EXPECT_EQ(decoded.dlc, frame.dlc);
  EXPECT_EQ(decoded.length, frame.length);
  EXPECT_EQ(decoded.payload[0], 1U);
  EXPECT_EQ(decoded.payload[63], 64U);
}

TEST(J784McanQnxMessageRamTest, RejectsUnsupportedFrameCapabilities) {
  auto capabilities = fd_capabilities();
  capabilities.flags &= ~evsn::can_drivers::j784_mcan_qnx::capability_value(
      McanCapabilityFlag::brs);
  auto element = McanMessageRamElement{};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::encode_tx_message_ram_element(
                fd_frame(), capabilities, element),
            McanStatus::unsupported_mode);
}
