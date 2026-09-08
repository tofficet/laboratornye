#include "bitops.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
    cout << "=== БИТОВЫЕ ОПЕРАЦИИ (задание 3) ===" << endl;
    
    vector<uint8_t> data = {0b10110011, 0b01011101};
    cout << "\nИсходные данные:" << endl;
    cout << "Биты: ";
    printBits(data);
    cout << "Hex: ";
    printBytes(data);
    
    // 3a. Сдвиг влево на 3
    auto left = rotateLeft(data, 3);
    cout << "\n--- 3a. Циклический сдвиг влево на 3 ---" << endl;
    cout << "Биты: ";
    printBits(left);
    
    // 3b. Сдвиг вправо на 3
    auto right = rotateRight(data, 3);
    cout << "\n--- 3b. Циклический сдвиг вправо на 3 ---" << endl;
    cout << "Биты: ";
    printBits(right);
    
    // 3c. Маска
    vector<uint8_t> mask = {0x0F, 0x0F};
    auto masked = applyMask(data, mask);
    cout << "\n--- 3c. Применение маски 0x0F ---" << endl;
    cout << "Биты: ";
    printBits(masked);
    
    // 3d. Выделение битов 4-10
    auto extracted = extractBits(data, 4, 10);
    cout << "\n--- 3d. Выделение битов с 4 по 10 ---" << endl;
    cout << "Биты: ";
    printBits(extracted);
    
    // 3e. Обмен битов 2 и 14
    vector<uint8_t> swapData = data;
    swapBits(swapData, 2, 14);
    cout << "\n--- 3e. Обмен битов 2 и 14 ---" << endl;
    cout << "Было: ";
    printBits(data);
    cout << "Стало: ";
    printBits(swapData);
    
    // 3f. Установка бита
    vector<uint8_t> setData = {0b00000000};
    setBitValue(setData, 5, true);
    cout << "\n--- 3f. Установка бита 5 в 1 ---" << endl;
    cout << "Биты: ";
    printBits(setData);
    
    return 0;
}