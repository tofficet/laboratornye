#include <iostream>
#include <iomanip>
#include "wiener_attack.h"

using namespace std;

int main() {
    cout << "АТАКА ВИНЕРА НА RSA" << endl;
    
    VulnerableRSAKeyGenerator generator;
    auto keys = generator.generateVulnerableToWiener(64, 0.99);
    
    cout << "\nПОСТУПИЛ ПУБЛИЧНЫЙ КЛЮЧ" << endl;
    cout << "n = " << keys.n << endl;
    cout << "e = " << keys.e << endl;
    
    cout << "\nАТАКА ВИНЕРА" << endl;
    cout << "Атакую..." << endl;
    
    WienerAttack attack;
    auto [p_found, q_found] = attack.attack(keys.n, keys.e);
    
    cout << "Найдено p = " << p_found << endl;
    cout << "Найдено q = " << q_found << endl;
    
    cout << "\nПРОВЕРКА" << endl;
    cout << "p * q = " << p_found * q_found << endl;
    cout << "n = " << keys.n << endl;
    
    if (p_found * q_found == keys.n) {
        cout << "\nАтака Винера успешна" << endl;
        cout << "p = " << p_found << ", q = " << q_found << endl;
    } else {
        cout << "\nАтака провалилась" << endl;
    }
    
    return 0;
}