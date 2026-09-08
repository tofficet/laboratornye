#pragma once

#include "../hm5/1.h"
#include "primality_test.h"
#include <cstdint>
#include <random>

class SolovayStrassenTest : public BasePrimalityTest {
protected:
    bool testIteration(uint64_t number, uint64_t base) override {
        // символ Якоби (a/n)
        int jacobi = Jacobi(base, number);

        if (jacobi == 0)
            return false;

        // a^((n-1)/2) mod n
        uint64_t left = powmod(base, (number - 1) / 2, number);
        
        if (left == 1)
            return jacobi == 1;
        if (left == number - 1)
            return jacobi == -1;
        return false;
    }
    
    double getErrorPerIteration() const override {
        return 0.5;
    }
};