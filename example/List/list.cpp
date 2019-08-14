#include "list.h"

List::List()
{
    head = NULL;
}

List::~List()
{
    struct node *temp;
    while(head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void List::Push(int x)
{
    struct node *temp;
    temp = (struct node*)malloc(sizeof(struct node));
    temp->data = x;
    temp->next = head;
    head = temp;
}

int List::Pop()
{
    struct node *temp;
    temp = head;
    int a;
    a = temp->data;
    head = head->next;
    free (temp);
    return a;
}

int List::Is_Empty()
{
    return head==NULL;
}

void List::Exchange(node *x, node *y)
{
    int data;
    data = x->data;
    x->data = y->data;
    y->data = data;
}

void List::Sort()
{
    if(head->next == NULL || head->next->next == NULL)
        return;

    node *p,*q,*end;
    end = NULL;

    while(head->next != end)
    {
        for(p=head, q=p->next; q != end; p=p->next, q=q->next)
        {
            if(p->data > q->data)
                Exchange(p,q);
        }
        end = p;
    }
}
