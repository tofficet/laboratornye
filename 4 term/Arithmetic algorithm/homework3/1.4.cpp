#include <iostream>
#include <cmath>

using namespace std;

void analyzeA(double a, double b) {
    cout << "\nПункт а: phi(x) = c + a·sin^2x + b·cos^2x\n";
    cout << "Аналитическое условие сходимости: |a - b| < 1\n";
    
    if (fabs(a - b) < 1) {
        cout << "Метод сходится при любом начальном приближении\n";
    } else {
        cout << "Метод может не сходиться\n";
    }
}

void analyzeB(double a, double b) {
    cout << "\nПункт б: phi(x) = c + a·e^(-bx^2)\n";
    
    if (b <= 0) {
        cout << "Аналитическое условие: b > 0 для сходимости\n";
        return;
    }
    
    cout << "Аналитическое условие: |a|·√(2b/e) < 1\n";
    
    if (fabs(a) * sqrt(2 * b / M_E) < 1) {
        cout << "Метод сходится при любом начальном приближении\n";
    } else {
        cout << "Метод может не сходиться\n";
    }
}

int main() {
    double a = 0.5;
    double b = 0;
    double c = 1;
    
    cout << "ИССЛЕДОВАНИЕ СХОДИМОСТИ МЕТОДА ПРОСТОЙ ИТЕРАЦИИ\n";
    cout << "Параметры: a = " << a << ", b = " << b << ", c = " << c << endl;
    
    
    cout << "ИССЛЕДОВАНИЕ СХОДИМОСТИ МЕТОДА ПРОСТОЙ ИТЕРАЦИИ\n";
    cout << "Введите параметры a b c: ";
    
    cout << "\nПараметры: a = " << a << ", b = " << b << ", c = " << c << endl;
    
    analyzeA(a, b);
    analyzeB(a, b);
    
    cout << "\nВывод: c не влияет на сходимость\n";
    
    return 0;
}