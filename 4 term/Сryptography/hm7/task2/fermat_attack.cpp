#include <iostream>
#include <iomanip>
#include "../vulnerable_rsa_keygen.h"
#include "fermat_attack.h"

using namespace std;

int main() {
    cout << "АТАКА ФЕРМА НА RSA" << endl;
    
    VulnerableRSAKeyGenerator generator;
    auto keys = generator.generateVulnerableToFermat(32, 0.99);
    
    cout << "\nПОСТУПИЛ ПУБЛИЧНЫЙ КЛЮЧ" << endl;
    cout << "n = " << keys.n << endl;
    cout << "e = " << keys.e << endl;
    
    cout << "\nАТАКА ФЕРМА" << endl;
    cout << "n = " << keys.n << endl;
    cout << "Атакую..." << endl;
    
    FermatAttack attack;
    auto [p_found, q_found] = attack.attack(keys.n);
    
    cout << "Найдено p = " << p_found << endl;
    cout << "Найдено q = " << q_found << endl;
    
    cout << "\nПРОВЕРКА" << endl;
    cout << "p * q = " << p_found * q_found << endl;
    cout << "n = " << keys.n << endl;
    
    if (p_found * q_found == keys.n) {
        cout << "\nАтака Ферма успешна hehe" << endl;
        cout << "p = " << p_found << ", q = " << q_found << endl;
    } else {
        cout << "\nАтака провалилась =(" << endl;
    }
    
    return 0;
}