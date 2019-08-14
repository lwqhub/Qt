#include "list.h"
#include <stdio.h>

void main()
{
    List mylist;

    for(int i = 1; i <= 10; i++)
        mylist.Push(i);

    node *p = mylist.head;
    do
    {
        printf("%d\t",p->data);
        p = p->next;
    }while(p != NULL);

    printf("\n");

    mylist.Sort();

    p = mylist.head;
    do
    {
        printf("%d\t",p->data);
        p = p->next;
    }while(p != NULL);

}
