#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Pair {
    string first;
    string second;
};

class RelationAnalyzer {
private:
    vector<string> elements;
    vector<Pair> pairs;
    
    // Проверка существования пары в отношении
    bool hasPair(const string& a, const string& b) {
        for (const auto& pair : pairs) {
            if (pair.first == a && pair.second == b) {
                return true;
            }
        }
        return false;
    }
    
    // Получить индекс элемента
    int getElementIndex(const string& elem) {
        for (int i = 0; i < elements.size(); i++) {
            if (elements[i] == elem) return i;
        }
        return -1;
    }
    
public:
    void readFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Ошибка открытия файла!" << endl;
            return;
        }
        
        // Чтение элементов множества
        string line;
        getline(file, line);
        
        string element = "";
        for (char c : line) {
            if (c == ' ') {
                if (!element.empty()) {
                    elements.push_back(element);
                    element = "";
                }
            } else {
                element += c;
            }
        }
        if (!element.empty()) {
            elements.push_back(element);
        }
        
        // Чтение пар отношения
        while (getline(file, line)) {
            Pair pair;
            string current = "";
            bool isFirst = true;
            
            for (char c : line) {
                if (c == ' ') {
                    if (isFirst) {
                        pair.first = current;
                        current = "";
                        isFirst = false;
                    }
                } else {
                    current += c;
                }
            }
            pair.second = current;
            
            if (!pair.first.empty() && !pair.second.empty()) {
                pairs.push_back(pair);
            }
        }
        
        file.close();
    }
    
    bool isReflexive() {
        for (const auto& elem : elements) {
            if (!hasPair(elem, elem)) {
                return false;
            }
        }
        return true;
    }
    
    bool isAntiReflexive() {
        for (const auto& elem : elements) {
            if (hasPair(elem, elem)) {
                return false;
            }
        }
        return true;
    }


    bool isSymmetric() {
        for (const auto& pair : pairs) {
            if (!hasPair(pair.second, pair.first)) {
                return false;
            }
        }
        return true;
    }
    
    bool isAntiSymmetric() {
        for (const auto& pair : pairs) {
            if (pair.first != pair.second && hasPair(pair.second, pair.first)) {
                return false;
            }
        }
        return true;
    }
    
    bool isTransitive() {
        for (const auto& p1 : pairs) {
            for (const auto& p2 : pairs) {
                if (p1.second == p2.first) {
                    if (!hasPair(p1.first, p2.second)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    
    // Полнота (связность)
    bool isConnected() {
        for (int i = 0; i < elements.size(); i++) {
            for (int j = i + 1; j < elements.size(); j++) {
                if (!hasPair(elements[i], elements[j]) && 
                    !hasPair(elements[j], elements[i])) {
                    return false;
                }
            }
        }
        return true;
    }
    
    bool isAsymmetric() {
        return isAntiReflexive() && isAntiSymmetric();
    }
    
    // Проверка отношения эквивалентности
    bool isEquivalence() {
        return isReflexive() && isSymmetric() && isTransitive();
    }
    
    // Проверка отношения порядка
    bool isOrder() {
        return isReflexive() && isAntiSymmetric() && isTransitive();
    }
    
    // Нахождение классов эквивалентности
    vector<vector<string>> findEquivalenceClasses() {
        vector<vector<string>> classes;
        vector<bool> visited(elements.size(), false);
        
        for (int i = 0; i < elements.size(); i++) {
            if (!visited[i]) {
                vector<string> equivalenceClass;
                
                for (int j = 0; j < elements.size(); j++) {
                    if (hasPair(elements[i], elements[j]) && 
                        hasPair(elements[j], elements[i])) {
                        equivalenceClass.push_back(elements[j]);
                        visited[j] = true;
                    }
                }
                
                classes.push_back(equivalenceClass);
            }
        }
        
        return classes;
    }
    
    vector<string> findMinimalElements() {
        vector<string> minimal;
        
        for (int i = 0; i < elements.size(); i++) {
            bool isMinimal = true;
            for (int j = 0; j < elements.size(); j++) {
                if (i != j && hasPair(elements[j], elements[i])) {
                    isMinimal = false;
                    break;
                }
            }
            if (isMinimal) {
                minimal.push_back(elements[i]);
            }
        }
        
        return minimal;
    }

    vector<string> findMaximalElements() {
        vector<string> maximal;
        
        for (int i = 0; i < elements.size(); i++) {
            bool isMaximal = true;
            for (int j = 0; j < elements.size(); j++) {
                if (i != j && hasPair(elements[i], elements[j])) {
                    isMaximal = false;
                    break;
                }
            }
            if (isMaximal) {
                maximal.push_back(elements[i]);
            }
        }
        
        return maximal;
    }
    
    void analyze() {
        cout << "Свойства отношения:" << endl;
        cout << "Рефлексивность: " << (isReflexive() ? "+" : "-") << endl;
        cout << "Антирефлексивность: " << (isAntiReflexive() ? "+" : "-") << endl;
        cout << "Симметричность: " << (isSymmetric() ? "+" : "-") << endl;
        cout << "Антисимметричность: " << (isAntiSymmetric() ? "+" : "-") << endl;
        cout << "Транзитивность: " << (isTransitive() ? "+" : "-") << endl;
        cout << "Полнота: " << (isConnected() ? "+" : "-") << endl;
        cout << "Ассиметричность: " << (isAsymmetric() ? "+" : "-") << endl;
        
        if (isEquivalence()) {
            cout << "\nОтношение является отношением эквивалентности!" << endl;
            
            auto classes = findEquivalenceClasses();
            cout << "Классы эквивалентности (" << classes.size() << "):" << endl;
            for (int i = 0; i < classes.size(); i++) {
                cout << "Класс " << i + 1 << ": {";
                for (int j = 0; j < classes[i].size(); j++) {
                    cout << classes[i][j];
                    if (j < classes[i].size() - 1) cout << ", ";
                }
                cout << "}" << endl;
            }
            cout << "Индекс разбиения: " << classes.size() << endl;
        }
        
        if (isOrder()) {
            cout << "\nОтношение является отношением порядка!" << endl;
            
            auto minimal = findMinimalElements();
            auto maximal = findMaximalElements();
            
            cout << "Минимальные элементы: {";
            for (int i = 0; i < minimal.size(); i++) {
                cout << minimal[i];
                if (i < minimal.size() - 1) cout << ", ";
            }
            cout << "}" << endl;
            
            cout << "Максимальные элементы: {";
            for (int i = 0; i < maximal.size(); i++) {
                cout << maximal[i];
                if (i < maximal.size() - 1) cout << ", ";
            }
            cout << "}" << endl;
        }
    }
};

int main() {
    string filename;
    cout << "Введите название файла: ";
    cin >> filename;
    
    RelationAnalyzer analyzer;
    analyzer.readFromFile(filename);
    analyzer.analyze();
    
    return 0;
}