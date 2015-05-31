/* Implement a queue with a single linked list with a head node.
 * Node head points to the first node. Node tail is the last node if 
 * there is.
 * head->node1->node2->node3(tail).
 * Enqueue an element to the tail and dequeue an element from head.
 */
#include "Queue.h"
#include <iostream>
using namespace std;

#define TRUE (1)
#define FALSE (0)

// Create a head node.
Queue::Queue()
{
   QNode* p = new QNode();
   if(p == NULL)
       cout << "Not enough space." << endl;
   p->value = 0;
   p->pNext = NULL;
   head = p;
   tail = p;
   size = 0;
}

Queue::~Queue()
{
    QNode* p = new QNode();
    while(head != NULL)
    {
        p = head;
        head = head->pNext;
        delete p;
    }
}

bool Queue::IsEmpty()
{
    if(size == 0)
        return TRUE;
    else
        return FALSE;
}


void Queue::EnQueue(int x)
{
    QNode* p = new QNode();
    if(p == NULL)
    {
        cout << "There is no space." << endl;
        return;
    }
    // Initialize p.
    p->value = x;
    p->pNext = NULL;
    // Insert p at the tail and move tail to it.
    tail->pNext = p;
    tail = p;
    size++;
}


int Queue::DeQueue()
{
    if(size == 0)
    {
        cout << "The queue is empty." << endl;
        return -1;
    }

    // Delete from head.
    QNode* p = head->pNext;
    head->pNext = p->pNext;
    int result = p->value;
    // Delete the node.
    delete p;
    p = NULL;
    size--;
    return result;
}

unsigned int Queue::len()
{
    return size;
}

void Queue::PrintQueue()
{
    QNode* p = head->pNext;
    while(p != NULL)
    {
        cout << p->value << " ";
        p = p->pNext;
    }
    cout << endl;
}
