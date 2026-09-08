#include <gtest/gtest.h>
#include <vector>
#include <cstdint>
#include <stdexcept>

#include "P-block.h"

using namespace std;

TEST(GetBitPositionTest, LSB_FIRST) {
    auto [byte, bit] = getBitPosition(8, 0, BitOrder::LSB_FIRST);
    EXPECT_EQ(byte, 0);
    EXPECT_EQ(bit, 0);
    
    auto [byte2, bit2] = getBitPosition(8, 7, BitOrder::LSB_FIRST);
    EXPECT_EQ(byte2, 0);
    EXPECT_EQ(bit2, 7);
    
    auto [byte3, bit3] = getBitPosition(16, 8, BitOrder::LSB_FIRST);
    EXPECT_EQ(byte3, 1);
    EXPECT_EQ(bit3, 0);
}

TEST(GetBitPositionTest, MSB_FIRST) {
    auto [byte, bit] = getBitPosition(8, 0, BitOrder::MSB_FIRST);
    EXPECT_EQ(byte, 0);
    EXPECT_EQ(bit, 7);
    
    auto [byte2, bit2] = getBitPosition(8, 7, BitOrder::MSB_FIRST);
    EXPECT_EQ(byte2, 0);
    EXPECT_EQ(bit2, 0);
}

TEST(GetBitPositionTest, OutOfRange) {
    EXPECT_THROW(getBitPosition(8, 8, BitOrder::LSB_FIRST), out_of_range);
    EXPECT_THROW(getBitPosition(8, -1, BitOrder::LSB_FIRST), out_of_range);
}

TEST(BitOperationsTest, GetBit) {
    vector<uint8_t> data = {0b10101010};
    EXPECT_TRUE(getBit(data, 0, 7));
    EXPECT_FALSE(getBit(data, 0, 6));
    EXPECT_FALSE(getBit(data, 0, 0));
}

TEST(BitOperationsTest, SetBit) {
    vector<uint8_t> data = {0b00000000};
    setBit(data, 0, 0, true);
    EXPECT_EQ(data[0], 0b00000001);
    
    setBit(data, 0, 7, true);
    EXPECT_EQ(data[0], 0b10000001);
    
    setBit(data, 0, 0, false);
    EXPECT_EQ(data[0], 0b10000000);
}

TEST(PermuteBitsTest, ReverseBits) {
    vector<uint8_t> data = {0xAA};
    vector<int> perm = {7,6,5,4,3,2,1,0};
    auto result = permuteBits(data, perm, BitOrder::LSB_FIRST, IndexBase::ZERO_BASED);
    EXPECT_EQ(result[0], 0x55);
}

TEST(PermuteBitsTest, Identity) {
    vector<uint8_t> data = {0xAA};
    vector<int> perm = {0,1,2,3,4,5,6,7};
    auto result = permuteBits(data, perm, BitOrder::LSB_FIRST, IndexBase::ZERO_BASED);
    EXPECT_EQ(result[0], 0xAA);
}

TEST(PermuteBitsTest, LeftShiftBy3) {
    vector<uint8_t> data = {0xAA};
    vector<int> perm(8);
    for (int i = 0; i < 8; i++) {
        perm[i] = (i + 3) % 8;
    }
    auto result = permuteBits(data, perm, BitOrder::LSB_FIRST, IndexBase::ZERO_BASED);
    EXPECT_EQ(result[0], 0x55);
}

TEST(PermuteBitsTest, SwapHalves) {
    vector<uint8_t> data = {0xAB};
    vector<int> perm(8);
    for (int i = 0; i < 8; i++) {
        if (i < 4) perm[i] = i + 4;
        else perm[i] = i - 4;
    }
    auto result = permuteBits(data, perm, BitOrder::LSB_FIRST, IndexBase::ZERO_BASED);
    EXPECT_EQ(result[0], 0xBA);
}

TEST(PermuteBitsTest, OneBased) {
    vector<uint8_t> data = {0x01};
    vector<int> perm = {8,7,6,5,4,3,2,1};
    auto result = permuteBits(data, perm, BitOrder::LSB_FIRST, IndexBase::ONE_BASED);
    EXPECT_EQ(result[0], 0x80);
}

TEST(PermuteBitsTest, SwapBytes) {
    vector<uint8_t> data = {0x12, 0x34};
    vector<int> perm(16);
    for (int i = 0; i < 8; i++) {
        perm[i] = i + 8;
        perm[i + 8] = i;
    }
    auto result = permuteBits(data, perm, BitOrder::LSB_FIRST, IndexBase::ZERO_BASED);
    EXPECT_EQ(result[0], 0x34);
    EXPECT_EQ(result[1], 0x12);
}

TEST(PermuteBitsTest, WrongPermLength) {
    vector<uint8_t> data = {0xAA};
    vector<int> perm = {0,1,2,3,4,5,6};
    EXPECT_THROW(permuteBits(data, perm, BitOrder::LSB_FIRST, IndexBase::ZERO_BASED),
                 invalid_argument);
}

TEST(PermuteBitsTest, IndexOutOfRange) {
    vector<uint8_t> data = {0xAA};
    vector<int> perm = {10,1,2,3,4,5,6,7};
    EXPECT_THROW(permuteBits(data, perm, BitOrder::LSB_FIRST, IndexBase::ZERO_BASED),
                 out_of_range);
}

TEST(PermuteBitsTest, EmptyData) {
    vector<uint8_t> empty;
    vector<int> perm;
    auto result = permuteBits(empty, perm, BitOrder::LSB_FIRST, IndexBase::ZERO_BASED);
    EXPECT_TRUE(result.empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}