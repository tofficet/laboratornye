#include <gtest/gtest.h>
#include "bitops.h"

using namespace std;

TEST(RotateLeftTest, OneByte) {
    vector<uint8_t> data = {0b10101010};
    auto result = rotateLeft(data, 3);
    EXPECT_EQ(result[0], 0b01010101);
}

TEST(RotateLeftTest, TwoBytes) {
    vector<uint8_t> data = {0b10110011, 0b01011101};
    auto result = rotateLeft(data, 5);
    EXPECT_EQ(result.size(), 2);
}

TEST(RotateRightTest, OneByte) {
    vector<uint8_t> data = {0b10101010};
    auto result = rotateRight(data, 3);
    EXPECT_EQ(result[0], 0b01010101);
}

TEST(ApplyMaskTest, Basic) {
    vector<uint8_t> data = {0b11111111, 0b11111111};
    vector<uint8_t> mask = {0b00001111, 0b11110000};
    auto result = applyMask(data, mask);
    EXPECT_EQ(result[0], 0b00001111);
    EXPECT_EQ(result[1], 0b11110000);
}

TEST(ApplyMaskTest, EmptyData) {
    vector<uint8_t> data;
    vector<uint8_t> mask = {0x0F};
    auto result = applyMask(data, mask);
    EXPECT_TRUE(result.empty());
}

TEST(ExtractBitsTest, OneByte) {
    vector<uint8_t> data = {0b10101010};
    auto result = extractBits(data, 2, 5);
    EXPECT_EQ(result.size(), 1);
}

TEST(ExtractBitsTest, InvalidRange) {
    vector<uint8_t> data = {0xAA};
    auto result = extractBits(data, 8, 10);
    EXPECT_TRUE(result.empty());
}

TEST(SwapBitsTest, SameByte) {
    vector<uint8_t> data = {0b10100000};
    swapBits(data, 0, 7);
    EXPECT_EQ(data[0], 0b00100001);
}

TEST(SwapBitsTest, DifferentBytes) {
    vector<uint8_t> data = {0b00000001, 0b10000000};
    swapBits(data, 0, 8);
    EXPECT_EQ(data[0], 0b00000000);
    EXPECT_EQ(data[1], 0b10000001);
}

TEST(SetBitValueTest, SetTo1) {
    vector<uint8_t> data = {0b00000000};
    setBitValue(data, 3, true);
    EXPECT_EQ(data[0], 0b00001000);
}

TEST(SetBitValueTest, SetTo0) {
    vector<uint8_t> data = {0b11111111};
    setBitValue(data, 3, false);
    EXPECT_EQ(data[0], 0b11110111);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}