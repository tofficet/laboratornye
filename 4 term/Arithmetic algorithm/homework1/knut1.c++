#include <iostream>
#include <vector>
#include <cmath>


double evaluateOddPolynomial(const std::vector<double>& coeffs, double x) {
    if (coeffs.empty()) return 0.0;
    
    int n = coeffs.size() - 1;        // степень полинома от t = x²
    double t = x * x;                
    double result = coeffs[n];        // начинаем с u_{2n+1}
    
    for (int i = n - 1; i >= 0; --i) {
        result = result * t + coeffs[i];
    }
    
    return x * result;
}


void printPolynomial(const std::vector<double>& coeffs) {
    int n = coeffs.size() - 1;
    std::cout << "u(x) = ";
    
    bool first = true;
    for (int i = n; i >= 0; --i) {
        double coeff = coeffs[i];
        if (std::abs(coeff) < 1e-10) continue;
        
        int degree = 2 * i + 1;
        
        if (!first && coeff > 0) std::cout << " + ";
        if (!first && coeff < 0) std::cout << " - ";
        if (first && coeff < 0) std::cout << "-";
        
        double absCoeff = std::abs(coeff);
        
        if (std::abs(absCoeff - 1.0) < 1e-10) {
            if (degree == 1) std::cout << "x";
            else std::cout << "x^" << degree;
        } else {
            if (degree == 1) std::cout << absCoeff << "x";
            else std::cout << absCoeff << "x^" << degree;
        }
        
        first = false;
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "ВЫЧИСЛЕНИЕ НЕЧЁТНОГО ПОЛИНОМА (СХЕМА ГОРНЕРА ДЛЯ X²)\n\n";
    
    // Пример: 2x⁷ - 3x⁵ + 5x³ - 7x
    std::vector<double> coeffs = {-7.0, 5.0, -3.0, 2.0};
    
    printPolynomial(coeffs);
    
    double x = 1.5;
    double result = evaluateOddPolynomial(coeffs, x);
    
    std::cout << "x = " << x << "\n";
    std::cout << "u(x) = " << result << "\n\n";
    
    // Детали вычислений
    std::cout << "Детали:\n";
    std::cout << "t = x² = " << x * x << "\n";
    
    double t = x * x;
    double step = coeffs[3];
    std::cout << "b₂ = " << step << "\n";
    
    step = step * t + coeffs[2];
    std::cout << "b₁ = " << step << "\n";
    
    step = step * t + coeffs[1];
    std::cout << "b₀ = " << step << "\n";
    
    step = step * t + coeffs[0];
    std::cout << "P(t) = " << step << "\n";
    std::cout << "u(x) = x·P(t) = " << x * step << "\n";
    
    return 0;
}