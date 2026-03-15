#include <iostream>
#include <cmath>

using namespace std;

double phi_a(double x) {
    return pow(2, x - 1);
}

double phi_b(double x) {
    return exp(2 * x) - 1;
}

double phi_c(double x, double A) {
    return A - log(x);
}

double phi_d(double x, double a, double beta) {
    return a * exp(-x) + beta * x;
}

void analyzeA() {
    cout << "\n";
    cout << "а) x_{n+1} = 2^{x_n - 1}\n";
    
    double x0 = 1.5;
    double x = x0;
    double xPrev;
    
    cout << "Начальное приближение: x0 = " << x0 << "\n";
    cout << "Итерационный процесс:\n";
    
    for (int i = 0; i < 50; i++) {
        xPrev = x;
        x = phi_a(xPrev);
        cout << "  Итерация " << i+1 << ": x = " << x << "\n";
        
        if (fabs(x - xPrev) < 1e-10) {
            cout << "Сошлось! Корень: " << x << "\n";
            break;
        }
    }
    if (fabs(x - phi_a(x)) < 1e-10) {
        cout << "Корень найден: x = " << x << "\n";
    }
    cout << "\n";
}

void analyzeB() {
    cout << "\n";
    cout << "б) x_{n+1} = e^{2x_n} - 1\n";
    
    double x0 = 0.1;
    double x = x0;
    double xPrev;
    
    cout << "Начальное приближение: x0 = " << x0 << "\n";
    cout << "Итерационный процесс:\n";
    
    for (int i = 0; i < 50; i++) {
        xPrev = x;
        x = phi_b(xPrev);
        cout << "  Итерация " << i+1 << ": x = " << x << "\n";
        
        if (isinf(x) || isnan(x)) {
            cout << "  Процесс расходится\n";
            break;
        }
        
        if (fabs(x - xPrev) < 1e-10) {
            cout << "Сошлось! Корень: " << x << "\n";
            break;
        }
    }
    cout << "\n";
}

void analyzeC() {
    cout << "\n";
    cout << "в) x_{n+1} = A - ln x_n, A = 2.0\n";
    
    double A = 2.0;
    double x0 = 1.0;
    double x = x0;
    double xPrev;
    
    cout << "Начальное приближение: x0 = " << x0 << "\n";
    cout << "Итерационный процесс:\n";
    
    for (int i = 0; i < 50; i++) {
        if (x <= 0) {
            cout << "  Ошибка: логарифм от неположительного числа\n";
            break;
        }
        
        xPrev = x;
        x = phi_c(xPrev, A);
        cout << "  Итерация " << i+1 << ": x = " << x << "\n";
        
        if (fabs(x - xPrev) < 1e-10) {
            cout << "Сошлось! Корень: " << x << "\n";
            break;
        }
    }
    cout << "\n";
}

void analyzeD() {
    cout << "\n";
    cout << "г) x_{n+1} = a·e^{-x_n} + β·x_n, a = 1.0, β = 0.5\n";
    
    double a = 1.0;
    double beta = 0.5;
    double x0 = 0.5;
    double x = x0;
    double xPrev;
    
    cout << "Начальное приближение: x0 = " << x0 << "\n";
    cout << "Итерационный процесс:\n";
    
    for (int i = 0; i < 50; i++) {
        xPrev = x;
        x = phi_d(xPrev, a, beta);
        cout << "  Итерация " << i+1 << ": x = " << x << "\n";
        
        if (fabs(x - xPrev) < 1e-10) {
            cout << "Сошлось! Корень: " << x << "\n";
            break;
        }
    }
    cout << "\n";
}

int main() {
    cout << "ИССЛЕДОВАНИЕ СХОДИМОСТИ МЕТОДА ПРОСТОЙ ИТЕРАЦИИ\n";
    
    analyzeA();
    analyzeB();
    analyzeC();
    analyzeD();
    
    return 0;
}