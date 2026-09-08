#pragma once

#include "../../hm5/1.h"
#include "primality_test_bigint.h"
#include <cstdint>
#include <random>
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;

class MillerRabinTest : public BasePrimalityTest {
protected:
    bool testIteration(const cpp_int& n, const cpp_int& base) override {
        if (gcd(base, n) > 1)
            return false;

        // n-1 = s * 2^d
        cpp_int s = n - 1;
        cpp_int d = 0;
        while (s % 2 == 0) {
            s /= 2;
            d++;
        }
        
        // a^s (mod n)
        cpp_int x = powmod(base, s, n);
        
        // a^s ≡ 1 (mod n) или a^s ≡ -1 (mod n)
        if (x == 1 || x == n - 1)
            return true;
        
        // a^(s * 2^i) ≡ -1 (mod n)
        for (cpp_int i = 1; i < d; ++i) {
            x = (x * x) % n;
            if (x == n - 1)
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