#include <gtest/gtest.h>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <functional>

#include "S-block.h"

using namespace std;

TEST(SBoxMapTest, BasicSubstitution) {
    unordered_map<uint8_t, uint8_t> sbox = {
        {0x01, 0x02},
        {0x02, 0x03},
        {0x03, 0x01}
    };
    
    vector<uint8_t> input = {0x01, 0x02, 0x03, 0xFF};
    vector<uint8_t> expected = {0x02, 0x03, 0x01, 0xFF};
    
    auto result = substituteBits(input, sbox);
    EXPECT_EQ(result, expected);
}

TEST(SBoxMapTest, EmptySBox) {
    unordered_map<uint8_t, uint8_t> empty;
    vector<uint8_t> input = {0x01, 0x02, 0x03};
    
    auto result = substituteBits(input, empty);
    EXPECT_EQ(result, input);
}

TEST(SBoxMapTest, EmptyInput) {
    unordered_map<uint8_t, uint8_t> sbox = {{0x01, 0x02}};
    vector<uint8_t> input;
    
    auto result = substituteBits(input, sbox);
    EXPECT_TRUE(result.empty());
}

TEST(SBoxFunctionTest, XORWithConstant) {
    auto xorSbox = [](uint8_t x) { return x ^ 0x55; };
    
    vector<uint8_t> input = {0xAA, 0x00, 0xFF};
    vector<uint8_t> expected = {0xFF, 0x55, 0xAA};
    
    auto result = substituteBits(input, xorSbox);
    EXPECT_EQ(result, expected);
}

TEST(SBoxFunctionTest, ReverseBits) {
    auto reverseBits = [](uint8_t x) {
        uint8_t result = 0;
        for (int i = 0; i < 8; i++) {
            if (x & (1 << i)) result |= (1 << (7 - i));
        }
        return result;
    };
    
    vector<uint8_t> input = {0xAA};
    vector<uint8_t> expected = {0x55};
    
    auto result = substituteBits(input, reverseBits);
    EXPECT_EQ(result[0], expected[0]);
}

TEST(SBoxFunctionTest, Identity) {
    auto identity = [](uint8_t x) { return x; };
    
    vector<uint8_t> input = {0x01, 0x02, 0x03, 0xFF};
    auto result = substituteBits(input, identity);
    EXPECT_EQ(result, input);
}

TEST(SBoxFunctionTest, EmptyInput) {
    auto func = [](uint8_t x) { return x ^ 0x55; };
    vector<uint8_t> input;
    
    auto result = substituteBits(input, func);
    EXPECT_TRUE(result.empty());
}

TEST(SBoxTest, TextData) {
    auto toUpper = [](uint8_t x) -> uint8_t {
        return (x >= 'a' && x <= 'z') ? (x - 32) : x;
    };
    
    string text = "Hello, World!";
    vector<uint8_t> input(text.begin(), text.end());
    
    auto result = substituteBits(input, toUpper);
    
    string expected = "HELLO, WORLD!";
    for (size_t i = 0; i < result.size(); i++) {
        EXPECT_EQ(result[i], (uint8_t)expected[i]);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}