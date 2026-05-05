#include "rabin.h"
#include <stdexcept>
#include <cstring>
#include <ctime>

static const std::vector<int> SMALL_PRIMES = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97
};

bool Rabin::isPrime(const mpz_class& n, int rounds, gmp_randclass& rng) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;

    for (int p : SMALL_PRIMES) {
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    mpz_class d = n - 1;
    int s = 0;
    while (d % 2 == 0) {
        d /= 2;
        ++s;
    }

    mpz_class a, x;
    for (int i = 0; i < rounds; ++i) {
        a = rng.get_z_range(n - 3) + 2;
        mpz_powm(x.get_mpz_t(), a.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

bool Rabin::isPrime(const mpz_class& n, int rounds) {
    gmp_randclass rng(gmp_randinit_default);
    rng.seed(time(nullptr));
    return isPrime(n, rounds, rng);
}

mpz_class Rabin::generatePrime(int bits, gmp_randclass& rng, int rounds) {
    mpz_class candidate;
    while (true) {
        candidate = rng.get_z_bits(bits);
        mpz_setbit(candidate.get_mpz_t(), bits - 1);
        mpz_setbit(candidate.get_mpz_t(), 0);
        mpz_setbit(candidate.get_mpz_t(), 1);
        if (isPrime(candidate, rounds, rng))
            return candidate;
        candidate += 4;
        while (!isPrime(candidate, rounds, rng))
            candidate += 4;
        return candidate;
    }
}

std::pair<RabinPublicKey, RabinPrivateKey> Rabin::generateKeyPair(int bits, int millerRounds) {
    if (bits < 32) throw std::invalid_argument("bits must be >= 32");

    gmp_randclass rng(gmp_randinit_default);
    rng.seed(time(nullptr));

    int pBits = bits / 2;
    int qBits = bits - pBits;

    RabinPrivateKey priv;
    RabinPublicKey pub;

    do {
        priv.p = generatePrime(pBits, rng, millerRounds);
        priv.q = generatePrime(qBits, rng, millerRounds);
        if (priv.p == priv.q) continue;
        pub.n = priv.p * priv.q;
    } while (mpz_sizeinbase(pub.n.get_mpz_t(), 2) != bits);

    return {pub, priv};
}

size_t Rabin::byteLength(const mpz_class& n) {
    return (mpz_sizeinbase(n.get_mpz_t(), 2) + 7) / 8;
}

std::vector<uint8_t> Rabin::toBytes(const mpz_class& num, size_t length) {
    std::vector<uint8_t> buf(length, 0);
    mpz_export(buf.data(), nullptr, 1, 1, 1, 0, num.get_mpz_t());
    return buf;
}

mpz_class Rabin::fromBytes(const std::vector<uint8_t>& bytes) {
    mpz_class num;
    mpz_import(num.get_mpz_t(), bytes.size(), 1, 1, 1, 0, bytes.data());
    return num;
}

mpz_class Rabin::crt(const mpz_class& a, const mpz_class& p, const mpz_class& b, const mpz_class& q) {
    mpz_class q_inv_p, p_inv_q;
    mpz_invert(q_inv_p.get_mpz_t(), q.get_mpz_t(), p.get_mpz_t());
    mpz_invert(p_inv_q.get_mpz_t(), p.get_mpz_t(), q.get_mpz_t());
    return (a * q * q_inv_p + b * p * p_inv_q) % (p * q);
}

std::vector<uint8_t> Rabin::encrypt(const std::vector<uint8_t>& data, const RabinPublicKey& pubKey) {
    const size_t len_n = byteLength(pubKey.n);
    const size_t blockDataLen = len_n - 3;
    if (len_n < 4) throw std::runtime_error("Ключ слишком короткий для корректного дополнения");

    uint32_t msgLen = static_cast<uint32_t>(data.size());
    std::vector<uint8_t> lenBlockData(blockDataLen, 0);
    for (int i = 3; i >= 0; --i)
        lenBlockData[blockDataLen - 4 + (3 - i)] = (msgLen >> (i * 8)) & 0xFF;

    std::vector<uint8_t> fullBlock;
    fullBlock.push_back(0x01);
    fullBlock.insert(fullBlock.end(), lenBlockData.begin(), lenBlockData.end());
    fullBlock.push_back(0xFF);

    mpz_class m = fromBytes(fullBlock);
    mpz_class c = (m * m) % pubKey.n;
    std::vector<uint8_t> ciphertext = toBytes(c, len_n);

    for (size_t offset = 0; offset < data.size(); offset += blockDataLen) {
        std::vector<uint8_t> chunkData(blockDataLen, 0);
        size_t copyLen = std::min(blockDataLen, data.size() - offset);
        std::memcpy(chunkData.data(), data.data() + offset, copyLen);

        fullBlock.clear();
        fullBlock.push_back(0x01);
        fullBlock.insert(fullBlock.end(), chunkData.begin(), chunkData.end());
        fullBlock.push_back(0xFF);

        m = fromBytes(fullBlock);
        c = (m * m) % pubKey.n;
        auto encryptedBlock = toBytes(c, len_n);
        ciphertext.insert(ciphertext.end(), encryptedBlock.begin(), encryptedBlock.end());
    }
    return ciphertext;
}

std::vector<uint8_t> Rabin::decrypt(const std::vector<uint8_t>& data, const RabinPrivateKey& privKey) {
    const mpz_class& p = privKey.p;
    const mpz_class& q = privKey.q;
    const mpz_class n = p * q;
    const size_t len_n = byteLength(n);
    const size_t blockDataLen = len_n - 3;

    if (data.size() % len_n != 0)
        throw std::runtime_error("Размер шифротекста не кратен длине блока");

    size_t numBlocks = data.size() / len_n;
    if (numBlocks == 0) return {};

    std::vector<uint8_t> buffer;

    for (size_t blockIdx = 0; blockIdx < numBlocks; ++blockIdx) {
        std::vector<uint8_t> encBlock(data.begin() + blockIdx * len_n,
                                      data.begin() + (blockIdx + 1) * len_n);
        mpz_class c = fromBytes(encBlock);

        mpz_class mp, mq;
        mpz_class exp_p = (p + 1) / 4;
        mpz_class exp_q = (q + 1) / 4;
        mpz_powm(mp.get_mpz_t(), c.get_mpz_t(), exp_p.get_mpz_t(), p.get_mpz_t());
        mpz_powm(mq.get_mpz_t(), c.get_mpz_t(), exp_q.get_mpz_t(), q.get_mpz_t());
        std::vector<mpz_class> candidates = {
            crt(mp, p, mq, q),
            crt(mp, p, (q - mq) % q, q),
            crt((p - mp) % p, p, mq, q),
            crt((p - mp) % p, p, (q - mq) % q, q)
        };

        bool found = false;
        for (auto& cand : candidates) {
            auto blockBytes = toBytes(cand, len_n - 1);
            if (blockBytes.size() >= 2 && blockBytes.front() == 0x01 && blockBytes.back() == 0xFF) {
                std::vector<uint8_t> dataBlock(blockBytes.begin() + 1, blockBytes.end() - 1);
                buffer.insert(buffer.end(), dataBlock.begin(), dataBlock.end());
                found = true;
                break;
            }
        }
        if (!found)
            throw std::runtime_error("Расшифровка не удалась: корректное дополнение не найдено");
    }

    if (buffer.size() < blockDataLen)
        throw std::runtime_error("Расшифровка не удалась: получены слишком короткие данные");

    const uint8_t* lenPtr = buffer.data() + blockDataLen - 4;
    uint32_t msgLen = 0;
    for (int i = 0; i < 4; ++i)
        msgLen = (msgLen << 8) | lenPtr[i];

    buffer.erase(buffer.begin(), buffer.begin() + blockDataLen);
    if (msgLen > buffer.size())
        throw std::runtime_error("Расшифровка не удалась: некорректная длина сообщения");
    buffer.resize(msgLen);
    return buffer;
}
