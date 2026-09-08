#pragma once

#include <cstdint>
#include <array>

class RijndaelSBox {
private:
    static constexpr uint8_t IRREDUCIBLE = 0x1B;  // x⁸ + x⁴ + x³ + x + 1
    static constexpr uint8_t AFFINE_MATRIX[8] = {
        0x8F, 0x4F, 0x27, 0x13, 0x09, 0x04, 0x02, 0x01
    };
    
    // Умножение в GF(2⁸)
    static uint8_t gf256_mul(uint8_t a, uint8_t b) {
        uint8_t result = 0;
        for (int i = 0; i < 8; i++) {
            if (b & 1) result ^= a;
            a = (a & 0x80) ? ((a << 1) ^ IRREDUCIBLE) : (a << 1);
            b >>= 1;
        }
        return result;
    }
    
    // Обратный элемент в GF(2⁸)
    static uint8_t gf256_inv(uint8_t x) {
        if (x == 0) return 0;
        
        // Возведение в степень 254 (x^(2⁸-2)) через быстрое возведение
        uint8_t result = 1;
        uint8_t base = x;
        
        for (int i = 0; i < 7; i++) {
            result = gf256_mul(result, result);
            result = gf256_mul(result, base);
            base = gf256_mul(base, base);
        }
        
        return result;
    }
    
    // Аффинное преобразование
    static uint8_t affineTransform(uint8_t x) {
        uint8_t result = 0;
        for (int i = 0; i < 8; i++) {
            uint8_t bit = 0;
            for (int j = 0; j < 8; j++) {
                if (x & (1 << j)) {
                    bit ^= (AFFINE_MATRIX[i] >> j) & 1;
                }
            }
            result |= (bit ^ (i == 0 ? 1 : 0)) << i;  // XOR с 0x63
        }
        return result;
    }
    
public:
    // Построение S-блока
    static std::array<uint8_t, 256> buildSBox() {
        std::array<uint8_t, 256> sbox;
        
        for (int i = 0; i < 256; i++) {
            uint8_t inv = gf256_inv(i);
            sbox[i] = affineTransform(inv);
        }
        
        return sbox;
    }
    
    // Построение обратного S-блока (InvS-Box)
    static std::array<uint8_t, 256> buildInvSBox() {
        auto sbox = buildSBox();
        std::array<uint8_t, 256> invSbox;
        
        for (int i = 0; i < 256; i++) {
            invSbox[sbox[i]] = i;
        }
        
        return invSbox;
    }
};