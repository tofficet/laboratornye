#include "ElGamal.h"
#include <stdexcept>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <iostream>

namespace {
    const uint32_t SHA256_INIT[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    const uint32_t SHA256_K[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
        0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
        0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
        0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    inline uint32_t rotr(uint32_t x, unsigned n) { return (x >> n) | (x << (32 - n)); }
    inline uint32_t Ch(uint32_t x, uint32_t y, uint32_t z)  { return (x & y) ^ (~x & z); }
    inline uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) { return (x & y) ^ (x & z) ^ (y & z); }
    inline uint32_t Sigma0(uint32_t x) { return rotr(x,2) ^ rotr(x,13) ^ rotr(x,22); }
    inline uint32_t Sigma1(uint32_t x) { return rotr(x,6) ^ rotr(x,11) ^ rotr(x,25); }
    inline uint32_t sigma0(uint32_t x) { return rotr(x,7) ^ rotr(x,18) ^ (x>>3); }
    inline uint32_t sigma1(uint32_t x) { return rotr(x,17) ^ rotr(x,19) ^ (x>>10); }

    void sha256_transform(uint32_t state[8], const uint8_t block[64]) {
        uint32_t w[64];
        for (int i = 0; i < 16; ++i)
            w[i] = (block[i*4]<<24) | (block[i*4+1]<<16) | (block[i*4+2]<<8) | block[i*4+3];
        for (int i = 16; i < 64; ++i)
            w[i] = sigma1(w[i-2]) + w[i-7] + sigma0(w[i-15]) + w[i-16];
        uint32_t a=state[0], b=state[1], c=state[2], d=state[3],
                 e=state[4], f=state[5], g=state[6], h=state[7];
        for (int i = 0; i < 64; ++i) {
            uint32_t t1 = h + Sigma1(e) + Ch(e,f,g) + SHA256_K[i] + w[i];
            uint32_t t2 = Sigma0(a) + Maj(a,b,c);
            h = g; g = f; f = e; e = d + t1;
            d = c; c = b; b = a; a = t1 + t2;
        }
        state[0] += a; state[1] += b; state[2] += c; state[3] += d;
        state[4] += e; state[5] += f; state[6] += g; state[7] += h;
    }
}

std::vector<uint8_t> ElGamal::sha256(const std::vector<uint8_t>& data) {
    uint32_t state[8];
    memcpy(state, SHA256_INIT, sizeof(SHA256_INIT));
    uint64_t bitLen = data.size() * 8ULL;
    size_t newLen = data.size() + 1 + 8;
    size_t paddedLen = ((newLen + 63) / 64) * 64;
    std::vector<uint8_t> padded(paddedLen, 0);
    memcpy(padded.data(), data.data(), data.size());
    padded[data.size()] = 0x80;
    for (int i = 0; i < 8; ++i)
        padded[paddedLen - 1 - i] = (bitLen >> (i * 8)) & 0xFF;
    for (size_t i = 0; i < paddedLen; i += 64)
        sha256_transform(state, &padded[i]);
    std::vector<uint8_t> hash(32);
    for (int i = 0; i < 8; ++i) {
        hash[i*4]     = (state[i] >> 24) & 0xFF;
        hash[i*4 + 1] = (state[i] >> 16) & 0xFF;
        hash[i*4 + 2] = (state[i] >> 8)  & 0xFF;
        hash[i*4 + 3] =  state[i]        & 0xFF;
    }
    return hash;
}

mpz_class ElGamal::hashToNumber(const std::vector<uint8_t>& message, const mpz_class& max) {
    if (max <= 1) {
        throw std::invalid_argument("max must be greater than 1");
    }
    auto h = sha256(message);
    mpz_class num;
    mpz_import(num.get_mpz_t(), h.size(), 1, 1, 1, 0, h.data());
    return num % max;
}

static const std::vector<int> SMALL_PRIMES = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};

bool ElGamal::isPrime(const mpz_class& n, int rounds, gmp_randclass& rng) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int p : SMALL_PRIMES) {
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    mpz_class d = n - 1;
    int s = 0;
    while (d % 2 == 0) { d /= 2; ++s; }
    mpz_class a, x;
    for (int i = 0; i < rounds; ++i) {
        a = rng.get_z_range(n - 3) + 2;
        mpz_powm(x.get_mpz_t(), a.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

gmp_randclass& ElGamal::getRng() {
    static gmp_randclass rng(gmp_randinit_default);
    static bool seeded = false;
    if (!seeded) {
        rng.seed(time(nullptr));
        seeded = true;
    }
    return rng;
}

mpz_class ElGamal::generateSafePrime(int bits, gmp_randclass& rng, int rounds) {
    std::cerr << "Генерация безопасного простого " << bits << " бит...";
    int attempt = 0;
    while (true) {
        mpz_class q = rng.get_z_bits(bits - 1);
        mpz_setbit(q.get_mpz_t(), bits - 2);
        mpz_setbit(q.get_mpz_t(), 0);
        while (!isPrime(q, rounds, rng)) {
            q += 2;
            ++attempt;
            if (attempt % 200 == 0) std::cerr << ".";
        }
        mpz_class p = 2 * q + 1;
        if (mpz_sizeinbase(p.get_mpz_t(), 2) == static_cast<unsigned long>(bits) && isPrime(p, rounds, rng)) {
            std::cerr << " готово\n";
            return p;
        }
        std::cerr << "!";
    }
}

mpz_class ElGamal::findGenerator(const mpz_class& p, const mpz_class& q, gmp_randclass& rng) {
    mpz_class g = 2;
    while (true) {
        mpz_class tmp;
        mpz_powm(tmp.get_mpz_t(), g.get_mpz_t(), q.get_mpz_t(), p.get_mpz_t());
        if (tmp != 1) {
            return g;
        }
        ++g;
    }
}

std::pair<ElGamalPublicKey, ElGamalPrivateKey> ElGamal::generateKeyPair(int bits, int rounds) {
    if (bits < 64) throw std::invalid_argument("bits must be >= 64");
    gmp_randclass& rng = getRng();
    mpz_class p = generateSafePrime(bits, rng, rounds);
    mpz_class q = (p - 1) / 2;
    mpz_class g = findGenerator(p, q, rng);
    
    mpz_class x = rng.get_z_range(p - 1) + 1;
    mpz_class y;
    mpz_powm(y.get_mpz_t(), g.get_mpz_t(), x.get_mpz_t(), p.get_mpz_t());

    return { ElGamalPublicKey{p, g, y}, ElGamalPrivateKey{p, g, x} };
}

ElGamalSignature ElGamal::sign(const std::vector<uint8_t>& message, const ElGamalPrivateKey& priv) {
    const mpz_class& p = priv.p;
    mpz_class pMinus1 = p - 1;
    mpz_class h = hashToNumber(message, pMinus1);
    gmp_randclass& rng = getRng();

    int attempts = 0;
    while (true) {
        mpz_class k = rng.get_z_range(pMinus1 - 2) + 2;
        mpz_class gcdVal;
        mpz_gcd(gcdVal.get_mpz_t(), k.get_mpz_t(), pMinus1.get_mpz_t());
        if (gcdVal != 1) {
            continue;
        }
        
        mpz_class r, s;
        mpz_powm(r.get_mpz_t(), priv.g.get_mpz_t(), k.get_mpz_t(), p.get_mpz_t());
        
        mpz_class kInv;
        mpz_invert(kInv.get_mpz_t(), k.get_mpz_t(), pMinus1.get_mpz_t());

        mpz_class temp = (h - priv.x * r) % pMinus1;
        if (temp < 0) {
            temp += pMinus1;
        }
        s = (temp * kInv) % pMinus1;
        
        if (s != 0) {
            return {r, s};
        }
        
        ++attempts;
        if (attempts > 100) {
            throw std::runtime_error("Too many attempts in sign (s=0)");
        }
    }
}

bool ElGamal::verify(const std::vector<uint8_t>& message, const ElGamalSignature& sig, const ElGamalPublicKey& pub) {
    const mpz_class& p = pub.p;
    const mpz_class& g = pub.g;
    const mpz_class& y = pub.y;
    const mpz_class& r = sig.r;
    const mpz_class& s = sig.s;

    if (r <= 0 || r >= p || s <= 0 || s >= p - 1) {
        return false;
    }

    mpz_class h = hashToNumber(message, p - 1);
    
    mpz_class v1, v2;
    mpz_powm(v1.get_mpz_t(), y.get_mpz_t(), r.get_mpz_t(), p.get_mpz_t());
    mpz_powm(v2.get_mpz_t(), r.get_mpz_t(), s.get_mpz_t(), p.get_mpz_t());
    v1 = (v1 * v2) % p;

    mpz_class v2_calc;
    mpz_powm(v2_calc.get_mpz_t(), g.get_mpz_t(), h.get_mpz_t(), p.get_mpz_t());

    return v1 == v2_calc;
}

std::vector<uint8_t> ElGamal::signatureToBytes(const ElGamalSignature& sig, size_t pByteLen) {
    std::vector<uint8_t> buf(pByteLen * 2, 0);
    
    size_t rSize = (mpz_sizeinbase(sig.r.get_mpz_t(), 2) + 7) / 8;
    if (rSize > pByteLen) {
        throw std::runtime_error("r too large for buffer");
    }
    mpz_export(buf.data() + (pByteLen - rSize), nullptr, 1, 1, 1, 0, sig.r.get_mpz_t());

    size_t sSize = (mpz_sizeinbase(sig.s.get_mpz_t(), 2) + 7) / 8;
    if (sSize > pByteLen) {
        throw std::runtime_error("s too large for buffer");
    }
    mpz_export(buf.data() + pByteLen + (pByteLen - sSize), nullptr, 1, 1, 1, 0, sig.s.get_mpz_t());
    
    return buf;
}

ElGamalSignature ElGamal::signatureFromBytes(const std::vector<uint8_t>& data, size_t pByteLen) {
    if (data.size() != pByteLen * 2) {
        throw std::invalid_argument("Invalid signature data length");
    }
    
    mpz_class r, s;
    
    mpz_import(r.get_mpz_t(), pByteLen, 1, 1, 1, 0, data.data());

    mpz_import(s.get_mpz_t(), pByteLen, 1, 1, 1, 0, data.data() + pByteLen);
    
    return {r, s};
}