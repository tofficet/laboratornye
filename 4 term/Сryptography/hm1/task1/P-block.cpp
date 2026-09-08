#include <iostream>
#include <vector>
#include <cstdint>
#include <bitset>

using namespace std;

enum class BitOrder {
    LSB_FIRST,
    MSB_FIRST
};

enum class IndexBase {
    ZERO_BASED,
    ONE_BASED
};

pair<size_t, int> getBitPosition(size_t totalBits, int bitIndex, BitOrder order) {
    if (bitIndex < 0 || bitIndex >= (int)totalBits) {
        throw out_of_range("Индекс бита вне диапазона");
    }
    
    size_t byteIndex;
    int bitPos;
    
    if (order == BitOrder::LSB_FIRST) {
        byteIndex = bitIndex / 8;
        bitPos = bitIndex % 8;
    } else {
        byteIndex = bitIndex / 8;
        bitPos = 7 - (bitIndex % 8);
    }
    
    return {byteIndex, bitPos};
}

bool getBit(const vector<uint8_t>& data, size_t byteIndex, int bitPos) {
    return (data[byteIndex] >> bitPos) & 1;
}

void setBit(vector<uint8_t>& data, size_t byteIndex, int bitPos, bool value) {
    if (value)
        data[byteIndex] |= (1 << bitPos);
    else
        data[byteIndex] &= ~(1 << bitPos);
}

vector<uint8_t> permuteBits(const vector<uint8_t>& data,
                             const vector<int>& permutation,
                             BitOrder order,
                             IndexBase base) {
    
    size_t totalBits = data.size() * 8;
    
    if (permutation.size() != totalBits) {
        throw invalid_argument("Длина перестановки не равна количеству битов");
    }
    
    // индексы -> 0-based
    vector<int> adjustedPerm(permutation.size());
    for (size_t i = 0; i < permutation.size(); i++) {
        if (base == IndexBase::ONE_BASED) {
            adjustedPerm[i] = permutation[i] - 1;
        } else {
            adjustedPerm[i] = permutation[i];
        }
        if (adjustedPerm[i] < 0 || adjustedPerm[i] >= (int)totalBits) {
            throw out_of_range("Индекс вне диапазона");
        }
    }
    
    vector<uint8_t> result(data.size(), 0);
    
    for (size_t i = 0; i < permutation.size(); i++) {
        int srcIdx = adjustedPerm[i];
        
        // берем бит их исходных данных
        auto [srcByte, srcBit] = getBitPosition(totalBits, srcIdx, order);
        bool bitValue = getBit(data, srcByte, srcBit);
        
        // записываем его в результат
        auto [destByte, destBit] = getBitPosition(totalBits, i, order);
        setBit(result, destByte, destBit, bitValue);
    }
    
    return result;
}

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