#include <iostream>
#include <vector>
#include <bitset>
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

void printBits(const std::vector<uint8_t>& data) {
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << std::bitset<8>(data[i]);
        if (i < data.size() - 1) std::cout << " ";
    }
}

void printBytes(const std::vector<uint8_t>& data) {
    for (uint8_t b : data) {
        printf("%02X ", b);
    }
}

int main() {
    
    std::vector<uint8_t> data = {0xAB, 0xCD};
    
    std::cout << "Исходные данные (HEX): ";
    printBytes(data);
    std::cout << "\n";
    std::cout << "Исходные данные (биты): ";
    printBits(data);
    std::cout << "\n\n";
    
    std::cout << "Пример 1: Меняем местами бит 0 и бит 7\n";
    std::vector<size_t> perm1 = {7, 1, 2, 3, 4, 5, 6, 0};
    auto result1 = permuteBits(data, perm1, BitNumberingOrder::LsbToMsb, 0);
    std::cout << "Результат (HEX): ";
    printBytes(result1);
    std::cout << "\n";
    std::cout << "Результат (биты): ";
    printBits(result1);
    std::cout << "\n\n";
    
    std::cout << "Пример 2: Та же перестановка, индексация от 1\n";
    std::vector<size_t> perm2 = {8, 2, 3, 4, 5, 6, 7, 1};
    auto result2 = permuteBits(data, perm2, BitNumberingOrder::LsbToMsb, 1);
    std::cout << "Результат (HEX): ";
    printBytes(result2);
    std::cout << "\n";
    std::cout << "Результат (биты): ";
    printBits(result2);
    std::cout << "\n\n";
    
    std::cout << "Пример 3: Инвертирование всех битов\n";
    std::vector<size_t> perm3;
    for (size_t i = 0; i < 16; ++i) perm3.push_back(15 - i);
    auto result3 = permuteBits(data, perm3, BitNumberingOrder::LsbToMsb, 0);
    std::cout << "Результат (HEX): ";
    printBytes(result3);
    std::cout << "\n";
    std::cout << "Результат (биты): ";
    printBits(result3);
    std::cout << "\n\n";

    std::cout << "Пример 4: Нумерация от старшего бита (MSB)\n";
    std::vector<size_t> perm4 = {7, 1, 2, 3, 4, 5, 6, 0};
    auto result4 = permuteBits(data, perm4, BitNumberingOrder::MsbToLsb, 0);
    std::cout << "Результат (HEX): ";
    printBytes(result4);
    std::cout << "\n";
    std::cout << "Результат (биты): ";
    printBits(result4);
    std::cout << "\n\n";
    
    std::cout << "Пример 5: Перестановка байтов местами\n";
    std::vector<size_t> perm5;
    for (size_t i = 8; i < 16; ++i) perm5.push_back(i);
    for (size_t i = 0; i < 8; ++i) perm5.push_back(i);
    auto result5 = permuteBits(data, perm5, BitNumberingOrder::LsbToMsb, 0);
    std::cout << "Результат (HEX): ";
    printBytes(result5);
    std::cout << "\n";
    std::cout << "Результат (биты): ";
    printBits(result5);
    std::cout << "\n\n";
    
    return 0;
}