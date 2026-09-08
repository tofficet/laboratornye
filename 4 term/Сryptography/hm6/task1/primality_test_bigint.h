#pragma once

#include <cstdint>
#include <random>
#include <cmath>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using namespace boost::multiprecision;

class IPrimalityTest {
public:
    virtual ~IPrimalityTest() = default;
    
    virtual bool isPrime(const cpp_int& number, double minProbability) = 0;
};

class BasePrimalityTest : public IPrimalityTest {
private:
    static const vector<cpp_int>& getSmallPrimes() {
        static const vector<cpp_int> primes = {
            2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
            41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97
        };
        return primes;
    }
    
    bool checkSmallPrimes(const cpp_int& n) {
        const auto& primes = getSmallPrimes();
        for (const auto& p : primes) {
            if (n % p == 0) {
                return n == p;
            }
        }
        return true;
    }

    size_t calculateIterations(double minProbability, double errorPerIteration) {
        double maxError = 1.0 - minProbability;
        if (maxError <= 0) 
            return 1000;
        
        size_t k = 0;
        double currentError = 1.0;
        while (currentError > maxError && k < 1000) {
            currentError *= errorPerIteration;
            k++;
        }
        return k;
    }
    
    cpp_int random_cpp_int(const cpp_int& min, const cpp_int& max) {
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);

        cpp_int range = max - min + 1;
        cpp_int result = 0;
        int bits = boost::multiprecision::msb(range) + 1;
        int words = (bits + 63) / 64;

        do {
            result = 0;
            for (int i = 0; i < words; ++i) {
                result <<= 64;
                result |= dist(gen);
            }
            result %= range;
        } while (result < 0);

        return result + min;
    }
    
protected:
    virtual bool testIteration(const cpp_int& number, const cpp_int& base) = 0;
    
    virtual double getErrorPerIteration() const = 0;
    
public:
    bool isPrime(const cpp_int& number, double minProbability) override {
        if (number < 2) 
            return false;
        if (number == 2 || number == 3) 
            return true;
        if (number % 2 == 0) 
            return false;
        if (!checkSmallPrimes(number)) {
            return false;
        }
        
        size_t iterations = calculateIterations(minProbability, getErrorPerIteration());
        
        for (size_t i = 0; i < iterations; i++) {
            cpp_int base = random_cpp_int(2, number - 2);
            if (!testIteration(number, base)) {
                return false;
            }
        }
        
        return true;
    }
};