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

void newtonMethodForMultipleRoots(double x0, double tolerance, int maxIterations, int rootNumber) {
    cout << "\nПоиск корня " << rootNumber << " с начальным приближением x0 = " << x0 << endl;
    cout << "Метод Ньютона для кратных корней" << endl;
    
    double x = x0;
    double xPrev;
    int iteration = 0;
    
    cout << "Итерация " << iteration << ": x = " << fixed << setprecision(10) << x << ", f(x) = " << f(x) << endl;
    
    do {
        xPrev = x;
        double fx = f(x);
        double dfx = df(x);
        
        if (abs(dfx) < 1e-12) {
            cout << "Производная близка к нулю. Метод расходится." << endl;
            return;
        }
        
        x = x - fx / dfx;
        iteration++;
        
        cout << "Итерация " << iteration << ": x = " << fixed << setprecision(10) << x 
             << ", f(x) = " << f(x) << ", |x - xPrev| = " << abs(x - xPrev) << endl;
        
        if (abs(x - xPrev) < tolerance) {
            break;
        }
        
        if (iteration >= maxIterations) {
            cout << "Достигнуто максимальное количество итераций" << endl;
            break;
        }
        
    } while (true);
    
    cout << "Найден корень: x = " << fixed << setprecision(10) << x << endl;
    cout << "Значение функции в корне: f(x) = " << f(x) << endl;
    cout << "Количество итераций: " << iteration << endl;
}

vector<double> findInitialApproximations(double start, double end, int numPoints) {
    vector<double> points;
    double step = (end - start) / numPoints;
    
    for (double x = start; x <= end; x += step) {
        points.push_back(x);
    }
    
    vector<double> roots;
    
    for (size_t i = 0; i < points.size() - 1; i++) {
        double x1 = points[i];
        double x2 = points[i + 1];
        
        if (f(x1) * f(x2) <= 0) {
            roots.push_back((x1 + x2) / 2);
        }
    }
    
    sort(roots.begin(), roots.end());
    
    return roots;
}

int main() {
    double tolerance = 1e-10;
    int maxIterations = 100;
    
    cout << "Уравнение: (x - 1)^3 * sin(πx) * (cos(2πx) - 1) = 0" << endl;
    cout << "Поиск первых трех положительных корней" << endl;
    cout << "Корни уравнения: x = 1, x = k (целые числа), x = m/2 (полуцелые)" << endl;
    
    vector<double> initialApproximations = findInitialApproximations(0, 5, 1000);
    
    vector<double> foundRoots;
    int rootCount = 0;
    
    for (double x0 : initialApproximations) {
        double x = x0;
        
        for (int iter = 0; iter < 50; iter++) {
            double fx = f(x);
            double dfx = df(x);
            
            if (abs(dfx) < 1e-12) break;
            
            double xNew = x - fx / dfx;
            
            if (abs(xNew - x) < 1e-8) {
                bool alreadyFound = false;
                for (double r : foundRoots) {
                    if (abs(r - xNew) < 1e-6) {
                        alreadyFound = true;
                        break;
                    }
                }
                
                if (!alreadyFound && xNew > 0) {
                    foundRoots.push_back(xNew);
                    rootCount++;
                    
                    if (rootCount <= 3) {
                        cout << "\nКорень " << rootCount << ":" << endl;
                        newtonMethodForMultipleRoots(xNew * 0.95, tolerance, maxIterations, rootCount);
                    }
                    
                    if (rootCount >= 6) break;
                }
                break;
            }
            
            x = xNew;
        }
        
        if (rootCount >= 6) break;
    }
    
    cout << "\nПервые три положительных корня:" << endl;
    for (int i = 0; i < min(3, (int)foundRoots.size()); i++) {
        cout << "Корень " << i + 1 << ": x = " << fixed << setprecision(10) << foundRoots[i] << endl;
    }
    
    return 0;
}