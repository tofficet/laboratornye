#include <iostream>
#include <cmath>
#include <functional>
#include <vector>
#include <iomanip>

using namespace std;

class EquationSolver {
private:
    double epsilon;
    int n;
    
public:
    EquationSolver(int precision) : n(precision) {
        epsilon = pow(10.0, -n);
    }
    
    void bisectionMethod(function<double(double)> f, double a, double b, const string& equationName) {
        cout << "\n=== Метод дихотомии для " << equationName << " ===" << endl;
        cout << "Точность: 10^-" << n << " = " << epsilon << endl;
        
        if (f(a) * f(b) >= 0) {
            cout << "На интервале [" << a << ", " << b << "] нет гарантированного корня или их четное количество" << endl;
            return;
        }
        
        double c;
        int iteration = 0;
        
        while ((b - a) / 2.0 > epsilon) {
            c = (a + b) / 2.0;
            iteration++;
            
            cout << "Итерация " << iteration << ": x = " << fixed << setprecision(10) << c 
                 << ", f(x) = " << f(c) << endl;
            
            if (f(c) == 0.0) {
                break;
            } else if (f(c) * f(a) < 0) {
                b = c;
            } else {
                a = c;
            }
        }
        
        cout << "Найден корень: x = " << fixed << setprecision(10) << c << endl;
        cout << "Количество итераций: " << iteration << endl;
    }
    
    void newtonMethod(function<double(double)> f, function<double(double)> df, 
                      double x0, const string& equationName) {
        cout << "\n=== Метод Ньютона для " << equationName << " ===" << endl;
        cout << "Точность: 10^-" << n << " = " << epsilon << endl;
        cout << "Начальное приближение: x0 = " << x0 << endl;
        
        double x = x0;
        double x_next;
        int iteration = 0;
        
        do {
            x_next = x - f(x) / df(x);
            iteration++;
            
            cout << "Итерация " << iteration << ": x = " << fixed << setprecision(10) << x_next 
                 << ", f(x) = " << f(x_next) << endl;
            
            if (abs(x_next - x) < epsilon) {
                break;
            }
            
            x = x_next;
        } while (true);
        
        cout << "Найден корень: x = " << fixed << setprecision(10) << x_next << endl;
        cout << "Количество итераций: " << iteration << endl;
    }
    
    void simpleIterationMethod(function<double(double)> phi, double x0, 
                                const string& equationName) {
        cout << "\n=== Метод простой итерации для " << equationName << " ===" << endl;
        cout << "Точность: 10^-" << n << " = " << epsilon << endl;
        cout << "Начальное приближение: x0 = " << x0 << endl;
        
        double x = x0;
        double x_next;
        int iteration = 0;
        
        do {
            x_next = phi(x);
            iteration++;
            
            cout << "Итерация " << iteration << ": x = " << fixed << setprecision(10) << x_next 
                 << endl;
            
            if (abs(x_next - x) < epsilon) {
                break;
            }
            
            x = x_next;
        } while (iteration < 1000);
        
        cout << "Найден корень: x = " << fixed << setprecision(10) << x_next << endl;
        cout << "Количество итераций: " << iteration << endl;
    }
};

int main() {
    int n;
    cout << "Введите точность n (10^-n): ";
    cin >> n;
    
    EquationSolver solver(n);
    
    auto f_a = [](double x) { return sin(x) - 2*x*x + 0.5; };
    auto df_a = [](double x) { return cos(x) - 4*x; };
    auto phi_a = [](double x) { return sqrt((sin(x) + 0.5) / 2); };
    
    cout << "\n=========================================";
    cout << "\nУравнение a: sin x - 2x^2 + 0.5 = 0";
    solver.bisectionMethod(f_a, -1, 0, "sin x - 2x^2 + 0.5 = 0 (корень 1)");
    solver.bisectionMethod(f_a, 0, 1, "sin x - 2x^2 + 0.5 = 0 (корень 2)");
    solver.newtonMethod(f_a, df_a, -0.5, "sin x - 2x^2 + 0.5 = 0 (корень 1)");
    solver.newtonMethod(f_a, df_a, 0.5, "sin x - 2x^2 + 0.5 = 0 (корень 2)");
    
    double a_val;
    int power;
    cout << "\n=========================================";
    cout << "\nУравнение b: x^n = a";
    cout << "\nВведите a (>0): ";
    cin >> a_val;
    cout << "Введите n (натуральное): ";
    cin >> power;
    
    auto f_b = [a_val, power](double x) { return pow(x, power) - a_val; };
    auto df_b = [power](double x) { return power * pow(x, power - 1); };
    auto phi_b = [a_val, power](double x) { return pow(a_val, 1.0/power); };
    
    solver.bisectionMethod(f_b, 0, max(1.0, a_val + 1), "x^" + to_string(power) + " = " + to_string(a_val));
    solver.newtonMethod(f_b, df_b, a_val > 1 ? a_val : 1, "x^" + to_string(power) + " = " + to_string(a_val));
    
    auto f_c = [](double x) { return sqrt(1 - x*x) - exp(x) + 0.1; };
    auto df_c = [](double x) { return -x / sqrt(1 - x*x) - exp(x); };
    
    cout << "\n=========================================";
    cout << "\nУравнение c: sqrt(1 - x^2) - e^x + 0.1 = 0";
    solver.bisectionMethod(f_c, -1, 1, "sqrt(1 - x^2) - e^x + 0.1 = 0");
    solver.newtonMethod(f_c, df_c, 0, "sqrt(1 - x^2) - e^x + 0.1 = 0");
    
    auto f_d = [](double x) { return x*x*x*x*x*x - 5*x*x*x - 2; };
    auto df_d = [](double x) { return 6*x*x*x*x*x - 15*x*x; };
    
    cout << "\n=========================================";
    cout << "\nУравнение d: x^6 = 5x^3 + 2";
    solver.bisectionMethod(f_d, -3, -1, "x^6 = 5x^3 + 2 (корень 1)");
    solver.bisectionMethod(f_d, 1, 3, "x^6 = 5x^3 + 2 (корень 2)");
    solver.newtonMethod(f_d, df_d, -2, "x^6 = 5x^3 + 2 (корень 1)");
    solver.newtonMethod(f_d, df_d, 2, "x^6 = 5x^3 + 2 (корень 2)");
    
    auto f_e = [](double x) { return log2(x) - 1.0/(1 + x*x); };
    auto df_e = [](double x) { return 1.0/(x * log(2)) + 2*x/((1 + x*x)*(1 + x*x)); };
    
    cout << "\n=========================================";
    cout << "\nУравнение e: log2(x) = 1/(1+x^2)";
    solver.bisectionMethod(f_e, 0.1, 2, "log2(x) = 1/(1+x^2)");
    solver.newtonMethod(f_e, df_e, 1, "log2(x) = 1/(1+x^2)");
    
    auto f_f = [](double x) { return sin(x/2) - 1; };
    auto df_f = [](double x) { return 0.5 * cos(x/2); };
    
    cout << "\n=========================================";
    cout << "\nУравнение f: sin(x/2) = 1 (первые 3 наименьших по модулю корня)";
    solver.bisectionMethod(f_f, 3, 4, "sin(x/2) = 1 (корень π)");
    solver.bisectionMethod(f_f, 10, 11, "sin(x/2) = 1 (корень 5π)");
    solver.bisectionMethod(f_f, -11, -10, "sin(x/2) = 1 (корень -3π)");
    
    auto f_g = [](double x) { return log(x) - 1; };
    auto df_g = [](double x) { return 1.0/x; };
    
    cout << "\n=========================================";
    cout << "\nУравнение g: ln x = 1";
    solver.bisectionMethod(f_g, 2, 3, "ln x = 1");
    solver.newtonMethod(f_g, df_g, 2, "ln x = 1");
    
    return 0;
}