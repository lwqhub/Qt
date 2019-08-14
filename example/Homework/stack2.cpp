#include "stack2.h"

Stack2::Stack2()
{

}

void Stack2::Push(char x)
{
    double a=x-'0';
    vec.push_back(a);
}

void Stack2::Push(double x)
{
    vec.push_back(x);
}

double Stack2::Pop()
{
    double t;
    t = vec.back();
    vec.pop_back();
    return t;

}

int Stack2::IsEmpty()
{
    return vec.empty();
}

Stack2::~Stack2()
{

}
