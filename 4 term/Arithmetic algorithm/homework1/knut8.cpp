#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <iomanip>

using namespace std;

double evaluate_factorial_polynomial(const vector<double>& a, double x) {
    int n = a.size() - 1; 
    double result = a[n];
    
    for (int k = n - 1; k >= 1; --k) {
        result = result * (x - k) + a[k];
    }
    
    result = result * x + a[0];
    return result;
}

void printFactorialPolynomial(const vector<double>& a) {
    int n = a.size() - 1;
    cout << "P(x) = ";
    bool first = true;
    
    for (int i = n; i >= 0; i--) {
        if (abs(a[i]) < 1e-10) continue;
        
        if (!first && a[i] > 0) cout << " + ";
        else if (!first && a[i] < 0) cout << " - ";
        else if (a[i] < 0) cout << "-";
        
        double absCoeff = abs(a[i]);
        
        if (i == 0) {
            cout << absCoeff;
        } else if (i == 1) {
            if (abs(absCoeff - 1.0) < 1e-10) cout << "x";
            else cout << absCoeff << "x";
        } else {
            if (abs(absCoeff - 1.0) < 1e-10) {
                if (i == n) cout << "x(x-1)...(x-" << n-1 << ")";
                else cout << "x(x-1)...(x-" << i-1 << ")";
            } else {
                if (i == n) cout << absCoeff << "·x(x-1)...(x-" << n-1 << ")";
                else cout << absCoeff << "·x(x-1)...(x-" << i-1 << ")";
            }
        }
        
        first = false;
    }
    cout << endl;
}

vector<double> inputFactorialCoefficients() {
    int n;
    cout << "Введите степень факториального полинома: ";
    cin >> n;
    
    vector<double> a(n + 1, 0.0);
    cout << "Введите коэффициенты a0, a1, ..., a" << n << ":" << endl;
    cout << "(где P(x) = a0 + a1·x + a2·x(x-1) + a3·x(x-1)(x-2) + ...)" << endl;
    
    for (int i = 0; i <= n; i++) {
        cout << "  a" << i << " = ";
        cin >> a[i];
    }
    
    return a;
}

int main() {
    cout << "==========================================================" << endl;
    cout << "ВЫЧИСЛЕНИЕ ФАКТОРИАЛЬНОГО ПОЛИНОМА" << endl;
    cout << "==========================================================" << endl;
    cout << "Программа вычисляет значение факториального полинома вида:" << endl;
    cout << "  P(x) = a₀ + a₁·x + a₂·x(x-1) + a₃·x(x-1)(x-2) + ..." << endl;
    cout << "используя схему Горнера для факториального базиса." << endl;
    cout << "----------------------------------------------------------" << endl;
    
    int choice;
    cout << "Выберите режим работы:" << endl;
    cout << "1 - Использовать тестовый пример (P(x) = 4 + 3x + 2x(x-1), x = 5)" << endl;
    cout << "2 - Ввести свои данные" << endl;
    cout << "Ваш выбор: ";
    cin >> choice;
    
    vector<double> a;
    double x;
    
    if (choice == 1) {
    // P(x) = 4 + 3x + 2x(x-1)
        a = {4, 3, 2};
        x = 5.0;
        
        cout << "\n--- ТЕСТОВЫЙ ПРИМЕР ---" << endl;
        printFactorialPolynomial(a);
        cout << "x = " << x << endl;
    } else {
        cout << "\n--- ВВОД ДАННЫХ ---" << endl;
        
        a = inputFactorialCoefficients();
        
        cout << "Введите значение x: ";
        cin >> x;
        
        cout << "\n--- ВВЕДЁННЫЕ ДАННЫЕ ---" << endl;
        printFactorialPolynomial(a);
        cout << "x = " << x << endl;
    }
    
    cout << "\n--- ВЫЧИСЛЕНИЕ ---" << endl;
    cout << "----------------------------------------------------------" << endl;
    
    double val = evaluate_factorial_polynomial(a, x);
    
    cout << "Результат: P(" << x << ") = " << fixed << setprecision(4) << val << endl;
    
    cout << "Факториальный полином использует базис из падающих факториалов:" << endl;
    cout << "x, x(x-1), x(x-1)(x-2), ..." << endl;
    cout << "Схема Горнера для этого базиса аналогична обычной, но с (x - k) вместо x." << endl;
    cout << "Это эффективно при вычислении конечных разностей и интерполяции." << endl;
    
    
    return 0;
}