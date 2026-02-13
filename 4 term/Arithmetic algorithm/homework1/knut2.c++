#include <iostream>
#include <vector>
#include <cmath>

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

void printPolynomial(const vector<double>& coeffs, char var) {
    int n = coeffs.size() - 1;
    cout << (var == 'x' ? "u(x) = " : "v(y) = ");
    
    bool first = true;
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

int main() {
    cout << "ВЫЧИСЛЕНИЕ СДВИГА ПОЛИНОМА (СХЕМА ГОРНЕРА ДЛЯ ПОЛИНОМОВ)\n\n";
    
    vector<double> coeffs = {-7.0, -3.0, 5.0, 2.0};
    
    printPolynomial(coeffs, 'x');
    
    double x0 = 1.0;
    cout << "Сдвиг: x = y + " << x0 << "\n\n";
    
    vector<double> result = shift_polynomial(coeffs, x0);
    
    printPolynomial(result, 'y');
    
    return 0;
}