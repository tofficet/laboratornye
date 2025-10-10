#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>

class SetManager {
private:
    std::map<char, std::set<char>> sets;

    void print_set(const std::set<char>& s) {
        std::cout << "{";
        bool first = true;
        for (char elem : s) {
            if (!first) std::cout << ", ";
            std::cout << elem;
            first = false;
        }
        std::cout << "}";
    }

    bool is_printable_char(char c) {
        return c >= 32 && c <= 126; 
    }

public:
    void new_set(char name) {
        if (name < 'A' || name > 'Z') {
            std::cout << "Имя множества должно быть от A до Z" << std::endl;
            return;
        }
        
        if (sets.find(name) != sets.end()) {
            std::cout << "Множество " << name << " уже существует" << std::endl;
            return;
        }
        
        sets[name] = std::set<char>();
        std::cout << "Множество " << name << " создано" << std::endl;
    }


    void delete_set(char name) {
        auto it = sets.find(name);
        if (it == sets.end()) {
            std::cout << "Множество " << name << " не найдено" << std::endl;
            return;
        }
        
        sets.erase(it);
        std::cout << "Множество " << name << " удалено" << std::endl;
    }

    void add_element(char set_name, char element) {
        if (!is_printable_char(element)) {
            std::cout << "Элемент должен быть печатным символом" << std::endl;
            return;
        }
        
        auto it = sets.find(set_name);
        if (it == sets.end()) {
            std::cout << "Множество " << set_name << " не найдено" << std::endl;
            return;
        }
        
        it->second.insert(element);
        std::cout << "Элемент '" << element << "' добавлен в множество " << set_name << std::endl;
    }

    void remove_element(char set_name, char element) {
        auto it = sets.find(set_name);
        if (it == sets.end()) {
            std::cout << "Множество " << set_name << " не найдено" << std::endl;
            return;
        }
        
        if (it->second.erase(element) > 0) {
            std::cout << "Элемент '" << element << "' удален из множества " << set_name << std::endl;
        } else {
            std::cout << "Элемент '" << element << "' не найден в множестве " << set_name << std::endl;
        }
    }

    void power_set(char set_name) {
        auto it = sets.find(set_name);
        if (it == sets.end()) {
            std::cout << "Множество " << set_name << " не найдено" << std::endl;
            return;
        }
        
        const std::set<char>& original_set = it->second;
        std::vector<char> elements(original_set.begin(), original_set.end());
        int n = elements.size();
        int total_subsets = 1 << n;
        
        std::cout << "Булеан множества " << set_name << ":" << std::endl;
        std::cout << "{∅";
        
        for (int i = 1; i < total_subsets; i++) {
            std::cout << ", {";
            bool first = true;
            for (int j = 0; j < n; j++) {
                if (i & (1 << j)) {
                    if (!first) std::cout << ", ";
                    std::cout << elements[j];
                    first = false;
                }
            }
            std::cout << "}";
        }
        std::cout << "}" << std::endl;
    }

    void see_sets(char set_name = '\0') {
        if (set_name == '\0') {
            if (sets.empty()) {
                std::cout << "Нет множеств" << std::endl;
                return;
            }
            
            for (const auto& pair : sets) {
                std::cout << pair.first << ": ";
                print_set(pair.second);
                std::cout << std::endl;
            }
        } else {
            auto it = sets.find(set_name);
            if (it == sets.end()) {
                std::cout << "Множество " << set_name << " не найдено" << std::endl;
                return;
            }
            
            std::cout << set_name << ": ";
            print_set(it->second);
            std::cout << std::endl;
        }
    }

    void union_sets(char a, char b) {
        auto it_a = sets.find(a);
        auto it_b = sets.find(b);
        
        if (it_a == sets.end() || it_b == sets.end()) {
            std::cout << "Одно из множеств не найдено" << std::endl;
            return;
        }
        
        std::set<char> result;
        std::set_union(
            it_a->second.begin(), it_a->second.end(),
            it_b->second.begin(), it_b->second.end(),
            std::inserter(result, result.begin())
        );
        
        std::cout << "Объединение " << a << " + " << b << ": ";
        print_set(result);
        std::cout << std::endl;
    }

    void intersection_sets(char a, char b) {
        auto it_a = sets.find(a);
        auto it_b = sets.find(b);
        
        if (it_a == sets.end() || it_b == sets.end()) {
            std::cout << "Одно из множеств не найдено" << std::endl;
            return;
        }
        
        std::set<char> result;
        std::set_intersection(
            it_a->second.begin(), it_a->second.end(),
            it_b->second.begin(), it_b->second.end(),
            std::inserter(result, result.begin())
        );
        
        std::cout << "Пересечение " << a << " & " << b << ": ";
        print_set(result);
        std::cout << std::endl;
    }

    void difference_sets(char a, char b) {
        auto it_a = sets.find(a);
        auto it_b = sets.find(b);
        
        if (it_a == sets.end() || it_b == sets.end()) {
            std::cout << "Одно из множеств не найдено" << std::endl;
            return;
        }
        
        std::set<char> result;
        std::set_difference(
            it_a->second.begin(), it_a->second.end(),
            it_b->second.begin(), it_b->second.end(),
            std::inserter(result, result.begin())
        );
        
        std::cout << "Разность " << a << " - " << b << ": ";
        print_set(result);
        std::cout << std::endl;
    }

    void subset_check(char a, char b) {
        auto it_a = sets.find(a);
        auto it_b = sets.find(b);
        
        if (it_a == sets.end() || it_b == sets.end()) {
            std::cout << "Одно из множеств не найдено" << std::endl;
            return;
        }
        
        bool is_subset = std::includes(
            it_b->second.begin(), it_b->second.end(),
            it_a->second.begin(), it_a->second.end()
        );
        
        if (is_subset) {
            std::cout << a << " ЯВЛЯЕТСЯ подмножеством " << b << std::endl;
        } else {
            std::cout << a << " НЕ является подмножеством " << b << std::endl;
        }
    }

    void equality_check(char a, char b) {
        auto it_a = sets.find(a);
        auto it_b = sets.find(b);
        
        if (it_a == sets.end() || it_b == sets.end()) {
            std::cout << "Одно из множеств не найдено" << std::endl;
            return;
        }
        
        if (it_a->second == it_b->second) {
            std::cout << "Множества " << a << " и " << b << " равны" << std::endl;
        } else {
            std::cout << "Множества " << a << " и " << b << " НЕ равны" << std::endl;
        }
    }

    void run() {
        std::cout << "Программа для работы с множествами" << std::endl;
        std::cout << "Доступные команды:" << std::endl;
        std::cout << "new A, del A, add A x, rem A x, pow A, see [A]" << std::endl;
        std::cout << "A + B, A & B, A - B, A < B, A = B" << std::endl;
        std::cout << "exit - для выхода" << std::endl << std::endl;
        
        std::string line;
        while (true) {
            std::cout << "> ";
            if (!std::getline(std::cin, line) || line == "exit") {
                break;
            }
            
            std::stringstream ss(line);
            std::string command;
            ss >> command;
            
            if (command == "new") {
                char name;
                if (ss >> name) new_set(name);
                else std::cout << "Неверный формат: new A" << std::endl;
            }
            else if (command == "del") {
                char name;
                if (ss >> name) delete_set(name);
                else std::cout << "Неверный формат: del A" << std::endl;
            }
            else if (command == "add") {
                char name, element;
                if (ss >> name >> element) add_element(name, element);
                else std::cout << "Неверный формат: add A x" << std::endl;
            }
            else if (command == "rem") {
                char name, element;
                if (ss >> name >> element) remove_element(name, element);
                else std::cout << "Неверный формат: rem A x" << std::endl;
            }
            else if (command == "pow") {
                char name;
                if (ss >> name) power_set(name);
                else std::cout << "Неверный формат: pow A" << std::endl;
            }
            else if (command == "see") {
                char name;
                if (ss >> name) see_sets(name);
                else see_sets();
            }
            else if (command.length() == 1) {
                char op, b;
                if (ss >> op >> b) {
                    char a = command[0];
                    if (op == '+') union_sets(a, b);
                    else if (op == '&') intersection_sets(a, b);
                    else if (op == '-') difference_sets(a, b);
                    else if (op == '<') subset_check(a, b);
                    else if (op == '=') equality_check(a, b);
                    else std::cout << "Неизвестная операция: " << op << std::endl;
                } else {
                    std::cout << "Неверный формат операции" << std::endl;
                }
            }
            else {
                std::cout << "Неизвестная команда: " << line << std::endl;
            }
        }
        
        std::cout << "Программа завершена" << std::endl;
    }
};

int main() {
    SetManager manager;
    manager.run();
    return 0;
}