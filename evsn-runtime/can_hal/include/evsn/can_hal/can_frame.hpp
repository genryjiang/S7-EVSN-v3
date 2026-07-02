#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace evsn::can_hal {

constexpr auto kMaxStandardCanId = std::uint32_t{0x7FFU};
constexpr auto kMaxExtendedCanId = std::uint32_t{0x1FFFFFFFU};
constexpr auto kClassicPayloadCapacity = std::uint8_t{8U};
constexpr auto kFdPayloadCapacity = std::uint8_t{64U};

enum class CanIdFormat : std::uint8_t {
    standard_11_bit = 0U,
    extended_29_bit = 1U,
};

enum class CanFrameFormat : std::uint8_t {
    classic = 0U,
    fd = 1U,
};

enum class CanFrameKind : std::uint8_t {
    data = 0U,
    remote = 1U,
};

struct CanFdFlags {
    bool brs{false};
    bool esi{false};
    bool fdf{false};
};

struct CanTimestamp {
    bool valid{false};
    std::uint64_t monotonic_ns{0U};
};

struct CanFrame {
    std::uint32_t id{0U};
    CanIdFormat id_format{CanIdFormat::standard_11_bit};
    CanFrameFormat frame_format{CanFrameFormat::classic};
    CanFrameKind kind{CanFrameKind::data};
    std::uint8_t dlc{0U};
    std::uint8_t length{0U};
    std::array<std::uint8_t, kFdPayloadCapacity> payload{};
    CanFdFlags fd_flags{};
    CanTimestamp timestamp{};
};

[[nodiscard]] bool is_fd_payload_length(std::uint8_t length) noexcept;
[[nodiscard]] bool is_classic_payload_length(std::uint8_t length) noexcept;
[[nodiscard]] bool can_id_is_valid(std::uint32_t id, CanIdFormat format) noexcept;
[[nodiscard]] bool dlc_from_length(std::uint8_t length, std::uint8_t& dlc) noexcept;
[[nodiscard]] bool length_from_dlc(std::uint8_t dlc, std::uint8_t& length) noexcept;

}  // namespace evsn::can_hal
