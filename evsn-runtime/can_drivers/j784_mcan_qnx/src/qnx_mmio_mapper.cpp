#if defined(__QNXNTO__) && !defined(_QNX_SOURCE)
#define _QNX_SOURCE
#endif

#include "evsn/can_drivers/j784_mcan_qnx/qnx_mmio_mapper.hpp"

#if defined(__QNXNTO__)
#include <cerrno>
#include <cstdint>
#include <sys/mman.h>
#include <sys/neutrino.h>
#include <unistd.h>
#endif

namespace evsn::can_drivers::j784_mcan_qnx {
namespace {

[[nodiscard]] bool mapped_region_is_valid(
    const McanMappedMmioRegion &mapped, const std::uint32_t offset) noexcept {
  return mapped.address != nullptr && (offset & 0x3U) == 0U &&
         mapped.size_bytes >= sizeof(std::uint32_t) &&
         offset <= mapped.size_bytes - sizeof(std::uint32_t);
}

} // namespace

McanStatus
validate_mmio_mapping_request(const McanHardwareRegion &region) noexcept {
  return validate_hardware_region(region, sizeof(std::uint32_t));
}

McanStatus request_qnx_io_privileges() noexcept {
#if defined(__QNXNTO__)
  const auto level = static_cast<std::intptr_t>(_NTO_IO_LEVEL_1);
  return ThreadCtl(_NTO_TCTL_IO_LEVEL, reinterpret_cast<void *>(level)) == -1
             ? McanStatus::hardware_access_unavailable
             : McanStatus::ok;
#else
  return McanStatus::hardware_access_unavailable;
#endif
}

McanStatus map_qnx_mmio_region(const McanHardwareRegion &region,
                               McanMappedMmioRegion &mapped) noexcept {
  mapped = McanMappedMmioRegion{};
  const auto request_status = validate_mmio_mapping_request(region);
  if (!status_ok(request_status)) {
    return request_status;
  }
#if defined(__QNXNTO__)
  void *const address = mmap_device_memory(
      nullptr, region.size_bytes, PROT_READ | PROT_WRITE | PROT_NOCACHE, 0,
      region.base_address);
  if (address == MAP_FAILED) {
    return McanStatus::hardware_access_unavailable;
  }
  mapped.address = static_cast<volatile std::uint8_t *>(address);
  mapped.size_bytes = region.size_bytes;
  mapped.physical_base = region.base_address;
  return McanStatus::ok;
#else
  return McanStatus::hardware_access_unavailable;
#endif
}

McanStatus unmap_qnx_mmio_region(McanMappedMmioRegion &mapped) noexcept {
  if (mapped.address == nullptr) {
    return McanStatus::ok;
  }
#if defined(__QNXNTO__)
  void *const address =
      const_cast<void *>(static_cast<const volatile void *>(mapped.address));
  const auto result = munmap_device_memory(address, mapped.size_bytes);
  if (result == -1) {
    return McanStatus::hardware_access_unavailable;
  }
  mapped = McanMappedMmioRegion{};
  return McanStatus::ok;
#else
  return McanStatus::hardware_access_unavailable;
#endif
}

McanStatus read_qnx_mmio32(const McanMappedMmioRegion &mapped,
                           const std::uint32_t offset,
                           std::uint32_t &value) noexcept {
  value = 0U;
  if (mapped.address == nullptr || mapped.size_bytes < sizeof(std::uint32_t) ||
      (offset & 0x3U) != 0U ||
      offset > mapped.size_bytes - sizeof(std::uint32_t)) {
    return McanStatus::invalid_hardware_mapping;
  }
  const auto *const word = reinterpret_cast<volatile const std::uint32_t *>(
      mapped.address + offset);
  value = *word;
  return McanStatus::ok;
}

McanStatus write_qnx_mmio32(const McanMappedMmioRegion &mapped,
                            const std::uint32_t offset,
                            const std::uint32_t value) noexcept {
  if (!mapped_region_is_valid(mapped, offset)) {
    return McanStatus::invalid_hardware_mapping;
  }
  auto *const word =
      reinterpret_cast<volatile std::uint32_t *>(mapped.address + offset);
  *word = value;
  return McanStatus::ok;
}

} // namespace evsn::can_drivers::j784_mcan_qnx
