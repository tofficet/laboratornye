#pragma once

#include <cstdint>
#include <vector>
#include <tuple>
#include <cmath>
#include <stdexcept>
#include "../vulnerable_rsa_keygen.h"
#include "../task3/continued_fraction.h"

using namespace std;

class WienerAttack {
public:
    tuple<uint64_t, uint64_t> attack(uint64_t n, uint64_t e) {
        // цепная дробь из e/n
        vector<uint64_t> cf = ContinuedFraction::fromFraction(e, n);
        
        // все подходящие дроби
        auto suitableFractions = ContinuedFraction::suitabFractions(cf);
        
        // перебираем подходящие дроби
        for (auto [k, d] : suitableFractions) {
            if (k == 0) 
                continue;
            
            // (e*d - 1) % k == 0
            if ((e * d - 1) % k != 0) 
                continue;
            
            uint64_t phi = (e * d - 1) / k;
            
            // x^2 - (n - phi + 1)x + n = 0
            auto [p, q] = solveQuadratic(n, phi);
            
            if (p != 0 && q != 0) {
                return {p, q};
            }
        }
        
        throw runtime_error("Wiener attack failed: n is not vulnerable");
    }
    
private:
    static uint64_t isqrt_u128(__uint128_t x) {
        if (x == 0) return 0;
        __uint128_t r = 1ULL << 63; // начальное приближение (2^63)
        while (true) {
            __uint128_t nr = (r + x / r) >> 1;
            if (nr >= r) 
                break;
            r = nr;
        }
        while ((__uint128_t)(r + 1) * (r + 1) <= x) 
            ++r;
        while ((__uint128_t)r * r > x) 
            --r;
        return (uint64_t)r; // r <= 2^64, т.к. x < 2^128
    }

    tuple<uint64_t, uint64_t> solveQuadratic(uint64_t n, uint64_t phi) {
        __int128_t S_int = (__int128_t)n - (__int128_t)phi + 1;
        if (S_int < 0) 
            return {0, 0};   // для правильного phi такого не бывает

        __uint128_t S = (__uint128_t)S_int;
        __uint128_t fourN = 4 * (__uint128_t)n;

        if (S * S < fourN) 
            return {0, 0};  // дискриминант отрицателен

        __uint128_t D = S * S - fourN;
        uint64_t sqrtD = isqrt_u128(D);

        if ((__uint128_t)sqrtD * sqrtD != D) 
            return {0, 0};

        __uint128_t p_u = (S - sqrtD) / 2;
        __uint128_t q_u = (S + sqrtD) / 2;

        if (p_u == 0 || q_u == 0) 
            return {0, 0};
        if (p_u * q_u != (__uint128_t)n) 
            return {0, 0};

        return {(uint64_t)p_u, (uint64_t)q_u};
    }
};