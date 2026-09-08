#include <vector>
#include <cstdint>
#include <stdexcept>
#include <algorithm>
#include "mars_tables.h"

using namespace std;

class mars {
private:
    vector<uint32_t> round_keys_;  // 40 раундовых ключей

    // циклический сдвиг влево
    uint32_t rol32(uint32_t x, int n) const {
        n &= 31;
        return (x << n) | (x >> (32 - n));
    }

    // циклический сдвиг вправо
    uint32_t ror32(uint32_t x, int n) const {
        n &= 31;
        return (x >> n) | (x << (32 - n));
    }

    void e_function(uint32_t in, uint32_t k_mul, uint32_t k_add,
                    uint32_t& out_l, uint32_t& out_m, uint32_t& out_r) const {
        uint32_t m = in + k_add;             // M = вход + ключ_сложения
        uint32_t r = rol32(in, 13) * k_mul;  // R = (вход << 13) * ключ_умножения
        uint32_t s = mars_s_box[m & 0x1FF];  // S = S-бокс[младшие 9 бит M]
        out_l = rol32(s, r & 0x1F);          // выход L = S << (R & 31)
        out_m = rol32(r, 5);                 // выход M = R << 5
        out_r = rol32(m, out_m & 0x1F);      // выход R = M << (M & 31)
    }

    // прямое перемешивание
    void forward_mixing(uint32_t d[4]) const {
        for (int i = 0; i < 8; ++i) {
            d[1] ^= mars_s_box[d[0] & 0xFF];                  // XOR S0[b0]
            d[1] += mars_s_box[((d[0] >> 8) & 0xFF) + 256];   // ADD S1[b1]
            d[2] += mars_s_box[(d[0] >> 16) & 0xFF];          // ADD S0[b2]
            d[3] ^= mars_s_box[((d[0] >> 24) & 0xFF) + 256];  // XOR S1[b3]
            d[0] = ror32(d[0], 24);                           // сдвиг источника на 24 вправо
            d[0] += d[3];                                     // дополнительное перемешивание
            d[2] ^= d[0];                                     // дополнительное перемешивание
            uint32_t t = d[0]; d[0] = d[1]; d[1] = d[2]; d[2] = d[3]; d[3] = t;  // (A,B,C,D) -> (B,C,D,A)
        }
    }

    // обратное прямое перемешивание
    void inv_forward_mixing(uint32_t d[4]) const {
        for (int i = 0; i < 8; ++i) {
            uint32_t t = d[3]; d[3] = d[2]; d[2] = d[1]; d[1] = d[0]; d[0] = t;  // (A,B,C,D) -> (D,C,B,A)
            d[2] ^= d[0];
            d[0] -= d[3];
            d[0] = rol32(d[0], 24);
            d[3] ^= mars_s_box[((d[0] >> 24) & 0xFF) + 256];
            d[2] -= mars_s_box[(d[0] >> 16) & 0xFF];
            d[1] -= mars_s_box[((d[0] >> 8) & 0xFF) + 256];
            d[1] ^= mars_s_box[d[0] & 0xFF];
        }
    }

    // обратное перемешивание - 8 раундов после криптографического ядра
    void backward_mixing(uint32_t d[4]) const {
        for (int i = 0; i < 8; ++i) {
            d[2] ^= d[0];                                     // XOR
            d[0] -= d[3];                                     // вычитание
            d[0] = ror32(d[0], 24);                           // сдвиг вправо на 24
            d[3] ^= mars_s_box[((d[0] >> 24) & 0xFF) + 256];  // XOR S1[b3]
            d[2] -= mars_s_box[(d[0] >> 16) & 0xFF];          // SUB S0[b2]
            d[1] -= mars_s_box[((d[0] >> 8) & 0xFF) + 256];   // SUB S1[b1]
            d[1] ^= mars_s_box[d[0] & 0xFF];                  // XOR S0[b0]
            uint32_t t = d[0]; d[0] = d[1]; d[1] = d[2]; d[2] = d[3]; d[3] = t;  // (A,B,C,D) -> (B,C,D,A)
        }
    }

    // обратное обратное перемешивание
    void inv_backward_mixing(uint32_t d[4]) const {
        for (int i = 0; i < 8; ++i) {
            uint32_t t = d[3]; d[3] = d[2]; d[2] = d[1]; d[1] = d[0]; d[0] = t;  // (A,B,C,D) -> (D,C,B,A)
            d[1] ^= mars_s_box[d[0] & 0xFF];
            d[1] += mars_s_box[((d[0] >> 8) & 0xFF) + 256];
            d[2] += mars_s_box[(d[0] >> 16) & 0xFF];
            d[3] ^= mars_s_box[((d[0] >> 24) & 0xFF) + 256];
            d[0] = rol32(d[0], 24);
            d[0] += d[3];
            d[2] ^= d[0];
        }
    }

public:
    // генерация 40 раундовых ключей из пользовательского ключа
    void setup_round_keys(const vector<uint8_t>& key) {
        size_t key_len = key.size();

        if (key_len < 16 || key_len > 56) {
            throw invalid_argument("MARS: key must be between 16 and 56 bytes");
        }

        round_keys_.resize(40);

        int n = key_len / 4;  // количество 32-битных слов в ключе
        vector<uint32_t> T(15, 0);  // временный массив T размером 15 слов

        // заполняем T байтами ключа
        for (int i = 0; i < n; ++i) {
            T[i] = static_cast<uint32_t>(key[4 * i]) |
                   (static_cast<uint32_t>(key[4 * i + 1]) << 8) |
                   (static_cast<uint32_t>(key[4 * i + 2]) << 16) |
                   (static_cast<uint32_t>(key[4 * i + 3]) << 24);
        }

        T[n] = n;  // устанавливаем длину ключа в T[n]

        // 4 прохода генерации ключей (по 10 ключей за проход)
        for (int j = 0; j < 4; ++j) {
            // линейное расширение
            for (int i = 0; i < 15; ++i) {
                T[i] ^= rol32(T[(i + 8) % 15] ^ T[(i + 13) % 15], 3) ^ (4 * j + i);
            }

            // S-бокс перемешивание (4 повторения)
            for (int repeat = 0; repeat < 4; ++repeat) {
                for (int i = 0; i < 15; ++i) {
                    T[i] = rol32(T[i] + mars_s_box[T[(i + 14) % 15] & 0x1FF], 9);
                }
            }

            // сохраняем 10 ключей в round_keys
            for (int i = 0; i < 10; ++i) {
                round_keys_[10 * j + i] = T[4 * i % 15];
            }
        }

        // модификация ключей, используемых в умножении (устанавливаем два младших бита в 1)
        for (int i = 5; i <= 35; i += 2) {
            round_keys_[i] |= 0x03;
        }
    }

    // шифрование 16-байтового блока
    vector<uint8_t> encrypt_block(const vector<uint8_t>& plain_text) {
        if (plain_text.size() != 16) {
            throw invalid_argument("MARS: block size must be 16 bytes");
        }

        uint32_t d[4];

        // разбиваем входной блок на 4 слова
        for (int i = 0; i < 4; ++i) {
            d[i] = static_cast<uint32_t>(plain_text[4 * i]) |
                   (static_cast<uint32_t>(plain_text[4 * i + 1]) << 8) |
                   (static_cast<uint32_t>(plain_text[4 * i + 2]) << 16) |
                   (static_cast<uint32_t>(plain_text[4 * i + 3]) << 24);
            d[i] += round_keys_[i];  // входное отбеливание: XOR с K[0...3]
        }

        forward_mixing(d);  // прямое перемешивание (8 раундов)

        // 16 раундов
        for (int i = 0; i < 16; ++i) {
            uint32_t L, M, R;
            e_function(d[0], round_keys_[2 * i + 5], round_keys_[2 * i + 4], L, M, R);
            d[0] = rol32(d[0], 13);  // сдвиг источника на 13 влево

            if (i < 8) {  // 8 раундов прямой порядок
                d[1] += L;
                d[2] += M;
                d[3] ^= R;
            } else {  // 8 раундов обратный порядок
                d[3] += L;
                d[1] += M;
                d[2] ^= R;
            }

            uint32_t t = d[0]; d[0] = d[1]; d[1] = d[2]; d[2] = d[3]; d[3] = t;  // (A,B,C,D) -> (B,C,D,A)
        }

        backward_mixing(d);  // обратное перемешивание (8 раундов)

        vector<uint8_t> cipher_text(16);

        // выходное отбеливание: вычитание K[36...39]
        for (int i = 0; i < 4; ++i) {
            d[i] -= round_keys_[36 + i];
            cipher_text[4 * i] = static_cast<uint8_t>(d[i] & 0xFF);
            cipher_text[4 * i + 1] = static_cast<uint8_t>((d[i] >> 8) & 0xFF);
            cipher_text[4 * i + 2] = static_cast<uint8_t>((d[i] >> 16) & 0xFF);
            cipher_text[4 * i + 3] = static_cast<uint8_t>((d[i] >> 24) & 0xFF);
        }

        return cipher_text;
    }

    // дешифрование 16-байтового блока
    vector<uint8_t> decrypt_block(const vector<uint8_t>& cipher_text) {
        if (cipher_text.size() != 16) {
            throw invalid_argument("MARS: block size must be 16 bytes");
        }

        uint32_t d[4];

        // разбиваем входной блок на 4 слова
        for (int i = 0; i < 4; ++i) {
            d[i] = static_cast<uint32_t>(cipher_text[4 * i]) |
                   (static_cast<uint32_t>(cipher_text[4 * i + 1]) << 8) |
                   (static_cast<uint32_t>(cipher_text[4 * i + 2]) << 16) |
                   (static_cast<uint32_t>(cipher_text[4 * i + 3]) << 24);
            d[i] += round_keys_[36 + i];  // входное отбеливание для дешифрования
        }

        inv_backward_mixing(d);  // обратное обратное перемешивание

        // 16 раундов в обратном порядке
        for (int i = 15; i >= 0; --i) {
            uint32_t t = d[3]; d[3] = d[2]; d[2] = d[1]; d[1] = d[0]; d[0] = t;  // (A,B,C,D) -> (D,C,B,A)
            uint32_t d0_orig = ror32(d[0], 13);  // восстанавливаем исходное d[0] до E-функции

            uint32_t L, M, R;
            e_function(d0_orig, round_keys_[2 * i + 5], round_keys_[2 * i + 4], L, M, R);

            if (i < 8) {  // обратные операции для первых 8 раундов
                d[1] -= L;
                d[2] -= M;
                d[3] ^= R;
            } else {  // обратные операции для последних 8 раундов
                d[3] -= L;
                d[1] -= M;
                d[2] ^= R;
            }

            d[0] = d0_orig;
        }

        inv_forward_mixing(d);  // обратное прямое перемешивание

        vector<uint8_t> plain_text(16);

        // выходное отбеливание для дешифрования: вычитание K[0...3]
        for (int i = 0; i < 4; ++i) {
            d[i] -= round_keys_[i];
            plain_text[4 * i] = static_cast<uint8_t>(d[i] & 0xFF);
            plain_text[4 * i + 1] = static_cast<uint8_t>((d[i] >> 8) & 0xFF);
            plain_text[4 * i + 2] = static_cast<uint8_t>((d[i] >> 16) & 0xFF);
            plain_text[4 * i + 3] = static_cast<uint8_t>((d[i] >> 24) & 0xFF);
        }

        return plain_text;
    }
};