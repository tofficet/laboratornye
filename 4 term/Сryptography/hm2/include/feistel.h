#pragma once

#include <iostream>
#include <vector>
#include <cstdint>
#include <stdexcept>

using namespace std;

// 1a
class IKeySchedule {
public:
    virtual ~IKeySchedule() = default;
    
    virtual vector<vector<uint8_t>> expandKey(const vector<uint8_t>& key) = 0;
};

// 1b
class IFunctionF {
public:
    virtual ~IFunctionF() = default;
    
    virtual vector<uint8_t> apply(const vector<uint8_t>& halfBlock,
                                   const vector<uint8_t>& roundKey) = 0;
};

// 1c
class IBlockCipher {
public:
    virtual ~IBlockCipher() = default;

    virtual void setKey(const vector<uint8_t>& key) = 0;
    
    virtual void setRoundKeys(const vector<vector<uint8_t>>& keys) = 0;
    
    virtual vector<uint8_t> encryptBlock(const vector<uint8_t>& block) = 0;
    
    virtual vector<uint8_t> decryptBlock(const vector<uint8_t>& block) = 0;

    virtual size_t getBlockSize() const = 0;
};

// 1d
class FeistelNetwork : public IBlockCipher {
private:
    IFunctionF* m_fFunction;
    IKeySchedule* m_keySchedule;  // генератор раундовых ключей
    int m_rounds;                       
    vector<vector<uint8_t>> m_roundKeys;
    size_t m_blockSize;
    
public:
    FeistelNetwork(IFunctionF* fFunction, IKeySchedule* keySchedule, int rounds, size_t blockSize)
        : m_fFunction(fFunction)
        , m_keySchedule(keySchedule)
        , m_rounds(rounds)
        , m_blockSize(blockSize) {
        
        if (!m_fFunction || !m_keySchedule) {
            throw invalid_argument("F-function and KeySchedule cannot be null");
        }
        
        if (m_rounds <= 0) {
            throw invalid_argument("The number of rounds must be positive");
        }
    }

    ~FeistelNetwork() {
        delete m_fFunction;
        delete m_keySchedule;
    }
    
    // генерация раундовых ключей
    void setKey(const vector<uint8_t>& key) override {
        m_roundKeys = m_keySchedule->expandKey(key);
        if (m_roundKeys.size() != (size_t)m_rounds) {
            throw runtime_error("KeySchedule returned an incorrect number of round keys");
        }
    }
    
    void setRoundKeys(const vector<vector<uint8_t>>& keys) override {
        m_roundKeys = keys;
        m_rounds = (int)keys.size();
        if (!m_roundKeys.empty() && !m_roundKeys[0].empty()) {
            m_blockSize = m_roundKeys[0].size() * 2;
        }
    }
    
    pair<vector<uint8_t>, vector<uint8_t>> splitBlock(const vector<uint8_t>& block) {
        size_t half = block.size() / 2;
        vector<uint8_t> left(block.begin(), block.begin() + half);
        vector<uint8_t> right(block.begin() + half, block.end());
        return {left, right};
    }
    
    vector<uint8_t> joinBlocks(const vector<uint8_t>& left, const vector<uint8_t>& right) {
        vector<uint8_t> result;
        result.insert(result.end(), left.begin(), left.end());
        result.insert(result.end(), right.begin(), right.end());
        return result;
    }
    
    vector<uint8_t> encryptBlock(const vector<uint8_t>& block) override {
        if (block.size() != m_blockSize) {
            throw invalid_argument("The block size does not match the expected size");
        }

        if (m_roundKeys.size() != (size_t)m_rounds) {
            throw runtime_error("Round keys are not installed");
        }
        
        auto [left, right] = splitBlock(block);
        
        for (int round = 0; round < m_rounds; round++) {
            // F(right, roundKey)
            vector<uint8_t> fOut = m_fFunction->apply(right, m_roundKeys[round]);
            
            // XOR с левой половиной
            vector<uint8_t> newLeft(right.size());
            for (size_t i = 0; i < right.size(); i++) {
                newLeft[i] = left[i] ^ fOut[i];
            }
            
            // swap для следующего раунда
            if (round != m_rounds - 1) {  // не меняем местами на последнем раунде
                left = right;
                right = newLeft;
            } else {
                left = newLeft;
            }
        }
        
        return joinBlocks(right, left);
    }
    
    // дешифрование
    vector<uint8_t> decryptBlock(const vector<uint8_t>& block) override {
        if (block.size() != m_blockSize) {
            throw invalid_argument("The block size does not match the expected size");
        }

        if (m_roundKeys.size() != (size_t)m_rounds) {
            throw runtime_error("Round keys are not installed");
        }
        
        auto [left, right] = splitBlock(block);
        
        for (int round = m_rounds - 1; round >= 0; round--) {
            // F(left, roundKey)
            vector<uint8_t> fOut = m_fFunction->apply(left, m_roundKeys[round]);
            
            vector<uint8_t> newRight(left.size());
            for (size_t i = 0; i < left.size(); i++) {
                newRight[i] = right[i] ^ fOut[i];
            }
            
            if (round != 0) {
                right = left;
                left = newRight;
            } else {
                right = newRight;
            }
        }
        
        return joinBlocks(right, left);
    }
    
    size_t getBlockSize() const override { return m_blockSize; }
    int getRounds() const { return m_rounds; }
};

// 2
class FeistelCipher : public IBlockCipher {
protected:
    FeistelNetwork m_network;
    
    virtual vector<uint8_t> preprocess(const vector<uint8_t>& block) {
        return block;
    }
    
    virtual vector<uint8_t> postprocess(const vector<uint8_t>& block) {
        return block;
    }
    
public:
    FeistelCipher(IFunctionF* fFunction, IKeySchedule* keySchedule, int rounds, size_t blockSize)
        : m_network(fFunction, keySchedule, rounds, blockSize) {}
    
    void setKey(const vector<uint8_t>& key) override {
        m_network.setKey(key);
    }
    
    void setRoundKeys(const vector<vector<uint8_t>>& keys) override {
        m_network.setRoundKeys(keys);
    }
    
    vector<uint8_t> encryptBlock(const vector<uint8_t>& block) override {
        auto step1 = preprocess(block);
        auto step2 = m_network.encryptBlock(step1);
        return postprocess(step2);
    }
    
    vector<uint8_t> decryptBlock(const vector<uint8_t>& block) override {
        auto step1 = preprocess(block);
        auto step2 = m_network.decryptBlock(step1);
        return postprocess(step2);
    }
    
    size_t getBlockSize() const override {
        return m_network.getBlockSize();
    }
};