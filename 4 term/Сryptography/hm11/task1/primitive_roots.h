#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

int gcd(int a, int b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

long long powmod(long long a, long long b, long long mod) {
    long long res = 1;
    long long base = a % mod;
    while (b) {
        if (b & 1) 
            res = (res * base) % mod;
        base = (base * base) % mod;
        b >>= 1;
    }
    return res;
}

int phi(int n) {
    int result = n;
    int temp = n;
    for (int p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) 
                temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1) 
        result -= result / temp;
    return result;
}

vector<int> primeFactors(int n) {
    vector<int> factors;
    int temp = n;
    for (int p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            factors.push_back(p);
            while (temp % p == 0) 
                temp /= p;
        }
    }
    if (temp > 1) 
        factors.push_back(temp);
    return factors;
}

bool primitiveRootExists(int n) {
    if (n == 2 || n == 4) 
        return true;
    
    int temp = n;
    if (n % 2 == 0) 
        temp /= 2;

    if (temp % 2 == 0)
            return false;

    if (temp == 1)
        return false;
    
    int p = -1;
    for (int i = 3; i * i <= temp; i+=2) {
        if (temp % i == 0) {
            p = i;
            break;
        }
    }
    
    if (p == -1) 
        return true;
    
    while (temp % p == 0) 
        temp /= p;
    
    return temp == 1;
}

int findOnePrimitiveRoot(int n, int phi_n, const vector<int>& factors) {
    for (int g = 2; g < n; g++) {
        if (gcd(g, n) != 1) 
            continue;
        
        bool ok = true;
        for (int q : factors) {
            if (powmod(g, phi_n / q, n) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) 
            return g;
    }
    return -1;
}

vector<int> primitiveRoots(int n) {
    vector<int> result;
    
    if (n < 2) 
        return result;

    if (n == 2) {
        result.push_back(1);
        return result;
    }

    if (!primitiveRootExists(n)) 
        return result;
    
    int phi_n = phi(n);
    vector<int> factors = primeFactors(phi_n);
    
    int g = findOnePrimitiveRoot(n, phi_n, factors);
    if (g == -1) 
        return result;
    
    for (int i = 1; i < phi_n; i++) {
        if (gcd(i, phi_n) == 1) {
            result.push_back(powmod(g, i, n));
        }
    }
    
    sort(result.begin(), result.end());
    return result;
}