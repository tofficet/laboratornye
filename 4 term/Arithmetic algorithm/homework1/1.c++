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

bool solveLinearSystem(vector<vector<double>> A, vector<double> b, vector<double>& x) {
    int n = A.size();
    
    for (int i = 0; i < n; i++) {
        int maxRow = i;
        for (int k = i + 1; k < n; k++) {
            if (abs(A[k][i]) > abs(A[maxRow][i])) {
                maxRow = k;
            }
        }
        
        if (abs(A[maxRow][i]) < 1e-10) return false;
        
        swap(A[i], A[maxRow]);
        swap(b[i], b[maxRow]);
        
        double pivot = A[i][i];
        for (int j = i; j < n; j++) A[i][j] /= pivot;
        b[i] /= pivot;
        
        for (int k = i + 1; k < n; k++) {
            double factor = A[k][i];
            for (int j = i; j < n; j++) A[k][j] -= factor * A[i][j];
            b[k] -= factor * b[i];
        }
    }
    
    x.resize(n);
    for (int i = n - 1; i >= 0; i--) {
        x[i] = b[i];
        for (int j = i + 1; j < n; j++) x[i] -= A[i][j] * x[j];
    }
    return true;
}

int getDegree(const vector<double>& coeffs) {
    for (int i = coeffs.size() - 1; i >= 0; i--) {
        if (abs(coeffs[i]) > 1e-10) return i;
    }
    return -1;
}

bool checkLinearSpan(const vector<double>& f, const vector<vector<double>>& g, vector<double>& coeffs) {
    int n = g.size();
    if (n == 0) return false;
    
    int maxDegree = getDegree(f);
    for (auto& poly : g) maxDegree = max(maxDegree, getDegree(poly));
    if (maxDegree < 0) return false;
    
    int m = maxDegree + 1;
    vector<vector<double>> A(m, vector<double>(n, 0.0));
    vector<double> b(m, 0.0);
    
    for (int j = 0; j < n; j++) {
        for (int i = 0; i <= getDegree(g[j]); i++) {
            if (i < g[j].size()) A[i][j] = g[j][i];
        }
    }
    
    for (int i = 0; i <= getDegree(f); i++) {
        if (i < f.size()) b[i] = f[i];
    }
    
    if (m >= n) {
        vector<vector<double>> AtA(n, vector<double>(n, 0.0));
        vector<double> Atb(n, 0.0);
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                double sum = 0.0;
                for (int k = 0; k < m; k++) sum += A[k][i] * A[k][j];
                AtA[i][j] = sum;
            }
            double sum = 0.0;
            for (int k = 0; k < m; k++) sum += A[k][i] * b[k];
            Atb[i] = sum;
        }
        
        if (!solveLinearSystem(AtA, Atb, coeffs)) return false;
        
        double norm = 0.0;
        for (int i = 0; i < m; i++) {
            double sum = 0.0;
            for (int j = 0; j < n; j++) sum += A[i][j] * coeffs[j];
            norm += (sum - b[i]) * (sum - b[i]);
        }
        return norm < 1e-8;
    }
    else {
        vector<vector<double>> A_square(n, vector<double>(n, 0.0));
        vector<double> b_square(n, 0.0);
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) A_square[i][j] = A[i][j];
            b_square[i] = b[i];
        }
        for (int i = m; i < n; i++) {
            A_square[i][i] = 1.0;
            b_square[i] = 0.0;
        }
        
        if (!solveLinearSystem(A_square, b_square, coeffs)) return false;
        
        double norm = 0.0;
        for (int i = 0; i < m; i++) {
            double sum = 0.0;
            for (int j = 0; j < n; j++) sum += A[i][j] * coeffs[j];
            norm += (sum - b[i]) * (sum - b[i]);
        }
        return norm < 1e-8;
    }
}

int main() {
    vector<double> f = {1, 2, 1};
    vector<double> g1 = {1, 1};
    vector<double> g2 = {-1, 0, 1};
    vector<vector<double>> basis = {g1, g2};
    
    cout << "f(x) = x^2 + 2x + 1" << endl;
    cout << "g1(x) = x + 1" << endl;
    cout << "g2(x) = x^2 - 1" << endl << endl;
    
    vector<double> coeffs;
    if (checkLinearSpan(f, basis, coeffs)) {
        cout << "f(x) принадлежит линейной оболочке" << endl;
        cout << "f(x) = ";
        bool first = true;
        for (int i = 0; i < coeffs.size(); i++) {
            if (abs(coeffs[i]) < 1e-8) continue;
            if (!first && coeffs[i] > 0) cout << " + ";
            else if (!first && coeffs[i] < 0) cout << " - ";
            else if (coeffs[i] < 0) cout << "-";
            cout << abs(coeffs[i]) << "*g" << i + 1 << "(x)";
            first = false;
        }
        cout << endl;
    } else {
        cout << "f(x) НЕ принадлежит линейной оболочке" << endl;
    }
    
    return 0;
}