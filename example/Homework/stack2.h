#ifndef STACK2_H//若没有编译时不会出错，链接时出现函数重定义错误
#define STACK2_H

#include <vector>

class Stack2
{
public:
    Stack2();
    ~Stack2();
    std::vector<double> vec;//需要std：：
    void Push(char x);
    void Push(double x);
    double Pop(void);
    int IsEmpty(void);
};

#endif // STACK2_H
