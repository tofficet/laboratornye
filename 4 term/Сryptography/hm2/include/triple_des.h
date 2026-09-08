#pragma once

#include "des.h"
#include "encryption_and_decryption.h"
#include <vector>

using namespace std;

enum class TripleDESMode {
    EEE3,
    EDE3,
    EEE2,
    EDE2
};

class TripleDES : public IBlockCipher {
private:
    DESCipher m_des1;
    DESCipher m_des2;
    DESCipher m_des3;
    TripleDESMode m_mode;
    
    vector<uint8_t> applyEEE3(const vector<uint8_t>& block, bool encrypt);
    vector<uint8_t> applyEDE3(const vector<uint8_t>& block, bool encrypt);
    vector<uint8_t> applyEEE2(const vector<uint8_t>& block, bool encrypt);
    vector<uint8_t> applyEDE2(const vector<uint8_t>& block, bool encrypt);
    
public:
    TripleDES(TripleDESMode mode);
    
    void setKey(const vector<uint8_t>& key) override;
    
    void setRoundKeys(const vector<vector<uint8_t>>& keys) override;
    
    vector<uint8_t> encryptBlock(const vector<uint8_t>& block) override;
    vector<uint8_t> decryptBlock(const vector<uint8_t>& block) override;
    
    size_t getBlockSize() const override { return 8; }
};