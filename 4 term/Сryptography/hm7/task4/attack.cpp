#include <iostream>
#include <iomanip>
#include "wiener_attack.h"

using namespace std;

int main() {
    cout << "АТАКА ВИНЕРА НА RSA" << endl;
    
    uint64_t n = 12131299;
    uint64_t e = 9326357;
    uint64_t d = 13;
    uint64_t p = 4013;
    uint64_t q = 3023;
    
    cout << "\nПОСТУПИЛ ПУБЛИЧНЫЙ КЛЮЧ" << endl;
    cout << "n = " << n << endl;
    cout << "e = " << e << endl;
    
    cout << "\nАТАКА ВИНЕРА" << endl;
    cout << "Атакую..." << endl;
    
    WienerAttack attack;
    auto [p_found, q_found] = attack.attack(n, e);
    
    cout << "Найдено p = " << p_found << endl;
    cout << "Найдено q = " << q_found << endl;
    
    cout << "\nПРОВЕРКА" << endl;
    cout << "p * q = " << p_found * q_found << endl;
    cout << "n = " << n << endl;
    
    if (p_found * q_found == n) {
        cout << "\nАтака Винера успешна" << endl;
        cout << "p = " << p_found << ", q = " << q_found << endl;
    } else {
        cout << "\nАтака провалилась" << endl;
    }
    
    return 0;
}