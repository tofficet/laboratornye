#include <iostream>
#include <cstring>
#include <cctype>


struct ListNode {
    char data;
    ListNode* next;
    
    ListNode(char val) : data(val), next(nullptr) {}
};

struct Set {
    char name;
    ListNode* elements;
    Set* next;
    
    Set(char n) : name(n), elements(nullptr), next(nullptr) {}
};

Set* setsHead = nullptr;

bool isPrintableChar(char c) {
    return c >= 32 && c <= 126;
}

Set* findSet(char name) {
    Set* current = setsHead;
    while (current != nullptr) {
        if (current->name == name) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void insertSorted(ListNode** head, char element) {
    ListNode* newNode = new ListNode(element);
    
    if (*head == nullptr || element < (*head)->data) {
        newNode->next = *head;
        *head = newNode;
        return;
    }
    
    ListNode* current = *head;
    while (current->next != nullptr && current->next->data < element) {
        current = current->next;
    }

    if (current->data == element) {
        delete newNode;
        return;
    }
    
    newNode->next = current->next;
    current->next = newNode;
}

void removeFromList(ListNode** head, char element) {
    if (*head == nullptr) return;
    
    if ((*head)->data == element) {
        ListNode* temp = *head;
        *head = (*head)->next;
        delete temp;
        return;
    }
    
    ListNode* current = *head;
    while (current->next != nullptr && current->next->data != element) {
        current = current->next;
    }
    
    if (current->next != nullptr && current->next->data == element) {
        ListNode* temp = current->next;
        current->next = current->next->next;
        delete temp;
    }
}

bool contains(ListNode* head, char element) {
    ListNode* current = head;
    while (current != nullptr) {
        if (current->data == element) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void printSet(Set* set) {
    std::cout << "{";
    ListNode* current = set->elements;
    bool first = true;
    while (current != nullptr) {
        if (!first) std::cout << ", ";
        std::cout << current->data;
        first = false;
        current = current->next;
    }
    std::cout << "}";
}

void newSet(char name) {
    if (name < 'A' || name > 'Z') {
        std::cout << "Имя множества должно быть от A до Z" << std::endl;
        return;
    }
    
    if (findSet(name) != nullptr) {
        std::cout << "Множество " << name << " уже существует" << std::endl;
        return;
    }
    
    Set* newSet = new Set(name);
    newSet->next = setsHead;
    setsHead = newSet;
    
    std::cout << "Множество " << name << " создано" << std::endl;
}

void deleteSet(char name) {
    if (setsHead == nullptr) {
        std::cout << "Множество " << name << " не найдено" << std::endl;
        return;
    }
    
    if (setsHead->name == name) {
        Set* temp = setsHead;
        setsHead = setsHead->next;
        
        ListNode* current = temp->elements;
        while (current != nullptr) {
            ListNode* next = current->next;
            delete current;
            current = next;
        }
        delete temp;
        
        std::cout << "Множество " << name << " удалено" << std::endl;
        return;
    }
    
    Set* current = setsHead;
    while (current->next != nullptr && current->next->name != name) {
        current = current->next;
    }
    
    if (current->next != nullptr && current->next->name == name) {
        Set* temp = current->next;
        current->next = current->next->next;
        
        ListNode* elemCurrent = temp->elements;
        while (elemCurrent != nullptr) {
            ListNode* next = elemCurrent->next;
            delete elemCurrent;
            elemCurrent = next;
        }
        delete temp;
        
        std::cout << "Множество " << name << " удалено" << std::endl;
    } else {
        std::cout << "Множество " << name << " не найдено" << std::endl;
    }
}

void addElement(char setName, char element) {   //add A x
    if (!isPrintableChar(element)) {
        std::cout << "Элемент должен быть печатным символом" << std::endl;
        return;
    }
    
    Set* set = findSet(setName);
    if (set == nullptr) {
        std::cout << "Множество " << setName << " не найдено" << std::endl;
        return;
    }
    
    insertSorted(&(set->elements), element);
    std::cout << "Элемент '" << element << "' добавлен в множество " << setName << std::endl;
}

void removeElement(char setName, char element) {   //rem A x
    Set* set = findSet(setName);
    if (set == nullptr) {
        std::cout << "Множество " << setName << " не найдено" << std::endl;
        return;
    }
    
    if (contains(set->elements, element)) {
        removeFromList(&(set->elements), element);
        std::cout << "Элемент '" << element << "' удален из множества " << setName << std::endl;
    } else {
        std::cout << "Элемент '" << element << "' не найден в множестве " << setName << std::endl;
    }
}

void powerSet(char setName) {     //pow A 
    Set* set = findSet(setName);
    if (set == nullptr) {
        std::cout << "Множество " << setName << " не найдено" << std::endl;
        return;
    }
    
    int count = 0;
    ListNode* current = set->elements;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    
    char* elements = new char[count];
    current = set->elements;
    for (int i = 0; i < count; i++) {
        elements[i] = current->data;
        current = current->next;
    }
    
    std::cout << "Булеан множества " << setName << ":" << std::endl;
    std::cout << "{∅";
    
    int totalSubsets = 1 << count;
    for (int i = 1; i < totalSubsets; i++) {
        std::cout << ", {";
        bool first = true;
        for (int j = 0; j < count; j++) {
            if (i & (1 << j)) {
                if (!first) std::cout << ", ";
                std::cout << elements[j];
                first = false;
            }
        }
        std::cout << "}";
    }
    std::cout << "}" << std::endl;
    
    delete[] elements;
}

void seeSets(char setName = '\0') {   //see [A] 
    if (setName == '\0') {
        if (setsHead == nullptr) {
            std::cout << "Нет множеств" << std::endl;
            return;
        }
        
        Set* current = setsHead;
        while (current != nullptr) {
            std::cout << current->name << ": ";
            printSet(current);
            std::cout << std::endl;
            current = current->next;
        }
    } else {
        Set* set = findSet(setName);
        if (set == nullptr) {
            std::cout << "Множество " << setName << " не найдено" << std::endl;
            return;
        }
        
        std::cout << setName << ": ";
        printSet(set);
        std::cout << std::endl;
    }
}

void unionSets(char a, char b) {   // A + B 
    Set* setA = findSet(a);
    Set* setB = findSet(b);
    
    if (setA == nullptr || setB == nullptr) {
        std::cout << "Одно из множеств не найдено" << std::endl;
        return;
    }
    
    std::cout << "Объединение " << a << " + " << b << ": {";
    
    ListNode* currA = setA->elements;
    ListNode* currB = setB->elements;
    bool first = true;
    
    while (currA != nullptr && currB != nullptr) {
        if (!first) std::cout << ", ";
        
        if (currA->data < currB->data) {
            std::cout << currA->data;
            currA = currA->next;
        } else if (currB->data < currA->data) {
            std::cout << currB->data;
            currB = currB->next;
        } else {
            std::cout << currA->data;
            currA = currA->next;
            currB = currB->next;
        }
        first = false;
    }

    while (currA != nullptr) {
        if (!first) std::cout << ", ";
        std::cout << currA->data;
        currA = currA->next;
        first = false;
    }
    
    while (currB != nullptr) {
        if (!first) std::cout << ", ";
        std::cout << currB->data;
        currB = currB->next;
        first = false;
    }
    
    std::cout << "}" << std::endl;
}


void intersectionSets(char a, char b) {    //A & B
    Set* setA = findSet(a);
    Set* setB = findSet(b);
    
    if (setA == nullptr || setB == nullptr) {
        std::cout << "Одно из множеств не найдено" << std::endl;
        return;
    }
    
    std::cout << "Пересечение " << a << " & " << b << ": {";
    
    ListNode* currA = setA->elements;
    ListNode* currB = setB->elements;
    bool first = true;
    
    while (currA != nullptr && currB != nullptr) {
        if (currA->data == currB->data) {
            if (!first) std::cout << ", ";
            std::cout << currA->data;
            first = false;
            currA = currA->next;
            currB = currB->next;
        } else if (currA->data < currB->data) {
            currA = currA->next;
        } else {
            currB = currB->next;
        }
    }
    
    std::cout << "}" << std::endl;
}

void differenceSets(char a, char b) {      //A - B 
    Set* setA = findSet(a);
    Set* setB = findSet(b);
    
    if (setA == nullptr || setB == nullptr) {
        std::cout << "Одно из множеств не найдено" << std::endl;
        return;
    }
    
    std::cout << "Разность " << a << " - " << b << ": {";
    
    ListNode* currA = setA->elements;
    ListNode* currB = setB->elements;
    bool first = true;
    
    while (currA != nullptr && currB != nullptr) {
        if (currA->data == currB->data) {
            currA = currA->next;
            currB = currB->next;
        } else if (currA->data < currB->data) {
            if (!first) std::cout << ", ";
            std::cout << currA->data;
            first = false;
            currA = currA->next;
        } else {
            currB = currB->next;
        }
    }
    
    while (currA != nullptr) {
        if (!first) std::cout << ", ";
        std::cout << currA->data;
        currA = currA->next;
        first = false;
    }
    
    std::cout << "}" << std::endl;
}

void subsetCheck(char a, char b) {     //A < B
    Set* setA = findSet(a);
    Set* setB = findSet(b);
    
    if (setA == nullptr || setB == nullptr) {
        std::cout << "Одно из множеств не найдено" << std::endl;
        return;
    }
    
    ListNode* currA = setA->elements;
    ListNode* currB = setB->elements;
    
    while (currA != nullptr && currB != nullptr) {
        if (currA->data == currB->data) {
            currA = currA->next;
            currB = currB->next;
        } else if (currB->data < currA->data) {
            currB = currB->next;
        } else {
            break;
        }
    }
    
    if (currA == nullptr) {
        std::cout << a << " ЯВЛЯЕТСЯ подмножеством " << b << std::endl;
    } else {
        std::cout << a << " НЕ является подмножеством " << b << std::endl;
    }
}

void equalityCheck(char a, char b) {    // A = B
    Set* setA = findSet(a);
    Set* setB = findSet(b);
    
    if (setA == nullptr || setB == nullptr) {
        std::cout << "Одно из множеств не найдено" << std::endl;
        return;
    }
    
    ListNode* currA = setA->elements;
    ListNode* currB = setB->elements;
    bool equal = true;
    
    while (currA != nullptr && currB != nullptr) {
        if (currA->data != currB->data) {
            equal = false;
            break;
        }
        currA = currA->next;
        currB = currB->next;
    }
    

    if (currA != nullptr || currB != nullptr) {
        equal = false;
    }
    
    if (equal) {
        std::cout << "Множества " << a << " и " << b << " равны" << std::endl;
    } else {
        std::cout << "Множества " << a << " и " << b << " НЕ равны" << std::endl;
    }
}


void cleanup() {
    Set* currentSet = setsHead;
    while (currentSet != nullptr) {
        Set* nextSet = currentSet->next;
        
        ListNode* currentElem = currentSet->elements;
        while (currentElem != nullptr) {
            ListNode* nextElem = currentElem->next;
            delete currentElem;
            currentElem = nextElem;
        }
        
        delete currentSet;
        currentSet = nextSet;
    }
    setsHead = nullptr;
}

int main() {
    std::cout << "Программа для работы с множествами (без STL)" << std::endl;
    std::cout << "Доступные команды:" << std::endl;
    std::cout << "new A, del A, add A x, rem A x, pow A, see [A]" << std::endl;
    std::cout << "A + B, A & B, A - B, A < B, A = B" << std::endl;
    std::cout << "exit - для выхода" << std::endl << std::endl;
    
    char line[100];
    while (true) {
        std::cout << "> ";
        if (!std::cin.getline(line, 100) || strcmp(line, "exit") == 0) {
            break;
        }
        
        char command[10], arg1, arg2;
        int parsed = sscanf(line, "%s %c %c", command, &arg1, &arg2);
        
        if (strcmp(command, "new") == 0 && parsed >= 2) {
            newSet(arg1);
        }
        else if (strcmp(command, "del") == 0 && parsed >= 2) {
            deleteSet(arg1);
        }
        else if (strcmp(command, "add") == 0 && parsed >= 3) {
            addElement(arg1, arg2);
        }
        else if (strcmp(command, "rem") == 0 && parsed >= 3) {
            removeElement(arg1, arg2);
        }
        else if (strcmp(command, "pow") == 0 && parsed >= 2) {
            powerSet(arg1);
        }
        else if (strcmp(command, "see") == 0) {
            if (parsed >= 2) seeSets(arg1);
            else seeSets();
        }
        else if (strlen(command) == 1 && parsed >= 3) {
            char a = command[0];
            char op = arg1;
            char b = arg2;
            
            if (op == '+') unionSets(a, b);
            else if (op == '&') intersectionSets(a, b);
            else if (op == '-') differenceSets(a, b);
            else if (op == '<') subsetCheck(a, b);
            else if (op == '=') equalityCheck(a, b);
            else std::cout << "Неизвестная операция: " << op << std::endl;
        }
        else {
            std::cout << "Неизвестная команда: " << line << std::endl;
        }
    }
    
    cleanup();
    std::cout << "Программа завершена" << std::endl;
    return 0;
}