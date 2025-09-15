#include <iostream>

class BST {
private:
    struct Node {
        int value;
        Node* left;
        Node* right;

        Node(int val) : value(val), left(nullptr), right(nullptr) {}
    };

    Node* root;

    // ====================
    // 🔄 Вспомогательные методы
    // ====================

    Node* insert(Node* node, int value) {
        if (!node) return new Node(value);
        if (value < node->value)
            node->left = insert(node->left, value);
        else if (value > node->value)
            node->right = insert(node->right, value);
        return node; // без дубликатов
    }

    bool search(Node* node, int value) const {
        if (!node) return false;
        if (value == node->value) return true;
        if (value < node->value)
            return search(node->left, value);
        else
            return search(node->right, value);
    }

    Node* findMin(Node* node) {
        while (node->left) node = node->left;
        return node;
    }

    Node* remove(Node* node, int value) {
        if (!node) return nullptr;

        if (value < node->value) {
            node->left = remove(node->left, value);
        } else if (value > node->value) {
            node->right = remove(node->right, value);
        } else {
            // нашли узел
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                Node* temp = node->left;
                delete node;
                return temp;
            } else {
                Node* minNode = findMin(node->right);
                node->value = minNode->value;
                node->right = remove(node->right, minNode->value);
            }
        }

        return node;
    }

    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    Node* copy(Node* node) {
        if (!node) return nullptr;
        Node* newNode = new Node(node->value);
        newNode->left = copy(node->left);
        newNode->right = copy(node->right);
        return newNode;
    }

public:
    // ====================
    // 🔧 Конструкторы и деструктор (Правило 5)
    // ====================

    // Конструктор
    BST() : root(nullptr) {}

    // Деструктор
    ~BST() {
        destroy(root);
    }

    // Копирующий конструктор
    BST(const BST& other) {
        root = copy(other.root);
    }

    // Копирующее присваивание
    BST& operator=(const BST& other) {
        if (this != &other) {
            destroy(root);
            root = copy(other.root);
        }
        return *this;
    }

    // Перемещающий конструктор
    BST(BST&& other) noexcept : root(other.root) {
        other.root = nullptr;
    }

    // Перемещающее присваивание
    BST& operator=(BST&& other) noexcept {
        if (this != &other) {
            destroy(root);
            root = other.root;
            other.root = nullptr;
        }
        return *this;
    }

    // ====================
    // 🔍 Интерфейс
    // ====================

    void insert(int value) {
        root = insert(root, value);
    }

    bool search(int value) const {
        return search(root, value);
    }

    void remove(int value) {
        root = remove(root, value);
    }

};
