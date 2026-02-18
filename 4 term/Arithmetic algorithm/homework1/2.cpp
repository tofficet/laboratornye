#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>

using namespace std;

void printPolynomial(const vector<double>& coeffs, const string& name) {
    cout << name << "(x) = ";
    bool first = true;
    for (int i = coeffs.size() - 1; i >= 0; i--) {
        if (abs(coeffs[i]) < 1e-10) continue;
        
        if (!first && coeffs[i] > 0) cout << " + ";
        else if (!first && coeffs[i] < 0) cout << " - ";
        else if (coeffs[i] < 0) cout << "-";
        
        if (i == 0) cout << abs(coeffs[i]);
        else if (i == 1) {
            if (abs(abs(coeffs[i]) - 1.0) < 1e-10) cout << "x";
            else cout << abs(coeffs[i]) << "x";
        }
        else {
            if (abs(abs(coeffs[i]) - 1.0) < 1e-10) cout << "x^" << i;
            else cout << abs(coeffs[i]) << "x^" << i;
        }
        first = false;
    }
    if (first) cout << "0";
    cout << endl;
}

void printTaylorForm(const vector<double>& coeffs, double a) {
    bool first = true;
    for (int i = coeffs.size() - 1; i >= 0; i--) {
        if (abs(coeffs[i]) < 1e-10) continue;
        
        if (!first && coeffs[i] > 0) cout << " + ";
        else if (!first && coeffs[i] < 0) cout << " - ";
        else if (coeffs[i] < 0) cout << "-";
        
        if (i == 0) cout << abs(coeffs[i]);
        else if (i == 1) {
            if (abs(abs(coeffs[i]) - 1.0) < 1e-10) {
                if (a == 0) cout << "(x)";
                else cout << "(x";
                if (a > 0) cout << " - " << a << ")";
                else if (a < 0) cout << " + " << -a << ")";
                else cout << ")";
            }
            else {
                cout << abs(coeffs[i]) << "*";
                if (a == 0) cout << "(x)";
                else cout << "(x";
                if (a > 0) cout << " - " << a << ")";
                else if (a < 0) cout << " + " << -a << ")";
                else cout << ")";
            }
        }
        else {
            if (abs(abs(coeffs[i]) - 1.0) < 1e-10) {
                if (a == 0) cout << "(x)^" << i;
                else cout << "(x";
                if (a > 0) cout << " - " << a << ")^" << i;
                else if (a < 0) cout << " + " << -a << ")^" << i;
                else cout << ")^" << i;
            }
            else {
                cout << abs(coeffs[i]) << "*";
                if (a == 0) cout << "(x)^" << i;
                else cout << "(x";
                if (a > 0) cout << " - " << a << ")^" << i;
                else if (a < 0) cout << " + " << -a << ")^" << i;
                else cout << ")^" << i;
            }
        }
        first = false;
    }
    cout << endl;
}

vector<double> taylorExpansion(const vector<double>& f, double a) {
    int n = f.size() - 1;
    vector<double> result(n + 1, 0.0);
    
    vector<double> temp = f;
    
    for (int k = 0; k <= n; k++) {
        result[k] = temp[0];
        
        for (int i = 0; i < n - k; i++) {
            temp[i] = temp[i + 1] + (i + 1) * a * temp[i + 1];
        }
        temp.pop_back();
    }
    
    return result;
}

vector<double> inputPolynomial(const string& name) {
    int degree;
    cout << "Введите степень полинома " << name << ": ";
    cin >> degree;
    
    vector<double> coeffs(degree + 1, 0.0);
    cout << "Введите коэффициенты от младшей степени к старшей (a0, a1, ..., a" << degree << "):" << endl;
    for (int i = 0; i <= degree; i++) {
        cout << "  a" << i << " = ";
        cin >> coeffs[i];
    }
    
    return coeffs;
}

int main() {
    cout << "==========================================================" << endl;
    cout << "РАЗЛОЖЕНИЕ МНОГОЧЛЕНА ПО СТЕПЕНЯМ (x - a)^k" << endl;
    cout << "==========================================================" << endl;
    cout << "Программа представляет полином f(x) в виде линейной комбинации" << endl;
    cout << "степеней (x - a)^k, используя схему Горнера." << endl;
    cout << "----------------------------------------------------------" << endl;
    
    int choice;
    cout << "Выберите режим работы:" << endl;
    cout << "1 - Использовать тестовый пример (f(x) = x^2 + 2x + 1, a = 1)" << endl;
    cout << "2 - Ввести свои данные" << endl;
    cout << "Ваш выбор: ";
    cin >> choice;
    
    vector<double> f;
    double a;
    
    if (choice == 1) {
        f = {1, 2, 1}; 
        a = 1.0;
        
        cout << "\n--- ТЕСТОВЫЙ ПРИМЕР ---" << endl;
        printPolynomial(f, "f");
        cout << "a = " << a << endl;
    } else {
        cout << "\n--- ВВОД ДАННЫХ ---" << endl;
        
        f = inputPolynomial("f");
        
        cout << "\nВведите точку a для разложения (x - a): ";
        cin >> a;
        
        cout << "\n--- ВВЕДЁННЫЕ ДАННЫЕ ---" << endl;
        printPolynomial(f, "f");
        cout << "a = " << a << endl;
    }
    
    cout << "\n--- ВЫЧИСЛЕНИЕ РАЗЛОЖЕНИЯ ---" << endl;
    
    vector<double> taylor = taylorExpansion(f, a);
    
    cout << "\nРазложение по степеням (x - " << a << ")^k:" << endl;
    cout << "f(x) = ";
    printTaylorForm(taylor, a);

    return 0;
}