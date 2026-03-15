#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>

using namespace std;

const double PI = 3.14159265358979323846;
const double EPS = 1e-12;

void printMatrix(const vector<vector<double>>& A, const string& name) {
    cout << name << ":" << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << setw(12) << fixed << setprecision(6) << A[i][j] << " ";
        }
        cout << endl;
    }
}

void printVector(const vector<double>& v, const string& name) {
    cout << name << ": ";
    for (int i = 0; i < 3; i++) {
        cout << setw(12) << fixed << setprecision(6) << v[i] << " ";
    }
    cout << endl;
}

vector<vector<double>> inverseMatrix3x3(const vector<vector<double>>& J) {
    double a = J[0][0], b = J[0][1], c = J[0][2];
    double d = J[1][0], e = J[1][1], f = J[1][2];
    double g = J[2][0], h = J[2][1], i = J[2][2];
    
    double det = a*(e*i - f*h) - b*(d*i - f*g) + c*(d*h - e*g);
    
    if (abs(det) < EPS) {
        cout << "Определитель близок к нулю: " << det << endl;
    }
    
    vector<vector<double>> inv(3, vector<double>(3));
    
    inv[0][0] = (e*i - f*h) / det;
    inv[0][1] = (c*h - b*i) / det;
    inv[0][2] = (b*f - c*e) / det;
    inv[1][0] = (f*g - d*i) / det;
    inv[1][1] = (a*i - c*g) / det;
    inv[1][2] = (c*d - a*f) / det;
    inv[2][0] = (d*h - e*g) / det;
    inv[2][1] = (b*g - a*h) / det;
    inv[2][2] = (a*e - b*d) / det;
    
    return inv;
}

double normInf(const vector<double>& v) {
    double maxVal = 0;
    for (int i = 0; i < 3; i++) {
        if (abs(v[i]) > maxVal) {
            maxVal = abs(v[i]);
        }
    }
    return maxVal;
}

void solveNewtonSystem3D(
    void (*F)(const vector<double>&, vector<double>&),
    void (*Jacobian)(const vector<double>&, vector<vector<double>>&),
    vector<double> X0, double eps, int maxIter, int systemNum) {
    
    cout << "\n===========================================" << endl;
    cout << "Система " << systemNum << endl;
    cout << "Точность: " << eps << endl;
    cout << "Начальное приближение: ";
    for (int i = 0; i < 3; i++) {
        cout << "x" << i+1 << "0 = " << X0[i] << " ";
    }
    cout << endl;
    cout << "-------------------------------------------" << endl;
    
    vector<double> X = X0;
    vector<double> X_prev;
    vector<double> FX(3);
    vector<vector<double>> J(3, vector<double>(3));
    int iter = 0;
    
    cout << "Итерация " << iter << ": ";
    for (int i = 0; i < 3; i++) {
        cout << "x" << i+1 << " = " << fixed << setprecision(8) << X[i] << " ";
    }
    F(X, FX);
    cout << "|F| = " << normInf(FX) << endl;
    
    do {
        X_prev = X;
        
        F(X, FX);
        Jacobian(X, J);
        
        vector<vector<double>> J_inv = inverseMatrix3x3(J);
        
        vector<double> delta(3, 0);
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                delta[i] -= J_inv[i][j] * FX[j];
            }
        }
        
        for (int i = 0; i < 3; i++) {
            X[i] += delta[i];
        }
        
        iter++;
        
        cout << "Итерация " << iter << ": ";
        for (int i = 0; i < 3; i++) {
            cout << "x" << i+1 << " = " << fixed << setprecision(8) << X[i] << " ";
        }
        F(X, FX);
        cout << "|F| = " << normInf(FX) << ", |Δ| = " << normInf(delta) << endl;
        
        if (normInf(delta) < eps) {
            break;
        }
        
        if (iter >= maxIter) {
            cout << "Достигнуто максимальное количество итераций" << endl;
            break;
        }
        
    } while (true);
    
    cout << "-------------------------------------------" << endl;
    cout << "Решение системы " << systemNum << ":" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "x" << i+1 << " = " << fixed << setprecision(10) << X[i] << endl;
    }
    cout << "Количество итераций: " << iter << endl;
    cout << "Норма невязки: " << normInf(FX) << endl;
}

void F1(const vector<double>& X, vector<double>& F) {
    double x1 = X[0], x2 = X[1], x3 = X[2];
    
    F[0] = x1*x1*x1 + x1*x1*x2 - x1*x3 + 6.0;
    F[1] = exp(x1) + exp(x2) - x3;
    F[2] = x2*x2 - 2.0*x1*x3 - 4.0;
}

void J1(const vector<double>& X, vector<vector<double>>& J) {
    double x1 = X[0], x2 = X[1], x3 = X[2];
    
    J[0][0] = 3.0*x1*x1 + 2.0*x1*x2 - x3;
    J[0][1] = x1*x1;
    J[0][2] = -x1;
    
    J[1][0] = exp(x1);
    J[1][1] = exp(x2);
    J[1][2] = -1.0;
    
    J[2][0] = -2.0*x3;
    J[2][1] = 2.0*x2;
    J[2][2] = -2.0*x1;
}

void F2(const vector<double>& X, vector<double>& F) {
    double x1 = X[0], x2 = X[1], x3 = X[2];
    
    F[0] = 6.0*x1 - 2.0*cos(x2*x3) - 1.0;
    F[1] = 9.0*x2 + sqrt(x1*x1 + sin(x3) + 1.06) + 0.9;
    F[2] = 60.0*x3 + 3.0*exp(-x1*x2) + 10.0*PI - 3.0;
}

void J2(const vector<double>& X, vector<vector<double>>& J) {
    double x1 = X[0], x2 = X[1], x3 = X[2];
    
    J[0][0] = 6.0;
    J[0][1] = 2.0*x3*sin(x2*x3);
    J[0][2] = 2.0*x2*sin(x2*x3);
    
    double denom = sqrt(x1*x1 + sin(x3) + 1.06);
    J[1][0] = x1 / denom;
    J[1][1] = 9.0;
    J[1][2] = cos(x3) / (2.0 * denom);
    
    J[2][0] = -3.0*x2*exp(-x1*x2);
    J[2][1] = -3.0*x1*exp(-x1*x2);
    J[2][2] = 60.0;
}

int main() {
    int n;
    cout << "Введите точность n (10^-n): ";
    cin >> n;
    double eps = pow(10.0, -n);
    int maxIter = 100;
    
    vector<double> X0_1 = {-1.0, 1.0, 2.0};
    solveNewtonSystem3D(F1, J1, X0_1, eps, maxIter, 1);
    
    vector<double> X0_2 = {0.5, -0.5, 0.1};
    solveNewtonSystem3D(F2, J2, X0_2, eps, maxIter, 2);
    
    return 0;
}