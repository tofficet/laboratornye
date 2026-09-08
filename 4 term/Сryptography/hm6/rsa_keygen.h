#pragma once

#include <cstdint>
#include <vector>
#include <random>
#include <cmath>
#include <stdexcept>
#include "task1/miller-rabin_test.h"

using namespace std;

struct RSAKeyPair {
    uint64_t n;  // модуль (открытый)
    uint64_t e;  // открытая экспонента
    uint64_t d;  // закрытая экспонента
    uint64_t p;  // простое
    uint64_t q;  // простое
};

class RSAKeyGenerator {
private:
    MillerRabinTest m_test;
    
    // генерация простого числа заданной длины
    uint64_t generatePrime(int bits, double probability) {
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<uint64_t> dist(
            1ULL << (bits - 1),
            (1ULL << bits) - 1
        );
        
        uint64_t candidate;
        do {
            candidate = dist(gen);
        } while (!m_test.isPrime(candidate, probability));
        
        return candidate;
    }
    
    bool isWienerSafe(uint64_t d, uint64_t n) {
        // d > n^(1/4)
        uint64_t n_sqrt = static_cast<uint64_t>(sqrt(n));
        uint64_t n_fourth = static_cast<uint64_t>(sqrt(n_sqrt));
        return d > n_fourth;
    }
    
public:
    RSAKeyPair generateKeys(int bits, double probability = 0.99) {
        if (bits < 8) {
            throw invalid_argument("Bits must be at least 8");
        }
        
        RSAKeyPair keys;
        
        // генерируем p и q
        uint64_t p, q;
        while (true) {
            p = generatePrime(bits, probability);
            q = generatePrime(bits, probability);

            int secondBitP = (p >> (bits - 2)) & 1;
            int secondBitQ = (q >> (bits - 2)) & 1;

            if (secondBitP == secondBitQ)
                continue;

            break;
        };
        
        keys.p = p;
        keys.q = q;
        keys.n = p * q;
        
        uint64_t phi = (p - 1) * (q - 1);
        
        // e: (e, phi(N)) = 1
        keys.e = 65537;

        while (keys.e < phi && gcd(keys.e, phi) != 1)
            keys.e += 2;

        if (keys.e >= phi) {
            throw runtime_error("Cannot find e");
        }
        
        // d = e^(-1) (mod phi(N))
        keys.d = invmod(keys.e, phi);
        
        // защита от атаки Винера
        if (!isWienerSafe(keys.d, keys.n)) {
            return generateKeys(bits, probability);
        }
        
        return keys;
    }
};