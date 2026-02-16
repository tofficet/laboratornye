#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

double evaluateOddPolynomial(const vector<double>& coeffs, double x) {
    if (coeffs.empty()) return 0.0;
    
    int n = coeffs.size() - 1;       
    double t = x * x;                
    double result = coeffs[n];    
    
    for (int i = n - 1; i >= 0; --i) {
        result = result * t + coeffs[i];
    }
    
    return x * result;
}

void printOddPolynomial(const vector<double>& coeffs) {
    int n = coeffs.size() - 1;
    cout << "u(x) = ";
    
    bool first = true;
    for (int i = n; i >= 0; --i) {
        double coeff = coeffs[i];
        if (abs(coeff) < 1e-10) continue;
        
        int degree = 2 * i + 1;
        
        if (!first && coeff > 0) cout << " + ";
        if (!first && coeff < 0) cout << " - ";
        if (first && coeff < 0) cout << "-";
        
        double absCoeff = abs(coeff);
        
        if (abs(absCoeff - 1.0) < 1e-10) {
            if (degree == 1) cout << "x";
            else cout << "x^" << degree;
        } else {
            if (degree == 1) cout << absCoeff << "x";
            else cout << absCoeff << "x^" << degree;
        }
        
        first = false;
    }
    cout << endl;
}

vector<double> inputOddPolynomial() {
    int m;
    cout << "Введите количество коэффициентов (для степеней x^1, x^3, x^5, ...): ";
    cin >> m;
    
    vector<double> coeffs(m);
    cout << "Введите коэффициенты от младшей нечётной степени к старшей:" << endl;
    for (int i = 0; i < m; i++) {
        int degree = 2 * i + 1;
        cout << "  коэффициент при x^" << degree << " = ";
        cin >> coeffs[i];
    }
    
    return coeffs;
}

int main() {
    cout << "==========================================================" << endl;
    cout << "ВЫЧИСЛЕНИЕ НЕЧЁТНОГО ПОЛИНОМА (СХЕМА ГОРНЕРА ДЛЯ X²)" << endl;
    cout << "==========================================================" << endl;
    cout << "Программа вычисляет значение нечётного полинома вида:" << endl;
    cout << "  u(x) = a₀x + a₁x³ + a₂x⁵ + ... + aₙx^{2n+1}" << endl;
    cout << "используя схему Горнера с заменой t = x²." << endl;
    cout << "----------------------------------------------------------" << endl;
    
    int choice;
    cout << "Выберите режим работы:" << endl;
    cout << "1 - Использовать тестовый пример (u(x) = 2x^7 - 3x^5 + 5x^3 - 7x)" << endl;
    cout << "2 - Ввести свои данные" << endl;
    cout << "Ваш выбор: ";
    cin >> choice;
    
    vector<double> coeffs;
    double x;
    
    if (choice == 1) {
        // 2x^7 - 3x^5 + 5x^3 - 7x
        // [-7, 5, -3, 2] для x^1, x^3, x^5, x^7
        coeffs = {-7.0, 5.0, -3.0, 2.0};
        x = 1.5;
        
        cout << "\n--- ТЕСТОВЫЙ ПРИМЕР ---" << endl;
        printOddPolynomial(coeffs);
        cout << "x = " << x << endl;
    } else {
        cout << "\n--- ВВОД ДАННЫХ ---" << endl;
        
        coeffs = inputOddPolynomial();
        
        cout << "Введите значение x: ";
        cin >> x;
        
        cout << "\n--- ВВЕДЁННЫЕ ДАННЫЕ ---" << endl;
        printOddPolynomial(coeffs);
        cout << "x = " << x << endl;
    }
    
    cout << "\n--- ВЫЧИСЛЕНИЕ ---" << endl;
    cout << "----------------------------------------------------------" << endl;
    
    double result = evaluateOddPolynomial(coeffs, x);
    
    cout << "Результат: u(" << x << ") = " << result << endl;
    if (choice == 1) {
        cout << "\nСхема вычисления (t = x² = " << x*x << "):" << endl;
        cout << "  b₃ = a₃ = " << coeffs[3] << endl;
        cout << "  b₂ = b₃·t + a₂ = " << coeffs[3] << "·" << x*x << " + " << coeffs[2] << " = " 
             << coeffs[3]*x*x + coeffs[2] << endl;
        cout << "  b₁ = b₂·t + a₁ = " << (coeffs[3]*x*x + coeffs[2]) << "·" << x*x << " + " << coeffs[1] << " = " 
             << ((coeffs[3]*x*x + coeffs[2])*x*x + coeffs[1]) << endl;
        cout << "  b₀ = b₁·t + a₀ = " << ((coeffs[3]*x*x + coeffs[2])*x*x + coeffs[1]) << "·" << x*x << " + " << coeffs[0] << " = " 
             << (((coeffs[3]*x*x + coeffs[2])*x*x + coeffs[1])*x*x + coeffs[0]) << endl;
        cout << "  u(x) = x · b₀ = " << x << " · " << (((coeffs[3]*x*x + coeffs[2])*x*x + coeffs[1])*x*x + coeffs[0]) 
             << " = " << result << endl;
    }
    cout << "----------------------------------------------------------" << endl;

    cout << "Схема Горнера для нечётных полиномов использует замену t = x²." << endl;
    cout << "Это сокращает количество операций умножения в 2 раза." << endl;
    cout << "Результат вычисляется как u(x) = x·P(x²), где P — вспомогательный полином." << endl;
    
    
    return 0;
}