#include "S-block.h"
#include <iostream>

using namespace std;

int main() {
    cout << "=== ДЕМОНСТРАЦИЯ S-БЛОКОВ ===" << endl;
    
    vector<uint8_t> data;
    string text = "Hello, World!";
    for (char c : text) {
        data.push_back(static_cast<uint8_t>(c));
    }
    
    cout << "\nИсходные данные:" << endl;
    printBytes(data, "  hex");
    printAsText(data, "  text");

    cout << "\n--- ДЕМОНСТРАЦИЯ 1 (через map) ---" << endl;
    
    // отображение 'a' -> 'z', 'b' -> 'y', ...
    unordered_map<uint8_t, uint8_t> reverseAlphabet;
    for (int i = 0; i < 26; i++) {
        reverseAlphabet['a' + i] = 'z' - i;
        reverseAlphabet['A' + i] = 'Z' - i;
    }
    
    auto mappedResult = substituteBits(data, reverseAlphabet);
    printBytes(mappedResult, "  результат hex");
    printAsText(mappedResult, "  результат text");
    
    cout << "\n--- ДЕМОНСТРАЦИЯ 2 (через function) ---" << endl;
    
    // S-блок: XOR с 0x55
    auto xorSbox = [](uint8_t x) -> uint8_t {
        return x ^ 0x55;
    };
    
    auto xorResult = substituteBits(data, xorSbox);
    printBytes(xorResult, "  XOR с 0x55");
    
    
    cout << "\n--- ДЕМОНСТРАЦИЯ 3 (функция-обёртка) ---" << endl;
    
    // умножение на 2 по модулю 256
    auto multiplyBy2 = [](uint8_t x) -> uint8_t {
        return (x * 2) % 256;
    };
    
    auto mulResult = substituteBits(data, multiplyBy2);
    printBytes(mulResult, "  умножить на 2");
    
    cout << "\n--- ДЕМОНСТРАЦИЯ 4 (бинарные данные) ---" << endl;
    
    vector<uint8_t> binaryData = {0x00, 0x01, 0x02, 0x03, 0xFF, 0xFE, 0xFD, 0xFC};
    cout << "Исходные бинарные данные: ";
    printBytes(binaryData);
    
    // S-блок: обратный порядок битов в байте
    auto reverseBits = [](uint8_t x) -> uint8_t {
        uint8_t result = 0;
        for (int i = 0; i < 8; i++) {
            if (x & (1 << i)) {
                result |= (1 << (7 - i));
            }
        }
        return result;
    };
    
    auto reversed = substituteBits(binaryData, reverseBits);
    cout << "После переворота битов: ";
    printBytes(reversed);
    
    return 0;
}