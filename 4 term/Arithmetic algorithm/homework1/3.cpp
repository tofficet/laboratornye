#include <iostream>
#include <vector>
#include <cmath>

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

void printShiftedForm(const vector<double>& coeffs, double shift, const string& var = "x") {
    bool first = true;
    for (int i = coeffs.size() - 1; i >= 0; i--) {
        if (abs(coeffs[i]) < 1e-10) continue;
        
        if (!first && coeffs[i] > 0) cout << " + ";
        else if (!first && coeffs[i] < 0) cout << " - ";
        else if (coeffs[i] < 0) cout << "-";
        
        if (i == 0) cout << abs(coeffs[i]);
        else if (i == 1) {
            if (abs(abs(coeffs[i]) - 1.0) < 1e-10) {
                if (shift == 0) cout << var;
                else cout << "(" << var;
                if (shift > 0) cout << " - " << shift << ")";
                else if (shift < 0) cout << " + " << -shift << ")";
                else cout << ")";
            }
            else {
                cout << abs(coeffs[i]) << "*";
                if (shift == 0) cout << var;
                else cout << "(" << var;
                if (shift > 0) cout << " - " << shift << ")";
                else if (shift < 0) cout << " + " << -shift << ")";
                else cout << ")";
            }
        }
        else {
            if (abs(abs(coeffs[i]) - 1.0) < 1e-10) {
                if (shift == 0) cout << var << "^" << i;
                else cout << "(" << var;
                if (shift > 0) cout << " - " << shift << ")^" << i;
                else if (shift < 0) cout << " + " << -shift << ")^" << i;
                else cout << ")^" << i;
            }
            else {
                cout << abs(coeffs[i]) << "*";
                if (shift == 0) cout << var << "^" << i;
                else cout << "(" << var;
                if (shift > 0) cout << " - " << shift << ")^" << i;
                else if (shift < 0) cout << " + " << -shift << ")^" << i;
                else cout << ")^" << i;
            }
        }
        first = false;
    }
}

//Ключевая идея: (x - a) и (x - b) связаны соотношением:
//(x - a) = (x - b) + (b - a)
//Если обозначить shift = b - a, то:
//x - a) = (x - b) + shift
vector<double> taylorFromShifted(const vector<double>& f, double a, double b) {
    int n = f.size() - 1;
    vector<double> result(n + 1, 0.0);
    
    vector<double> temp = f;
    double shift = b - a;
    
    for (int k = 0; k <= n; k++) {
        result[k] = temp[0];
        
        for (int i = 0; i < n - k; i++) {
            temp[i] = temp[i + 1] + (i + 1) * shift * temp[i + 1];
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
    cout << "ПЕРЕХОД МЕЖДУ РАЗЛОЖЕНИЯМИ ПО СТЕПЕНЯМ (x - a) И (x - B)" << endl;
    cout << "==========================================================" << endl;
    cout << "Программа преобразует разложение полинома f(x) по степеням (x - a)" << endl;
    cout << "в разложение по степеням (x - B), используя схему Горнера." << endl;
    cout << "----------------------------------------------------------" << endl;
    
    int choice;
    cout << "Выберите режим работы:" << endl;
    cout << "1 - Использовать тестовый пример (f(x) = (x-1)^2 + 3(x-1) + 4, a = 1, B = 2)" << endl;
    cout << "2 - Ввести свои данные" << endl;
    cout << "Ваш выбор: ";
    cin >> choice;
    
    vector<double> f;
    double a, B;
    
    if (choice == 1) {
        // f(x) = 4 + 3(x-1) + 1(x-1)^2
        f = {4, 3, 1};
        a = 1.0;
        B = 2.0;
        
        cout << "\n--- ТЕСТОВЫЙ ПРИМЕР ---" << endl;
        cout << "Исходное разложение: f(x) = ";
        printShiftedForm(f, a);
        cout << endl;
        cout << "a = " << a << ", B = " << B << endl;
    } else {
        cout << "\n--- ВВОД ДАННЫХ ---" << endl;
        
        int k;
        cout << "Введите степень разложения (максимальная степень (x - a)^k): ";
        cin >> k;
        
        cout << "Введите значение a: ";
        cin >> a;
        
        f.resize(k + 1);
        cout << "Введите коэффициенты от младшей степени к старшей (c0, c1, ..., c" << k << "):" << endl;
        cout << "(где f(x) = c0 + c1*(x-a) + c2*(x-a)^2 + ...)" << endl;
        for (int j = 0; j <= k; j++) {
            cout << "  c" << j << " = ";
            cin >> f[j];
        }
        
        cout << "Введите значение B: ";
        cin >> B;
        
        cout << "\n--- ВВЕДЁННЫЕ ДАННЫЕ ---" << endl;
        cout << "Исходное разложение: f(x) = ";
        printShiftedForm(f, a);
        cout << endl;
        cout << "a = " << a << ", B = " << B << endl;
    }
    
    cout << "\n--- ВЫЧИСЛЕНИЕ НОВОГО РАЗЛОЖЕНИЯ ---" << endl;
    
    vector<double> result = taylorFromShifted(f, a, B);
    
    cout << "\nРазложение по степеням (x - " << B << "):" << endl;
    cout << "f(x) = ";
    printShiftedForm(result, B);
    cout << endl;
    
    return 0;
}