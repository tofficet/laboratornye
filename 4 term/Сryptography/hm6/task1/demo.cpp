#include <iostream>
#include <vector>
#include <iomanip>
#include "solovay-strassen_test.h"
#include "miller-rabin_test.h"

using namespace std;

int main() {
    cout << "=== ТЕСТЫ ПРОСТОТЫ ===" << endl;
    cout << "Соловей-Штрассен и Миллер-Рабин\n" << endl;
    
    SolovayStrassenTest ssTest;
    MillerRabinTest mrTest;
    
    cout << "1. БАЗОВЫЕ ТЕСТЫ" << endl;
    
    vector<uint64_t> numbers = {
        2, 3, 4, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25,
        97, 101, 103, 107, 109, 113,
        1000000007
    };
    
    for (uint64_t n : numbers) {
        bool expected;
        if (n == 2 || n == 3 || n == 5 || n == 7 || n == 11 || n == 13 ||
            n == 17 || n == 19 || n == 23 || n == 97 || n == 101 ||
            n == 103 || n == 107 || n == 109 || n == 113 || n == 1000000007) {
            expected = true;
        } else {
            expected = false;
        }
        
        bool ssResult = ssTest.isPrime(n, 0.99);
        bool mrResult = mrTest.isPrime(n, 0.99);
        
        cout << "n = " << setw(12) << n << " | "
             << "Сол-Штр: " << (ssResult ? "ПРОСТОЕ" : "СОСТАВНОЕ") << " | "
             << "Мил-Раб: " << (mrResult ? "ПРОСТОЕ" : "СОСТАВНОЕ") << endl;
    }
    
    cout << "\n2. ЧИСЛА КАРМАЙКЛА (составные, но обманывают тест Ферма)" << endl;
    
    vector<uint64_t> carmichael = {561, 1105, 1729, 2465, 2821, 6601};
    
    for (uint64_t n : carmichael) {
        bool ssResult = ssTest.isPrime(n, 0.99);
        bool mrResult = mrTest.isPrime(n, 0.99);
        
        cout << "n = " << setw(12) << n << " | "
             << "Сол-Штр: " << (ssResult ? "ПРОСТОЕ" : "СОСТАВНОЕ") << " | "
             << "Мил-Раб: " << (mrResult ? "ПРОСТОЕ" : "СОСТАВНОЕ") << endl;
    }
    
    cout << "\n3. РАЗНАЯ ВЕРОЯТНОСТЬ (n = 1009)" << endl;
    
    vector<double> probs = {0.5, 0.75, 0.9, 0.95, 0.99, 0.999};
    
    cout << "Вероятность | Соловей-Штрассен | Миллер-Рабин" << endl;
    cout << "------------|------------------|--------------" << endl;
    
    for (double p : probs) {
        bool ssResult = ssTest.isPrime(1009, p);
        bool mrResult = mrTest.isPrime(1009, p);
        
        cout << "   " << setw(5) << p << "    | "
             << "   " << (ssResult ? "ПРОСТОЕ" : "СОСТАВНОЕ") << "       | "
             << "   " << (mrResult ? "ПРОСТОЕ" : "СОСТАВНОЕ") << endl;
    }
    
    cout << "\n4. БОЛЬШИЕ ЧИСЛА" << endl;
    
    vector<uint64_t> bigNumbers = {
        1000000007,
        1000000009,
        1000000033,
        1000000087,
        1000000123,
        1000000181
    };
    
    for (uint64_t n : bigNumbers) {
        bool ssResult = ssTest.isPrime(n, 0.999);
        bool mrResult = mrTest.isPrime(n, 0.999);
        
        cout << "n = " << n << " | "
             << "Сол-Штр: " << (ssResult ? "ПРОСТОЕ" : "СОСТАВНОЕ") << " | "
             << "Мил-Раб: " << (mrResult ? "ПРОСТОЕ" : "СОСТАВНОЕ") << endl;
    }

    return 0;
}