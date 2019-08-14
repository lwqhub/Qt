#ifndef QUEUE_H
#define QUEUE_H

#include <deque>


class Queue
{
public:
    Queue();
    ~Queue();
    std::deque<int> deq;
    void Push(int x);
    int Pop(void);
    int IsEmpty(void);
    int CheckFirst();
    int CheckSize();
};

#endif // QUEUE_H
