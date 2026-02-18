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

double limitPoint(Polynomial& f, Polynomial& g, double a) {
    double fv = f.eval(a), gv = g.eval(a);
    //gv знаменатель
    if (abs(gv) > 1e-10) return fv / gv;   
    if (abs(fv) > 1e-10) return (fv > 0 ? 1 : -1) * INFINITY;
    
    while (abs(f.eval(a)) < 1e-10 && abs(g.eval(a)) < 1e-10 && f.deg() >= 0 && g.deg() >= 0) {
        for (int i = 0; i < f.deg(); i++) f.coeffs[i] = (i + 1) * f.coeffs[i + 1];
        f.coeffs.pop_back();
        for (int i = 0; i < g.deg(); i++) g.coeffs[i] = (i + 1) * g.coeffs[i + 1];
        g.coeffs.pop_back();
    }

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


#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

class Polynomial {
public:
    vector<double> coeffs;
    
    Polynomial(const vector<double>& c) : coeffs(c) {}
    
    double eval(double x) const {
        double result = 0;
        for (int i = coeffs.size() - 1; i >= 0; i--) {
            result = result * x + coeffs[i];
        }
        return result;
    }
    
    int deg() const {
        return coeffs.size() - 1;
    }
};


vector<double> taylorExpansion(const vector<double>& f, double a) {
    int n = f.size() - 1;
    vector<double> result(n + 1, 0.0);
    vector<double> temp = f;
    
    for (int k = 0; k <= n; k++) {
        result[k] = temp[0];
        
        for (int i = 0; i < n - k; i++) {
            temp[i] = temp[i + 1] * (i + 1) + a * temp[i + 1] * (i + 1);
        }
        temp.pop_back();
    }
    
    double fact = 1.0;
    for (int k = 1; k <= n; k++) {
        fact *= k;
        result[k] /= fact;
    }
    
    return result;
}

double limitPoint(Polynomial& f, Polynomial& g, double a) {
    double fv = f.eval(a);
    double gv = g.eval(a);

    if (abs(gv) > 1e-10) {
        return fv / gv;
    }
    if (abs(fv) > 1e-10) {
        return (fv > 0 ? 1.0 : -1.0) * INFINITY;
    }

    vector<double> fTaylor = taylorExpansion(f.coeffs, a);
    vector<double> gTaylor = taylorExpansion(g.coeffs, a);
    
    int fOrder = -1;
    for (int i = 0; i < fTaylor.size(); i++) {
        if (abs(fTaylor[i]) > 1e-10) {
            fOrder = i;
            break;
        }
    }

    int gOrder = -1;
    for (int i = 0; i < gTaylor.size(); i++) {
        if (abs(gTaylor[i]) > 1e-10) {
            gOrder = i;
            break;
        }
    }
    
    if (fOrder == -1 && gOrder == -1) {
        return 0.0;  
    }
    
    if (fOrder == -1) fOrder = fTaylor.size(); 
    if (gOrder == -1) gOrder = gTaylor.size(); 
    
    if (fOrder < gOrder) {
        return 0.0;
    }
    else if (fOrder > gOrder) {
        double sign = (fTaylor[fOrder] * gTaylor[gOrder] > 0) ? 1.0 : -1.0;
        return sign * INFINITY;
    }
    else {
        return fTaylor[fOrder] / gTaylor[gOrder];
    }
}


double limitInf(Polynomial& f, Polynomial& g, bool pos) {
    int fd = f.deg(), gd = g.deg();
    if (fd < gd) return 0;
    if (fd > gd) {
        double s = pos ? 1 : -1;
        if ((fd - gd) % 2) s *= -1;
        return (f.coeffs[fd] / g.coeffs[gd] > 0 ? 1 : -1) * s * INFINITY;
    }
    return f.coeffs[fd] / g.coeffs[gd];
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
    cout << "ВЫЧИСЛЕНИЕ ПРЕДЕЛОВ ПОЛИНОМИАЛЬНЫХ ФУНКЦИЙ" << endl;
    cout << "==========================================================" << endl;
    cout << "Программа вычисляет пределы отношения двух полиномов:" << endl;
    cout << "  lim f(x)/g(x) при x → a, x → +∞ и x → -∞" << endl;
    cout << "----------------------------------------------------------" << endl;
    
    int choice;
    cout << "Выберите режим работы:" << endl;
    cout << "1 - Использовать тестовый пример (f(x) = x^2 + 1, g(x) = x^2 - 2x + 1, a = 1)" << endl;
    cout << "2 - Ввести свои данные" << endl;
    cout << "Ваш выбор: ";
    cin >> choice;
    
    Polynomial f({0}), g({0});
    double a;
    
    if (choice == 1) {
        // f(x) = x^2 + 1, g(x) = x^2 - 2x + 1
        f = Polynomial({1, 0, 1});  // 1 + 0*x + 1*x^2
        g = Polynomial({1, -2, 1}); // 1 - 2x + 1*x^2
        a = 1.0;
        
        cout << "\n--- ТЕСТОВЫЙ ПРИМЕР ---" << endl;
        printPolynomial(f, "f");
        printPolynomial(g, "g");
        cout << "a = " << a << endl;
    } else {
        cout << "\n--- ВВОД ДАННЫХ ---" << endl;
        
        f = inputPolynomial("f");
        g = inputPolynomial("g");
        
        cout << "Введите точку a: ";
        cin >> a;
        
        cout << "\n--- ВВЕДЁННЫЕ ДАННЫЕ ---" << endl;
        printPolynomial(f, "f");
        printPolynomial(g, "g");
        cout << "a = " << a << endl;
    }
    
    cout << "\n--- ВЫЧИСЛЕНИЕ ПРЕДЕЛОВ ---" << endl;
    cout << "----------------------------------------------------------" << endl;
    
    double la = limitPoint(f, g, a);
    cout << "lim x->" << a << " f(x)/g(x) = ";
    if (isinf(la)) {
        cout << (la > 0 ? "+∞" : "-∞") << endl;
    } else {
        cout << la << endl;
    }
    
    double lp = limitInf(f, g, true);
    cout << "lim x->+∞ f(x)/g(x) = ";
    if (isinf(lp)) {
        cout << (lp > 0 ? "+∞" : "-∞") << endl;
    } else {
        cout << lp << endl;
    }
    
    double ln = limitInf(f, g, false);
    cout << "lim x->-∞ f(x)/g(x) = ";
    if (isinf(ln)) {
        cout << (ln > 0 ? "+∞" : "-∞") << endl;
    } else {
        cout << ln << endl;
    }
    cout << "----------------------------------------------------------" << endl;
    cout << "Если степени числителя и знаменателя равны, предел равен отношению" << endl;
    cout << "старших коэффициентов." << endl;
    cout << "Если степень числителя меньше степени знаменателя, предел равен 0." << endl;
    cout << "Если степень числителя больше степени знаменателя, предел равен ±∞." << endl;
    cout << "В точке a используется правило Лопиталя при неопределённости 0/0." << endl;
    
    
    return 0;
}
