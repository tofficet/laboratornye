#pragma once

#include <cstdint>
#include <random>
#include <cmath>

using namespace std;

class IPrimalityTest {
public:
    virtual ~IPrimalityTest() = default;
    
    virtual bool isPrime(uint64_t number, double minProbability) = 0;
};

class BasePrimalityTest : public IPrimalityTest {
private:
    static const vector<uint64_t>& getSmallPrimes() {
        static const vector<uint64_t> primes = {
            2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
            41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97
        };
        return primes;
    }
    
    bool checkSmallPrimes(uint64_t n) {
        const auto& primes = getSmallPrimes();
        for (uint64_t p : primes) {
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
    
protected:
    virtual bool testIteration(uint64_t number, uint64_t base) = 0;
    
    virtual double getErrorPerIteration() const = 0;
    
public:
    bool isPrime(uint64_t number, double minProbability) override {
        if (number < 2) 
            return false;
        if (number == 2 || number == 3) 
            return true;
        if (number % 2 == 0) 
            return false;
        if (!checkSmallPrimes(number)) {
            return false;
        }
        
        // вычисляем количество итераций
        size_t iterations = calculateIterations(minProbability, getErrorPerIteration());
        
        // генератор случайных оснований
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<uint64_t> dist(2, number - 1);
        
        // итерации
        for (size_t i = 0; i < iterations; i++) {
            uint64_t base = dist(gen);
            if (!testIteration(number, base)) {
                return false;
            }
        }
        
        return true;
    }
};