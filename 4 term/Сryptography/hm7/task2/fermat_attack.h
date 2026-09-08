#pragma once

#include <cstdint>
#include <cmath>
#include <tuple>
#include <stdexcept>

using namespace std;

class FermatAttack {
public:
    tuple<uint64_t, uint64_t> attack(uint64_t n) {
        if (n % 2 == 0) {
            return {2, n / 2};
        }
        
        uint64_t a = ceil_sqrt(n);
        uint64_t b2 = a * a - n;
        
        while (!isPerfectSquare(b2)) {
            a++;
            b2 = a * a - n;
            
            if (a > n / 2) {
                throw runtime_error("Fermat attack failed: n is not vulnerable");
            }
        }
        
        uint64_t b = (uint64_t)sqrt(b2);
        uint64_t p = a - b;
        uint64_t q = a + b;
        
        if (p < 2 || q < 2 || p * q != n) {
            throw runtime_error("Fermat attack failed: incorrect factorization");
        }
        
        return {p, q};
    }
    
private:
    uint64_t ceil_sqrt(uint64_t n) {
        uint64_t root = (uint64_t)sqrt(n);
        if (root * root < n) 
            root++;
        return root;
    }
    
    bool isPerfectSquare(uint64_t n) {
        uint64_t root = (uint64_t)sqrt(n);
        return root * root == n;
    }
};