#include "priority_queue.h"
#include "node.h"
#include <stdexcept>

char* PriorityQueue::copyString(const char* src) {
    if (!src) return nullptr;
    
    int length = 0;
    while (src[length]) length++;
    
    char* dest = new char[length + 1];
    for (int i = 0; i <= length; i++) {
        dest[i] = src[i];
    }
    return dest;
}

PriorityQueue::PriorityQueue() : head(nullptr) {}

PriorityQueue::~PriorityQueue() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete[] temp->value;
        delete temp;
    }
}

void PriorityQueue::insert(int prior, const char* value) {
    char* copy = copyString(value);
    Node* newNode = new Node(prior, copy, nullptr);

    if (!head || prior > head->prior) {
        newNode->next = head;
        head = newNode;
        return;
    }
    
    Node* current = head;
    while (current->next && current->next->prior >= prior) {
        current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
}

const char* PriorityQueue::findMax() const {
    if (!head) {
        throw std::runtime_error("Queue is empty");
    }
    return head->value;
}

char* PriorityQueue::deleteMax() {
    if (!head) {
        throw std::runtime_error("Queue is empty");
    }
    
    Node* maxNode = head;
    head = head->next;
    char* result = maxNode->value;
    delete maxNode;
    return result;
}

PriorityQueue& PriorityQueue::merge(PriorityQueue& other) {
    if (!other.head) {
        return *this;
    }

    while (other.head) {
        int priority = other.head->prior;
        char* value = other.head->value;
        
        this->insert(priority, value);
        
        Node* temp = other.head;
        other.head = other.head->next;
        delete temp;
    }

    return *this;
}