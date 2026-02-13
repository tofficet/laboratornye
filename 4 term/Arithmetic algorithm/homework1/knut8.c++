#include <iostream>
#include <vector>

double evaluate_factorial_polynomial(const std::vector<double>& a, double x) {
    int n = a.size() - 1; 
    double result = a[n];
    for (int k = n - 1; k >= 1; --k) {
        result = result * (x - k) + a[k];
    }
    result = result * x + a[0];
    return result;
}

int main() {
    std::vector<double> a = {4, 3, 2};
    double x = 5.0;

    double val = evaluate_factorial_polynomial(a, x);
    std::cout << "P(" << x << ") = " << val << std::endl;

    return 0;
}