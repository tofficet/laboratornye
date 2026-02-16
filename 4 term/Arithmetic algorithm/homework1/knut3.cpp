#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <iomanip>

using namespace std;

class Polynom2D
{
private:
    int n;
    vector<vector<double>> coeffs;
    
public:
    Polynom2D(int degree) : n(degree){
        coeffs.resize(n+1);
        for(int i=0; i<=n; i++){
            coeffs[i].resize(n-i+1, 0.0);
        }
    }

    void setCoeff(int i, int j, double value){
        if (i+j <= n){
            coeffs[i][j] = value;
        }
    }

    double getCoeff(int i, int j) const{
        if (i+j <= n){
            return coeffs[i][j];
        }
        return 0.0;
    }

    double evaluate(double x, double y) const{
        int multCount = 0;
        int addCount = 0;
        vector<double> polyY(n+1, 0.0);

        for (int i = 0; i <= n; i++){
            int degreeY = n - i;
            if (degreeY >= 0){
                polyY[i] = coeffs[i][degreeY];

                for (int j = degreeY - 1; j >= 0; j--){
                    polyY[i] = polyY[i] * y + coeffs[i][j];
                    multCount++;
                    addCount++;
                }
            }
        }

        double result = polyY[n];

        for (int i = n-1; i >= 0; i--){
            result = result * x + polyY[i];
            multCount++;
            addCount++;
        }
        
        cout << "  Операций умножения: " << multCount << endl;
        cout << "  Операций сложения: " << addCount << endl;
        cout << "  Всего операций: " << (multCount + addCount) << endl;
        
        return result;
    }

    void print() const {
        bool first = true;
        for (int i = 0; i <= n; i++) {
            for (int j = 0; j <= n - i; j++) {
                double val = coeffs[i][j];
                if (abs(val) < 1e-10) continue;
                
                if (!first && val > 0) cout << " + ";
                else if (!first && val < 0) cout << " - ";
                else if (first && val < 0) cout << "-";
                else if (first) cout << "";
                
                double absVal = abs(val);
                if (abs(absVal - 1.0) > 1e-10 || (i == 0 && j == 0)) {
                    cout << absVal;
                }
                
                if (i > 0) cout << "x";
                if (i > 1) cout << "^" << i;
                if (j > 0) cout << "y";
                if (j > 1) cout << "^" << j;
                
                first = false;
            }
        }
        if (first) cout << "0";
        cout << endl;
    }
    
    int getDegree() const {
        return n;
    }
};

void inputPolynomial2D(Polynom2D& p) {
    int n = p.getDegree();
    cout << "Введите коэффициенты полинома (для x^i y^j, где i+j ≤ " << n << "):" << endl;
    
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n - i; j++) {
            double val;
            cout << "  коэффициент при x";
            if (i > 0) {
                cout << "^" << i;
                if (j > 0) cout << "y";
            } else {
                if (j > 0) cout << "y";
            }
            if (j > 1) cout << "^" << j;
            cout << " = ";
            cin >> val;
            p.setCoeff(i, j, val);
        }
    }
}

int main() {
    cout << "==========================================================" << endl;
    cout << "МЕТОД ГОРНЕРА ДЛЯ ПОЛИНОМА ОТ ДВУХ ПЕРЕМЕННЫХ" << endl;
    cout << "==========================================================" << endl;
    cout << "Программа вычисляет значение полинома P(x,y) степени n" << endl;
    cout << "используя двумерную схему Горнера." << endl;
    cout << "----------------------------------------------------------" << endl;

    cout << "Двумерная схема Горнера сначала сворачивает полином по y," << endl;
        cout << "получая полиномы от x, а затем сворачивает по x." << endl;
        cout << "Количество операций: O(n²/2) умножений и сложений." << endl;
        cout << "Это оптимальный метод вычисления полиномов от двух переменных." << endl;
    
    int choice;
    cout << "Выберите режим работы:" << endl;
    cout << "1 - Использовать тестовый пример (полином 3-й степени со всеми коэффициентами = 1)" << endl;
    cout << "2 - Ввести свои данные" << endl;
    cout << "Ваш выбор: ";
    cin >> choice;
    
    int degree;
    double x, y;
    
    if (choice == 1) {
        degree = 3;
        Polynom2D p(degree);
        
       
        p.setCoeff(0, 0, 1);  
        p.setCoeff(1, 0, 1);  
        p.setCoeff(0, 1, 1); 
        p.setCoeff(2, 0, 1);  
        p.setCoeff(1, 1, 1);  
        p.setCoeff(0, 2, 1);  
        p.setCoeff(3, 0, 1);  
        p.setCoeff(2, 1, 1);  
        p.setCoeff(1, 2, 1);  
        p.setCoeff(0, 3, 1); 
        
        x = 1.0;
        y = 2.0;
        
        cout << "\n--- ТЕСТОВЫЙ ПРИМЕР ---" << endl;
        cout << "Полином степени " << degree << ":" << endl;
        p.print();
        cout << "x = " << x << ", y = " << y << endl;
    } else {
        cout << "\n--- ВВОД ДАННЫХ ---" << endl;
        
        cout << "Введите степень полинома n: ";
        cin >> degree;
        
        

        Polynom2D p(degree);
        inputPolynomial2D(p);
        
        cout << "Введите значение x: ";
        cin >> x;
        
        cout << "Введите значение y: ";
        cin >> y;
        
        cout << "\n--- ВВЕДЁННЫЕ ДАННЫЕ ---" << endl;
        cout << "Полином степени " << degree << ":" << endl;
        p.print();
        cout << "x = " << x << ", y = " << y << endl;
        cout << "\n--- ВЫЧИСЛЕНИЕ ---" << endl;
        cout << "----------------------------------------------------------" << endl;
        
        double result = p.evaluate(x, y);
        cout << "Результат: P(" << x << ", " << y << ") = " << result << endl;
        cout << "----------------------------------------------------------" << endl;
        
        
        
        return 0;
    }
    
    
    return 0;
}