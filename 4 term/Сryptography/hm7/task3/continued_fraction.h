#pragma once

#include <vector>
#include <cstdint>
#include <tuple>

using namespace std;

class ContinuedFraction {
public:
    // a) цепная дробь из обыкновенной
    static vector<uint64_t> fromFraction(uint64_t a, uint64_t b) {
        vector<uint64_t> result;
        while (b != 0) {
            result.push_back(a / b);
            uint64_t remainder = a % b;
            a = b;
            b = remainder;
        }
        return result;
    }
    
    // b) обыкновенная дробь из цепной
    static pair<uint64_t, uint64_t> toFraction(const vector<uint64_t>& cf) {
        if (cf.empty()) 
            return {0, 1};
        
        uint64_t num = cf.back();
        uint64_t den = 1;
        
        for (int i = cf.size() - 2; i >= 0; i--) {
            uint64_t new_num = cf[i] * num + den;
            den = num;
            num = new_num;
        }
        
        return {num, den};
    }
    
    // c) все подходящие дроби
    static vector<pair<uint64_t, uint64_t>> suitabFractions(const vector<uint64_t>& cf) {
        vector<pair<uint64_t, uint64_t>> result;
        
        if (cf.empty()) 
            return result;
        
        uint64_t num_prev = 1;
        uint64_t den_prev = 0;
        uint64_t num_curr = cf[0];
        uint64_t den_curr = 1;
        
        result.push_back({num_curr, den_curr});
        
        for (size_t i = 1; i < cf.size(); i++) {
            uint64_t num_next = cf[i] * num_curr + num_prev;
            uint64_t den_next = cf[i] * den_curr + den_prev;
            
            result.push_back({num_next, den_next});
            
            num_prev = num_curr;
            den_prev = den_curr;
            num_curr = num_next;
            den_curr = den_next;
        }
        
        return result;
    }
};