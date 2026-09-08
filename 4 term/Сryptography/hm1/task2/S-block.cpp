#include <iostream>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <functional>

using namespace std;

vector<uint8_t> substituteBits(const vector<uint8_t>& input,
                                const unordered_map<uint8_t, uint8_t>& sbox) {
    vector<uint8_t> result;
    result.reserve(input.size());
    
    for (uint8_t byte : input) {
        auto it = sbox.find(byte);
        if (it != sbox.end()) {
            result.push_back(it->second);
        } else {
            result.push_back(byte);
        }
    }
    return result;
}

vector<uint8_t> substituteBits(const vector<uint8_t>& input,
                                const function<uint8_t(uint8_t)>& func) {
    vector<uint8_t> result;
    result.reserve(input.size());
    
    for (uint8_t byte : input) {
        result.push_back(func(byte));
    }
    return result;
}

void printBytes(const vector<uint8_t>& data, const string& title = "") {
    if (!title.empty()) {
        cout << title << ": ";
    }
    for (uint8_t b : data) {
        cout << hex << (int)b << " ";
    }
    cout << dec << endl;
}

void printAsText(const vector<uint8_t>& data, const string& title = "") {
    if (!title.empty()) {
        cout << title << ": ";
    }
    for (uint8_t b : data) {
        if (b >= 32 && b <= 126) {
            cout << (char)b;
        } else {
            cout << '.';
        }
    }
    cout << endl;
}