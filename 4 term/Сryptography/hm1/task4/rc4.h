#pragma once

#include <vector>
#include <cstdint>

using namespace std;

class RC4 {
private:
    vector<uint8_t> S;
    int i, j;
    
    void swap(uint8_t& a, uint8_t& b) {
        uint8_t t = a;
        a = b;
        b = t;
    }
    
public:
    RC4(const vector<uint8_t>& key) {
        S.resize(256);
        for (int k = 0; k < 256; k++) {
            S[k] = k;
        }
        
        j = 0;
        for (int k = 0; k < 256; k++) {
            j = (j + S[k] + key[k % key.size()]) % 256;
            swap(S[k], S[j]);
        }
        
        i = 0;
    }
    
    uint8_t nextByte() {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        swap(S[i], S[j]);
        return S[(S[i] + S[j]) % 256];
    }
    
    vector<uint8_t> process(const vector<uint8_t>& data) {
        vector<uint8_t> result(data.size());
        for (size_t k = 0; k < data.size(); k++) {
            result[k] = data[k] ^ nextByte();
        }
        return result;
    }
};