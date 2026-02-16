#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

vector<double> shift_polynomial(const vector<double>& coeffs, double x0) {
    int n = coeffs.size() - 1;
    vector<double> p = { coeffs[n] };
    
    for (int i = n - 1; i >= 0; --i) {
        vector<double> new_p(p.size() + 1, 0.0);
        
        for (int j = 0; j < p.size(); ++j) {
            new_p[j] += p[j] * x0;
            new_p[j + 1] += p[j];
        }
        
        new_p[0] += coeffs[i];
        p = new_p;
    }
    return p;
}

void printPolynomial(const vector<double>& coeffs, char var, const string& name = "") {
    if (!name.empty()) {
        cout << name << "(" << var << ") = ";
    } else {
        cout << (var == 'x' ? "u(x) = " : "v(y) = ");
    }
    
    bool first = true;
    int n = coeffs.size() - 1;
    for (int i = n; i >= 0; --i) {
        double coeff = coeffs[i];
        if (abs(coeff) < 1e-10) continue;
        
        if (!first && coeff > 0) cout << " + ";
        if (!first && coeff < 0) cout << " - ";
        if (first && coeff < 0) cout << "-";
        
        double absCoeff = abs(coeff);
        
        if (abs(absCoeff - 1.0) < 1e-10) {
            if (i == 0) cout << "1";
            else cout << var << (i == 1 ? "" : "^" + to_string(i));
        } else {
            if (i == 0) cout << absCoeff;
            else cout << absCoeff << var << (i == 1 ? "" : "^" + to_string(i));
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
    cout << "ВЫЧИСЛЕНИЕ СДВИГА ПОЛИНОМА (СХЕМА ГОРНЕРА)" << endl;
    cout << "==========================================================" << endl;
    cout << "Программа выполняет сдвиг полинома: при подстановке x = y + x0" << endl;
    cout << "получает коэффициенты нового полинома от переменной y." << endl;
    cout << "----------------------------------------------------------" << endl;
    
    int choice;
    cout << "Выберите режим работы:" << endl;
    cout << "1 - Использовать тестовый пример (u(x) = 2x³ + 5x² - 3x - 7, сдвиг x = y + 1)" << endl;
    cout << "2 - Ввести свои данные" << endl;
    cout << "Ваш выбор: ";
    cin >> choice;
    
    vector<double> coeffs;
    double x0;
    
    if (choice == 1) {
        //2x³ + 5x² - 3x - 7
        coeffs = {-7.0, -3.0, 5.0, 2.0};
        x0 = 1.0;
        
        cout << "\n--- ТЕСТОВЫЙ ПРИМЕР ---" << endl;
        printPolynomial(coeffs, 'x', "u");
        cout << "Сдвиг: x = y + " << x0 << endl;
    } else {
        cout << "\n--- ВВОД ДАННЫХ ---" << endl;
        
        coeffs = inputPolynomial();
        
        cout << "Введите значение сдвига x0 (x = y + x0): ";
        cin >> x0;
        
        cout << "\n--- ВВЕДЁННЫЕ ДАННЫЕ ---" << endl;
        printPolynomial(coeffs, 'x', "u");
        cout << "Сдвиг: x = y + " << x0 << endl;
    }
    
    cout << "\n--- ВЫЧИСЛЕНИЕ СДВИГА ---" << endl;
    cout << "----------------------------------------------------------" << endl;
    
    vector<double> result = shift_polynomial(coeffs, x0);
    
    cout << "Результат: ";
    printPolynomial(result, 'y', "v");
    
    if (choice == 1) {
        cout << "\nСхема вычисления (метод Горнера для сдвига):" << endl;
        cout << "  Шаг 1: p₁ = a₃ = " << coeffs[3] << endl;
        cout << "  Шаг 2: p₂ = p₁·x₀ + a₂ = " << coeffs[3] << "·" << x0 << " + " << coeffs[2] << " = " 
             << coeffs[3]*x0 + coeffs[2] << endl;
        cout << "         добавляем новую степень: p₂ = " << coeffs[3] << "y + " << coeffs[3]*x0 + coeffs[2] << endl;
        cout << "  Шаг 3: p₃ = (p₂·x₀) с добавлением степени + a₁" << endl;
        cout << "         p₂·x₀: " << coeffs[3] << "·" << x0 << " = " << coeffs[3]*x0 << " для свободного члена" << endl;
        cout << "         (p₂·x₀) + a₁ = " << coeffs[3]*x0 + coeffs[1] << " для свободного члена" << endl;
        cout << "         p₃ = " << coeffs[3] << "y² + " << coeffs[3]*x0 + coeffs[2] << "y + " 
             << coeffs[3]*x0*x0 + coeffs[2]*x0 + coeffs[1] << endl;
        cout << "  Шаг 4: p₄ = (p₃·x₀) с добавлением степени + a₀" << endl;
        cout << "         результат: v(y) = ";
        printPolynomial(result, 'y');
    }
    cout << "----------------------------------------------------------" << endl;

    cout << "Схема Горнера для сдвига полинома позволяет получить коэффициенты" << endl;
    cout << "нового полинома v(y) = u(y + x₀) без раскрытия скобок." << endl;
    cout << "Алгоритм выполняет последовательное умножение на (y + x₀)." << endl;
    cout << "Это эквивалентно разложению по степеням (x - (-x₀))." << endl;
    
    return 0;
}