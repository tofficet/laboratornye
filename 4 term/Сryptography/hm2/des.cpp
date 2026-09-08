#include "include/des.h"
#include <cstring>
#include <stdexcept>

using namespace std;

// преобразование массива байтов в 64-битное число (big-endian)
uint64_t bytesToBits(const vector<uint8_t>& bytes) {
    if (bytes.empty()) 
        return 0;
    uint64_t result = 0;
    for (size_t i = 0; i < bytes.size() && i < 8; i++) {
        result = (result << 8) | bytes[i];
    }
    return result;
}

// преобразование 64-битного числа в массив байтов (big-endian)
vector<uint8_t> bitsToBytes(uint64_t bits, size_t byteCount) {
    vector<uint8_t> bytes(byteCount);
    for (size_t i = 0; i < byteCount; i++) {
        bytes[byteCount - 1 - i] = bits & 0xFF;
        bits >>= 8;
    }
    return bytes;
}

// применение перестановки к 64-битному блоку
uint64_t applyPermutation64(uint64_t input, const uint8_t* table, int tableSize) {
    uint64_t output = 0;
    for (int i = 0; i < tableSize; i++) {
        int srcBit = 64 - table[i];  // таблица использует 1-based нумерацию слева
        uint64_t bit = (input >> srcBit) & 1;
        output = (output << 1) | bit;  // проще: сдвигаем результат и добавляем бит
    }
    return output;
}

// расширение 32 -> 48 битов (таблица E)
uint64_t expand32to48(uint32_t right) {
    uint64_t result = 0;
    for (int i = 0; i < 48; i++) {
        int srcBit = 32 - E[i];  // E использует 1-based нумерацию для 32 бит
        uint64_t bit = (right >> srcBit) & 1;
        result = (result << 1) | bit;
    }
    return result;
}

// S-блоки: 48 -> 32 бита
uint32_t sboxSubstitution(uint64_t expanded) {
    uint32_t result = 0;
    
    for (int i = 0; i < 8; i++) {
        // берём 6 битов для S-блока i
        int shift = 42 - i * 6;
        uint8_t sixBits = (expanded >> shift) & 0x3F;
        
        // строка (первый и последний бит) и столбец (средние 4 бита)
        int row = ((sixBits & 0x20) >> 4) | (sixBits & 0x01);
        int col = (sixBits >> 1) & 0x0F;
        
        uint8_t sboxValue = S[i][row][col];
        
        // 4 бита в результат
        result = (result << 4) | sboxValue;
    }
    
    return result;
}

// перестановка P (32 -> 32 бита)
uint32_t permuteP(uint32_t input) {
    uint32_t result = 0;
    for (int i = 0; i < 32; i++) {
        int srcBit = 32 - P[i];  // P использует 1-based нумерацию для 32 бит
        uint32_t bit = (input >> srcBit) & 1;
        result = (result << 1) | bit;
    }
    return result;
}

uint32_t DES_F::f(uint32_t right, uint64_t roundKey) {
    // 1) расширение
    uint64_t expanded = expand32to48(right);
    
    // 2) XOR с раундовым ключом
    uint64_t xored = expanded ^ roundKey;
    
    // 3) S-блоки (48 -> 32)
    uint32_t afterSbox = sboxSubstitution(xored);
    
    // 4) перестановка P
    uint32_t result = permuteP(afterSbox);
    
    return result;
}

vector<uint8_t> DES_F::apply(const vector<uint8_t>& halfBlock,
                              const vector<uint8_t>& roundKey) {
    if (halfBlock.size() != 4) {
        throw invalid_argument("DES_F: halfBlock must be 4 bytes");
    }
    if (roundKey.size() != 6) {
        throw invalid_argument("DES_F: roundKey must be 6 bytes");
    }
    
    uint32_t right = bytesToBits(halfBlock);
    uint64_t key = bytesToBits(roundKey);
    
    uint32_t result32 = f(right, key);
    
    // преобразуем результат в 4 байта
    vector<uint8_t> result = bitsToBytes(result32, 4);
    return result;
}

uint64_t DESKeySchedule::applyPC1(uint64_t key) {
    uint64_t result = 0;
    for (int i = 0; i < 56; i++) {
        int srcBit = 64 - PC1[i];  // PC1 использует 1-based нумерацию для 64 бит
        uint64_t bit = (key >> srcBit) & 1;
        result = (result << 1) | bit;
    }
    return result;
}

uint64_t DESKeySchedule::applyPC2(uint64_t key56) {
    uint64_t result = 0;
    for (int i = 0; i < 48; i++) {
        int srcBit = 56 - PC2[i];  // PC2 использует 1-based нумерацию для 56 бит
        uint64_t bit = (key56 >> srcBit) & 1;
        result = (result << 1) | bit;
    }
    return result;
}

uint64_t DESKeySchedule::leftShift(uint64_t key56, int shift) {
    // разделение на левую и правую половины (по 28 битов)
    uint32_t left = (key56 >> 28) & 0x0FFFFFFF;
    uint32_t right = key56 & 0x0FFFFFFF;
    
    // циклический сдвиг влево
    left = ((left << shift) | (left >> (28 - shift))) & 0x0FFFFFFF;
    right = ((right << shift) | (right >> (28 - shift))) & 0x0FFFFFFF;
    
    return ((uint64_t)left << 28) | right;
}

vector<vector<uint8_t>> DESKeySchedule::expandKey(const vector<uint8_t>& key) {
    if (key.size() != 8) {
        throw invalid_argument("DES key must be 8 bytes");
    }
    
    uint64_t key64 = bytesToBits(key);
    uint64_t key56 = applyPC1(key64);
    
    vector<vector<uint8_t>> roundKeys(16);
    
    uint64_t currentKey = key56;
    for (int round = 0; round < 16; round++) {
        currentKey = leftShift(currentKey, SHIFTS[round]);
        uint64_t roundKey48 = applyPC2(currentKey);
        roundKeys[round] = bitsToBytes(roundKey48, 6);
    }
    
    return roundKeys;
}

DESCipher::DESCipher()
    : FeistelCipher(new DES_F(), new DESKeySchedule(), 16, 8) {}

vector<uint8_t> DESCipher::preprocess(const vector<uint8_t>& block) {
    if (block.size() != 8) {
        throw invalid_argument("DES block must be 8 bytes");
    }
    uint64_t block64 = bytesToBits(block);
    uint64_t afterIP = applyPermutation64(block64, IP, 64);
    return bitsToBytes(afterIP, 8);
}

vector<uint8_t> DESCipher::postprocess(const vector<uint8_t>& block) {
    if (block.size() != 8) {
        throw invalid_argument("DES block must be 8 bytes");
    }
    uint64_t block64 = bytesToBits(block);
    uint64_t afterFP = applyPermutation64(block64, FP, 64);
    return bitsToBytes(afterFP, 8);
}