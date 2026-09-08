#include "bitops.h"
#include <iostream>
#include <bitset>
#include <stdexcept>

using namespace std;

void printBits(const vector<uint8_t>& data) {
    for (size_t i = 0; i < data.size(); i++) {
        cout << bitset<8>(data[i]) << " ";
    }
    cout << endl;
}

void printBytes(const vector<uint8_t>& data) {
    for (auto b : data) {
        cout << hex << (int)b << " ";
    }
    cout << dec << endl;
}

vector<uint8_t> rotateLeft(const vector<uint8_t>& data, int k) {
    if (data.empty()) return {};
    
    int totalBits = data.size() * 8;
    k %= totalBits;
    if (k == 0) 
        return data;
    
    vector<uint8_t> result(data.size(), 0);
    
    for (int i = 0; i < totalBits; i++) {
        int srcByte = i / 8;
        int srcBit = i % 8;
        int dstBit = (i + k) % totalBits;
        int dstByte = dstBit / 8;
        int dstBitPos = dstBit % 8;
        
        bool bit = (data[srcByte] >> srcBit) & 1;
        if (bit) 
            result[dstByte] |= (1 << dstBitPos);
    }
    return result;
}

vector<uint8_t> rotateRight(const vector<uint8_t>& data, int k) {
    if (data.empty()) return {};
    
    int totalBits = data.size() * 8;
    k %= totalBits;
    if (k == 0) 
        return data;
    
    vector<uint8_t> result(data.size(), 0);
    
    for (int i = 0; i < totalBits; i++) {
        int srcByte = i / 8;
        int srcBit = i % 8;
        int dstBit = (i - k + totalBits) % totalBits;
        int dstByte = dstBit / 8;
        int dstBitPos = dstBit % 8;
        
        bool bit = (data[srcByte] >> srcBit) & 1;
        if (bit) 
            result[dstByte] |= (1 << dstBitPos);
    }
    return result;
}

vector<uint8_t> applyMask(const vector<uint8_t>& data, const vector<uint8_t>& mask) {
    vector<uint8_t> result(data.size());
    for (size_t i = 0; i < data.size(); i++) {
        uint8_t m = (i < mask.size()) ? mask[i] : 0xFF;
        result[i] = data[i] & m;
    }
    return result;
}

vector<uint8_t> extractBits(const vector<uint8_t>& data, int i, int j) {
    int totalBits = data.size() * 8;
    if (i < 0 || j >= totalBits || i > j) 
        return {};
    
    int len = j - i + 1;
    vector<uint8_t> result((len + 7) / 8, 0);
    
    for (int bit = 0; bit < len; bit++) {
        int srcBit = i + bit;
        int srcByte = srcBit / 8;
        int srcBitPos = srcBit % 8;
        
        bool value = (data[srcByte] >> srcBitPos) & 1;
        
        int dstBit = bit;
        int dstByte = dstBit / 8;
        int dstBitPos = dstBit % 8;
        
        if (value) result[dstByte] |= (1 << dstBitPos);
    }
    return result;
}

void swapBits(vector<uint8_t>& data, int i, int j) {
    int totalBits = data.size() * 8;
    if (i < 0 || j < 0 || i >= totalBits || j >= totalBits) return;
    if (i == j) return;
    
    int byteI = i / 8, bitI = i % 8;
    int byteJ = j / 8, bitJ = j % 8;
    
    bool valI = (data[byteI] >> bitI) & 1;
    bool valJ = (data[byteJ] >> bitJ) & 1;
    
    if (valI == valJ) 
        return;
    
    if (valJ)
        data[byteI] |= (1 << bitI);
    else
        data[byteI] &= ~(1 << bitI);
    
    if (valI)
        data[byteJ] |= (1 << bitJ);
    else
        data[byteJ] &= ~(1 << bitJ);
}

void setBitValue(vector<uint8_t>& data, int i, bool value) {
    int totalBits = data.size() * 8;
    if (i < 0 || i >= totalBits) 
        return;
    
    int byteIdx = i / 8;
    int bitPos = i % 8;
    
    if (value)
        data[byteIdx] |= (1 << bitPos);
    else
        data[byteIdx] &= ~(1 << bitPos);
}