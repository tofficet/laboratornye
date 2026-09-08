#pragma once
#include <gmpxx.h>
#include <vector>
#include <cstdint>
#include <string>

struct ElGamalPublicKey {
    mpz_class p;
    mpz_class g;
    mpz_class y;
};

struct ElGamalPrivateKey {
    mpz_class p;
    mpz_class g;
    mpz_class x;
};

struct ElGamalSignature {
    mpz_class r;
    mpz_class s;
};

class ElGamal {
public:
    static std::pair<ElGamalPublicKey, ElGamalPrivateKey> generateKeyPair(int bits, int rounds = 40);
    static ElGamalSignature sign(const std::vector<uint8_t>& message, const ElGamalPrivateKey& privKey);
    static bool verify(const std::vector<uint8_t>& message, const ElGamalSignature& sig, const ElGamalPublicKey& pubKey);
    static std::vector<uint8_t> signatureToBytes(const ElGamalSignature& sig, size_t pByteLen);
    static ElGamalSignature signatureFromBytes(const std::vector<uint8_t>& data, size_t pByteLen);

private:
    static std::vector<uint8_t> sha256(const std::vector<uint8_t>& data);
    static mpz_class hashToNumber(const std::vector<uint8_t>& message, const mpz_class& max);
    static mpz_class generateSafePrime(int bits, gmp_randclass& rng, int rounds);
    static bool isPrime(const mpz_class& n, int rounds, gmp_randclass& rng);
    static mpz_class findGenerator(const mpz_class& p, const mpz_class& q, gmp_randclass& rng);
    static gmp_randclass& getRng();
};