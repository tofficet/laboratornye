#ifndef GF2N_HPP
#define GF2N_HPP

#include <string>
#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class GF2n {
private:
    uint64_t value;
    int n;
    uint64_t modulus;
    
    int degree(uint64_t poly) const;
    uint64_t reduce(uint64_t poly) const;

public:
    GF2n(uint64_t val = 0, int degree = 8, uint64_t mod = 0x11B);
    
    GF2n add(const GF2n& other) const;
    GF2n multiply(const GF2n& other) const;
    GF2n multiplicativeInverse() const;
    static uint64_t multiplyRaw(uint64_t a, uint64_t b, int maxDegree = 32);
    
    std::string toString() const;
    static GF2n fromString(const std::string& polyStr, int n, uint64_t mod);
    
    uint64_t getValue() const { return value; }
    int getN() const { return n; }
    uint64_t getModulus() const { return modulus; }
    
    bool operator==(const GF2n& other) const;
    bool operator!=(const GF2n& other) const;
};

#endif