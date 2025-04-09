#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

struct Node;  

class PriorityQueue {
public:
    virtual ~PriorityQueue();
    virtual void insert(int prior, const char* value);
    virtual const char* findMax() const;
    virtual char* deleteMax();
    virtual PriorityQueue& merge(PriorityQueue* other);
};

#endif // PRIORITY_QUEUE_H