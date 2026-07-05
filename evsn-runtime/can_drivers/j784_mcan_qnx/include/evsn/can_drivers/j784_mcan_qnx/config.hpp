#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace evsn::can_drivers::j784_mcan_qnx {

constexpr auto kMaxControllers = std::size_t{8U};
constexpr auto kMaxQueuesPerController = std::uint8_t{4U};
constexpr auto kMaxQueueCapacity = std::uint16_t{64U};
constexpr auto kEndpointPathCapacity = std::size_t{48U};
constexpr auto kMinArbitrationBitrate = std::uint32_t{10'000U};
constexpr auto kMaxArbitrationBitrate = std::uint32_t{1'000'000U};
constexpr auto kMaxDataBitrate = std::uint32_t{8'000'000U};

enum class McanStatus : std::uint8_t {
  ok = 0U,
  invalid_argument = 1U,
  path_too_long = 2U,
  invalid_instance = 3U,
  invalid_queue = 4U,
  duplicate_instance = 5U,
  too_many_controllers = 6U,
  invalid_queue_capacity = 7U,
  invalid_bitrate = 8U,
  unsupported_mode = 9U,
  missing_hardware_evidence = 10U,
  invalid_state = 11U,
  rx_queue_full = 12U,
  tx_queue_full = 13U,
  no_frame = 14U,
  invalid_hardware_mapping = 15U,
  hardware_access_unavailable = 16U,
};

enum class EndpointKind : std::uint8_t {
  rx = 0U,
  tx = 1U,
  ctl = 2U,
};

enum class McanMode : std::uint8_t {
  classic = 0U,
  fd = 1U,
};

struct McanEndpointPath {
  std::array<char, kEndpointPathCapacity> value{};
};

struct McanHardwareRegion {
  std::uint64_t base_address{0U};
  std::uint32_t size_bytes{0U};
};

struct McanHardwareInstanceConfig {
  McanHardwareRegion subsystem{};
  McanHardwareRegion controller{};
  McanHardwareRegion message_ram{};
  std::uint32_t tisci_device_id{0U};
  std::uint32_t qnx_logical_irq{0U};
};

struct McanControllerTimingOverride {
  bool valid{false};
  std::uint32_t source_clock_hz{0U};
  std::uint32_t bitrate{0U};
  std::uint16_t prescaler{0U};
  std::uint8_t time_segment_before_sample{0U};
  std::uint8_t time_segment_after_sample{0U};
  std::uint8_t sync_jump_width{0U};
};

struct McanControllerConfig {
  std::uint8_t instance_id{0U};
  std::uint8_t rx_queue_count{1U};
  std::uint8_t tx_queue_count{1U};
  std::uint16_t rx_queue_capacity{16U};
  std::uint16_t tx_queue_capacity{16U};
  McanMode mode{McanMode::classic};
  bool brs_enabled{false};
  bool arbitration_bitrate_valid{false};
  std::uint32_t arbitration_bitrate{0U};
  bool data_bitrate_valid{false};
  std::uint32_t data_bitrate{0U};
  McanControllerTimingOverride nominal_timing_override{};
  McanControllerTimingOverride data_timing_override{};
  bool qnx_direct_ownership_confirmed{false};
  bool board_mapping_evidence_confirmed{false};
  bool target_startup_evidence_confirmed{false};
  bool transceiver_control_evidence_confirmed{false};
  bool qnx_irq_routing_evidence_confirmed{false};
  bool external_can_bench_evidence_confirmed{false};
  McanHardwareInstanceConfig hardware{};
};

struct McanResourceManagerConfig {
  std::array<McanControllerConfig, kMaxControllers> controllers{};
  std::size_t controller_count{0U};
};

[[nodiscard]] bool status_ok(McanStatus status) noexcept;
[[nodiscard]] const char *status_name(McanStatus status) noexcept;
[[nodiscard]] McanStatus make_endpoint_path(EndpointKind kind,
                                            std::uint8_t instance_id,
                                            std::uint8_t queue_id,
                                            McanEndpointPath &path) noexcept;
[[nodiscard]] McanStatus
validate_controller_skeleton(const McanControllerConfig &config) noexcept;
[[nodiscard]] McanStatus
validate_hardware_region(const McanHardwareRegion &region,
                         std::uint32_t minimum_size_bytes) noexcept;
[[nodiscard]] McanStatus validate_controller_hardware_mapping(
    const McanControllerConfig &config) noexcept;
[[nodiscard]] McanStatus validate_controller_for_hardware_start(
    const McanControllerConfig &config) noexcept;
[[nodiscard]] McanStatus validate_controller_for_interrupt_start(
    const McanControllerConfig &config) noexcept;
[[nodiscard]] McanStatus validate_resource_manager_skeleton(
    const McanResourceManagerConfig &config) noexcept;
[[nodiscard]] McanStatus validate_resource_manager_hardware_start(
    const McanResourceManagerConfig &config) noexcept;
[[nodiscard]] McanStatus validate_resource_manager_interrupt_start(
    const McanResourceManagerConfig &config) noexcept;

} // namespace evsn::can_drivers::j784_mcan_qnx
