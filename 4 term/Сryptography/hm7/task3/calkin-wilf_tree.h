#pragma once

#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

class CalkinWilfTree {
public:
    // d) путь к дроби
    static vector<bool> pathToFraction(uint64_t a, uint64_t b) {
        vector<bool> path;
        
        while (a != b) {
            if (a < b) {
                path.push_back(false);  // L = 0
                b -= a;
            } else {
                path.push_back(true);  // R = 1
                a -= b;
            }
        }

        reverse(path.begin(), path.end());
        
        return path;
    }
    
    // e) дробь по пути
    static pair<uint64_t, uint64_t> fractionByPath(const vector<bool>& path) {
        uint64_t a = 1, b = 1;
        
        for (bool bit : path) {
            if (!bit) {
                b += a;
            } else {
                a += b;
            }
        }
        
        return {a, b};
    }
    
};