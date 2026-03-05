#include "GF2n.hpp"

GF2n::GF2n(uint64_t val, int degree, uint64_t mod) 
    : value(val), n(degree), modulus(mod) {
    if (n < 64) {
        value &= ((1ULL << n) - 1);
    }
}

uint64_t GF2n::multiplyRaw(uint64_t a, uint64_t b, int maxDegree) {
    uint64_t result = 0;
    
    for (int i = 0; i <= maxDegree; i++) {
        if ((b >> i) & 1) {
            result ^= (a << i);
        }
    }
    
    return result;
}


int GF2n::degree(uint64_t poly) const {
    if (poly == 0) return -1;
    int deg = 0;
    uint64_t temp = poly;
    while (temp > 0) {
        deg++;
        temp >>= 1;
    }
    return deg - 1;
}

uint64_t GF2n::reduce(uint64_t poly) const {
    while (degree(poly) >= n) {
        int deg_poly = degree(poly);
        int deg_mod = degree(modulus);
        int shift = deg_poly - deg_mod;
        poly ^= (modulus << shift);
    }
    return poly;
}

GF2n GF2n::add(const GF2n& other) const {
    return GF2n(value ^ other.value, n, modulus);
}

GF2n GF2n::multiply(const GF2n& other) const {
    uint64_t result = 0;
    uint64_t a = value;
    uint64_t b = other.value;
    
    for (int i = 0; i < n; i++) {
        if ((b >> i) & 1) {
            result ^= (a << i);
        }
    }
    
    result = reduce(result);
    return GF2n(result, n, modulus);
}

GF2n GF2n::multiplicativeInverse() const {
    if (value == 0) {
        return GF2n(0, n, modulus);
    }

    uint64_t r0 = modulus;
    uint64_t r1 = value;
    uint64_t s0 = 1;
    uint64_t s1 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 1;
    
    while (r1 != 0) {
        int deg_r0 = degree(r0);
        int deg_r1 = degree(r1);
        
        if (deg_r0 < deg_r1) {
            swap(r0, r1);
            swap(s0, s1);
            swap(t0, t1);
            continue;
        }
        
        int shift = deg_r0 - deg_r1;
        uint64_t q = 1ULL << shift;
        
        r0 ^= (r1 << shift);
        s0 ^= (s1 << shift);
        t0 ^= (t1 << shift);
        
        if (degree(r0) >= n) {
            r0 = reduce(r0);
        }
    }
    
    t0 &= ((1ULL << n) - 1);
    return GF2n(t0, n, modulus);
}

string GF2n::toString() const {
    if (value == 0) return "0";

    string result = "";
    bool first = true;

    for (int i = n - 1; i >= 0; i--) {
        if ((value >> i) & 1) {
            if (!first) result += " + ";
            
            if (i == 0) result += "1";
            else if (i == 1) result += "x";
            else result += "x^" + to_string(i);
            
            first = false;
        }
    }
    return result;
}

GF2n GF2n::fromString(const string& polyStr, int n, uint64_t mod) {
    uint64_t result = 0;
    
    string clean = "";
    for (char c : polyStr) {
        if (c != ' ') clean += c;
    }

    if (clean == "0") return GF2n(0, n, mod);

    vector<string> terms;
    string term = "";
    for (char c : clean) {
        if (c == '+') {
            if (!term.empty()) {
                terms.push_back(term);
                term = "";
            }
        }
        else {
            term += c;
        }
    }
    if (!term.empty()) terms.push_back(term);

    for (const string& t : terms) {
        if (t == "1") result |= 1;
        else if (t == "x") result |= (1ULL << 1);
        else if (t.substr(0, 2) == "x^") {
            int power = stoi(t.substr(2));
            if (power < n) result |= (1ULL << power);
        }
    }
    
    return GF2n(result, n, mod);
}

bool GF2n::operator==(const GF2n& other) const {
    return (value == other.value && n == other.n && modulus == other.modulus);
}

bool GF2n::operator!=(const GF2n& other) const {
    return !(*this == other);
}