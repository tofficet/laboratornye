#include "GF2n.hpp"

int main() {
    cout << "Умножение многочленов\n" << endl;
    const int N = 8;
    const uint64_t MODULUS = 0x11B;
    GF2n a(0b1011, 8, 0x11B);  
    GF2n b(0b110, 8, 0x11B); 
    
    uint64_t r = GF2n::multiplyRaw(a.getValue(), b.getValue(), 32);
    
    GF2n result_obj(r, N, MODULUS);
    cout << "(" << a.toString() << ") * (" 
        << b.toString() << ") = " 
        << result_obj.toString() << endl;
    return 0;
}