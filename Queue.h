/* Implement a queue with linked list of class T */
#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
using namespace std;

struct QNode
{
    int value;
    QNode* pNext;
};

// Dequeue from head, enqueue from tail.

class Queue{
    unsigned int size;
    QNode* head;
    QNode* tail;
public:
    Queue();
    ~Queue();
    bool IsEmpty();
    void EnQueue(int x);
    int DeQueue();
    void PrintQueue();
    unsigned int len();
};
#endif
