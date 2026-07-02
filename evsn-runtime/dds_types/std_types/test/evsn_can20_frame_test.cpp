#include "std_types.hpp"

#include <array>
#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

namespace {
EVSNCan20Frame make_data_frame(
    const std::uint32_t id,
    const bool extended_id,
    const std::array<std::uint8_t, 8U>& payload) {
    auto frame = EVSNCan20Frame{};
    frame.id(id);
    frame.extended_id(extended_id);
    frame.remote_frame(false);
    frame.dlc(static_cast<std::uint8_t>(payload.size()));
    frame.data().resize(payload.size());
    for (auto index = std::size_t{0U}; index < payload.size(); ++index) {
        frame.data()[index] = payload[index];
    }
    return frame;
}
}

TEST(EVSNCan20FrameTest, DefaultsToEmptyStandardDataFrame) {
    const auto frame = EVSNCan20Frame{};

    EXPECT_EQ(frame.id(), 0U);
    EXPECT_FALSE(frame.extended_id());
    EXPECT_FALSE(frame.remote_frame());
    EXPECT_EQ(frame.dlc(), 0U);
    EXPECT_TRUE(frame.data().empty());
}

TEST(EVSNCan20FrameTest, StoresStandardDataFrameMetadataAndPayload) {
    const auto payload =
        std::array<std::uint8_t, 8U>{0x00U, 0x11U, 0x22U, 0x33U, 0x44U, 0x55U, 0x66U, 0x77U};
    const auto frame = make_data_frame(0x321U, false, payload);

    EXPECT_EQ(frame.id(), 0x321U);
    EXPECT_FALSE(frame.extended_id());
    EXPECT_FALSE(frame.remote_frame());
    EXPECT_EQ(frame.dlc(), payload.size());
    ASSERT_EQ(frame.data().size(), payload.size());
    for (auto index = std::size_t{0U}; index < payload.size(); ++index) {
        EXPECT_EQ(frame.data()[index], payload[index]) << "payload byte " << index;
    }
}

TEST(EVSNCan20FrameTest, StoresExtendedRemoteFrameDlcWithoutPayload) {
    auto frame = EVSNCan20Frame{};
    frame.id(0x1ABCDEU);
    frame.extended_id(true);
    frame.remote_frame(true);
    frame.dlc(8U);

    EXPECT_EQ(frame.id(), 0x1ABCDEU);
    EXPECT_TRUE(frame.extended_id());
    EXPECT_TRUE(frame.remote_frame());
    EXPECT_EQ(frame.dlc(), 8U);
    EXPECT_TRUE(frame.data().empty());
}

TEST(EVSNCan20FrameTest, EnforcesEightBytePayloadBound) {
    auto frame = EVSNCan20Frame{};

    frame.data().resize(8U);
    EXPECT_EQ(frame.data().size(), 8U);
    EXPECT_ANY_THROW(frame.data().resize(9U));
}

TEST(EVSNCan20FrameTest, RoundTripsThroughCdrBuffer) {
    const auto payload =
        std::array<std::uint8_t, 8U>{0xA0U, 0xA1U, 0xA2U, 0xA3U, 0xA4U, 0xA5U, 0xA6U, 0xA7U};
    const auto frame = make_data_frame(0x1ABCDEU, true, payload);

    auto buffer = std::vector<char>{};
    dds::topic::topic_type_support<EVSNCan20Frame>::to_cdr_buffer(buffer, frame);
    ASSERT_FALSE(buffer.empty());

    auto decoded = EVSNCan20Frame{};
    dds::topic::topic_type_support<EVSNCan20Frame>::from_cdr_buffer(decoded, buffer);

    EXPECT_EQ(decoded, frame);
}
