#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

const int MAX_ELEMENTS = 100;
const int MAX_PAIRS = 1000;
const int MAX_STRING_LENGTH = 50;

struct Pair {
    char first[MAX_STRING_LENGTH];
    char second[MAX_STRING_LENGTH];
};

class RelationAnalyzer {
private:
    char elements[MAX_ELEMENTS][MAX_STRING_LENGTH];
    int elementCount;
    Pair pairs[MAX_PAIRS];
    int pairCount;
    
    bool hasPair(const char* a, const char* b) {
        for (int i = 0; i < pairCount; i++) {
            if (strcmp(pairs[i].first, a) == 0 && strcmp(pairs[i].second, b) == 0) {
                return true;
            }
        }
        return false;
    }
    
    int getElementIndex(const char* elem) {
        for (int i = 0; i < elementCount; i++) {
            if (strcmp(elements[i], elem) == 0) return i;
        }
        return -1;
    }

public:
    RelationAnalyzer() : elementCount(0), pairCount(0) {
        // Инициализация массивов
        for (int i = 0; i < MAX_ELEMENTS; i++) {
            elements[i][0] = '\0';
        }
        for (int i = 0; i < MAX_PAIRS; i++) {
            pairs[i].first[0] = '\0';
            pairs[i].second[0] = '\0';
        }
    }
    
    void readFromFile(const char* filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Ошибка открытия файла!" << endl;
            return;
        }
        
        char line[1000];
        file.getline(line, 1000);
        
        char* token = strtok(line, " ");
        while (token != nullptr && elementCount < MAX_ELEMENTS) {
            strncpy(elements[elementCount], token, MAX_STRING_LENGTH - 1);
            elements[elementCount][MAX_STRING_LENGTH - 1] = '\0';
            elementCount++;
            token = strtok(nullptr, " ");
        }
        
        // Чтение пар отношения
        while (file.getline(line, 1000) && pairCount < MAX_PAIRS) {
            char first[MAX_STRING_LENGTH];
            char second[MAX_STRING_LENGTH];
            
            // Используем strtok для разделения строки на две части
            char* token1 = strtok(line, " ");
            char* token2 = strtok(nullptr, " ");
            
            if (token1 != nullptr && token2 != nullptr) {
                strncpy(pairs[pairCount].first, token1, MAX_STRING_LENGTH - 1);
                pairs[pairCount].first[MAX_STRING_LENGTH - 1] = '\0';
                strncpy(pairs[pairCount].second, token2, MAX_STRING_LENGTH - 1);
                pairs[pairCount].second[MAX_STRING_LENGTH - 1] = '\0';
                pairCount++;
            }
        }
        
        file.close();
    }
    
    bool isReflexive() {
        for (int i = 0; i < elementCount; i++) {
            if (!hasPair(elements[i], elements[i])) {
                return false;
            }
        }
        return true;
    }
    
    bool isAntiReflexive() {
        for (int i = 0; i < elementCount; i++) {
            if (hasPair(elements[i], elements[i])) {
                return false;
            }
        }
        return true;
    }

    bool isSymmetric() {
        for (int i = 0; i < pairCount; i++) {
            if (!hasPair(pairs[i].second, pairs[i].first)) {
                return false;
            }
        }
        return true;
    }
    
    bool isAntiSymmetric() {
        for (int i = 0; i < pairCount; i++) {
            if (strcmp(pairs[i].first, pairs[i].second) != 0 && 
                hasPair(pairs[i].second, pairs[i].first)) {
                return false;
            }
        }
        return true;
    }
    
    bool isTransitive() {
        for (int i = 0; i < pairCount; i++) {
            for (int j = 0; j < pairCount; j++) {
                if (strcmp(pairs[i].second, pairs[j].first) == 0) {
                    if (!hasPair(pairs[i].first, pairs[j].second)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    
    bool isConnected() {
        for (int i = 0; i < elementCount; i++) {
            for (int j = i + 1; j < elementCount; j++) {
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
    
    bool isEquivalence() {
        return isReflexive() && isSymmetric() && isTransitive();
    }
    
    bool isOrder() {
        return isReflexive() && isAntiSymmetric() && isTransitive();
    }
    
    void findEquivalenceClasses() {
        bool visited[MAX_ELEMENTS] = {false};
        int classCount = 0;
        
        cout << "Классы эквивалентности:" << endl;
        for (int i = 0; i < elementCount; i++) {
            if (!visited[i]) {
                cout << "Класс " << ++classCount << ": {";
                cout << elements[i];
                
                for (int j = i + 1; j < elementCount; j++) {
                    if (!visited[j] && hasPair(elements[i], elements[j]) && 
                        hasPair(elements[j], elements[i])) {
                        cout << ", " << elements[j];
                        visited[j] = true;
                    }
                }
                cout << "}" << endl;
                visited[i] = true;
            }
        }
        cout << "Индекс разбиения: " << classCount << endl;
    }
    
    void findMinimalElements() {
        cout << "Минимальные элементы: {";
        bool first = true;
        
        for (int i = 0; i < elementCount; i++) {
            bool isMinimal = true;
            for (int j = 0; j < elementCount; j++) {
                if (i != j && hasPair(elements[j], elements[i])) {
                    isMinimal = false;
                    break;
                }
            }
            if (isMinimal) {
                if (!first) cout << ", ";
                cout << elements[i];
                first = false;
            }
        }
        cout << "}" << endl;
    }

    void findMaximalElements() {
        cout << "Максимальные элементы: {";
        bool first = true;
        
        for (int i = 0; i < elementCount; i++) {
            bool isMaximal = true;
            for (int j = 0; j < elementCount; j++) {
                if (i != j && hasPair(elements[i], elements[j])) {
                    isMaximal = false;
                    break;
                }
            }
            if (isMaximal) {
                if (!first) cout << ", ";
                cout << elements[i];
                first = false;
            }
        }
        cout << "}" << endl;
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
            findEquivalenceClasses();
        }
        
        if (isOrder()) {
            cout << "\nОтношение является отношением порядка!" << endl;
            findMinimalElements();
            findMaximalElements();
        }
    }
};

int main() {
    char filename[100];
    cout << "Введите название файла: ";
    cin >> filename;
    
    RelationAnalyzer analyzer;
    analyzer.readFromFile(filename);
    analyzer.analyze();
    
    return 0;
}