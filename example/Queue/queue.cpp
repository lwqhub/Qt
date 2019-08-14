#include "queue.h"

Queue::Queue()
{

}

Queue::~Queue()
{

}

void Queue::Push(int x)
{
    deq.push_back(x);
}

int Queue::Pop()
{
    int t;
    t=deq.front();
    deq.pop_front();
    return t;
}

int Queue::IsEmpty()
{
    return deq.empty();
}

int Queue::CheckFirst()
{
    if(deq.empty()==0)
        return deq.front();
    else
        return 0;
}

int Queue::CheckSize()
{
    return deq.size();
}
