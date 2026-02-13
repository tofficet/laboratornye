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

void printShiftedForm(const vector<double>& coeffs, double shift, char var = 'x') {
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
    cout << endl;
}

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

int main() {
    cout << "Переход от разложения по степеням (x - a) к разложению по степеням (x - b)" << endl << endl;
    
    vector<double> f = {4, 3, 1};
    double a = 1.0;
    double b = 2.0;
    
    cout << "Исходное представление: f(x) = ";
    printShiftedForm(f, a);
    cout << "где a = " << a << ", b = " << b << endl << endl;
    
    vector<double> result = taylorFromShifted(f, a, b);
    
    cout << "Разложение по степеням (x - " << b << ")^k:" << endl;
    cout << "f(x) = ";
    printShiftedForm(result, b);
    
    cout << "Численные коэффициенты:" << endl;
    for (int i = result.size() - 1; i >= 0; i--) {
        if (abs(result[i]) > 1e-10) {
            cout << "  (x - " << b << ")^" << i << ": " << result[i] << endl;
        }
    }
    
    return 0;
}