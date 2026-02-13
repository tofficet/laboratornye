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

void printTaylorForm(const vector<double>& coeffs, double a) {
    cout << "f(x) = ";
    bool first = true;
    for (int i = coeffs.size() - 1; i >= 0; i--) {
        if (abs(coeffs[i]) < 1e-10) continue;
        
        if (!first && coeffs[i] > 0) cout << " + ";
        else if (!first && coeffs[i] < 0) cout << " - ";
        else if (coeffs[i] < 0) cout << "-";
        
        if (i == 0) cout << abs(coeffs[i]);
        else if (i == 1) {
            if (abs(abs(coeffs[i]) - 1.0) < 1e-10) {
                if (a == 0) cout << "x";
                else cout << "(x";
                if (a > 0) cout << " - " << a << ")";
                else if (a < 0) cout << " + " << -a << ")";
                else cout << ")";
            }
            else {
                cout << abs(coeffs[i]) << "*";
                if (a == 0) cout << "x";
                else cout << "(x";
                if (a > 0) cout << " - " << a << ")";
                else if (a < 0) cout << " + " << -a << ")";
                else cout << ")";
            }
        }
        else {
            if (abs(abs(coeffs[i]) - 1.0) < 1e-10) {
                if (a == 0) cout << "x^" << i;
                else cout << "(x";
                if (a > 0) cout << " - " << a << ")^" << i;
                else if (a < 0) cout << " + " << -a << ")^" << i;
                else cout << ")^" << i;
            }
            else {
                cout << abs(coeffs[i]) << "*";
                if (a == 0) cout << "x^" << i;
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

int main() {
    cout << "Представление многочлена в виде линейной комбинации степеней (x - a)^k" << endl << endl;
    
    vector<double> f = {1, 2, 1};
    double a = 1.0;
    
    cout << "Исходный многочлен: ";
    printPolynomial(f, "f");
    cout << "a = " << a << endl << endl;
    
    vector<double> taylor = taylorExpansion(f, a);
    
    cout << "Разложение по степеням (x - " << a << ")^k:" << endl;
    printTaylorForm(taylor, a);
    
    cout << "Численные коэффициенты:" << endl;
    for (int i = taylor.size() - 1; i >= 0; i--) {
        if (abs(taylor[i]) > 1e-10) {
            cout << "  (x - " << a << ")^" << i << ": " << taylor[i] << endl;
        }
    }
    
    return 0;
}