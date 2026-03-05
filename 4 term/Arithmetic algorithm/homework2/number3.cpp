#include <iostream>
#include "GF2n.hpp"


const int N = 8;
const uint64_t MODULUS = 0x11B;

void showMultiply(uint64_t aVal, uint64_t bVal, const string& comment = "") {
    GF2n a(aVal, N, MODULUS);
    GF2n b(bVal, N, MODULUS);
    GF2n result = a.multiply(b);
    
    if (!comment.empty()) cout << comment <<" = " << result.toString() << endl;
}

int main() {
    cout << "УМНОЖЕНИЕ В GF(2^8)" << endl;
    cout << "Неприводимый многочлен: x^8 + x^4 + x^3 + x + 1\n" << endl;

    showMultiply(0b101, 0b11, "(x^2 + 1) * (x + 1)");
    showMultiply(0b1011, 0b110, "(x^3 + x + 1) * (x^2 + x)");
    showMultiply(0b10011, 0b1001, "(x^4 + x + 1) * (x^3 + 1)");
    showMultiply(0xFF, 0xFF, "(x^7 + x^6 + x^5 + x^4 + x^3 + x^2 + x + 1) * (x^7 + x^6 + x^5 + x^4 + x^3 + x^2 + x + 1)");
    cout << endl;

    cout << "Умножение на 0 и 1" << endl;
    showMultiply(0x57, 0, "(x^6 + x^4 + x^2 + x + 1) * 0");
    showMultiply(0x57, 1, "(x^6 + x^4 + x^2 + x + 1) * 1");
    cout << endl;

    return 0;
}