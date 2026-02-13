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
};

double limitPoint(Polynomial& f, Polynomial& g, double a) {
    double fv = f.eval(a), gv = g.eval(a);
    
    if (abs(gv) > 1e-10) return fv / gv;
    if (abs(fv) > 1e-10) return (fv > 0 ? 1 : -1) * INFINITY;
    
    while (abs(f.eval(a)) < 1e-10 && abs(g.eval(a)) < 1e-10 && f.deg() >= 0 && g.deg() >= 0) {
        for (int i = 0; i < f.deg(); i++) f.coeffs[i] = (i + 1) * f.coeffs[i + 1];
        f.coeffs.pop_back();
        for (int i = 0; i < g.deg(); i++) g.coeffs[i] = (i + 1) * g.coeffs[i + 1];
        g.coeffs.pop_back();
    }
    
    double fd = f.eval(a), gd = g.eval(a);
    return abs(gd) > 1e-10 ? fd / gd : (fd > 0 ? 1 : -1) * INFINITY;
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

int main() {
    Polynomial f({1, 0, 1});
    Polynomial g({1, -2, 1});
    
    double a = 1.0;
    double la = limitPoint(f, g, a);
    cout << "lim x->" << a << ": " << (isinf(la) ? (la > 0 ? "+∞" : "-∞") : to_string(la)) << endl;
    
    double lp = limitInf(f, g, true);
    cout << "lim x->+∞: " << (isinf(lp) ? (lp > 0 ? "+∞" : "-∞") : to_string(lp)) << endl;
    
    double ln = limitInf(f, g, false);
    cout << "lim x->-∞: " << (isinf(ln) ? (ln > 0 ? "+∞" : "-∞") : to_string(ln)) << endl;
    
    return 0;
}