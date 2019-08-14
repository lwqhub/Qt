#ifndef STACK_H
#define STACK_H

#include <vector>

class Stack
{
public:
    Stack();
    ~Stack();
    std::vector<char> vec;//需要std：：
    void Push(char x);
    char Pop(void);
    int IsEmpty(void);
};

#endif // STACK_H
