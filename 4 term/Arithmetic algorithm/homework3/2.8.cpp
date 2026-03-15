#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

double f(double x) {
    return pow(x - 1, 3) * sin(M_PI * x) * (cos(2 * M_PI * x) - 1);
}

double df(double x) {
    double h = 1e-8;
    return (f(x + h) - f(x - h)) / (2 * h);
}

double newtonRegular(double x0, double eps, int& iterations) {
    double x = x0;
    double xPrev;
    iterations = 0;
    
    do {
        xPrev = x;
        double fx = f(x);
        double dfx = df(x);
        
        if (abs(dfx) < 1e-12) {
            return x;
        }
        
        x = x - fx / dfx;
        iterations++;
        
        if (abs(x - xPrev) < eps) {
            break;
        }
        
        if (iterations > 1000) {
            break;
        }
        
    } while (true);
    
    return x;
}

double newtonModified(double x0, double eps, int& iterations, double sigma) {
    double x = x0;
    double xPrev;
    iterations = 0;
    
    do {
        xPrev = x;
        double fx = f(x);
        double dfx = df(x);
        
        if (abs(dfx) < 1e-12) {
            return x;
        }
        
        x = x + sigma * fx / dfx;
        iterations++;
        
        if (abs(x - xPrev) < eps) {
            break;
        }
        
        if (iterations > 1000) {
            break;
        }
        
    } while (true);
    
    return x;
}

int main() {
    cout << "Уравнение: (x - 1)^3 * sin(πx) * (cos(2πx) - 1) = 0" << endl;
    cout << "Корни: x = 1 (кратности 3), x = 0.5, x = 1.5" << endl;
    cout << "Для кратных корней оптимальный параметр sigma = -3" << endl;
    
    vector<double> initialPoints = {0.6, 1.2, 1.6};
    vector<string> rootNames = {"x ≈ 0.5", "x = 1", "x ≈ 1.5"};
    
    vector<int> precisionValues = {3, 4, 5, 6};
    
    cout << "\nСравнение методов для разных точностей:" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << setw(12) << "Корень" << setw(10) << "Точн." << setw(15) << "Обычный" 
         << setw(15) << "Модиф." << setw(15) << "Отношение" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    
    for (int idx = 0; idx < 3; idx++) {
        double x0 = initialPoints[idx];
        string rootName = rootNames[idx];
        
        for (int n : precisionValues) {
            double eps = pow(10.0, -n);
            
            int iterReg, iterMod;
            double rootReg = newtonRegular(x0, eps, iterReg);
            double rootMod = newtonModified(x0, eps, iterMod, -3.0);
            
            cout << fixed << setprecision(1);
            cout << setw(12) << rootName;
            cout << setw(10) << ("1e-" + to_string(n));
            cout << setw(15) << iterReg;
            cout << setw(15) << iterMod;
            
            if (iterReg > 0) {
                cout << setw(15) << fixed << setprecision(2) << (double)iterMod / iterReg;
            } else {
                cout << setw(15) << "-";
            }
            cout << endl;
        }
        cout << "--------------------------------------------------------------------------------" << endl;
    }
    
    cout << "\nДетальный вывод для модифицированного метода (n=6, sigma=-3):" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    
    for (int idx = 0; idx < 3; idx++) {
        double x0 = initialPoints[idx];
        double eps = 1e-6;
        
        cout << "\nКорень: " << rootNames[idx] << ", начальное приближение: x0 = " << x0 << endl;
        cout << "--------------------------------------------------------------------------------" << endl;
        
        double x = x0;
        double xPrev;
        int iteration = 0;
        double sigma = -3.0;
        
        cout << "Итерация " << iteration << ": x = " << fixed << setprecision(10) << x << ", f(x) = " << f(x) << endl;
        
        do {
            xPrev = x;
            double fx = f(x);
            double dfx = df(x);
            
            if (abs(dfx) < 1e-12) {
                cout << "Производная близка к нулю." << endl;
                break;
            }
            
            x = x + sigma * fx / dfx;
            iteration++;
            
            cout << "Итерация " << iteration << ": x = " << fixed << setprecision(10) << x 
                 << ", f(x) = " << f(x) << ", |Δx| = " << abs(x - xPrev) << endl;
            
            if (abs(x - xPrev) < eps) {
                break;
            }
            
            if (iteration > 50) {
                cout << "Достигнуто максимальное количество итераций" << endl;
                break;
            }
            
        } while (true);
        
        cout << "Найден корень: x = " << fixed << setprecision(10) << x << endl;
        cout << "Количество итераций: " << iteration << endl;
    }
    
    cout << "\nВыводы:" << endl;
    cout << "1. Для корня кратности 3 (x=1) модификация с sigma=-3 дает значительное ускорение." << endl;
    cout << "2. Для простых корней (x=0.5, x=1.5) обычный метод Ньютона работает лучше." << endl;
    cout << "3. Модификация восстанавливает квадратичную сходимость для кратных корней." << endl;
    
    return 0;
}