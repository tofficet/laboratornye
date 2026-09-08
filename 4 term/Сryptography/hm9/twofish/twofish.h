#include "../../hm8/gf2.cpp"
#include "twofish_tables.h"
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <algorithm>

using namespace std;

uint8_t q0(uint8_t x);
uint8_t q1(uint8_t x);

class twofish {
private:
    vector<uint32_t> round_keys_;
    vector<uint32_t> s_box_keys_;

    // циклический сдвиг 32-битного слова влево
    uint32_t rol32(uint32_t x, int n) const {
        n &= 31;
        return (x << n) | (x >> (32 - n));
    }

    // циклический сдвиг 32-битного слова вправо
    uint32_t ror32(uint32_t x, int n) const {
        n &= 31;
        return (x >> n) | (x << (32 - n));
    }

    // умножение в GF(2^8) с полиномом для MDS: x^8 + x^6 + x^5 + x^3 + 1 (0x169)
    uint8_t gf_mul_mds(uint8_t a, uint8_t b) const {
        PolynomialGF2 pa(a);
        PolynomialGF2 pb(b);
        PolynomialGF2 mod(0x169);
        return static_cast<uint8_t>(pa.mulMod(pb, mod).getCoeffs());
    }

    // умножение в GF(2^8) с полиномом для RS: x^8 + x^6 + x^3 + x^2 + 1 (0x14D)
    uint8_t gf_mul_rs(uint8_t a, uint8_t b) const {
        PolynomialGF2 pa(a);
        PolynomialGF2 pb(b);
        PolynomialGF2 mod(0x14D);
        return static_cast<uint8_t>(pa.mulMod(pb, mod).getCoeffs());
    }

    /* MDS матрица (4x4)
      [01, EF, 5B, 5B]
      [5B, EF, EF, 01]
      [EF, 5B, 01, EF]
      [EF, 01, EF, 5B] */
    uint32_t mds_multiply(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) const {
        uint8_t r0 = gf_mul_mds(0x01, b0) ^ gf_mul_mds(0xEF, b1) ^ gf_mul_mds(0x5B, b2) ^ gf_mul_mds(0x5B, b3);
        uint8_t r1 = gf_mul_mds(0x5B, b0) ^ gf_mul_mds(0xEF, b1) ^ gf_mul_mds(0xEF, b2) ^ gf_mul_mds(0x01, b3);
        uint8_t r2 = gf_mul_mds(0xEF, b0) ^ gf_mul_mds(0x5B, b1) ^ gf_mul_mds(0x01, b2) ^ gf_mul_mds(0xEF, b3);
        uint8_t r3 = gf_mul_mds(0xEF, b0) ^ gf_mul_mds(0x01, b1) ^ gf_mul_mds(0xEF, b2) ^ gf_mul_mds(0x5B, b3);

        return (static_cast<uint32_t>(r3) << 24) |
               (static_cast<uint32_t>(r2) << 16) |
               (static_cast<uint32_t>(r1) << 8) |
               static_cast<uint32_t>(r0);
    }

    // RS матрица для key schedule
    uint32_t rs_multiply(const vector<uint8_t>& k) const {
        uint8_t r0 = gf_mul_rs(0x01, k[0]) ^ gf_mul_rs(0xA4, k[1]) ^ gf_mul_rs(0x55, k[2]) ^ gf_mul_rs(0x87, k[3]) ^
                     gf_mul_rs(0x5A, k[4]) ^ gf_mul_rs(0x58, k[5]) ^ gf_mul_rs(0x1C, k[6]) ^ gf_mul_rs(0x97, k[7]);
        uint8_t r1 = gf_mul_rs(0x5A, k[0]) ^ gf_mul_rs(0x58, k[1]) ^ gf_mul_rs(0x1C, k[2]) ^ gf_mul_rs(0x97, k[3]) ^
                     gf_mul_rs(0x01, k[4]) ^ gf_mul_rs(0xA4, k[5]) ^ gf_mul_rs(0x55, k[6]) ^ gf_mul_rs(0x87, k[7]);
        uint8_t r2 = gf_mul_rs(0xA3, k[0]) ^ gf_mul_rs(0xD9, k[1]) ^ gf_mul_rs(0x59, k[2]) ^ gf_mul_rs(0xEA, k[3]) ^
                     gf_mul_rs(0x4E, k[4]) ^ gf_mul_rs(0x38, k[5]) ^ gf_mul_rs(0x10, k[6]) ^ gf_mul_rs(0x14, k[7]);
        uint8_t r3 = gf_mul_rs(0x4E, k[0]) ^ gf_mul_rs(0x38, k[1]) ^ gf_mul_rs(0x10, k[2]) ^ gf_mul_rs(0x14, k[3]) ^
                     gf_mul_rs(0xA3, k[4]) ^ gf_mul_rs(0xD9, k[5]) ^ gf_mul_rs(0x59, k[6]) ^ gf_mul_rs(0xEA, k[7]);

        return (static_cast<uint32_t>(r3) << 24) |
               (static_cast<uint32_t>(r2) << 16) |
               (static_cast<uint32_t>(r1) << 8) |
               static_cast<uint32_t>(r0);
    }

    // функция h - ключевой элемент Twofish
    uint32_t h_func(uint32_t x, const vector<uint32_t>& l) const {
        uint8_t b0 = static_cast<uint8_t>(x & 0xFF);
        uint8_t b1 = static_cast<uint8_t>((x >> 8) & 0xFF);
        uint8_t b2 = static_cast<uint8_t>((x >> 16) & 0xFF);
        uint8_t b3 = static_cast<uint8_t>((x >> 24) & 0xFF);

        size_t k = l.size();

        // этап 3 (если k == 4)
        if (k == 4) {
            b0 = q1(b0) ^ static_cast<uint8_t>(l[3] & 0xFF);
            b1 = q0(b1) ^ static_cast<uint8_t>((l[3] >> 8) & 0xFF);
            b2 = q0(b2) ^ static_cast<uint8_t>((l[3] >> 16) & 0xFF);
            b3 = q1(b3) ^ static_cast<uint8_t>((l[3] >> 24) & 0xFF);
        }

        // этап 2 (если k >= 3)
        if (k >= 3) {
            b0 = q1(b0) ^ static_cast<uint8_t>(l[2] & 0xFF);
            b1 = q1(b1) ^ static_cast<uint8_t>((l[2] >> 8) & 0xFF);
            b2 = q0(b2) ^ static_cast<uint8_t>((l[2] >> 16) & 0xFF);
            b3 = q0(b3) ^ static_cast<uint8_t>((l[2] >> 24) & 0xFF);
        }

        // этап 1 (если k >= 2)
        if (k >= 2) {
            b0 = q1(q0(q0(b0) ^ static_cast<uint8_t>(l[1] & 0xFF)) ^ static_cast<uint8_t>(l[0] & 0xFF));
            b1 = q0(q0(q1(b1) ^ static_cast<uint8_t>((l[1] >> 8) & 0xFF)) ^ static_cast<uint8_t>((l[0] >> 8) & 0xFF));
            b2 = q1(q1(q0(b2) ^ static_cast<uint8_t>((l[1] >> 16) & 0xFF)) ^ static_cast<uint8_t>((l[0] >> 16) & 0xFF));
            b3 = q0(q1(q1(b3) ^ static_cast<uint8_t>((l[1] >> 24) & 0xFF)) ^ static_cast<uint8_t>((l[0] >> 24) & 0xFF));
        }

        return mds_multiply(b0, b1, b2, b3);
    }

public:
    // настройка раундовых ключей
    void setup_round_keys(const vector<uint8_t>& keys) {
        size_t key_len = keys.size();
        if (key_len != 16 && key_len != 24 && key_len != 32) {
            throw invalid_argument("Invalid key size for Twofish: must be 16, 24, or 32 bytes");
        }

        size_t k = key_len / 8;
        vector<uint32_t> m_even(k);
        vector<uint32_t> m_odd(k);
        s_box_keys_.resize(k);

        // разбиваем ключ на две половины и вычисляем S-box ключи через RS код
        for (size_t i = 0; i < k; ++i) {
            uint32_t m0 = static_cast<uint32_t>(keys[8 * i]) |
                          (static_cast<uint32_t>(keys[8 * i + 1]) << 8) |
                          (static_cast<uint32_t>(keys[8 * i + 2]) << 16) |
                          (static_cast<uint32_t>(keys[8 * i + 3]) << 24);

            uint32_t m1 = static_cast<uint32_t>(keys[8 * i + 4]) |
                          (static_cast<uint32_t>(keys[8 * i + 5]) << 8) |
                          (static_cast<uint32_t>(keys[8 * i + 6]) << 16) |
                          (static_cast<uint32_t>(keys[8 * i + 7]) << 24);

            m_even[i] = m0;
            m_odd[i] = m1;

            vector<uint8_t> rs_block;
            for (size_t j = 0; j < 8; ++j) {
                rs_block.push_back(keys[8 * i + j]);
            }
            s_box_keys_[k - 1 - i] = rs_multiply(rs_block);
        }

        // генерация раундовых ключей
        round_keys_.resize(40);
        for (size_t i = 0; i < 20; ++i) {
            uint32_t a = h_func(static_cast<uint32_t>(2 * i) * 0x01010101, m_even);
            uint32_t b = rol32(h_func(static_cast<uint32_t>(2 * i + 1) * 0x01010101, m_odd), 8);

            round_keys_[2 * i] = a + b;
            round_keys_[2 * i + 1] = rol32(a + 2 * b, 9);
        }
    }

    // шифрование блока
    vector<uint8_t> encrypt_block(const vector<uint8_t>& plain_text) {
        if (plain_text.size() != 16) {
            throw invalid_argument("Twofish: block size must be 16 bytes");
        }

        uint32_t R[4];
        for (int i = 0; i < 4; ++i) {
            R[i] = static_cast<uint32_t>(plain_text[4 * i]) |
                   (static_cast<uint32_t>(plain_text[4 * i + 1]) << 8) |
                   (static_cast<uint32_t>(plain_text[4 * i + 2]) << 16) |
                   (static_cast<uint32_t>(plain_text[4 * i + 3]) << 24);
        }

        // входное отбеливание
        for (int i = 0; i < 4; ++i)
            R[i] ^= round_keys_[i];

        // 16 раундов
        for (int r = 0; r < 16; ++r) {
            uint32_t t0 = h_func(R[0], s_box_keys_);
            uint32_t t1 = h_func(rol32(R[1], 8), s_box_keys_);

            uint32_t f0 = t0 + t1 + round_keys_[8 + 2 * r];
            uint32_t f1 = t0 + 2 * t1 + round_keys_[8 + 2 * r + 1];

            R[2] = ror32(R[2] ^ f0, 1);
            R[3] = rol32(R[3], 1) ^ f1;

            if (r < 15) {
                swap(R[0], R[2]);
                swap(R[1], R[3]);
            }
        }

        // выходное отбеливание (с отменой последнего swap)
        R[2] ^= round_keys_[4];
        R[3] ^= round_keys_[5];
        R[0] ^= round_keys_[6];
        R[1] ^= round_keys_[7];

        vector<uint8_t> cipher_text(16);
        for (int i = 0; i < 4; ++i) {
            cipher_text[4 * i] = static_cast<uint8_t>(R[i] & 0xFF);
            cipher_text[4 * i + 1] = static_cast<uint8_t>((R[i] >> 8) & 0xFF);
            cipher_text[4 * i + 2] = static_cast<uint8_t>((R[i] >> 16) & 0xFF);
            cipher_text[4 * i + 3] = static_cast<uint8_t>((R[i] >> 24) & 0xFF);
        }

        return cipher_text;
    }

    // дешифрование блока
    vector<uint8_t> decrypt_block(const vector<uint8_t>& cipher_text) {
        if (cipher_text.size() != 16) {
            throw invalid_argument("Twofish: block size must be 16 bytes");
        }

        uint32_t R[4];
        for (int i = 0; i < 4; ++i) {
            R[i] = static_cast<uint32_t>(cipher_text[4 * i]) |
                   (static_cast<uint32_t>(cipher_text[4 * i + 1]) << 8) |
                   (static_cast<uint32_t>(cipher_text[4 * i + 2]) << 16) |
                   (static_cast<uint32_t>(cipher_text[4 * i + 3]) << 24);
        }

        // входное отбеливание для дешифрования (ключи 4-7)
        R[0] ^= round_keys_[4];
        R[1] ^= round_keys_[5];
        R[2] ^= round_keys_[6];
        R[3] ^= round_keys_[7];

        // 16 раундов в обратном порядке
        for (int r = 15; r >= 0; --r) {
            uint32_t t0 = h_func(R[0], s_box_keys_);
            uint32_t t1 = h_func(rol32(R[1], 8), s_box_keys_);

            uint32_t f0 = t0 + t1 + round_keys_[8 + 2 * r];
            uint32_t f1 = t0 + 2 * t1 + round_keys_[8 + 2 * r + 1];

            // в дешифровании операции меняются местами
            R[2] = rol32(R[2], 1) ^ f0;
            R[3] = ror32(R[3] ^ f1, 1);

            if (r > 0) {
                swap(R[0], R[2]);
                swap(R[1], R[3]);
            }
        }

        // выходное отбеливание для дешифрования (ключи 0-3)
        for (int i = 0; i < 4; ++i)
            R[i] ^= round_keys_[i];

        vector<uint8_t> plain_text(16);
        for (int i = 0; i < 4; ++i) {
            plain_text[4 * i] = static_cast<uint8_t>(R[i] & 0xFF);
            plain_text[4 * i + 1] = static_cast<uint8_t>((R[i] >> 8) & 0xFF);
            plain_text[4 * i + 2] = static_cast<uint8_t>((R[i] >> 16) & 0xFF);
            plain_text[4 * i + 3] = static_cast<uint8_t>((R[i] >> 24) & 0xFF);
        }

        return plain_text;
    }
};