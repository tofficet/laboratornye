#pragma once

#include <cstdint>
#include <vector>
#include <random>
#include <cmath>
#include <stdexcept>
#include "../hm6/task1/miller-rabin_test.h"

using namespace std;

struct VulnerableRSAKeyPair {
    uint64_t n;
    uint64_t e;
    uint64_t d;
    uint64_t p;
    uint64_t q;
};

class VulnerableRSAKeyGenerator {
private:
    MillerRabinTest m_test;

    // генерация простого числа
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
    
public:
    VulnerableRSAKeyPair generateVulnerableToFermat(int bits, double probability) {
        VulnerableRSAKeyPair keys;
        
        // генерируем p
        keys.p = generatePrime(bits, probability);
        
        // берем старшие bits/2 бит от p
        int halfBits = bits / 2;
        uint64_t mask = (1ULL << halfBits) - 1;
        uint64_t prefix = (keys.p >> halfBits) << halfBits;
        
        // генерируем q с таким же префиксом
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<uint64_t> dist(0, mask);
        
        uint64_t suffix;
        do {
            suffix = dist(gen);
            keys.q = prefix | suffix;
        } while (!m_test.isPrime(keys.q, probability) || keys.q == keys.p);
        
        keys.n = keys.p * keys.q;
        uint64_t phi = (keys.p - 1) * (keys.q - 1);
        
        keys.e = 65537;
        while (keys.e < phi && gcd(keys.e, phi) != 1) {
            keys.e += 2;
        }
        if (keys.e >= phi) {
            throw runtime_error("Cannot find e");
        }
        
        keys.d = invmod(keys.e, phi);
        
        return keys;
    }
    
    VulnerableRSAKeyPair generateVulnerableToWiener(int bits, double probability) {
        VulnerableRSAKeyPair keys;
        
        while (true) {
            keys.p = generatePrime(bits, probability);
            do {
                keys.q = generatePrime(bits, probability);
            } while (keys.q == keys.p);
            
            keys.n = keys.p * keys.q;
            uint64_t phi = (keys.p - 1) * (keys.q - 1);
            
            for (uint64_t d = 3; d < 1000000; d += 2) {
                uint64_t e = invmod(d, phi);
                if (e != (uint64_t)-1 && gcd(e, phi) == 1) {
                    keys.d = d;
                    keys.e = e;
                    return keys;
                }
            }
        }
    }
};