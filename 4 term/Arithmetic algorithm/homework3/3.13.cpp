#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

vector<double> inverseSeries(const vector<double>& f, int n) {
    int N = n;
    vector<double> g(N, 0.0);
    
    double a0 = f[0];
    if (abs(a0) < 1e-12) {
        cout << "Ошибка: a0 должен быть обратим (не равен нулю)" << endl;
        return g;
    }
    
    g[0] = 1.0 / a0;
    
    for (int i = 1; i < N; i++) {
        double sum = 0.0;
        for (int k = 1; k <= i; k++) {
            if (k < f.size() && i - k >= 0) {
                sum += f[k] * g[i - k];
            }
        }
        g[i] = -sum / a0;
    }
    
    return g;
}

void printSeries(const vector<double>& coeffs, const string& name, int firstN) {
    cout << name << " = ";
    bool first = true;
    for (int i = 0; i < min(firstN, (int)coeffs.size()); i++) {
        if (abs(coeffs[i]) < 1e-12) continue;
        
        if (!first && coeffs[i] >= 0) cout << " + ";
        else if (!first && coeffs[i] < 0) cout << " - ";
        else if (first && coeffs[i] < 0) cout << "-";
        
        double val = abs(coeffs[i]);
        if (i == 0) {
            cout << val;
        } else if (i == 1) {
            if (abs(val - 1.0) < 1e-12) cout << "x";
            else cout << val << "x";
        } else {
            if (abs(val - 1.0) < 1e-12) cout << "x^" << i;
            else cout << val << "x^" << i;
        }
        first = false;
    }
    cout << endl;
}

int main() {
    int n;
    cout << "Введите количество коэффициентов для вычисления: ";
    cin >> n;
    
    cout << "\nПример 1: f(x) = exp(x) (частичная сумма до M членов)" << endl;
    int M;
    cout << "Введите M (количество членов ряда экспоненты): ";
    cin >> M;
    
    vector<double> expSeries(M + 1);
    double fact = 1.0;
    for (int k = 0; k <= M; k++) {
        if (k > 0) fact *= k;
        expSeries[k] = 1.0 / fact;
    }
    
    printSeries(expSeries, "f(x)", M + 1);
    
    vector<double> invExp = inverseSeries(expSeries, n);
    cout << "\nПервые " << n << " коэффициентов обратного ряда f^{-1}(x):" << endl;
    printSeries(invExp, "f^{-1}(x)", n);
    
    cout << "\nПроверка: f * f^{-1} (первые " << n << " членов)" << endl;
    vector<double> product(n, 0.0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            if (j < expSeries.size() && i - j < invExp.size()) {
                product[i] += expSeries[j] * invExp[i - j];
            }
        }
    }
    printSeries(product, "f*f^{-1}", n);
    
    cout << "\nПример 2: f(x) = -1 - x + x^2" << endl;
    vector<double> f2 = {-1.0, -1.0, 1.0};
    for (int i = 3; i < n; i++) {
        f2.push_back(0.0);
    }
    
    printSeries(f2, "f(x)", n);
    
    vector<double> invF2 = inverseSeries(f2, n);
    cout << "\nПервые " << n << " коэффициентов обратного ряда f^{-1}(x):" << endl;
    printSeries(invF2, "f^{-1}(x)", n);
    
    cout << "\nПроверка: f * f^{-1} (первые " << n << " членов)" << endl;
    vector<double> product2(n, 0.0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            if (j < f2.size() && i - j < invF2.size()) {
                product2[i] += f2[j] * invF2[i - j];
            }
        }
    }
    printSeries(product2, "f*f^{-1}", n);
    
    cout << "\nПервые три коэффициента обратного ряда (теоретически):" << endl;
    cout << "g0 = 1/a0" << endl;
    cout << "g1 = -a1 / a0^2" << endl;
    cout << "g2 = -(a2 * a0 - a1^2) / a0^3" << endl;
    
    return 0;
}