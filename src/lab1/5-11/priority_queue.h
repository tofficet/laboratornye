#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

struct Node;  // Предварительное объявление

class PriorityQueue {
private:
    Node* head;
    
    char* copyString(const char* src);

public:
    PriorityQueue();
    ~PriorityQueue();
    
    void insert(int prior, const char* value);
    const char* findMax() const;
    char* deleteMax();
    PriorityQueue& merge(PriorityQueue& other);
    
    PriorityQueue(const PriorityQueue&) = delete;
    PriorityQueue& operator=(const PriorityQueue&) = delete;
};

#endif // PRIORITY_QUEUE_H