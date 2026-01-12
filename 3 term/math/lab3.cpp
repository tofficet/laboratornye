#include <cstddef>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <cctype>
#include <iomanip>
#include <algorithm>
#include <stack>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <fstream>

class TruthTable {
private:
    std::vector<std::string> variables;
    std::vector<std::vector<bool>> table;

public:
    TruthTable(const std::vector<std::string>& vars, const std::vector<std::vector<bool>>& truthTable) 
        : variables(vars), table(truthTable) {
    }

    TruthTable() : variables({}), table({}) {}

    std::vector<std::string> getVariables() const {
        return variables;
    }

    const std::vector<std::vector<bool>>& getTable() const {
        return table;
    }

    void printTruthTable() const {
        if (table.empty()) {
            std::cout << "Нет переменных в выражении" << std::endl;
            return;
        }

        std::vector<std::string> sortedVars = variables;
        std::sort(sortedVars.begin(), sortedVars.end());

        std::vector<size_t> columnWidths;
        for (const auto& var : sortedVars) {
            columnWidths.push_back(std::max(var.length(), size_t(3)));
        }
        size_t resultWidth = 6;

        std::cout << "|";
        for (size_t i = 0; i < sortedVars.size(); i++) {
            std::cout << " " << std::setw(columnWidths[i]) << sortedVars[i] << " |";
        }
        std::cout << " " << std::setw(resultWidth) << "Результат" << " |" << std::endl;

        std::cout << "|";
        for (size_t i = 0; i < sortedVars.size(); i++) {
            std::cout << std::string(columnWidths[i] + 2, '-') << "|";
        }
        std::cout << std::string(resultWidth + 2, '-') << "|" << std::endl;

        for (size_t i = 0; i < table.size(); i++) {
            std::cout << "|";
            for (size_t j = 0; j < table[i].size() - 1; j++) {
                std::cout << " " << std::setw(columnWidths[j]) 
                         << (table[i][j] ? "1" : "0") << " |";
            }
            std::cout << " " << std::setw(resultWidth) 
                     << (table[i].back() ? "1" : "0") << " |" << std::endl;
        }
        std::cout << std::endl;
    }

    bool isFectiveVariable(int varIndex) const
    {
        if (table.empty()) return true;
        
        int numVarsInTable = table[0].size() - 1;
        if (varIndex >= numVarsInTable) return true;

        for (int i = 0; i < static_cast<int>(table.size()); i++)
        {
            for (int j = i + 1; j < static_cast<int>(table.size()); j++)
            {
                if (table[i][varIndex] == table[j][varIndex]) continue;
                bool differsOnlyInVar = true;
                for (int k = 0; k < numVarsInTable; k++) {
                    if (k != varIndex && table[i][k] != table[j][k]) {
                        differsOnlyInVar = false;
                        break;
                    }
                }

                if (differsOnlyInVar 
                    && table[i][numVarsInTable] != table[j][numVarsInTable]) {
                    return false;
                }
            }
        }
        return true;
    }

    bool deleteFectiveVariables() {
        std::vector<size_t> essentialIndices;
        for (size_t i = 0; i < variables.size(); i++) {
            if (!isFectiveVariable(i)) {
                essentialIndices.push_back(i);
            }
        }

        std::vector<std::string> newVars;
        for (size_t idx : essentialIndices) {
            newVars.push_back(variables[idx]);
        }

        std::vector<std::vector<bool>> newTable;
        std::unordered_set<std::string> seenRows;
        
        for (const auto& row : table) {
            std::vector<bool> newRow;
            for (size_t idx : essentialIndices) {
                newRow.push_back(row[idx]);
            }
            newRow.push_back(row.back());
            
            std::string key;
            for (bool val : newRow) {
                key += val ? '1' : '0';
            }
            
            if (seenRows.find(key) == seenRows.end()) {
                seenRows.insert(key);
                newTable.push_back(newRow);
            }
        }
        
        bool res = newVars.size() < variables.size();
        this->table = newTable;
        this->variables = newVars;

        return res;
    }

    std::string getSDNF() const {
        auto variables = getVariables();
        
        if (variables.empty()) {
            if (!table.empty() && table[0].back()) {
                return "1";
            } else {
                return "0";
            }
        }
        
        if (table.empty()) {
            return "0";
        }
        
        std::vector<std::string> pieces;
        int last_el = table[0].size() - 1;
        for (const auto& row : table) {
            if (!row[last_el]) continue;
            
            std::string piece = "(";
            for (size_t i = 0; i < variables.size(); i++) {
                piece += (row[i] ? "" : "-") + variables[i];
                if (i < variables.size() - 1) {
                    piece += " & ";
                }
            }
            piece += ")";
            pieces.push_back(piece);
        }
    
        if (pieces.empty()) return "";
        std::string result;
        for (size_t i = 0; i < pieces.size(); i++) {
            result += pieces[i];
            if (i < pieces.size() - 1) {
                result += " + ";
            }
        }
        return result;
    }

    std::string getSKNF() const {
        auto variables = getVariables();
        
        if (variables.empty()) {
            if (!table.empty() && table[0].back()) {
                return "1";
            } else {
                return "0";
            }
        }
        
        if (table.empty()) {
            return "0";
        }
        
        std::vector<std::string> pieces;
        
        for (const auto& row : table) {
            if (row.back()) continue;
            
            std::vector<std::string> literals;
            for (size_t i = 0; i < variables.size(); i++) {
                literals.push_back(row[i] ? "-" + variables[i] : variables[i]);
            }
            
            std::string piece = "(";
            for (size_t j = 0; j < literals.size(); j++) {
                piece += literals[j];
                if (j < literals.size() - 1) {
                    piece += " + ";
                }
            }
            piece += ")";
            pieces.push_back(piece);
        }
        
        if (pieces.empty()) return "";
        
        std::string result;
        for (size_t i = 0; i < pieces.size(); i++) {
            result += pieces[i];
            if (i < pieces.size() - 1) {
                result += " & ";
            }
        }
        
        return result;
    }

    TruthTable getDualFunction() const {
        auto variables = getVariables();
        
        if (table.empty() && variables.empty()) {
            return TruthTable();
        }
        
        std::vector<std::vector<bool>> dualTable;

        for (int i = 0; i < static_cast<int>(table.size()); i++)
        {
            std::vector<bool> dualRow = table[i];
            std::transform(dualRow.begin(), dualRow.end(), dualRow.begin(), 
                [](bool b) { return !b; });
            dualTable.push_back(dualRow);
        }
        
        return TruthTable(variables, dualTable);
    }

    std::string getANF() const {
        auto vars = getVariables();
        
        if (table.empty()) return "0";
        const int n = vars.size();
        const int N = 1 << n;
        
        std::vector<int> f(N);
        for (int i = 0; i < N; i++) {
            f[i] = table[i].back() ? 1 : 0;
        }
        
        for (int k = 1; k < N; k <<= 1) {
            for (int i = 0; i < N; i += (k << 1)) {
                for (int j = 0; j < k; j++) {
                    f[i + j + k] ^= f[i + j];
                }
            }
        }
        
        std::vector<std::string> terms;
        for (int i = 0; i < N; i++) {
            if (f[i] == 0) continue;
            
            if (i == 0) {
                terms.push_back("1");
                continue;
            }
            
            std::string term = "(";
            for (int j = 0; j < n; j++) {
                if (i & (1 << (n - 1 - j))) {
                    if (term != "(") term += " & ";
                    term += vars[j];
                }
            }
            term += ")";
            terms.push_back(term);
        }
        
        if (terms.empty()) return "0";
        std::string result;
        for (int i = 0; i < static_cast<int>(terms.size()); i++) {
            result += terms[i];
            if (i < static_cast<int>(terms.size()) - 1) {
                result += " @ ";
            }
        }
        
        return result;
    }
};

class BooleanExpression {
private: 
    
    struct TreeNode {
        std::string value;
        TreeNode* left;
        TreeNode* right;
        
        TreeNode(const std::string& val) : value(val), left(nullptr), right(nullptr) {}
        TreeNode(const std::string& val, TreeNode* l, TreeNode* r) : value(val), left(l), right(r) {}
    };

    struct OperatorInfo {
        char symbol;
        int precedence;
        std::function<bool(bool, bool)> function;
        bool isUnary;
        
        OperatorInfo(char sym, int prec, std::function<bool(bool, bool)> func, bool unary = false)
            : symbol(sym), precedence(prec), function(func), isUnary(unary) {}
        
        OperatorInfo() : symbol('\0'), precedence(-1), function(nullptr), isUnary(false) {}
    };
    
    std::unordered_map<char, OperatorInfo> operators;
    std::unordered_map<char, char> bracketPairs;

    std::vector<std::string> variables;
    TreeNode* expressionTree;
    
public:
    BooleanExpression() : expressionTree(nullptr) {
        initializeOperators();
        initializeBrackets();
    }

    BooleanExpression(const std::string& expression) : expressionTree(nullptr) {
        initializeOperators();
        initializeBrackets();
        expressionTree = parse(expression);
        extractVariables();
    }

    ~BooleanExpression() {
        clearTree(expressionTree);
    }

    void setBrackets(std::unordered_map<char, char> bracketPairs)
    {
        if (bracketPairs.empty())
        {
            throw std::runtime_error("Коллекция скобок пуста");
        }
        this->bracketPairs = bracketPairs;
    }
    
    void setOperator(char symbol, int precedence, std::function<bool(bool, bool)> function, bool isUnary = false) {
        operators[symbol] = OperatorInfo(symbol, precedence, function, isUnary);
    }
    
    int getPrecedence(char op) const {
        auto it = operators.find(op);
        if (it != operators.end()) {
            return it->second.precedence;
        }
        return -1;
    }
    
    bool isOperator(char c) const {
        return operators.find(c) != operators.end();
    }
    
    bool isUnaryOperator(char c) const {
        auto it = operators.find(c);
        return it != operators.end() && it->second.isUnary;
    }

    bool isOpenBracket(char c) const {
        return bracketPairs.find(c) != bracketPairs.end();
    }

    bool isCloseBracket(char c) const {
        for (const auto& pair : bracketPairs) {
            if (pair.second == c) {
                return true;
            }
        }
        return false;
    }
    
    TreeNode* parse(const std::string& expression) {
        std::stack<TreeNode*> operandStack;
        std::stack<char> operatorStack;
        
        int i = 0;
        bool expectOperand = true;

        if (operators.empty()) {
            throw std::runtime_error("Нет операторов в парсере");
        }
        if(bracketPairs.empty()) {
            throw std::runtime_error("Нет скобок в парсере");
        }
        
        while (i < expression.length())
        {
            if (std::isspace(expression[i])) {
                i++;
                continue;
            }
            
            if (isOpenBracket(expression[i])) {
                if (!expectOperand) {
                    throw std::runtime_error("Неожиданная открывающая скобка");
                }
                operatorStack.push(expression[i]);
                i++;
                continue;
            }
            
            if (isCloseBracket(expression[i])) {
                if (expectOperand) {
                    throw std::runtime_error("Неожиданная закрывающая скобка");
                }
                
                while (!operatorStack.empty() && !isOpenBracket(operatorStack.top())) {
                    processOperator(operandStack, operatorStack);
                }
                
                if (operatorStack.empty() || !isOpenBracket(operatorStack.top())) {
                    throw std::runtime_error("Несоответствие скобок");
                }
                
                char openBracket = operatorStack.top();
                if (bracketPairs[openBracket] != expression[i]) {
                    throw std::runtime_error("Несоответствие типа скобок");
                }
                
                operatorStack.pop();
                i++;
                continue;
            }
            
            if (isOperator(expression[i])) {
                char op = expression[i];
                
                if (expectOperand && isUnaryOperator(op)) {
                    operatorStack.push(op);
                    i++;
                    continue;
                }
                
                if (!expectOperand) {
                    while (!operatorStack.empty() && 
                           !isOpenBracket(operatorStack.top()) && 
                           getPrecedence(operatorStack.top()) < getPrecedence(op)) {
                        processOperator(operandStack, operatorStack);
                    }
                    operatorStack.push(op);
                    expectOperand = true;
                    i++;
                    continue;
                }
                else {
                    throw std::runtime_error("Неожиданный оператор");
                }
            }
            
            if (expectOperand) {
                std::string operand = extractVariable(expression, i);
                if (operand.empty()) {
                    throw std::runtime_error("Некорректное имя переменной");
                }
                operandStack.push(new TreeNode(operand));
                expectOperand = false;
                continue;
            }
            
            throw std::runtime_error("Неожиданный символ: " + std::string(1, expression[i]));
        }
        
        while (!operatorStack.empty()) {
            if (isOpenBracket(operatorStack.top())) {
                throw std::runtime_error("Несоответствие скобок");
            }
            processOperator(operandStack, operatorStack);
        }
        
        if (operandStack.size() != 1) {
            throw std::runtime_error("Некорректное выражение");
        }
        
        return operandStack.top();
    }

    bool evaluate(const std::unordered_map<std::string, bool>& variableValues) const {
        return evaluateInner(expressionTree, variableValues);
    }
    
    void clearTree(TreeNode* root) {
        if (root == nullptr) return;
        
        clearTree(root->left);
        clearTree(root->right);
        
        delete root;
    }

    std::vector<std::vector<bool>> generateTruthTable() const {
        std::vector<std::vector<bool>> truthTable;
        std::vector<std::string> sortedVars = getVariables();
        
        if (sortedVars.empty()) {
            return truthTable;
        }

        int varCount = sortedVars.size();
        int rowCount = 1 << varCount;

        for (int i = 0; i < rowCount; i++) {
            std::unordered_map<std::string, bool> currentValues;
            std::vector<bool> row;

            for (int j = 0; j < varCount; j++) {
                bool value = (i & (1 << (varCount - 1 - j))) != 0;
                currentValues[sortedVars[j]] = value;
                row.push_back(value);
            }

            try {
                bool result = evaluate(currentValues);
                row.push_back(result);
            } catch (const std::exception& e) {
                row.push_back(false);
            }

            truthTable.push_back(row);
        }

        return truthTable;
    }

    void printTree() const
    {
        printTreeInner(this->expressionTree);
        std::cout << std::endl;
    }

    void printTreeInner(TreeNode* root, int depth = 0) const {
        if (root == nullptr) return;
        
        printTreeInner(root->right, depth + 1);
        std::cout << std::string(depth * 4, ' ') << root->value << std::endl;
        printTreeInner(root->left, depth + 1);
    }
    
    std::vector<std::string> getVariables() const {
        std::vector<std::string> sortedVars = variables;
        std::sort(sortedVars.begin(), sortedVars.end());
        return sortedVars;
    }

private:
    void initializeOperators() {
        setOperator('+', 1, [](bool a, bool b) { return a || b; }); // OR (дизъюнкция)
        setOperator('&', 2, [](bool a, bool b) { return a && b; }); // AND (конъюнкция)
        setOperator('@', 3, [](bool a, bool b) { return a != b; }); // XOR (сложение по модулю 2)
        setOperator('~', 3, [](bool a, bool b) { return a == b; }); // EQUIV (эквивалентность)
        setOperator('>', 4, [](bool a, bool b) { return !a || b; }); // IMPLIES (импликация)
        setOperator('|', 5, [](bool a, bool b) { return !(a && b); }); // NAND (штрих Шеффера)
        setOperator('!', 5, [](bool a, bool b) { return !(a || b); }); // NOR (стрелка Пирса)
        setOperator('-', 6, [](bool a, bool) { return !a; }, true); // NOT (отрицание) - унарный
    }

    void initializeBrackets() {
        bracketPairs = {
            {'(', ')'}, {'{', '}'}, {'[', ']'}
        };
    }

    std::string extractVariable(const std::string& expression, int& index) {
        size_t start = index;
        
        if (index >= expression.length() || !std::isalpha(expression[index])) {
            return "";
        }
        index++;

        if (expression[index] != '_') {
            return "";
        }
        index++;

        while (index < expression.length() && std::isdigit(expression[index])) {
            index++;
        }
        
        return expression.substr(start, index - start);
    }
    
    void processOperator(std::stack<TreeNode*>& operandStack, std::stack<char>& operatorStack) {
        if (operatorStack.empty() || operandStack.empty()) {
            throw std::runtime_error("Некорректная структура выражения");
        }
        
        char op = operatorStack.top();
        operatorStack.pop();
        
        if (isUnaryOperator(op)) {
            if (operandStack.empty()) {
                throw std::runtime_error("Недостаточно операндов для унарного оператора");
            }
            TreeNode* operand = operandStack.top();
            operandStack.pop();
            TreeNode* newNode = new TreeNode(std::string(1, op), nullptr, operand);
            operandStack.push(newNode);
        }

        else {
            if (operandStack.size() < 2) {
                throw std::runtime_error("Недостаточно операндов для бинарного оператора");
            }
            TreeNode* right = operandStack.top();
            operandStack.pop();
            TreeNode* left = operandStack.top();
            operandStack.pop();
            TreeNode* newNode = new TreeNode(std::string(1, op), left, right);
            operandStack.push(newNode);
        }
    }

    void extractVariables() {
        variables.clear();
        extractVariablesFromTree(expressionTree);
        
        std::unordered_set<std::string> uniqueVars(variables.begin(), variables.end());
        variables.assign(uniqueVars.begin(), uniqueVars.end());
    }

    void extractVariablesFromTree(TreeNode* node) {
        if (node == nullptr) return;
        
        if (node->left == nullptr && node->right == nullptr) {
            if (!node->value.empty() && std::isalpha(node->value[0])) {
                variables.push_back(node->value);
            }
            return;
        }
        
        extractVariablesFromTree(node->left);
        extractVariablesFromTree(node->right);
    }

    bool evaluateInner(TreeNode* root, const std::unordered_map<std::string, bool>& variables) const {
        if (root == nullptr) return false;
        
        // variable
        if (root->left == nullptr && root->right == nullptr) {
            auto it = variables.find(root->value);
            if (it == variables.end()) {
                throw std::runtime_error("Неопределенная переменная: " + root->value);
            }
            return it->second;
        }
        
        // unary operator
        if (root->left == nullptr && root->right != nullptr) {
            bool operand = evaluateInner(root->right, variables);
            auto it = operators.find(root->value[0]);
            if (it != operators.end()) {
                return it->second.function(operand, false);
            }
            throw std::runtime_error("Неизвестный оператор: " + root->value);
        }
        
        // binary operator
        bool leftVal = evaluateInner(root->left, variables);
        bool rightVal = evaluateInner(root->right, variables);
        
        auto it = operators.find(root->value[0]);
        if (it != operators.end()) {
            return it->second.function(leftVal, rightVal);
        }
        throw std::runtime_error("Неизвестный оператор: " + root->value);
    }
};

std::vector<std::string> readExpressionsFromFile(const std::string& filename) {
    std::vector<std::string> expressions;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл '" << filename << "'" << std::endl;
        return expressions;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Убираем пробелы в начале и конце строки
        size_t start = line.find_first_not_of(" \t");
        size_t end = line.find_last_not_of(" \t");
        
        if (start != std::string::npos && end != std::string::npos) {
            std::string expression = line.substr(start, end - start + 1);
            // Пропускаем пустые строки и комментарии (начинающиеся с #)
            if (!expression.empty() && expression[0] != '#') {
                expressions.push_back(expression);
            }
        }
    }
    
    file.close();
    return expressions;
}

void processExpression(const std::string& expr);

int main() {
    std::string filename;
    std::cout << "Введите имя файла с логическими выражениями: ";
    std::getline(std::cin, filename);
    
    if (filename.empty()) {
        std::cout << "Имя файла не было введено. Использую стандартные выражения." << std::endl;
        
        std::vector<std::string> expressions = {
            "(x_1 & y_2)",
            "(-x_1 | y_2)",
            "((x_1 + y_2) & z_3)",
            "(x_1 @ (y_2 > z_3))",
            "((x_1 + y_2) & [-z_3 + x_123])",
            "(x_1 & (y_2 + -y_2))",
            "{       -([{x_123} & y_2])}",
            "(x_1 & (y_2 + -y_2))",
            "((x_2 > x_1) & (x_2 ! x_2))",
            "((x_1 > ((x_2 > x_3) > x_4)) ~ ((-x_1) & (x_2 > x_3) & (-x_4)))",
            "(x_1 + -x_1) & ( y_2+ -y_2) & (z_3 + -z_3)",
            "(x_1&-x_1)+(y_1&-y_1)+(z_1&-z_1)"
        };
        
        for (const auto& expr : expressions) {
            processExpression(expr);
        }
    } else {
        std::vector<std::string> expressions = readExpressionsFromFile(filename);
        
        if (expressions.empty()) {
            std::cout << "Файл пуст или не содержит допустимых выражений." << std::endl;
            return 1;
        }
        
        std::cout << "\nНайдено " << expressions.size() << " выражений в файле '" << filename << "':\n" << std::endl;
        
        for (size_t i = 0; i < expressions.size(); i++) {
            std::cout << "=== Выражение " << (i + 1) << " из " << expressions.size() << " ===" << std::endl;
            processExpression(expressions[i]);
        }
    }
    
    return 0;
}

void processExpression(const std::string& expr) {
    try {
        std::cout << "Разбор выражения: " << expr << std::endl;
        
        BooleanExpression bool_expr(expr);
        
        std::unordered_map<std::string, bool> vars = {
            {"x_1", true},
            {"x_123", true},
            {"y_2", false},
            {"z_3", true}
        };
        
        TruthTable table(bool_expr.getVariables(), bool_expr.generateTruthTable());

        std::cout << "Таблица истинности:" << std::endl;
        table.printTruthTable();

        std::cout << "Удаление фиктивных переменных:" << std::endl;
        if (table.deleteFectiveVariables()) {
            std::cout << std::endl;
            std::cout << "НОВАЯ таблица истинности:" << std::endl;
            table.printTruthTable();
        }
        else {
            std::cout << "Нет фиктивных переменных" << std::endl;
        }
        
        std::cout << "СДНФ: " << table.getSDNF() << std::endl;
        std::cout << "СКНФ: " << table.getSKNF() << std::endl;
        std::cout << "АНФ:  " << table.getANF() << std::endl;

        std::cout << "\n----- Двойственная функция -----" << std::endl;
        auto dual_function_table = table.getDualFunction();
        dual_function_table.printTruthTable();
        std::cout << "СДНФ: " << dual_function_table.getSDNF() << std::endl;
        std::cout << "СКНФ: " << dual_function_table.getSKNF() << std::endl;
        std::cout << "\n" << std::string(60, '=') << "\n\n";
    } 
    catch (const std::exception& e) {
        std::cerr << "*** Ошибка: " << e.what() << std::endl;
        std::cout << "\n" << std::string(60, '=') << "\n\n";
    }
}