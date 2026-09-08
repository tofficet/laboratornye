#include "include/triple_des.h"
#include <stdexcept>

using namespace std;

TripleDES::TripleDES(TripleDESMode mode)
    : m_mode(mode) {}

void TripleDES::setKey(const vector<uint8_t>& key) {
    size_t keySize = key.size();
    vector<uint8_t> k1, k2, k3;
    
    if (m_mode == TripleDESMode::EEE3 || m_mode == TripleDESMode::EDE3) {
        if (keySize != 24) {
            throw invalid_argument("3DES-3 requires 24 bytes key");
        }
        k1.assign(key.begin(), key.begin() + 8);
        k2.assign(key.begin() + 8, key.begin() + 16);
        k3.assign(key.begin() + 16, key.begin() + 24);
    } else {
        if (keySize != 16) {
            throw invalid_argument("3DES-2 requires 16 bytes key");
        }
        k1.assign(key.begin(), key.begin() + 8);
        k2.assign(key.begin() + 8, key.begin() + 16);
        k3 = k1;
    }
    
    m_des1.setKey(k1);
    m_des2.setKey(k2);
    m_des3.setKey(k3);
}

void TripleDES::setRoundKeys(const vector<vector<uint8_t>>& keys) {
    // 3DES не использует этот метод
}

vector<uint8_t> TripleDES::encryptBlock(const vector<uint8_t>& block) {
    switch (m_mode) {
        case TripleDESMode::EEE3:
            return applyEEE3(block, true);
        case TripleDESMode::EDE3:
            return applyEDE3(block, true);
        case TripleDESMode::EEE2:
            return applyEEE2(block, true);
        case TripleDESMode::EDE2:
            return applyEDE2(block, true);
        default:
            throw runtime_error("Unknown mode");
    }
}

vector<uint8_t> TripleDES::decryptBlock(const vector<uint8_t>& block) {
    switch (m_mode) {
        case TripleDESMode::EEE3:
            return applyEEE3(block, false);
        case TripleDESMode::EDE3:
            return applyEDE3(block, false);
        case TripleDESMode::EEE2:
            return applyEEE2(block, false);
        case TripleDESMode::EDE2:
            return applyEDE2(block, false);
        default:
            throw runtime_error("Unknown mode");
    }
}

vector<uint8_t> TripleDES::applyEEE3(const vector<uint8_t>& block, bool encrypt) {
    if (encrypt) {
        auto tmp = m_des1.FeistelCipher::encryptBlock(block);
        tmp = m_des2.FeistelCipher::encryptBlock(tmp);
        return m_des3.FeistelCipher::encryptBlock(tmp);
    } else {
        auto tmp = m_des3.FeistelCipher::decryptBlock(block);
        tmp = m_des2.FeistelCipher::decryptBlock(tmp);
        return m_des1.FeistelCipher::decryptBlock(tmp);
    }
}

vector<uint8_t> TripleDES::applyEDE3(const vector<uint8_t>& block, bool encrypt) {
    if (encrypt) {
        auto tmp = m_des1.FeistelCipher::encryptBlock(block);
        tmp = m_des2.FeistelCipher::decryptBlock(tmp);
        return m_des3.FeistelCipher::encryptBlock(tmp);
    } else {
        auto tmp = m_des3.FeistelCipher::decryptBlock(block);
        tmp = m_des2.FeistelCipher::encryptBlock(tmp);
        return m_des1.FeistelCipher::decryptBlock(tmp);
    }
}

vector<uint8_t> TripleDES::applyEEE2(const vector<uint8_t>& block, bool encrypt) {
    if (encrypt) {
        auto tmp = m_des1.FeistelCipher::encryptBlock(block);
        tmp = m_des2.FeistelCipher::encryptBlock(tmp);
        return m_des1.FeistelCipher::encryptBlock(tmp);
    } else {
        auto tmp = m_des1.FeistelCipher::decryptBlock(block);
        tmp = m_des2.FeistelCipher::decryptBlock(tmp);
        return m_des1.FeistelCipher::decryptBlock(tmp);
    }
}

vector<uint8_t> TripleDES::applyEDE2(const vector<uint8_t>& block, bool encrypt) {
    if (encrypt) {
        auto tmp = m_des1.FeistelCipher::encryptBlock(block);
        tmp = m_des2.FeistelCipher::decryptBlock(tmp);
        return m_des1.FeistelCipher::encryptBlock(tmp);
    } else {
        auto tmp = m_des1.FeistelCipher::decryptBlock(block);
        tmp = m_des2.FeistelCipher::encryptBlock(tmp);
        return m_des1.FeistelCipher::decryptBlock(tmp);
    }
}