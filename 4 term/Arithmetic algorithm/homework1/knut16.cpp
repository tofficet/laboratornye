#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

using digit_t = uint32_t;
using double_digit_t = uint64_t;

pair<vector<digit_t>, digit_t> short_division(
    const vector<digit_t>& u,
    digit_t v,
    double_digit_t b
) {
    vector<digit_t> w(u.size(), 0);
    double_digit_t r = 0;
    
    for (size_t i = 0; i < u.size(); ++i) {
        double_digit_t current = r * b + u[i];
        w[i] = current / v;
        r = current % v;
    }
    
    return {w, static_cast<digit_t>(r)};
}

void printNumber(const vector<digit_t>& digits, const string& name) {
    cout << name << ": ";
    for (size_t i = 0; i < digits.size(); i++) {
        cout << digits[i];
        if (i < digits.size() - 1) cout << " ";
    }
    cout << endl;
}

vector<digit_t> inputNumber(const string& name) {
    int num_digits;
    cout << "Введите количество разрядов числа " << name << ": ";
    cin >> num_digits;
    
    vector<digit_t> digits(num_digits);
    cout << "Введите цифры числа от старшего разряда к младшему (через пробел):" << endl;
    cout << "(например, число 123 при основании 10 вводится как 1 2 3)" << endl;
    
    for (int i = 0; i < num_digits; i++) {
        cout << "  разряд " << i + 1 << ": ";
        cin >> digits[i];
    }
    
    return digits;
}

int main() {
    cout << "==========================================================" << endl;
    cout << "ДЕЛЕНИЕ МНОГОРАЗРЯДНЫХ ЧИСЕЛ (КОРОТКОЕ ДЕЛЕНИЕ)" << endl;
    cout << "==========================================================" << endl;
    cout << "Программа выполняет деление многоразрядного числа" << endl;
    cout << "на одноразрядное число (короткое деление) по алгоритму Кнута." << endl;
    cout << "----------------------------------------------------------" << endl;
    
    int choice;
    cout << "Выберите режим работы:" << endl;
    cout << "1 - Использовать тестовый пример (делимое: 12, делитель: 3, основание: 10)" << endl;
    cout << "2 - Ввести свои данные" << endl;
    cout << "Ваш выбор: ";
    cin >> choice;
    
    vector<digit_t> dividend;
    digit_t divisor;
    double_digit_t base;
    
    if (choice == 1) {
        //  12 / 3 в системе с основанием 10
        dividend = {1, 2}; 
        divisor = 3;
        base = 10;
        
        cout << "\n--- ТЕСТОВЫЙ ПРИМЕР ---" << endl;
        printNumber(dividend, "Делимое");
        cout << "Делитель: " << divisor << endl;
        cout << "Основание системы счисления: " << base << endl;
    } else {
        cout << "\n--- ВВОД ДАННЫХ ---" << endl;
        
        dividend = inputNumber("делимого");
        
        cout << "Введите делитель (одноразрядное число): ";
        cin >> divisor;
        
        cout << "Введите основание системы счисления: ";
        cin >> base;
        
        cout << "\n--- ВВЕДЁННЫЕ ДАННЫЕ ---" << endl;
        printNumber(dividend, "Делимое");
        cout << "Делитель: " << divisor << endl;
        cout << "Основание: " << base << endl;
    }
    
    cout << "\n--- ВЫПОЛНЕНИЕ ДЕЛЕНИЯ ---" << endl;
    cout << "----------------------------------------------------------" << endl;
    
    pair<vector<digit_t>, digit_t> result = short_division(dividend, divisor, base);
    vector<digit_t> quotient = result.first;
    digit_t remainder = result.second;
    
    printNumber(quotient, "Частное");
    cout << "Остаток: " << remainder << endl;
    
    if (choice == 1) {
        cout << "\nПроцесс деления (алгоритм Кнута):" << endl;
        cout << "  Шаг 1: r = 0" << endl;
        cout << "  Текущее = r·b + u[0] = 0·10 + 1 = 1" << endl;
        cout << "  w[0] = 1 / 3 = 0 (целая часть), r = 1 % 3 = 1" << endl;
        cout << "  Шаг 2: Текущее = r·b + u[1] = 1·10 + 2 = 12" << endl;
        cout << "  w[1] = 12 / 3 = 4, r = 12 % 3 = 0" << endl;
        cout << "  Результат: частное = [0, 4] (число 04 = 4), остаток = 0" << endl;
    }
    
      
    cout << "• Алгоритм короткого деления" << endl;
    cout << "  выполняет деление многоразрядного числа на одноразрядное." << endl;
    cout << "• На каждом шаге: текущее = r·b + u[i], w[i] = текущее / v, r = текущее % v" << endl;
    cout << "• Сложность алгоритма: O(n) операций, где n — количество разрядов." << endl;
    
    return 0;
}