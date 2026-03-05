#include <iostream>
#include "GF2n.hpp"

const int N = 8;
const uint64_t MODULUS = 0x11B;

int main() {
    cout << "Поле GF(2^" << N << ")" << endl;
    cout << "Неприводимый многочлен: x^8 + x^4 + x^3 + x + 1" << endl;

    uint64_t num = 180;
    GF2n elem1(num, N, MODULUS);
    cout << "Число " << num << " -> Многочлен: " << elem1.toString() << endl;

    string poly = "x^7 + x^5 + x^4 + x^2";
    GF2n elem2 = GF2n::fromString(poly, N, MODULUS);
    cout << "Многочлен \"" << poly << "\" -> Число: " << elem2.getValue() << endl;

    return 0;
}