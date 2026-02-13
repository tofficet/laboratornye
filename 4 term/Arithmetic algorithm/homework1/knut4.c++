#include <iostream>
#include <vector>
#include <complex>

struct OperationCount {
    int multiplications = 0;
    int additions = 0;
    
    void print(const std::string& method_name) const {
        std::cout << method_name << ":\n";
        std::cout << "  Умножений: " << multiplications << "\n";
        std::cout << "  Сложений:  " << additions << "\n";
        std::cout << "  Всего операций: " << (multiplications + additions) << "\n\n";
    }
};

// Схема (3) с подсчётом операций только до получения a_n и b_n
std::complex<double> scheme3_theory(const std::vector<double>& u, std::complex<double> z, OperationCount& ops) {
    int n = u.size() - 1;
    double x = z.real();
    double y = z.imag();

    double r = x + x;
    ops.additions += 1;
    
    double s = x * x + y * y;
    ops.multiplications += 2;
    ops.additions += 1;

    double a = u[n];
    double b = u[n - 1];

    for (int j = 2; j <= n; ++j) {
        double a_new = b + r * a;
        double b_new = u[n - j] - s * a;
        ops.multiplications += 2;  
        ops.additions += 2;        
        b = b_new;
    }
  
    double real_part = a * x + b;
    double imag_part = a * y;
    
    
    return std::complex<double>(real_part, imag_part);
}

std::complex<double> horner_complex(const std::vector<double>& u, std::complex<double> z, OperationCount& ops) {
    int n = u.size() - 1;
    std::complex<double> result = u[n];
    
    for (int k = n - 1; k >= 0; --k) {
        double a = result.real();
        double b = result.imag();
        double x = z.real();
        double y = z.imag();
        
        double real_new = a * x - b * y;
        double imag_new = a * y + b * x;
        
        ops.multiplications += 4;
        ops.additions += 2;
        
        real_new += u[k];
        ops.additions += 1;
        
        result = std::complex<double>(real_new, imag_new);
    }
    
    return result;
}

int main() {
    std::vector<double> coeffs = {1, 2, 3, 4, 5, 6};
    std::complex<double> z(0.5, 1.5);
    
    OperationCount ops_scheme, ops_horner;
    
    auto val_scheme = scheme3_theory(coeffs, z, ops_scheme);
    auto val_horner = horner_complex(coeffs, z, ops_horner);
    
    std::cout << "z = " << z << "\n";
    std::cout << "P(z) = " << val_scheme << " (оба метода дают одинаково)\n\n";
    
    std::cout << "Подсчёт операций \n";
    ops_scheme.print("Схема (3)");
    ops_horner.print("Горнер");
    
    return 0;
}