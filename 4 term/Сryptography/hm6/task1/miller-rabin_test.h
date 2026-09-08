#pragma once

#include "../../hm5/1.h"
#include "primality_test.h"
#include <cstdint>
#include <random>

class MillerRabinTest : public BasePrimalityTest {
protected:
    bool testIteration(uint64_t number, uint64_t base) override {
        if (gcd(base, number) > 1)
            return false;

        // n-1 = s * 2^d
        uint64_t s = number - 1;
        uint64_t d = 0;
        while (s % 2 == 0) {
            s /= 2;
            d++;
        }
        
        // a^s (mod n)
        uint64_t x = powmod(base, s, number);
        
        // a^s ≡ 1 (mod n)
        if (x == 1 || x == number - 1)
            return true;
        
        // a^(s * 2^i) ≡ -1 (mod n)
        for (uint64_t i = 1; i < d; i++) {
            x = x * x % number;
            if (x == number - 1)
                return true;
            if (x == 1) 
                return false; 
        }
        
        return false;
    }
    
    double getErrorPerIteration() const override {
        return 0.25;
    }
};