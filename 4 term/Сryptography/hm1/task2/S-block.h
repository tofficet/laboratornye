#pragma once

#include <vector>
#include <cstdint>
#include <unordered_map>
#include <functional>
#include <string>

using namespace std;

vector<uint8_t> substituteBits(const vector<uint8_t>& input,
                                const unordered_map<uint8_t, uint8_t>& sbox);

vector<uint8_t> substituteBits(const vector<uint8_t>& input,
                                const function<uint8_t(uint8_t)>& func);

void printBytes(const vector<uint8_t>& data, const string& title = "");

void printAsText(const vector<uint8_t>& data, const string& title = "");