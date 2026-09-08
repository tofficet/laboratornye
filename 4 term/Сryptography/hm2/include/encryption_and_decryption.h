#pragma once

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "feistel.h"

using namespace std;

class CipherException : public exception {
private:
    string m_message;
public:
    explicit CipherException(const string& msg) : m_message(msg) {}
    const char* what() const noexcept override { return m_message.c_str(); }
};

class InvalidKeyException : public CipherException {
public:
    explicit InvalidKeyException(const string& msg) : CipherException(msg) {}
};

class InvalidDataException : public CipherException {
public:
    explicit InvalidDataException(const string& msg) : CipherException(msg) {}
};

class FileOperationException : public CipherException {
public:
    explicit FileOperationException(const string& msg) : CipherException(msg) {}
};

inline vector<uint8_t> addPadding(const vector<uint8_t>& data, size_t blockSize) {
    vector<uint8_t> result = data;
    size_t padLen = blockSize - (data.size() % blockSize);
    if (padLen == blockSize) 
        padLen = 0;
    
    if (padLen > 0) {
        result.resize(data.size() + padLen, 0);
        result.back() = (uint8_t)padLen;
    }
    
    return result;
}

inline vector<uint8_t> removePadding(const vector<uint8_t>& data) {
    if (data.empty()) 
        return data;
    
    uint8_t padLen = data.back();
    if (padLen == 0 || padLen > data.size()) {
        return data;
    }
    
    return vector<uint8_t>(data.begin(), data.end() - padLen);
}

class CipherContext {
private:
    IBlockCipher* m_algorithm;

public:
    explicit CipherContext(IBlockCipher* algorithm)
        : m_algorithm(algorithm) {}

    ~CipherContext() {
        delete m_algorithm;
    }

    vector<uint8_t> encrypt(const vector<uint8_t>& input, const vector<uint8_t>& key) {
        m_algorithm->setKey(key);
        size_t blockSize = m_algorithm->getBlockSize();
        vector<uint8_t> padded = addPadding(input, blockSize);

        vector<uint8_t> result;

        for (size_t i = 0; i < padded.size(); i += blockSize) {
            vector<uint8_t> block(padded.begin() + i, padded.begin() + i + blockSize);
            vector<uint8_t> output = m_algorithm->encryptBlock(block);
            result.insert(result.end(), output.begin(), output.end());
        }

        return result;
    }

    vector<uint8_t> decrypt(const vector<uint8_t>& input, const vector<uint8_t>& key) {
        m_algorithm->setKey(key);
        size_t blockSize = m_algorithm->getBlockSize();

        if (input.size() % blockSize != 0) {
            throw InvalidDataException("Encrypted data size must be multiple of block size");
        }

        vector<uint8_t> decrypted;

        for (size_t i = 0; i < input.size(); i += blockSize) {
            vector<uint8_t> block(input.begin() + i, input.begin() + i + blockSize);
            vector<uint8_t> output = m_algorithm->decryptBlock(block);
            decrypted.insert(decrypted.end(), output.begin(), output.end());
        }

        return removePadding(decrypted);
    }

    void encryptFile(const string& inputPath,
                      const string& outputPath,
                      const vector<uint8_t>& key) {
        ifstream inFile(inputPath, ios::binary);

        if (!inFile) {
            throw FileOperationException("Cannot open input file: " + inputPath);
        }

        vector<uint8_t> data((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
        inFile.close();

        vector<uint8_t> encrypted = encrypt(data, key);

        ofstream outFile(outputPath, ios::binary);

        if (!outFile) {
            throw FileOperationException("Cannot create output file: " + outputPath);
        }

        outFile.write(reinterpret_cast<const char*>(encrypted.data()), encrypted.size());
        outFile.close();
    }

    void decryptFile(const string& inputPath,
                      const string& outputPath,
                      const vector<uint8_t>& key) {
        ifstream inFile(inputPath, ios::binary);

        if (!inFile) {
            throw FileOperationException("Cannot open input file: " + inputPath);
        }

        vector<uint8_t> data((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
        inFile.close();

        vector<uint8_t> decrypted = decrypt(data, key);

        ofstream outFile(outputPath, ios::binary);
        
        if (!outFile) {
            throw FileOperationException("Cannot create output file: " + outputPath);
        }

        outFile.write(reinterpret_cast<const char*>(decrypted.data()), decrypted.size());
        outFile.close();
    }

    size_t getBlockSize() const { return m_algorithm->getBlockSize(); }
};