#ifndef PUKSRENJK_INT_WRAPPER_H
#define PUKSRENJK_INT_WRAPPER_H

#include <iostream>

class int_wrapper final {
private:
    int* _value_ptr;

public:
    explicit int_wrapper(int value = 0) : _value_ptr(new int(value)) {}
    
    ~int_wrapper() {
        delete _value_ptr;
    }

    int_wrapper(const int_wrapper& other) : _value_ptr(new int(*other._value_ptr)) {}
    
    int_wrapper& operator=(const int_wrapper& other) {
        if (this != &other) {
            int* temp = new int(*other._value_ptr);
            delete _value_ptr;
            _value_ptr = temp;
        }
        return *this;
    }

    int_wrapper(int_wrapper&& other) noexcept : _value_ptr(other._value_ptr) {
        other._value_ptr = nullptr;
    }
    
    int_wrapper& operator=(int_wrapper&& other) noexcept {
        if (this != &other) {
            delete _value_ptr;
            _value_ptr = other._value_ptr;
            other._value_ptr = nullptr;
        }
        return *this;
    }

    int_wrapper& operator+=(const int_wrapper& rhs) {
        *_value_ptr += *rhs._value_ptr;
        return *this;
    }
    
    int_wrapper& operator-=(const int_wrapper& rhs) {
        *_value_ptr -= *rhs._value_ptr;
        return *this;
    }
    
    int_wrapper& operator*=(const int_wrapper& rhs) {
        *_value_ptr *= *rhs._value_ptr;
        return *this;
    }
    
    int_wrapper& operator/=(const int_wrapper& rhs) {
        *_value_ptr /= *rhs._value_ptr;
        return *this;
    }
    
    int_wrapper& operator%=(const int_wrapper& rhs) {
        *_value_ptr %= *rhs._value_ptr;
        return *this;
    }

    

    int_wrapper operator+(const int_wrapper& rhs) const {
        return int_wrapper(*_value_ptr + *rhs._value_ptr);
    }
    
    int_wrapper operator-(const int_wrapper& rhs) const {
        return int_wrapper(*_value_ptr - *rhs._value_ptr);
    }
    
    int_wrapper operator*(const int_wrapper& rhs) const {
        return int_wrapper(*_value_ptr * *rhs._value_ptr);
    }
    
    int_wrapper operator/(const int_wrapper& rhs) const {
        return int_wrapper(*_value_ptr / *rhs._value_ptr);
    }
    
    int_wrapper operator%(const int_wrapper& rhs) const {
        return int_wrapper(*_value_ptr % *rhs._value_ptr);
    }

    int_wrapper& operator++() {
        ++(*_value_ptr);
        return *this;
    }
    
    int_wrapper operator++(int) {
        int_wrapper temp(*this);
        ++(*_value_ptr);
        return temp;
    }
    
    int_wrapper& operator--() {
        --(*_value_ptr);
        return *this;
    }
    
    int_wrapper operator--(int) {
        int_wrapper temp(*this);
        --(*_value_ptr);
        return temp;
    }

    int_wrapper& operator&=(const int_wrapper& rhs) {
        *_value_ptr &= *rhs._value_ptr;
        return *this;
    }
    
    int_wrapper& operator|=(const int_wrapper& rhs) {
        *_value_ptr |= *rhs._value_ptr;
        return *this;
    }
    
    int_wrapper& operator^=(const int_wrapper& rhs) {
        *_value_ptr ^= *rhs._value_ptr;
        return *this;
    }
    
    int_wrapper& operator<<=(const int_wrapper& rhs) {
        *_value_ptr <<= *rhs._value_ptr;
        return *this;
    }
    
    int_wrapper& operator>>=(const int_wrapper& rhs) {
        *_value_ptr >>= *rhs._value_ptr;
        return *this;
    }

    int_wrapper operator&(const int_wrapper& rhs) const {
        return int_wrapper(*_value_ptr & *rhs._value_ptr);
    }
    
    int_wrapper operator|(const int_wrapper& rhs) const {
        return int_wrapper(*_value_ptr | *rhs._value_ptr);
    }
    
    int_wrapper operator^(const int_wrapper& rhs) const {
        return int_wrapper(*_value_ptr ^ *rhs._value_ptr);
    }
    
    int_wrapper operator<<(const int_wrapper& rhs) const {
        return int_wrapper(*_value_ptr << *rhs._value_ptr);
    }
    
    int_wrapper operator>>(const int_wrapper& rhs) const {
        return int_wrapper(*_value_ptr >> *rhs._value_ptr);
    }

    int_wrapper operator+() const {
        return int_wrapper(+*_value_ptr);
    }
    
    int_wrapper operator-() const {
        return int_wrapper(-*_value_ptr);
    }
    
    int_wrapper operator~() const {
        return int_wrapper(~*_value_ptr);
    }

    bool operator==(const int_wrapper& rhs) const {
        return *_value_ptr == *rhs._value_ptr;
    }
    
    bool operator!=(const int_wrapper& rhs) const {
        return *_value_ptr != *rhs._value_ptr;
    }
    
    bool operator<(const int_wrapper& rhs) const {
        return *_value_ptr < *rhs._value_ptr;
    }
    
    bool operator<=(const int_wrapper& rhs) const {
        return *_value_ptr <= *rhs._value_ptr;
    }
    
    bool operator>(const int_wrapper& rhs) const {
        return *_value_ptr > *rhs._value_ptr;
    }
    
    bool operator>=(const int_wrapper& rhs) const {
        return *_value_ptr >= *rhs._value_ptr;
    }

    friend std::ostream& operator<<(std::ostream& os, const int_wrapper& obj) {
        return os << (obj._value_ptr ? *obj._value_ptr : 0);
    }
    
    friend std::istream& operator>>(std::istream& is, int_wrapper& obj) {
        if (obj._value_ptr) {
            is >> *obj._value_ptr;
        }
        return is;
    }

};

#endif // PUKSRENJK_INT_WRAPPER_H