#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

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

vector<double> inputPolynomial(const string& name) {
    int degree;
    cout << "Введите степень полинома " << name << ": ";
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
    cout << "ПРОВЕРКА ПРИНАДЛЕЖНОСТИ ПОЛИНОМА ЛИНЕЙНОЙ ОБОЛОЧКЕ" << endl;
    cout << "==========================================================" << endl;
    cout << "Программа проверяет, можно ли представить полином f(x)" << endl;
    cout << "как линейную комбинацию полиномов g1(x), g2(x), ..., gn(x)" << endl;
    cout << "----------------------------------------------------------" << endl;
    
    int choice;
    cout << "Выберите режим работы:" << endl;
    cout << "1 - Использовать тестовый пример (x^2 + 2x + 1 и базис {x+1, x^2-1})" << endl;
    cout << "2 - Ввести свои данные" << endl;
    cout << "Ваш выбор: ";
    cin >> choice;
    
    vector<double> f;
    vector<vector<double>> basis;
    
    if (choice == 1) {
        f = {1, 2, 1};  // x^2 + 2x + 1
        vector<double> g1 = {1, 1};  // x + 1
        vector<double> g2 = {-1, 0, 1};  // x^2 - 1
        basis = {g1, g2};
        
        cout << "\n--- ТЕСТОВЫЙ ПРИМЕР ---" << endl;
        printPolynomial(f, "f");
        cout << "Базисные полиномы:" << endl;
        printPolynomial(g1, "g1");
        printPolynomial(g2, "g2");
    } else {
        cout << "\n--- ВВОД ДАННЫХ ---" << endl;
    
        f = inputPolynomial("f");
        
        int basisSize;
        cout << "\nВведите количество базисных полиномов: ";
        cin >> basisSize;
        
        basis.clear();
        for (int i = 0; i < basisSize; i++) {
            cout << "\nПолином g" << i + 1 << ":" << endl;
            vector<double> g = inputPolynomial("g" + to_string(i + 1));
            basis.push_back(g);
        }
        
        cout << "\n--- ВВЕДЁННЫЕ ДАННЫЕ ---" << endl;
        printPolynomial(f, "f");
        cout << "Базисные полиномы:" << endl;
        for (int i = 0; i < basis.size(); i++) {
            printPolynomial(basis[i], "g" + to_string(i + 1));
        }
    }
    
    cout << "\n--- РЕЗУЛЬТАТ ---" << endl;
    
    vector<double> coeffs;
    if (checkLinearSpan(f, basis, coeffs)) {
        cout << "✓ f(x) ПРИНАДЛЕЖИТ линейной оболочке <g1, g2, ..., gn>" << endl;
        cout << "\nРазложение f(x) по базису:" << endl;
        cout << "f(x) = ";
        bool first = true;
        for (int i = 0; i < coeffs.size(); i++) {
            if (abs(coeffs[i]) < 1e-8) continue;
            
            if (!first && coeffs[i] > 0) cout << " + ";
            else if (!first && coeffs[i] < 0) cout << " - ";
            else if (coeffs[i] < 0) cout << "-";
            
            cout << abs(coeffs[i]) << " * g" << i + 1 << "(x)";
            first = false;
        }
        cout << endl;
        
    } else {
        cout << "✗ f(x) НЕ ПРИНАДЛЕЖИТ линейной оболочке <g1, g2, ..., gn>" << endl;
        cout << "Невозможно представить f(x) как линейную комбинацию заданных полиномов." << endl;
    }
    return 0;
}