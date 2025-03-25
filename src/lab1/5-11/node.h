#ifndef NODE_H
#define NODE_H

struct Node {
    int prior;
    char* value;
    Node* next;
    
    Node(int p, char* v, Node* n) : prior(p), value(v), next(n) {}
};

#endif // NODE_H