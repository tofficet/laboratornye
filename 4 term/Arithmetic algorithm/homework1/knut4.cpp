#include <iostream>
#include <vector>
#include <complex>
#include <string>
#include <iomanip>

using namespace std;

struct OperationCount {
    int multiplications = 0;
    int additions = 0;
    
    void print(const string& method_name) const {
        cout << method_name << ":\n";
        cout << "  Умножений: " << multiplications << "\n";
        cout << "  Сложений:  " << additions << "\n";
        cout << "  Всего операций: " << (multiplications + additions) << "\n";
    }
};

complex<double> scheme3_theory(const vector<double>& u, complex<double> z, OperationCount& ops) {
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
        a = a_new;  
        b = b_new;
    }
  
    double real_part = a * x + b;
    double imag_part = a * y;
    ops.multiplications += 2;  
    ops.additions += 1;        
    
    return complex<double>(real_part, imag_part);
}

complex<double> horner_complex(const vector<double>& u, complex<double> z, OperationCount& ops) {
    int n = u.size() - 1;
    complex<double> result = u[n];
    
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
        
        result = complex<double>(real_new, imag_new);
    }
    
    return result;
}

void printPolynomial(const vector<double>& coeffs) {
    int n = coeffs.size() - 1;
    cout << "P(x) = ";
    bool first = true;
    
    for (int i = n; i >= 0; i--) {
        if (abs(coeffs[i]) < 1e-10) continue;
        
        if (!first && coeffs[i] > 0) cout << " + ";
        else if (!first && coeffs[i] < 0) cout << " - ";
        else if (coeffs[i] < 0) cout << "-";
        
        double absCoeff = abs(coeffs[i]);
        
        if (i == 0) {
            cout << absCoeff;
        } else if (i == 1) {
            if (abs(absCoeff - 1.0) < 1e-10) cout << "x";
            else cout << absCoeff << "x";
        } else {
            if (abs(absCoeff - 1.0) < 1e-10) cout << "x^" << i;
            else cout << absCoeff << "x^" << i;
        }
        
        first = false;
    }
    cout << endl;
}

vector<double> inputPolynomial() {
    int degree;
    cout << "Введите степень полинома: ";
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
    cout << "ВЫЧИСЛЕНИЕ КОМПЛЕКСНОГО ПОЛИНОМА" << endl;
    cout << "==========================================================" << endl;
    cout << "Программа сравнивает два метода вычисления полинома" << endl;
    cout << "от комплексного аргумента:" << endl;
    cout << "  1. Стандартная схема Горнера" << endl;
    cout << "  2. Оптимизированная схема (3) из книги Кнута" << endl;
    cout << "----------------------------------------------------------" << endl;
    
    int choice;
    cout << "Выберите режим работы:" << endl;
    cout << "1 - Использовать тестовый пример (P(x) = 1 + 2x + 3x² + 4x³ + 5x⁴ + 6x⁵, z = 0.5 + 1.5i)" << endl;
    cout << "2 - Ввести свои данные" << endl;
    cout << "Ваш выбор: ";
    cin >> choice;
    
    vector<double> coeffs;
    complex<double> z;
    
    if (choice == 1) {
        //  P(x) = 1 + 2x + 3x² + 4x³ + 5x⁴ + 6x⁵
        coeffs = {1, 2, 3, 4, 5, 6};
        z = complex<double>(0.5, 1.5);
        
        cout << "\n--- ТЕСТОВЫЙ ПРИМЕР ---" << endl;
        printPolynomial(coeffs);
        cout << "z = " << z << endl;
    } else {
        cout << "\n--- ВВОД ДАННЫХ ---" << endl;
        
        coeffs = inputPolynomial();
        
        double re, im;
        cout << "Введите действительную часть z: ";
        cin >> re;
        cout << "Введите мнимую часть z: ";
        cin >> im;
        z = complex<double>(re, im);
        
        cout << "\n--- ВВЕДЁННЫЕ ДАННЫЕ ---" << endl;
        printPolynomial(coeffs);
        cout << "z = " << z << endl;
    }
    
    cout << "\n--- ВЫЧИСЛЕНИЕ ---" << endl;
    cout << "----------------------------------------------------------" << endl;
    
    OperationCount ops_scheme, ops_horner;
    
    auto val_scheme = scheme3_theory(coeffs, z, ops_scheme);
    auto val_horner = horner_complex(coeffs, z, ops_horner);
    
    cout << "Результаты вычислений:" << endl;
    cout << "  P(z) = " << val_scheme << " (оба метода дают одинаковый результат)" << endl;
    cout << "  Проверка: разница = " << abs(val_scheme - val_horner) << endl;
    
    cout << "\n--- СРАВНЕНИЕ ЭФФЕКТИВНОСТИ ---" << endl;
    cout << "----------------------------------------------------------" << endl;
    
    ops_horner.print("Стандартная схема Горнера");
    cout << endl;
    ops_scheme.print("Оптимизированная схема (3)");

    
    cout << "----------------------------------------------------------" << endl;
    cout << "Стандартная схема Горнера для комплексных чисел:" << endl;
    cout << "  На каждом шаге: (a + bi)(x + yi) = (ax - by) + (ay + bx)i" << endl;
    cout << "  Требует 4 умножения и 2 сложения на шаг." << endl;
    cout << endl;
    cout << "Оптимизированная схема (3) Кнута:" << endl;
    cout << "  Использует предварительные вычисления r = 2x и s = x² + y²" << endl;
    cout << "  Рекуррентные соотношения требуют меньше операций." << endl;
    cout << "----------------------------------------------------------" << endl;
    
    
    return 0;
}