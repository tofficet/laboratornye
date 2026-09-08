#pragma once
#include <gmpxx.h>
#include <vector>
#include <cstdint>

struct RabinPublicKey {
    mpz_class n;
};

struct RabinPrivateKey {
    mpz_class p;
    mpz_class q;
};

class Rabin {
public:
    static bool isPrime(const mpz_class& n, int rounds = 40);
    static std::pair<RabinPublicKey, RabinPrivateKey> generateKeyPair(int bits, int millerRounds = 40);
    static std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data, const RabinPublicKey& pubKey);
    static std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data, const RabinPrivateKey& privKey);

private:
    static bool isPrime(const mpz_class& n, int rounds, gmp_randclass& rng);
    static mpz_class generatePrime(int bits, gmp_randclass& rng, int rounds);
    static size_t byteLength(const mpz_class& n);
    static std::vector<uint8_t> toBytes(const mpz_class& num, size_t length);
    static mpz_class fromBytes(const std::vector<uint8_t>& bytes);
    static mpz_class crt(const mpz_class& a, const mpz_class& p, const mpz_class& b, const mpz_class& q);
};