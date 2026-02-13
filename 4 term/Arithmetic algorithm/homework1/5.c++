#include <iostream>
#include <vector>
#include <cmath>

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

int main() {
    cout << "T(x) = f1^k(s1(x)) / f2^l(s2(x))" << endl << endl;
    
    Polynomial f1({0, 1});
    Polynomial s1({0, 1});
    int k = 2;
    
    Polynomial f2({1, 1});
    Polynomial s2({0, 1});
    int l = 1;
    
    cout << "f1(x) = x" << endl;
    cout << "s1(x) = x" << endl;
    cout << "k = " << k << endl;
    cout << "f2(x) = 1 + x" << endl;
    cout << "s2(x) = x" << endl;
    cout << "l = " << l << endl << endl;
    
    double a = 0;
    double la = limitPoint(f1, s1, k, f2, s2, l, a);
    cout << "lim x->" << a << ": ";
    if (la > 1e307) cout << "∞" << endl;
    else cout << la << endl;
    
    double lp = limitInf(f1, s1, k, f2, s2, l, true);
    cout << "lim x->+∞: ";
    if (lp > 1e307) cout << "∞" << endl;
    else cout << lp << endl;
    
    double ln = limitInf(f1, s1, k, f2, s2, l, false);
    cout << "lim x->-∞: ";
    if (ln > 1e307) cout << "∞" << endl;
    else cout << ln << endl;
    
    return 0;
}