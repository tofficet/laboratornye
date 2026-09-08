#pragma once

#include <iostream>
#include <vector>
#include <cstdint>
#include <stdexcept>

using namespace std;

class PolynomialGF2;
PolynomialGF2 polynomialGCD(PolynomialGF2 a, PolynomialGF2 b);

class PolynomialGF2 {
private:
    uint32_t m_coeffs;   // коэффициенты (биты)
    int m_degree;        // степень полинома
    
    // обновление степени
    void updateDegree() {
        if (m_coeffs == 0) {
            m_degree = -1;
        } else {
            m_degree = 31 - __builtin_clz(m_coeffs);
        }
    }
    
public:
    PolynomialGF2() : m_coeffs(0), m_degree(-1) {}
    
    PolynomialGF2(uint32_t coeffs) : m_coeffs(coeffs) {
        updateDegree();
    }
    
    PolynomialGF2(const vector<int>& coeffs) : m_coeffs(0) {
        for (int i = 0; i < (int)coeffs.size(); i++) {
            if (coeffs[i] != 0) {
                m_coeffs |= (1 << i);
            }
        }
        updateDegree();
    }
    
    uint32_t getCoeffs() const { return m_coeffs; }
    int getDegree() const { return m_degree; }
    
    int getCoeff(int pos) const {
        return (m_coeffs >> pos) & 1;
    }
    
    PolynomialGF2 operator+(const PolynomialGF2& other) const {
        return PolynomialGF2(m_coeffs ^ other.m_coeffs);
    }
    
    PolynomialGF2& operator+=(const PolynomialGF2& other) {
        m_coeffs ^= other.m_coeffs;
        updateDegree();
        return *this;
    }
    
    PolynomialGF2 operator*(const PolynomialGF2& other) const {
        uint32_t result = 0;
        uint32_t a = m_coeffs;
        uint32_t b = other.m_coeffs;
        
        for (int i = 0; i <= m_degree; i++) {
            if (a & (1 << i)) {
                result ^= (b << i);
            }
        }
        
        return PolynomialGF2(result);
    }
    
    PolynomialGF2 mulMod(const PolynomialGF2& other, const PolynomialGF2& modulus) const {
        // умножаем
        PolynomialGF2 product = (*this) * other;
        
        // делим по модулю
        PolynomialGF2 result = product;
        int modDegree = modulus.getDegree();
        
        while (result.getDegree() >= modDegree) {
            int shift = result.getDegree() - modDegree;
            uint32_t divisor = modulus.m_coeffs << shift;
            result.m_coeffs ^= divisor;
            result.updateDegree();
        }
        
        return result;
    }
    
    // расширенный алгоритм Евклида для полиномов
    PolynomialGF2 inverseMod(const PolynomialGF2& modulus) const {
        if (m_coeffs == 0) {
            return PolynomialGF2(0);  // p(x)=0 → p⁻¹(x)=0
        }
        
        PolynomialGF2 a = modulus;
        PolynomialGF2 b = *this;
        PolynomialGF2 u0(1), u1(0);
        PolynomialGF2 v0(0), v1(1);
        
        while (b.m_coeffs != 0) {
            // Деление a на b
            PolynomialGF2 q;
            PolynomialGF2 r;
            
            if (a.getDegree() < b.getDegree()) {
                q = PolynomialGF2(0);
                r = a;
            } else {
                int shift = a.getDegree() - b.getDegree();
                q = PolynomialGF2(1 << shift);
                r = a + (b * q);
            }
            
            // обновление
            PolynomialGF2 u = u0 + (u1 * q);
            PolynomialGF2 v = v0 + (v1 * q);
            
            a = b;
            b = r;
            u0 = u1;
            u1 = u;
            v0 = v1;
            v1 = v;
        }
        
        // НОД должен быть 1 (иначе обратного нет)
        if (a.m_coeffs != 1) {
            throw runtime_error("Polynomial is not invertible modulo given modulus");
        }
        
        return u0;
    }
    
    bool isIrreducible() const {
        if (m_degree == 1) 
            return true;
        if (m_degree <= 0) 
            return false;
        
        // проверяем делимость на все полиномы степени <= degree/2
        PolynomialGF2 x(2);  // полином x (01 10)
        PolynomialGF2 temp = x;
        
        // тест: x^(2^k) mod p
        for (int i = 1; i <= m_degree / 2; i++) {
            temp = temp.mulMod(temp, *this);
            PolynomialGF2 diff = temp + x;
            if (diff.getDegree() >= 0) {
                PolynomialGF2 gcd = polynomialGCD(*this, diff);
                if (gcd.getDegree() > 0 && gcd.getDegree() < m_degree) {
                    return false;
                }
            }
        }
        
        return true;
    }
    
    void print() const {
        if (m_coeffs == 0) {
            cout << "0";
            return;
        }
        
        bool first = true;
        for (int i = m_degree; i >= 0; i--) {
            if (getCoeff(i)) {
                if (!first) 
                    cout << " + ";
                if (i == 0) 
                    cout << "1";
                else if (i == 1) 
                    cout << "x";
                else 
                    cout << "x^" << i;
                first = false;
            }
        }
    }
    
    void printHex() const {
        cout << "0x" << hex << m_coeffs << dec;
    }
};

PolynomialGF2 polynomialGCD(PolynomialGF2 a, PolynomialGF2 b) {
    while (b.getCoeffs() != 0) {
        PolynomialGF2 temp = b;
        
        // Деление a на b
        if (a.getDegree() < b.getDegree()) {
            b = a;
            a = temp;
        } else {
            int shift = a.getDegree() - b.getDegree();
            PolynomialGF2 q(1 << shift);
            b = a + (b * q);
            a = temp;
        }
    }
    return a;
}