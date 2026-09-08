#pragma once

#include <vector>
#include <cstdint>
#include <cmath>
#include <stdexcept>
#include "../hm5/1.h"

using namespace std;

class RSACipher {
private:
    uint64_t m_n;
    uint64_t m_e;
    uint64_t m_d;
    size_t m_blockSize;  // в байтах
    
    // битовая длина числа
    int bitLength(uint64_t n) {
        int bits = 0;
        while (n > 0) {
            n >>= 1;
            bits++;
        }
        return bits;
    }
    
    // преобразование байтов в число
    uint64_t bytesToNumber(const vector<uint8_t>& bytes) {
        uint64_t result = 0;
        for (uint8_t b : bytes) {
            result = (result << 8) | b;
        }
        return result;
    }
    
    // преобразование числа в байты
    vector<uint8_t> numberToBytes(uint64_t num, size_t size) {
        vector<uint8_t> bytes(size, 0);
        
        if (size == 0) 
            return bytes;

        for (size_t i = size; i > 0; i--) {
            bytes[i - 1] = num & 0xFF;
            num >>= 8;
        }
        return bytes;
    }
    
public:
    RSACipher(uint64_t n, uint64_t e, uint64_t d)
        : m_n(n), m_e(e), m_d(d) {
        // размер блока = ((битовая длина N) - 1) / 8
        int bits = bitLength(n);
        m_blockSize = (bits - 1) / 8;
        if (m_blockSize == 0) {
            throw invalid_argument("n is too small");
        }
    }
    
    // шифрование
    vector<uint8_t> encrypt(const vector<uint8_t>& data) {
        vector<uint8_t> result;
        
        for (size_t i = 0; i < data.size(); i += m_blockSize) {
            size_t len = min(m_blockSize, data.size() - i);
            vector<uint8_t> block(data.begin() + i, data.begin() + i + len);
            
            vector<uint8_t> padded = block;
            size_t padLen = m_blockSize - block.size();
            if (padLen > 0) {
                padded.resize(m_blockSize, (uint8_t)padLen);
            }
            
            // блок -> число
            uint64_t m = bytesToNumber(padded);
            
            // шифрование c = m^e (mod N)
            uint64_t c = powmod(m, m_e, m_n);
            
            // число -> байты (фиксированный размер = m_blockSize + 1)
            vector<uint8_t> encryptedBlock = numberToBytes(c, m_blockSize + 1);
            result.insert(result.end(), encryptedBlock.begin(), encryptedBlock.end());
        }
        
        return result;
    }
    
    // дешифрование
    vector<uint8_t> decrypt(const vector<uint8_t>& data) {
        vector<uint8_t> result;
        size_t blockSize = m_blockSize + 1;
        
        for (size_t i = 0; i < data.size(); i += blockSize) {
            vector<uint8_t> block(data.begin() + i, data.begin() + i + blockSize);
            
            // блок -> число
            uint64_t c = bytesToNumber(block);
            
            // дешифрование m = c^d (mod N)
            uint64_t m = powmod(c, m_d, m_n);
            
            // число -> байты (исходный размер)
            vector<uint8_t> decryptedBlock = numberToBytes(m, m_blockSize);
            
            if (!decryptedBlock.empty()) {
                uint8_t padLen = decryptedBlock.back();
                if (padLen > 0 && padLen <= m_blockSize) {
                    bool valid = true;
                    for (size_t j = decryptedBlock.size() - padLen; j < decryptedBlock.size(); j++) {
                        if (decryptedBlock[j] != padLen) {
                            valid = false;
                            break;
                        }
                    }
                    if (valid) {
                        decryptedBlock.resize(decryptedBlock.size() - padLen);
                    }
                }
            }
            
            // добавляем в результат
            for (uint8_t b : decryptedBlock) {
                result.push_back(b);
            }
        }
        
        return result;
    }
    
    size_t getBlockSize() const { return m_blockSize; }
};