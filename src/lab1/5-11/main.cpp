#include "priority_queue.h"
#include <iostream>

int main() {
    PriorityQueue q1, q2;
    
    q1.insert(3, "Hello");
    q1.insert(1, "World");
    q2.insert(5, "Priority");
    q2.insert(2, "Queue");
    
    q1.merge(q2);
    
    while (true) {
        try {
            char* maxStr = q1.deleteMax();
            std::cout << maxStr << std::endl;
            delete[] maxStr;
        } catch (const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
            break;
        }
    }
    
    return 0;
}