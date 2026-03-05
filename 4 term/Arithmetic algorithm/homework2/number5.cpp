#include "GF2n.hpp"


const int N = 8;
const uint64_t MODULUS = 0x11B;

int main() {

    cout << "РАСШИРЕННЫЙ АЛГОРИТМ ЕВКЛИДА" << endl;
    
    //x^3 + x + 1
    uint64_t val = 0xB;
    
    GF2n element(val, N, MODULUS);
    cout << "Элемент: " << element.toString() <<  endl;
    
    GF2n inverse = element.multiplicativeInverse();
    cout << "Обратный элемент: " << inverse.toString() <<  endl;

    GF2n product = element.multiply(inverse);
    cout << "Результат умножения: " << product.toString() << endl;
    
    return 0;
}