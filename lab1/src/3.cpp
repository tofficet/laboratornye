#include <iostream>
#include <cmath>
#include <limits>

class Complex {
private:
    double real;
    double imag;
    static const double epsilon;  // Заменили constexpr на const

public:
    Complex(double real = 0.0, double imag = 0.0);
    Complex add(const Complex& other) const;
    Complex subtract(const Complex& other) const;
    Complex multiply(const Complex& other) const;
    Complex divide(const Complex& other) const;
    double magnitude() const;
    double argument() const;
    bool isZero() const;

    friend std::ostream& operator<<(std::ostream &out, const Complex& c);
    friend std::istream& operator>>(std::istream &in, Complex& c);
    friend bool operator==(const Complex& a, const Complex& b);
};

// Инициализация статической константы
const double Complex::epsilon = 1e-9;

Complex::Complex(double real, double imag) : real(real), imag(imag) {}

Complex Complex::add(const Complex& other) const {
    return Complex(real + other.real, imag + other.imag);
}

Complex Complex::subtract(const Complex& other) const {
    return Complex(real - other.real, imag - other.imag);
}

Complex Complex::multiply(const Complex& other) const {
    return Complex(real * other.real - imag * other.imag,
                   real * other.imag + imag * other.real);
}

Complex Complex::divide(const Complex& other) const {
    double denominator = other.real * other.real + other.imag * other.imag;
    return Complex((real * other.real + imag * other.imag) / denominator,
                   (imag * other.real - real * other.imag) / denominator);
}

double Complex::magnitude() const {
    return std::sqrt(real * real + imag * imag);
}

double Complex::argument() const {
    return std::atan2(imag, real);
}

bool Complex::isZero() const {
    return std::abs(real) < epsilon && std::abs(imag) < epsilon;
}

std::ostream& operator<<(std::ostream &out, const Complex& c) {
    if (c.isZero()) {
        out << "0";
        return out;
    }

    bool realPrinted = false;
    
    if (std::abs(c.real) >= Complex::epsilon) {
        out << c.real;
        realPrinted = true;
    }

    if (std::abs(c.imag) >= Complex::epsilon) {
        if (realPrinted && c.imag > 0) {
            out << " + ";
        } else if (realPrinted && c.imag < 0) {
            out << " - ";
        }
        
        if (!realPrinted || std::abs(c.imag) >= Complex::epsilon) {
            if (!realPrinted) {
                if (c.imag < 0) out << "-";
            }
            if (std::abs(std::abs(c.imag) - 1.0) >= Complex::epsilon) {
                out << std::abs(c.imag);
            }
            out << "i";
        }
    } else if (!realPrinted) {
        out << "0";
    }

    return out;
}

std::istream& operator>>(std::istream &in, Complex& c) {
    in >> c.real >> c.imag;
    return in;
}

bool operator==(const Complex& a, const Complex& b) {
    return std::abs(a.real - b.real) < Complex::epsilon && 
           std::abs(a.imag - b.imag) < Complex::epsilon;
}

int main() {
    Complex c1(2.0, 1.0);
    Complex c2(2.0, -1.0);
    Complex zero(0.0, 0.0);

    std::cout << "c1 = " << c1 << std::endl;
    std::cout << "c2 = " << c2 << std::endl;
    std::cout << "zero = " << zero << std::endl;

    std::cout << "c1 + c2 = " << c1.add(c2) << std::endl;
    std::cout << "c1 - c2 = " << c1.subtract(c2) << std::endl;
    std::cout << "c1 * c2 = " << c1.multiply(c2) << std::endl;
    std::cout << "c1 / c2 = " << c1.divide(c2) << std::endl;

    std::cout << "|c1| = " << c1.magnitude() << std::endl;
    std::cout << "arg(c1) = " << c1.argument() << std::endl;

    std::cout << "Is c1 zero? " << (c1.isZero() ? "Yes" : "No") << std::endl;
    std::cout << "Is zero zero? " << (zero.isZero() ? "Yes" : "No") << std::endl;
    std::cout << "c1 == c2? " << (c1 == c2 ? "Yes" : "No") << std::endl;

    return 0;
}