#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "continued_fraction.h"
#include "stern-brocot_tree.h"
#include "calkin-wilf_tree.h"

using namespace std;

void printBits(const vector<bool>& bits) {
    for (bool b : bits) {
        cout << (b ? '1' : '0');
    }
}

void printFractions(const vector<pair<uint64_t, uint64_t>>& fracs) {
    for (auto [p, q] : fracs) {
        cout << p << "/" << q << " ";
    }
    cout << endl;
}

int main() {
    cout << "1. ЦЕПНЫЕ ДРОБИ" << endl;
    
    vector<pair<uint64_t, uint64_t>> fractions = {
        {10, 7},
        {3, 5},
        {4, 3},
        {13, 8},
        {5, 2}
    };
    
    for (auto [a, b] : fractions) {
        cout << "\nДробь: " << a << "/" << b << endl;
        
        auto cf = ContinuedFraction::fromFraction(a, b);
        cout << "  Цепная дробь: [";
        for (size_t i = 0; i < cf.size(); i++) {
            cout << cf[i];
            if (i + 1 < cf.size()) 
                cout << (i == 0 ? "; " : ", ");
        }
        cout << "]" << endl;
        
        auto restored = ContinuedFraction::toFraction(cf);
        cout << "  Восстановлено: " << restored.first << "/" << restored.second;
        
        auto conv = ContinuedFraction::suitabFractions(cf);
        cout << "\n  Подходящие дроби: ";
        for (auto [p, q] : conv) {
            cout << p << "/" << q << " ";
        }
        cout << endl;
    }
    
    cout << "\n2. ДЕРЕВО ШТЕРНА-БРОКО" << endl;
    
    vector<pair<uint64_t, uint64_t>> sbFractions = {
        {1, 1},
        {1, 2},
        {2, 1},
        {1, 3},
        {2, 3},
        {3, 2},
        {3, 1},
        {3, 5},
        {4, 3},
        {5, 3}
    };
    
    for (auto [a, b] : sbFractions) {
        cout << "\nДробь: " << a << "/" << b << endl;
        
        auto path = SternBrocotTree::pathToFraction(a, b);
        cout << "  Битовый путь: ";
        printBits(path);
        cout << endl;
        
        auto frac = SternBrocotTree::fractionByPath(path);
        cout << "  По пути получено: " << frac.first << "/" << frac.second;
        
        auto allFracs = SternBrocotTree::allFractionsByPath(path);
        cout << "\n  Все дроби по пути: ";
        for (auto [p, q] : allFracs) {
            cout << p << "/" << q << " ";
        }
        cout << endl;
    }
    
    cout << "\n3. ДЕРЕВО КАЛКИНА-УИЛФА" << endl;
    
    vector<pair<uint64_t, uint64_t>> cwFractions = {
        {1, 1},
        {1, 2},
        {2, 1},
        {1, 3},
        {3, 2},
        {2, 3},
        {3, 1},
        {3, 5},
        {4, 3},
        {5, 3}
    };
    
    for (auto [a, b] : cwFractions) {
        cout << "\nДробь: " << a << "/" << b << endl;
        
        auto path = CalkinWilfTree::pathToFraction(a, b);
        cout << "  Битовый путь: ";
        printBits(path);
        cout << endl;
        
        auto frac = CalkinWilfTree::fractionByPath(path);
        cout << "  По пути получено: " << frac.first << "/" << frac.second;
    }

    cout << endl;
    
    return 0;
}