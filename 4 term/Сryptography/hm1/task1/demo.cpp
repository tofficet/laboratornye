#include "P-block.h"
#include <iostream>

using namespace std;

int main() {
    cout << "=== ДЕМОНСТРАЦИЯ ПЕРЕСТАНОВКИ БИТОВ ===" << endl;

    cout << "\n--- ПРИМЕР 1: Обратный порядок битов ---" << endl;
    
    vector<uint8_t> data1 = {0xAA};  // 10101010
    cout << "Исходные данные (1 байт): " << endl;
    cout << "Биты: ";
    printBits(data1);
    cout << "Hex: 0x" << hex << (int)data1[0] << dec << endl;

    vector<int> reversePerm(8);
    for (int i = 0; i < 8; i++) {
        reversePerm[i] = 7 - i;
    }
    
    auto result1 = permuteBits(data1, reversePerm, BitOrder::LSB_FIRST, IndexBase::ZERO_BASED);
    
    cout << "Результат (обратный порядок): " << endl;
    cout << "Биты: ";
    printBits(result1);
    cout << "Hex: 0x" << hex << (int)result1[0] << dec << endl;

    cout << "\n--- ПРИМЕР 2: Циклический сдвиг влево на 3 бита ---" << endl;
    
    vector<uint8_t> data2 = {0xAA};  // 10101010
    cout << "Исходные данные: ";
    printBits(data2);
    
    vector<int> leftShiftPerm(8);
    for (int i = 0; i < 8; i++) {
        leftShiftPerm[i] = (i + 3) % 8;
    }
    
    auto result2 = permuteBits(data2, leftShiftPerm, BitOrder::LSB_FIRST, IndexBase::ZERO_BASED);
    cout << "Циклический сдвиг влево на 3: ";
    printBits(result2);

    cout << "\n--- ПРИМЕР 3: Обмен половин байта (старшие 4 бита <-> младшие 4 бита) ---" << endl;
    
    vector<uint8_t> data3 = {0xAB};  // 10101011
    cout << "Исходные данные: ";
    printBits(data3);
    
    vector<int> swapHalvesPerm(8);
    for (int i = 0; i < 8; i++) {
        if (i < 4) {
            swapHalvesPerm[i] = i + 4;
        } else {
            swapHalvesPerm[i] = i - 4;
        }
    }
    
    auto result3 = permuteBits(data3, swapHalvesPerm, BitOrder::LSB_FIRST, IndexBase::ZERO_BASED);
    cout << "После обмена половин: ";
    printBits(result3);

    cout << "\n--- ПРИМЕР 4: Перестановка с 2 байтами ---" << endl;
    
    vector<uint8_t> data4 = {0x12, 0x34};  // 00010010 00110100
    cout << "Исходные данные (2 байта):" << endl;
    cout << "Биты: ";
    printBits(data4);
    cout << "Hex: ";
    printBytes(data4);

    // обмен бита 0 и бита 15
    vector<int> swapPerm(16);
    for (int i = 0; i < 16; i++) {
        if (i == 0) swapPerm[i] = 15;
        else if (i == 15) swapPerm[i] = 0;
        else swapPerm[i] = i;
    }
    
    auto result4 = permuteBits(data4, swapPerm, BitOrder::LSB_FIRST, IndexBase::ZERO_BASED);
    cout << "После обмена бит 0 и 15:" << endl;
    cout << "Биты: ";
    printBits(result4);
    cout << "Hex: ";
    printBytes(result4);

    cout << "\n--- ПРИМЕР 5: Перемешивание битов (случайный шаблон) ---" << endl;
    
    vector<uint8_t> data5 = {0x55, 0xAA};  // 01010101 10101010
    cout << "Исходные данные (2 байта):" << endl;
    cout << "Биты: ";
    printBits(data5);
    
    vector<int> randomPerm = {7, 11, 3, 15, 1, 9, 5, 13, 6, 10, 2, 14, 0, 8, 4, 12};
    
    auto result5 = permuteBits(data5, randomPerm, BitOrder::LSB_FIRST, IndexBase::ZERO_BASED);
    cout << "После случайной перестановки:" << endl;
    cout << "Биты: ";
    printBits(result5);
    cout << "Hex: ";
    printBytes(result5);

    cout << "\n--- ПРИМЕР 6: ONE_BASED индексация (нумерация с 1) ---" << endl;
    
    vector<uint8_t> data6 = {0x01};  // 00000001
    cout << "Исходные данные: ";
    printBits(data6);
    
    vector<int> oneBasedPerm = {8, 7, 6, 5, 4, 3, 2, 1};
    
    auto result6 = permuteBits(data6, oneBasedPerm, BitOrder::LSB_FIRST, IndexBase::ONE_BASED);
    cout << "После перестановки (ONE_BASED): ";
    printBits(result6);
    
    cout << "\n--- ПРИМЕР 7: MSB_FIRST порядок битов ---" << endl;
    
    vector<uint8_t> data7 = {0xAA};  // 10101010
    cout << "Исходные данные (LSB_FIRST): ";
    printBits(data7);
    
    vector<int> reversePerm7(8);
    for (int i = 0; i < 8; i++) {
        reversePerm7[i] = 7 - i;
    }
    
    auto result7 = permuteBits(data7, reversePerm7, BitOrder::MSB_FIRST, IndexBase::ZERO_BASED);
    cout << "Результат (MSB_FIRST, обратный порядок): ";
    printBits(result7);
    
    return 0;
}