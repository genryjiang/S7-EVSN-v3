#pragma once

#include "evsn/can_drivers/j784_mcan_qnx/config.hpp"

#include <cstddef>
#include <cstdint>

namespace evsn::can_drivers::j784_mcan_qnx {

struct McanMappedMmioRegion {
  volatile std::uint8_t *address{nullptr};
  std::size_t size_bytes{0U};
  std::uint64_t physical_base{0U};
  bool writable{false};
};

[[nodiscard]] McanStatus
validate_mmio_mapping_request(const McanHardwareRegion &region) noexcept;
[[nodiscard]] McanStatus request_qnx_io_privileges() noexcept;
[[nodiscard]] McanStatus
map_qnx_mmio_region(const McanHardwareRegion &region,
                    McanMappedMmioRegion &mapped) noexcept;
[[nodiscard]] McanStatus
map_qnx_mmio_region_read_only(const McanHardwareRegion &region,
                              McanMappedMmioRegion &mapped) noexcept;
McanStatus unmap_qnx_mmio_region(McanMappedMmioRegion &mapped) noexcept;
[[nodiscard]] McanStatus read_qnx_mmio32(const McanMappedMmioRegion &mapped,
                                         std::uint32_t offset,
                                         std::uint32_t &value) noexcept;
[[nodiscard]] McanStatus write_qnx_mmio32(const McanMappedMmioRegion &mapped,
                                          std::uint32_t offset,
                                          std::uint32_t value) noexcept;

} // namespace evsn::can_drivers::j784_mcan_qnx
