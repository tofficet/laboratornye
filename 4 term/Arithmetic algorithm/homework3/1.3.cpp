#include <iostream>
#include <cmath>

using namespace std;

double simpleIteration(double (*phi)(double), double x0, double eps, int maxIter)
{
    double x = x0;
    double xPrev;
    int iter = 0;

    cout << "Начальное приближение: " << x0 << endl;

    do
    {
        xPrev = x;
        x = phi(xPrev);
        iter++;

        cout << "Итерация " << iter << " : x = " << x << endl;

        if (iter >= maxIter)
        {
            cout << "Достигнут максимум итераций\n";
            break;
        }

        if (isnan(x) || isinf(x))
        {
            cout << "Ошибка вычислений\n";
            return xPrev;
        }

    } while (fabs(x - xPrev) > eps);

    cout << "Найденный корень: " << x << endl;
    cout << "Количество итераций: " << iter << endl;

    return x;
}

double phi_a1(double x)
{
    return cbrt(1 - 3 * x * x);
}

double phi_a2(double x)
{
    return -sqrt((1 - x * x * x) / 3.0);
}

double phi_a3(double x)
{
    return -sqrt((x * x * x - 1) / 3.0);
}

double f_a(double x)
{
    return x*x*x + 3*x*x - 1;
}

double phi_b1(double x)
{
    return pow(x, 4);
}

double phi_b2(double x)
{
    return pow(x, 0.75);
}

double f_b(double x)
{
    return x*x*x*x - x*x*x;
}

double phi_c1(double x)
{
    return (x*x + 2) / 3.0;
}

double phi_c2(double x)
{
    return sqrt(3*x - 2);
}

double f_c(double x)
{
    return x*x - 3*x + 2;
}

int main()
{
    double eps = 1e-10;
    int maxIter = 100;

    cout << "МЕТОД ПРОСТОЙ ИТЕРАЦИИ\n";
    cout << "a) x^3 + 3x^2 - 1 = 0\n\n";

    double r1 = simpleIteration(phi_a1, 0.5, eps, maxIter);

    double r2 = simpleIteration(phi_a2, -0.7, eps, maxIter);

    double r3 = simpleIteration(phi_a3, -3.0, eps, maxIter);

    cout << "b) x^4 - x^3 = 0\n\n";

    double r4 = simpleIteration(phi_b1, 0.2, eps, maxIter);

    double r5 = simpleIteration(phi_b2, 0.9, eps, maxIter);

    cout << "c) x^2 - 3x + 2 = 0\n\n";

    double r6 = simpleIteration(phi_c1, 1.5, eps, maxIter);

    double r7 = simpleIteration(phi_c2, 2.5, eps, maxIter);

    return 0;
}