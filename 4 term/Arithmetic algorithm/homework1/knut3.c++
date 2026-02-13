#include <iostream>
#include <vector>

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

    void setCoeff(int i, int j , double value){
        if (i+j<=n){
            coeffs[i][j]=value;
        }
    }

    double getCoeff(int i, int j) const{
        if (i+j<=n){
            return coeffs[i][j];
        }
        return 0.0;
    }

    double evaluate(double x, double y) const{
        int multCount=0;
        int addCount=0;
        vector<double> polyY(n+1, 0.0);

        for (int i=0; i<=n; i++){
            int degreeY=n-i;
            if (degreeY>=0){
                polyY[i]=coeffs[i][degreeY];

                for (int j=degreeY-1; j>=0; j--){
                    polyY[i]=polyY[i]*y+coeffs[i][j];
                    multCount++;
                    addCount++;
                }


            }
        }

        double result =polyY[n];

        for (int i=n-1; i>=0; i--){
            result= result*x+polyY[i];
            multCount++;
            addCount++;
        }
        cout << "Операций умножения: " << multCount << endl;
        cout << "Операций сложения: " << addCount << endl;
        cout << "Всего операций: " << (multCount + addCount) << endl;
        
        return result;
    }


    void print() const {
        bool first = true;
        for (int i = 0; i <= n; i++) {
            for (int j = 0; j <= n - i; j++) {
                double val = coeffs[i][j];
                if (val != 0) {
                    if (!first && val > 0) cout << " + ";
                    else if (val < 0) cout << " - ";
                    else if (first) cout << "";
                    
                    double absVal = abs(val);
                    if (absVal != 1 || (i == 0 && j == 0)) {
                        cout << absVal;
                    }
                    
                    if (i > 0) cout << "x";
                    if (i > 1) cout << "^" << i;
                    if (j > 0) cout << "y";
                    if (j > 1) cout << "^" << j;
                    
                    first = false;
                }
            }
        }
        if (first) cout << "0";
        cout << endl;
    }


};






int main() {
    cout << "Демонстрация метода Горнера для полинома от двух переменных\n\n";
    
    Polynom2D p(3);
    
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
    
    cout << "Полином: ";
    p.print();
    
    double x = 1, y = 2;
    cout << "\nВычисление при x = " << x << ", y = " << y << ":\n";
    double result = p.evaluate(x, y);
    cout << "Результат: " << result << "\n\n";
    
    return 0;
}