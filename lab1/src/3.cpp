#include <iostream>
#include <cmath>

class complex {
private:
    double mn_value;
    double ds_value;

public:
    complex(double mn_value = 0, double ds_value = 0);
    complex sum(const complex& other) const;
    complex vic(const complex& other) const;
    complex umn(const complex& other) const;
    complex del(const complex& other) const;
    double abs() const;
    double tan() const;

    friend std::ostream& operator<<(std::ostream &out, const complex& c);
    friend std::istream& operator>>(std::istream &in, complex& c);
};

complex::complex(double mn_value, double ds_value) {
    this->mn_value = mn_value;
    this->ds_value = ds_value;
}


complex complex::sum(const complex& other) const {
    return complex(mn_value + other.mn_value, ds_value + other.ds_value);
}

complex complex::vic(const complex& other) const {
    return complex(mn_value - other.mn_value, ds_value - other.ds_value);
}

complex complex::umn(const complex& other) const {
    return complex(mn_value * other.mn_value - ds_value * other.ds_value,
                   mn_value * other.ds_value + ds_value * other.mn_value);
}

complex complex::del(const complex& other) const {
    double denominator = other.mn_value * other.mn_value + other.ds_value * other.ds_value;
    return complex((mn_value * other.mn_value + ds_value * other.ds_value) / denominator,
                   (ds_value * other.mn_value - mn_value * other.ds_value) / denominator);
}

double complex::abs() const {
    return std::sqrt(mn_value * mn_value + ds_value * ds_value);
}

double complex::tan() const {
    return std::atan2(ds_value, mn_value);
}

std::ostream& operator<<(std::ostream &out, const complex& c) {
    out << c.mn_value;
    if (c.ds_value < 0) {
        out << " - " << -c.ds_value << "i";
    } else if (c.ds_value > 0) {
        out << " + " << c.ds_value << "i";
    }
    return out;
}

std::istream& operator>>(std::istream &in, complex& c) {
    in >> c.mn_value >> c.ds_value;
    return in;
}

int main() {
    complex ob1(2, 1);
    complex ob2(2, -1);

    std::cout << "ob1 = " << ob1 << std::endl;
    std::cout << "ob2 = " << ob2 << std::endl;

    complex result = ob1.sum(ob2);
    std::cout << "ob1 + ob2 = " << result << std::endl;

    complex result1 = ob1.vic(ob2);
    std::cout << "ob1 - ob2 = " << result1 << std::endl;

    complex result2 = ob1.umn(ob2);
    std::cout << "ob1 * ob2 = " << result2 << std::endl;

    complex result3 = ob1.del(ob2);
    std::cout << "ob1 / ob2 = " << result3 << std::endl;

    double result4 = ob1.abs();
    std::cout << "|ob1| = " << result4 << std::endl;

    double result5 = ob1.tan();
    std::cout << "arg(ob1) = " << result5 << std::endl;

    return 0;
}