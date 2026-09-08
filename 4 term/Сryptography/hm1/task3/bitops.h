#pragma once

#include <vector>
#include <cstdint>

using namespace std;

vector<uint8_t> rotateLeft(const vector<uint8_t>& data, int k);

vector<uint8_t> rotateRight(const vector<uint8_t>& data, int k);

vector<uint8_t> applyMask(const vector<uint8_t>& data, const vector<uint8_t>& mask);

vector<uint8_t> extractBits(const vector<uint8_t>& data, int i, int j);

void swapBits(vector<uint8_t>& data, int i, int j);

void setBitValue(vector<uint8_t>& data, int i, bool value);

void printBits(const vector<uint8_t>& data);
void printBytes(const vector<uint8_t>& data);