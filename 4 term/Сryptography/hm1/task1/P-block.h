#pragma once

#include <vector>
#include <cstdint>

using namespace std;

enum class BitOrder { LSB_FIRST, MSB_FIRST };
enum class IndexBase { ZERO_BASED, ONE_BASED };

pair<size_t, int> getBitPosition(size_t totalBits, int bitIndex, BitOrder order);
bool getBit(const vector<uint8_t>& data, size_t byteIndex, int bitPos);
void setBit(vector<uint8_t>& data, size_t byteIndex, int bitPos, bool value);
vector<uint8_t> permuteBits(const vector<uint8_t>& data,
                             const vector<int>& permutation,
                             BitOrder order,
                             IndexBase base);
void printBits(const vector<uint8_t>& data);
void printBytes(const vector<uint8_t>& data);