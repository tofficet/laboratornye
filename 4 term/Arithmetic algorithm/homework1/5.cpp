#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

struct Polynomial {
    vector<double> coeffs;
    
    Polynomial(vector<double> c) : coeffs(c) {}
    
    int deg() {
        for (int i = coeffs.size() - 1; i >= 0; i--) {
            if (abs(coeffs[i]) > 1e-10) return i;
        }
        return -1;
    }
    
    double eval(double x) {
        double r = 0, p = 1;
        for (double c : coeffs) {
            r += c * p;
            p *= x;
        }
        return r;
    }
    
    Polynomial derivative() {
        if (deg() < 0) return Polynomial({0});
        vector<double> newCoeffs(deg());
        for (int i = 0; i < deg(); i++) {
            newCoeffs[i] = (i + 1) * coeffs[i + 1];
        }
        return Polynomial(newCoeffs);
    }
};

void printPolynomial(const Polynomial& p, const string& name) {
    cout << name << "(x) = ";
    bool first = true;
    for (int i = p.coeffs.size() - 1; i >= 0; i--) {
        if (abs(p.coeffs[i]) < 1e-10) continue;
        
        if (!first && p.coeffs[i] > 0) cout << " + ";
        else if (!first && p.coeffs[i] < 0) cout << " - ";
        else if (p.coeffs[i] < 0) cout << "-";
        
        if (i == 0) cout << abs(p.coeffs[i]);
        else if (i == 1) {
            if (abs(abs(p.coeffs[i]) - 1.0) < 1e-10) cout << "x";
            else cout << abs(p.coeffs[i]) << "x";
        }
        else {
            if (abs(abs(p.coeffs[i]) - 1.0) < 1e-10) cout << "x^" << i;
            else cout << abs(p.coeffs[i]) << "x^" << i;
        }
        first = false;
    }
    if (first) cout << "0";
    cout << endl;
}

double limitPoint(Polynomial& f1, Polynomial& s1, int k, Polynomial& f2, Polynomial& s2, int l, double a) {
    double num = f1.eval(s1.eval(a));
    double den = f2.eval(s2.eval(a));
    
    double numerator = pow(num, k);
    double denominator = pow(den, l);
    
    if (abs(denominator) > 1e-10) return numerator / denominator;
    if (abs(numerator) > 1e-10) return 1e308;
    
    Polynomial f1_der = f1.derivative();
    Polynomial s1_der = s1.derivative();
    Polynomial f2_der = f2.derivative();
    Polynomial s2_der = s2.derivative();
    
    double num_der = f1_der.eval(s1.eval(a)) * s1_der.eval(a);
    double den_der = f2_der.eval(s2.eval(a)) * s2_der.eval(a);
    
    if (abs(den_der) < 1e-10) return 1e308;
    
    return pow(num_der, k) / pow(den_der, l);
}

double limitInf(Polynomial& f1, Polynomial& s1, int k, Polynomial& f2, Polynomial& s2, int l, bool pos) {
    int f1_deg = f1.deg();
    int s1_deg = s1.deg();
    int f2_deg = f2.deg();
    int s2_deg = s2.deg();
    
    if (f1_deg < 0 || s1_deg < 0) return 0;
    if (f2_deg < 0 || s2_deg < 0) return 1e308;
    
    int num_deg = f1_deg * s1_deg * k;
    int den_deg = f2_deg * s2_deg * l;
    
    if (num_deg < den_deg) return 0;
    if (num_deg > den_deg) return 1e308;
    
    double sign = 1.0;
    if (!pos && (num_deg % 2 == 1)) sign = -1.0;
    
    double num_coef = f1.coeffs[f1_deg];
    for (int i = 0; i < k; i++) {
        num_coef *= num_coef;
    }
    
    double den_coef = f2.coeffs[f2_deg];
    for (int i = 0; i < l; i++) {
        den_coef *= den_coef;
    }
    
    return (num_coef / den_coef) * sign;
}

Polynomial inputPolynomial(const string& name) {
    int degree;
    cout << "Введите степень полинома " << name << ": ";
    cin >> degree;
    
    vector<double> coeffs(degree + 1, 0.0);
    cout << "Введите коэффициенты от младшей степени к старшей (a0, a1, ..., a" << degree << "):" << endl;
    for (int i = 0; i <= degree; i++) {
        cout << "  a" << i << " = ";
        cin >> coeffs[i];
    }
    
    return Polynomial(coeffs);
}

int main() {
    cout << "==========================================================" << endl;
    cout << "ВЫЧИСЛЕНИЕ ПРЕДЕЛОВ СЛОЖНЫХ ФУНКЦИЙ" << endl;
    cout << "==========================================================" << endl;
    cout << "Программа вычисляет пределы функции вида:" << endl;
    cout << "  T(x) = [f1(s1(x))]^k / [f2(s2(x))]^l" << endl;
    cout << "----------------------------------------------------------" << endl;
    
    int choice;
    cout << "Выберите режим работы:" << endl;
    cout << "1 - Использовать тестовый пример (f1(x)=x, s1(x)=x, k=2, f2(x)=1+x, s2(x)=x, l=1, a=0)" << endl;
    cout << "2 - Ввести свои данные" << endl;
    cout << "Ваш выбор: ";
    cin >> choice;
    
    Polynomial f1({0}), s1({0}), f2({0}), s2({0});
    int k, l;
    double a;
    
    if (choice == 1) {
        // Тестовый пример
        f1 = Polynomial({0, 1});  // x
        s1 = Polynomial({0, 1});  // x
        k = 2;
        f2 = Polynomial({1, 1});  // 1 + x
        s2 = Polynomial({0, 1});  // x
        l = 1;
        a = 0;
        
        cout << "\n--- ТЕСТОВЫЙ ПРИМЕР ---" << endl;
        printPolynomial(f1, "f1");
        printPolynomial(s1, "s1");
        cout << "k = " << k << endl;
        printPolynomial(f2, "f2");
        printPolynomial(s2, "s2");
        cout << "l = " << l << endl;
        cout << "a = " << a << endl;
    } else {
        cout << "\n--- ВВОД ДАННЫХ ---" << endl;
        
        cout << "\nВведите полином f1(y):" << endl;
        f1 = inputPolynomial("f1");
        
        cout << "\nВведите полином s1(x):" << endl;
        s1 = inputPolynomial("s1");
        
        cout << "\nВведите степень k: ";
        cin >> k;
        
        cout << "\nВведите полином f2(y):" << endl;
        f2 = inputPolynomial("f2");
        
        cout << "\nВведите полином s2(x):" << endl;
        s2 = inputPolynomial("s2");
        
        cout << "\nВведите степень l: ";
        cin >> l;
        
        cout << "\nВведите точку a: ";
        cin >> a;
        
        cout << "\n--- ВВЕДЁННЫЕ ДАННЫЕ ---" << endl;
        printPolynomial(f1, "f1");
        printPolynomial(s1, "s1");
        cout << "k = " << k << endl;
        printPolynomial(f2, "f2");
        printPolynomial(s2, "s2");
        cout << "l = " << l << endl;
        cout << "a = " << a << endl;
    }
    
    cout << "\n--- ВЫЧИСЛЕНИЕ ПРЕДЕЛОВ ---" << endl;
    cout << "T(x) = [f1(s1(x))]^" << k << " / [f2(s2(x))]^" << l << endl;
    cout << "----------------------------------------------------------" << endl;
    
    double la = limitPoint(f1, s1, k, f2, s2, l, a);
    cout << "lim x->" << a << " T(x) = ";
    if (la > 1e307) cout << "∞" << endl;
    else if (la < -1e307) cout << "-∞" << endl;
    else cout << la << endl;
    
    double lp = limitInf(f1, s1, k, f2, s2, l, true);
    cout << "lim x->+∞ T(x) = ";
    if (lp > 1e307) cout << "∞" << endl;
    else if (lp < -1e307) cout << "-∞" << endl;
    else cout << lp << endl;

    double ln = limitInf(f1, s1, k, f2, s2, l, false);
    cout << "lim x->-∞ T(x) = ";
    if (ln > 1e307) cout << "∞" << endl;
    else if (ln < -1e307) cout << "-∞" << endl;
    else cout << ln << endl;
    cout << "----------------------------------------------------------" << endl;
    
    cout << "В точке a используется правило Лопиталя при неопределённости." << endl;
    cout << "На бесконечности предел определяется степенями полиномов." << endl;
    cout << "Результат ∞ означает бесконечность (знак учитывается)." << endl;
    
    return 0;
}