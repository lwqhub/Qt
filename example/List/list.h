#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

struct node
{
    int data;
    struct node *next;
};

class List
{
public:
    List();
    ~List();

    struct node *head;

    void Push(int x);
    int Pop();
    int Is_Empty();
    void Exchange(struct node *x,struct node *y);
    void Sort();
};

#endif // LIST_H
