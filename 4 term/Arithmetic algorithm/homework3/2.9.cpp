#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>

using namespace std;

const double PI = 3.14159265358979323846;

struct SystemResult {
    double x;
    double y;
    int iterations;
    double normF;
    double normDelta;
};

double norm2(double x, double y) {
    return sqrt(x*x + y*y);
}

SystemResult solveNewtonSystem(
    void (*f)(double, double, double&, double&, const vector<double>&),
    void (*jacobian)(double, double, double&, double&, double&, double&, const vector<double>&),
    double x0, double y0, double eps, int maxIter, const vector<double>& params) {
    
    double x = x0;
    double y = y0;
    double fx, fy;
    double dfdx, dfdy, dgdx, dgdy;
    int iter = 0;
    double normF_val, normDelta_val;
    
    cout << "Итерация " << iter << ": x = " << fixed << setprecision(8) << x 
         << ", y = " << y << endl;
    
    do {
        f(x, y, fx, fy, params);
        normF_val = norm2(fx, fy);
        
        jacobian(x, y, dfdx, dfdy, dgdx, dgdy, params);
        
        double det = dfdx * dgdy - dfdy * dgdx;
        if (abs(det) < 1e-12) {
            cout << "Определитель Якобиана близок к нулю" << endl;
            break;
        }
        
        double dx = ( -fx * dgdy + fy * dfdy ) / det;
        double dy = ( -dfdx * fy + dgdx * fx ) / det;
        
        x += dx;
        y += dy;
        iter++;
        
        normDelta_val = norm2(dx, dy);
        
        cout << "Итерация " << iter << ": x = " << fixed << setprecision(8) << x 
             << ", y = " << y << ", |F| = " << normF_val 
             << ", |Δ| = " << normDelta_val << endl;
        
        f(x, y, fx, fy, params);
        normF_val = norm2(fx, fy);
        
        if (normF_val < eps && normDelta_val < eps) {
            break;
        }
        
        if (iter >= maxIter) {
            cout << "Достигнуто максимальное количество итераций" << endl;
            break;
        }
        
    } while (true);
    
    SystemResult result;
    result.x = x;
    result.y = y;
    result.iterations = iter;
    result.normF = normF_val;
    result.normDelta = normDelta_val;
    
    return result;
}

void systemA(double x, double y, double& f1, double& f2, const vector<double>& params) {
    double A = params[0];
    double alpha2 = params[1];
    double beta2 = params[2];
    
    f1 = tan(y + A) - x*x;
    f2 = x*x / alpha2 + y*y / beta2 - 1.0;
}

void jacobianA(double x, double y, double& df1dx, double& df1dy, double& df2dx, double& df2dy, const vector<double>& params) {
    double A = params[0];
    double alpha2 = params[1];
    double beta2 = params[2];
    
    double sec2 = 1.0 / (cos(y + A) * cos(y + A));
    
    df1dx = -2.0 * x;
    df1dy = sec2;
    df2dx = 2.0 * x / alpha2;
    df2dy = 2.0 * y / beta2;
}

void systemB(double x1, double x2, double& f1, double& f2, const vector<double>& params) {
    f1 = x1*x1 + x2*x2 - 2.0;
    f2 = exp(x1 - 1.0) + x2*x2 - 2.0;
}

void jacobianB(double x1, double x2, double& df1dx1, double& df1dx2, double& df2dx1, double& df2dx2, const vector<double>& params) {
    df1dx1 = 2.0 * x1;
    df1dx2 = 2.0 * x2;
    df2dx1 = exp(x1 - 1.0);
    df2dx2 = 2.0 * x2;
}

int main() {
    double eps = 1e-8;
    int maxIter = 100;
    
    cout << "===========================================" << endl;
    cout << "Часть a: Система tan(y + A) = x^2, x^2/α^2 + y^2/β^2 = 1" << endl;
    cout << "===========================================" << endl;
    
    vector<vector<double>> paramSets = {
        {0.2, 1.0/0.6, 1.0/2.0},
        {0.4, 1.0/0.8, 1.0/2.0},
        {0.3, 1.0/0.2, 1.0/3.0},
        {0.0, 1.0/0.6, 1.0/2.0}
    };
    
    vector<pair<double, double>> initialGuesses = {
        {0.5, -0.3},
        {0.6, -0.2},
        {0.4, -0.1},
        {0.5, -0.2}
    };
    
    for (int i = 0; i < paramSets.size(); i++) {
        double A = paramSets[i][0];
        double alpha2 = paramSets[i][1];
        double beta2 = paramSets[i][2];
        
        double x0 = initialGuesses[i].first;
        double y0 = initialGuesses[i].second;
        
        cout << "\nСлучай " << i+1 << ": A = " << A << ", α^2 = " << alpha2 
             << ", β^2 = " << beta2 << endl;
        cout << "Начальное приближение: x0 = " << x0 << ", y0 = " << y0 << endl;
        cout << "-------------------------------------------" << endl;
        
        SystemResult result = solveNewtonSystem(systemA, jacobianA, x0, y0, eps, maxIter, paramSets[i]);
        
        cout << "-------------------------------------------" << endl;
        cout << "Результат: x = " << fixed << setprecision(8) << result.x 
             << ", y = " << result.y << endl;
        cout << "Итераций: " << result.iterations << endl;
        cout << "Норма F: " << result.normF << endl;
        cout << "-------------------------------------------" << endl;
    }
    
    cout << "\n===========================================" << endl;
    cout << "Часть b: Система x1^2 + x2^2 - 2 = 0, e^(x1-1) + x2^2 - 2 = 0" << endl;
    cout << "===========================================" << endl;
    
    double x1_0 = 0.5;
    double x2_0 = 1.0;
    
    cout << "Начальное приближение: x1_0 = " << x1_0 << ", x2_0 = " << x2_0 << endl;
    cout << "-------------------------------------------" << endl;
    
    vector<double> emptyParams;
    
    auto systemB_wrapper = [](double x1, double x2, double& f1, double& f2, const vector<double>&) {
        systemB(x1, x2, f1, f2, vector<double>());
    };
    
    auto jacobianB_wrapper = [](double x1, double x2, double& df1dx1, double& df1dx2, double& df2dx1, double& df2dx2, const vector<double>&) {
        jacobianB(x1, x2, df1dx1, df1dx2, df2dx1, df2dx2, vector<double>());
    };
    
    SystemResult resultB = solveNewtonSystem(systemB_wrapper, jacobianB_wrapper, x1_0, x2_0, eps, maxIter, emptyParams);
    
    cout << "-------------------------------------------" << endl;
    cout << "Результат: x1 = " << fixed << setprecision(8) << resultB.x 
         << ", x2 = " << resultB.y << endl;
    cout << "Итераций: " << resultB.iterations << endl;
    cout << "Норма F: " << resultB.normF << endl;
    cout << "Проверка:" << endl;
    double f1, f2;
    systemB(resultB.x, resultB.y, f1, f2, emptyParams);
    cout << "f1 = x1^2 + x2^2 - 2 = " << f1 << endl;
    cout << "f2 = e^(x1-1) + x2^2 - 2 = " << f2 << endl;
    cout << "===========================================" << endl;
    
    return 0;
}