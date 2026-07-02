#include "evsn/can_drivers/j784_mcan_qnx/devctl_abi.hpp"

#include <gtest/gtest.h>

namespace {

using evsn::can_drivers::j784_mcan_qnx::McanCapabilityFlag;
using evsn::can_drivers::j784_mcan_qnx::McanControllerCapabilities;
using evsn::can_drivers::j784_mcan_qnx::McanFrameFlag;
using evsn::can_drivers::j784_mcan_qnx::McanFrameTransfer;
using evsn::can_drivers::j784_mcan_qnx::McanStatus;

constexpr std::uint32_t frame_flags(const McanFrameFlag first,
                                    const McanFrameFlag second) noexcept {
  return evsn::can_drivers::j784_mcan_qnx::flag_value(first) |
         evsn::can_drivers::j784_mcan_qnx::flag_value(second);
}

constexpr std::uint32_t capability_flags(
    const McanCapabilityFlag first, const McanCapabilityFlag second,
    const McanCapabilityFlag third, const McanCapabilityFlag fourth) noexcept {
  return evsn::can_drivers::j784_mcan_qnx::capability_value(first) |
         evsn::can_drivers::j784_mcan_qnx::capability_value(second) |
         evsn::can_drivers::j784_mcan_qnx::capability_value(third) |
         evsn::can_drivers::j784_mcan_qnx::capability_value(fourth);
}

McanControllerCapabilities full_capabilities() {
  auto capabilities = McanControllerCapabilities{};
  capabilities.flags = capability_flags(McanCapabilityFlag::classic_frame,
                                        McanCapabilityFlag::fd_frame,
                                        McanCapabilityFlag::extended_id,
                                        McanCapabilityFlag::brs) |
                       evsn::can_drivers::j784_mcan_qnx::capability_value(
                           McanCapabilityFlag::esi);
  return capabilities;
}

McanFrameTransfer classic_frame() {
  auto frame = McanFrameTransfer{};
  frame.can_id = 0x123U;
  frame.dlc = 8U;
  frame.length = 8U;
  return frame;
}

} // namespace

TEST(J784McanQnxDevctlAbiTest, HasStablePackedAbiSizes) {
  EXPECT_EQ(sizeof(McanFrameTransfer),
            evsn::can_drivers::j784_mcan_qnx::kMcanFrameTransferStructSize);
  EXPECT_EQ(
      sizeof(McanControllerCapabilities),
      evsn::can_drivers::j784_mcan_qnx::kMcanControllerCapabilitiesStructSize);
  EXPECT_EQ(
      sizeof(evsn::can_drivers::j784_mcan_qnx::McanDiagnosticsSnapshot),
      evsn::can_drivers::j784_mcan_qnx::kMcanDiagnosticsSnapshotStructSize);
}

TEST(J784McanQnxDevctlAbiTest,
     AcceptsClassicAndFdFramesWhenCapabilitiesAllowThem) {
  auto capabilities = full_capabilities();
  auto frame = classic_frame();

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_frame_transfer(
                frame, capabilities),
            McanStatus::ok);

  frame.flags = frame_flags(McanFrameFlag::fd_frame, McanFrameFlag::brs);
  frame.dlc = 13U;
  frame.length = 32U;
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_frame_transfer(
                frame, capabilities),
            McanStatus::ok);
}

TEST(J784McanQnxDevctlAbiTest, RejectsCanFdDlcAndLengthMismatch) {
  auto capabilities = full_capabilities();
  auto frame = classic_frame();
  frame.flags =
      evsn::can_drivers::j784_mcan_qnx::flag_value(McanFrameFlag::fd_frame);
  frame.dlc = 13U;
  frame.length = 24U;

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_frame_transfer(
                frame, capabilities),
            McanStatus::invalid_argument);
}

TEST(J784McanQnxDevctlAbiTest, RejectsUnsupportedFdAndBrsCapabilities) {
  auto capabilities = McanControllerCapabilities{};
  capabilities.flags = evsn::can_drivers::j784_mcan_qnx::capability_value(
      McanCapabilityFlag::classic_frame);
  auto frame = classic_frame();
  frame.flags = frame_flags(McanFrameFlag::fd_frame, McanFrameFlag::brs);
  frame.dlc = 9U;
  frame.length = 12U;

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_frame_transfer(
                frame, capabilities),
            McanStatus::unsupported_mode);

  capabilities.flags |= evsn::can_drivers::j784_mcan_qnx::capability_value(
      McanCapabilityFlag::fd_frame);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_frame_transfer(
                frame, capabilities),
            McanStatus::unsupported_mode);
}

TEST(J784McanQnxDevctlAbiTest, KeepsRemoteFramesClassicAndPayloadFree) {
  auto capabilities = full_capabilities();
  auto frame = classic_frame();
  frame.flags =
      evsn::can_drivers::j784_mcan_qnx::flag_value(McanFrameFlag::remote);
  frame.length = 0U;

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_frame_transfer(
                frame, capabilities),
            McanStatus::ok);

  frame.flags |=
      evsn::can_drivers::j784_mcan_qnx::flag_value(McanFrameFlag::fd_frame);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_frame_transfer(
                frame, capabilities),
            McanStatus::invalid_argument);
}

TEST(J784McanQnxDevctlAbiTest, RejectsIdentifierOutsideSelectedFormat) {
  auto capabilities = full_capabilities();
  auto frame = classic_frame();
  frame.can_id = 0x800U;

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_frame_transfer(
                frame, capabilities),
            McanStatus::invalid_argument);

  frame.flags =
      evsn::can_drivers::j784_mcan_qnx::flag_value(McanFrameFlag::extended_id);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_frame_transfer(
                frame, capabilities),
            McanStatus::ok);
}

TEST(J784McanQnxDevctlAbiTest, RejectsUnknownFrameFlagBits) {
  auto capabilities = full_capabilities();
  auto frame = classic_frame();
  frame.flags = 0x8000'0000U;

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_frame_transfer(
                frame, capabilities),
            McanStatus::invalid_argument);
}

TEST(J784McanQnxDevctlAbiTest, RejectsInvalidAbiHeadersAndReservedFields) {
  auto capabilities = full_capabilities();
  auto frame = classic_frame();

  frame.abi_version = 99U;
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_frame_transfer(
                frame, capabilities),
            McanStatus::invalid_argument);

  frame = classic_frame();
  frame.sizeof_struct = 4U;
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_frame_transfer(
                frame, capabilities),
            McanStatus::invalid_argument);

  frame = classic_frame();
  frame.reserved = 1U;
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_frame_transfer(
                frame, capabilities),
            McanStatus::invalid_argument);

  frame = classic_frame();
  capabilities.sizeof_struct = 4U;
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_frame_transfer(
                frame, capabilities),
            McanStatus::invalid_argument);
}

TEST(J784McanQnxDevctlAbiTest,
     RequiresTimestampCapabilityAndMatchingTimestampFlag) {
  auto capabilities = full_capabilities();
  auto frame = classic_frame();
  frame.flags = evsn::can_drivers::j784_mcan_qnx::flag_value(
      McanFrameFlag::timestamp_valid);
  frame.monotonic_timestamp_ns = 42U;

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_frame_transfer(
                frame, capabilities),
            McanStatus::unsupported_mode);

  capabilities.flags |= evsn::can_drivers::j784_mcan_qnx::capability_value(
      McanCapabilityFlag::hardware_timestamp);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_frame_transfer(
                frame, capabilities),
            McanStatus::ok);

  frame.flags = 0U;
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_frame_transfer(
                frame, capabilities),
            McanStatus::invalid_argument);
}
