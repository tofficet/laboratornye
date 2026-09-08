#pragma once

#include <vector>
#include <utility>
#include <string>

using namespace std;

class SternBrocotTree {
public:
    // d) путь к дроби
    static vector<bool> pathToFraction(uint64_t a, uint64_t b) {
        vector<bool> path;
        
        uint64_t leftNum = 0, leftDen = 1;   // 0/1
        uint64_t rightNum = 1, rightDen = 0; // 1/0
        
        while (true) {
            uint64_t midNum = leftNum + rightNum;
            uint64_t midDen = leftDen + rightDen;
            
            if (midNum == a && midDen == b) {
                break;
            }
            
            if (a * midDen < b * midNum) {  // a/b < midNum/minDen
                path.push_back(false);  // L = 0
                rightNum = midNum;
                rightDen = midDen;
            } else {
                path.push_back(true);  // R = 1
                leftNum = midNum;
                leftDen = midDen;
            }
        }
        
        return path;
    }
    
    // e) дробь по пути
    static pair<uint64_t, uint64_t> fractionByPath(const vector<bool>& path) {
        uint64_t leftNum = 0, leftDen = 1;
        uint64_t rightNum = 1, rightDen = 0;
        
        for (bool bit : path) {
            uint64_t midNum = leftNum + rightNum;
            uint64_t midDen = leftDen + rightDen;
            
            if (!bit) {
                rightNum = midNum;
                rightDen = midDen;
            } else {
                leftNum = midNum;
                leftDen = midDen;
            }
        }
        
        return {leftNum + rightNum, leftDen + rightDen};
    }
    
    // f) все подходящие дроби по пути
    static vector<pair<uint64_t, uint64_t>> allFractionsByPath(const vector<bool>& path) {
        vector<pair<uint64_t, uint64_t>> result;
        
        uint64_t leftNum = 0, leftDen = 1;
        uint64_t rightNum = 1, rightDen = 0;
        
        result.push_back({1, 1});  // корень
        
        for (bool bit : path) {
            uint64_t midNum = leftNum + rightNum;
            uint64_t midDen = leftDen + rightDen;
            
            if (!bit) {
                rightNum = midNum;
                rightDen = midDen;
            } else {
                leftNum = midNum;
                leftDen = midDen;
            }
            result.push_back({leftNum + rightNum, leftDen + rightDen});
        }
        
        return result;
    }
};