#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <cstdint>
#include <stdexcept>

enum class BitNumberingOrder {
    LsbToMsb,
    MsbToLsb
};

std::vector<uint8_t> permuteBits(const std::vector<uint8_t>& data,
                                  const std::vector<size_t>& permutation,
                                  BitNumberingOrder order,
                                  size_t startIndex) {
    if (permutation.empty()) {
        throw std::invalid_argument("Правило перестановки не может быть пустым");
    }
    
    size_t totalBits = data.size() * 8;
    
    for (size_t idx : permutation) {
        size_t adjustedIdx = idx - startIndex;
        if (adjustedIdx >= totalBits) {
            throw std::out_of_range("Индекс бита вне диапазона");
        }
    }
    
    std::vector<uint8_t> result((totalBits + 7) / 8, 0);
    
    for (size_t i = 0; i < permutation.size(); ++i) {
        size_t srcBitPos = permutation[i] - startIndex;
        
        size_t actualSrcBitPos;
        if (order == BitNumberingOrder::LsbToMsb) {
            actualSrcBitPos = srcBitPos;
        } else {
            actualSrcBitPos = totalBits - 1 - srcBitPos;
        }
        
        size_t srcByteIdx = actualSrcBitPos / 8;
        size_t srcBitIdx = actualSrcBitPos % 8;
        uint8_t bitValue = (data[srcByteIdx] >> srcBitIdx) & 1;
        
        size_t dstBitPos = i;
        size_t dstByteIdx = dstBitPos / 8;
        size_t dstBitIdx = dstBitPos % 8;
        
        if (bitValue) {
            result[dstByteIdx] |= (1 << dstBitIdx);
        } else {
            result[dstByteIdx] &= ~(1 << dstBitIdx);
        }
    }
    
    return result;
}

std::vector<uint8_t> substituteBits(const std::vector<uint8_t>& data,
                                     const std::map<std::vector<uint8_t>, std::vector<uint8_t>>& sbox) {
    if (sbox.empty()) {
        throw std::invalid_argument("S-блок не может быть пустым");
    }
    
    std::vector<uint8_t> result;
    result.reserve(data.size());
    
    for (size_t i = 0; i < data.size(); ++i) {
        std::vector<uint8_t> byteVec = {data[i]};
        auto it = sbox.find(byteVec);
        
        if (it != sbox.end()) {
            if (it->second.size() != 1) {
                throw std::runtime_error("S-блок должен возвращать один байт");
            }
            result.push_back(it->second[0]);
        } else {
            result.push_back(data[i]);
        }
    }
    
    return result;
}

std::vector<uint8_t> substituteBits(const std::vector<uint8_t>& data,
                                     const std::function<uint8_t(uint8_t)>& sbox_func) {
    if (!sbox_func) {
        throw std::invalid_argument("Функция S-блока не может быть пустой");
    }
    
    std::vector<uint8_t> result;
    result.reserve(data.size());
    
    for (uint8_t byte : data) {
        result.push_back(sbox_func(byte));
    }
    
    return result;
}

void printBytes(const std::vector<uint8_t>& data, const std::string& label) {
    std::cout << label << ": ";
    for (uint8_t b : data) {
        printf("%02X ", b);
    }
    std::cout << std::endl;
}

int main() {
    
    std::vector<uint8_t> data = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    
    printBytes(data, "Исходные данные");
    std::cout << "\n";

    std::cout << "Пример 1: S-блок через map \n";

    
    std::map<std::vector<uint8_t>, std::vector<uint8_t>> sbox_map = {
        {{0x12}, {0x21}},
        {{0x34}, {0x43}},
        {{0x56}, {0x65}},
        {{0x78}, {0x87}},
        {{0x9A}, {0xA9}},
        {{0xBC}, {0xCB}},
        {{0xDE}, {0xED}},
        {{0xF0}, {0x0F}}
    };
    
    auto result1 = substituteBits(data, sbox_map);
    printBytes(result1, "Результат замены");
    std::cout << "\n";
    
    std::cout << "Пример 2: S-блок через функциональный объект\n";
    
    auto sbox_func = [](uint8_t byte) -> uint8_t {
        uint8_t result = 0;
        for (int i = 0; i < 8; ++i) {
            result |= ((byte >> i) & 1) << (7 - i);
        }
        return result;
    };
    
    auto result2 = substituteBits(data, sbox_func);
    printBytes(result2, "Результат замены");
    std::cout << "\n";
    
    return 0;
}