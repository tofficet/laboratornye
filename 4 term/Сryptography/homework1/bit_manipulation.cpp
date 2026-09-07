#include <iostream>
#include <vector>
#include <bitset>
#include <cstdint>
#include <stdexcept>
#include <string>

class BitOperations {
private:
    static size_t getBitSize(const std::vector<uint8_t>& data) {
        return data.size() * 8;
    }
    
    static bool getBit(const std::vector<uint8_t>& data, size_t pos) {
        size_t byteIdx = pos / 8;
        size_t bitIdx = pos % 8;
        return (data[byteIdx] >> bitIdx) & 1;
    }
    
    static void setBit(std::vector<uint8_t>& data, size_t pos, bool value) {
        size_t byteIdx = pos / 8;
        size_t bitIdx = pos % 8;
        if (value) {
            data[byteIdx] |= (1 << bitIdx);
        } else {
            data[byteIdx] &= ~(1 << bitIdx);
        }
    }
    
public:
    static std::vector<uint8_t> rotateLeft(const std::vector<uint8_t>& data, size_t k) {
        if (data.empty()) return {};
        
        size_t totalBits = getBitSize(data);
        k = k % totalBits;
        
        if (k == 0) return data;
        
        std::vector<uint8_t> result(data.size(), 0);
        
        for (size_t i = 0; i < totalBits; ++i) {
            size_t newPos = (i + k) % totalBits;
            bool bit = getBit(data, i);
            setBit(result, newPos, bit);
        }
        
        return result;
    }
    
    static std::vector<uint8_t> rotateRight(const std::vector<uint8_t>& data, size_t k) {
        if (data.empty()) return {};
        
        size_t totalBits = getBitSize(data);
        k = k % totalBits;
        
        if (k == 0) return data;
        
        std::vector<uint8_t> result(data.size(), 0);
        
        for (size_t i = 0; i < totalBits; ++i) {
            size_t newPos = (i + totalBits - k) % totalBits;
            bool bit = getBit(data, i);
            setBit(result, newPos, bit);
        }
        
        return result;
    }
    
    static std::vector<uint8_t> applyMask(const std::vector<uint8_t>& data, const std::vector<uint8_t>& mask) {
        size_t minSize = std::min(data.size(), mask.size());
        std::vector<uint8_t> result(minSize, 0);
        
        for (size_t i = 0; i < minSize; ++i) {
            result[i] = data[i] & mask[i];
        }
        
        return result;
    }
    
    static std::vector<uint8_t> getBitsRange(const std::vector<uint8_t>& data, size_t i, size_t j) {
        if (i > j) {
            throw std::invalid_argument("i должен быть меньше или равен j");
        }
        
        size_t totalBits = getBitSize(data);
        if (j >= totalBits) {
            throw std::out_of_range("j выходит за пределы диапазона");
        }
        
        size_t rangeSize = j - i + 1;
        std::vector<uint8_t> result((rangeSize + 7) / 8, 0);
        
        for (size_t pos = i; pos <= j; ++pos) {
            bool bit = getBit(data, pos);
            size_t newPos = pos - i;
            setBit(result, newPos, bit);
        }
        
        return result;
    }
    
    static void swapBits(std::vector<uint8_t>& data, size_t i, size_t j) {
        size_t totalBits = getBitSize(data);
        
        if (i >= totalBits || j >= totalBits) {
            throw std::out_of_range("Индекс бита вне диапазона");
        }
        
        if (i == j) return;
        
        bool bitI = getBit(data, i);
        bool bitJ = getBit(data, j);
        
        setBit(data, i, bitJ);
        setBit(data, j, bitI);
    }
    
    static void setBitValue(std::vector<uint8_t>& data, size_t i, bool value) {
        size_t totalBits = getBitSize(data);
        
        if (i >= totalBits) {
            throw std::out_of_range("Индекс бита вне диапазона");
        }
        
        setBit(data, i, value);
    }
};

void printBits(const std::vector<uint8_t>& data, const std::string& label) {
    std::cout << label << ": ";
    if (data.empty()) {
        std::cout << "(пусто)";
    } else {
        for (size_t i = 0; i < data.size(); ++i) {
            std::cout << std::bitset<8>(data[i]);
            if (i < data.size() - 1) std::cout << " ";
        }
    }
    std::cout << std::endl;
}

void printBytes(const std::vector<uint8_t>& data, const std::string& label) {
    std::cout << label << ": ";
    for (uint8_t b : data) {
        printf("%02X ", b);
    }
    std::cout << std::endl;
}

int main() {

    std::vector<uint8_t> data = {0xAB, 0xCD};
    
    printBits(data, "Исходные данные (биты)");
    printBytes(data, "Исходные данные (HEX)");
    std::cout << "\n";
    
    std::cout << "a) Циклический сдвиг влево на 3 бита\n";

    auto left3 = BitOperations::rotateLeft(data, 3);
    printBits(left3, "Результат (биты)");
    printBytes(left3, "Результат (HEX)");
    std::cout << "\n";
    
    std::cout << "b) Циклический сдвиг вправо на 5 битов\n";
    auto right5 = BitOperations::rotateRight(data, 5);
    printBits(right5, "Результат (биты)");
    printBytes(right5, "Результат (HEX)");
    std::cout << "\n";

    std::cout << "c) Применение маски 0xF0\n";
    std::vector<uint8_t> mask = {0xF0, 0xF0};
    auto masked = BitOperations::applyMask(data, mask);
    printBytes(masked, "Результат (HEX)");
    std::cout << std::endl;
    
    std::cout << "d) Получение битов с 3-го по 10-й\n";
    auto range = BitOperations::getBitsRange(data, 3, 10);
    printBits(range, "Результат (биты)");
    printBytes(range, "Результат (HEX)");
    std::cout << "\n";
    
    std::cout << "e) Обмен местами 0-го и 15-го бита\n";
    auto swapped = data;
    std::cout << "До обмена: ";
    printBits(swapped, "");
    BitOperations::swapBits(swapped, 0, 15);
    std::cout << "После обмена: ";
    printBits(swapped, "");
    std::cout << std::endl;
    
    std::cout << "f) Установка 7-го бита в 1\n";
    auto set = data;
    std::cout << "До установки: ";
    printBits(set, "");
    BitOperations::setBitValue(set, 7, true);
    std::cout << "После установки: ";
    printBits(set, "");
    
    std::cout << "\n";
    std::cout << "f) Установка 8-го бита в 0\n";
    auto clear = data;
    std::cout << "До установки: ";
    printBits(clear, "");
    BitOperations::setBitValue(clear, 8, false);
    std::cout << "После установки: ";
    printBits(clear, "");
    
    
    return 0;
}