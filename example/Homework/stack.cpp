#include "stack.h"

Stack::Stack()
{

}

void Stack::Push(char x)
{
    vec.push_back(x);
}

char Stack::Pop()
{
    char t;
    t = vec.back();
    vec.pop_back();
    return t;

}

int Stack::IsEmpty()
{
    return vec.empty();
}

Stack::~Stack()
{

}
