#include "evsn/can_drivers/j784_mcan_qnx/qnx_mmio_mapper.hpp"

#include <array>
#include <cstdint>

#include <gtest/gtest.h>

namespace {

using evsn::can_drivers::j784_mcan_qnx::McanHardwareRegion;
using evsn::can_drivers::j784_mcan_qnx::McanMappedMmioRegion;
using evsn::can_drivers::j784_mcan_qnx::McanStatus;

} // namespace

TEST(J784McanQnxMmioMapperTest, ValidatesMappingRequestsBeforeTouchingQnx) {
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_mmio_mapping_request(
                McanHardwareRegion{0x2741000ULL, 0x200U}),
            McanStatus::ok);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_mmio_mapping_request(
                McanHardwareRegion{0U, 0x200U}),
            McanStatus::invalid_hardware_mapping);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_mmio_mapping_request(
                McanHardwareRegion{0x2741001ULL, 0x200U}),
            McanStatus::invalid_hardware_mapping);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::validate_mmio_mapping_request(
                McanHardwareRegion{0x2741000ULL, 0x3U}),
            McanStatus::invalid_hardware_mapping);
}

TEST(J784McanQnxMmioMapperTest, HostMappingFailsClosed) {
  auto mapped = McanMappedMmioRegion{};
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::map_qnx_mmio_region(
                McanHardwareRegion{0x2741000ULL, 0x200U}, mapped),
            McanStatus::hardware_access_unavailable);
}

#if !defined(__QNXNTO__)
TEST(J784McanQnxMmioMapperTest, HostUnmapFailurePreservesMappedState) {
  auto backing = std::array<std::uint32_t, 1U>{};
  auto mapped = McanMappedMmioRegion{
      reinterpret_cast<volatile std::uint8_t *>(backing.data()),
      backing.size() * sizeof(std::uint32_t), 0x1000U};

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::unmap_qnx_mmio_region(mapped),
            McanStatus::hardware_access_unavailable);
  EXPECT_EQ(mapped.address,
            reinterpret_cast<volatile std::uint8_t *>(backing.data()));
  EXPECT_EQ(mapped.size_bytes, backing.size() * sizeof(std::uint32_t));
  EXPECT_EQ(mapped.physical_base, 0x1000U);
}
#endif

TEST(J784McanQnxMmioMapperTest, ReadWriteRejectInvalidMappedBounds) {
  auto backing = std::array<std::uint32_t, 1U>{};
  auto mapped = McanMappedMmioRegion{
      reinterpret_cast<volatile std::uint8_t *>(backing.data()),
      backing.size() * sizeof(std::uint32_t), 0x1000U};
  auto value = std::uint32_t{0U};

  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::write_qnx_mmio32(
                mapped, 0U, 0xA5A55A5AU),
            McanStatus::ok);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::read_qnx_mmio32(mapped, 0U,
                                                              value),
            McanStatus::ok);
  EXPECT_EQ(value, 0xA5A55A5AU);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::write_qnx_mmio32(mapped, 1U, 0U),
            McanStatus::invalid_hardware_mapping);
  EXPECT_EQ(evsn::can_drivers::j784_mcan_qnx::read_qnx_mmio32(mapped, 4U,
                                                              value),
            McanStatus::invalid_hardware_mapping);
}
