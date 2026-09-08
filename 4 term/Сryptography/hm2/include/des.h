#pragma once

#include "feistel.h"
#include "des_tables.h"
#include "encryption_and_decryption.h"
#include <array>
#include <bitset>

// F-функция DES
class DES_F : public IFunctionF {
private:
    uint32_t f(uint32_t right, uint64_t roundKey);
    
public:
    vector<uint8_t> apply(const vector<uint8_t>& halfBlock,
                           const vector<uint8_t>& roundKey) override;
};

// Key Schedule для DES
class DESKeySchedule : public IKeySchedule {
private:
    uint64_t applyPC1(uint64_t key);
    uint64_t applyPC2(uint64_t key56);
    uint64_t leftShift(uint64_t key56, int shift);
    
public:
    vector<vector<uint8_t>> expandKey(const vector<uint8_t>& key) override;
};

// DES шифр
class DESCipher : public FeistelCipher {
public:
    DESCipher();
    
    size_t getBlockSize() const override { return 8; }
    
    vector<uint8_t> preprocess(const vector<uint8_t>& block) override;
    vector<uint8_t> postprocess(const vector<uint8_t>& block) override;
};