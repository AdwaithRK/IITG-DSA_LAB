#include<stdio.h>
#include<stdlib.h>
#include "myStack.h"



void createStack(myStack *S, int stackSize)
{
    S->arrStack = (int *)malloc(stackSize*sizeof(int));
    if(S->arrStack == NULL)
    {
        printf("createStack(): malloc failure.\n");
        exit(EXIT_FAILURE);
    }
    S->size = stackSize;
    S->top = -1;
}

// Returns -1 if stack is full, 0 otherwise
int push(myStack *S, int data)
{
    if (S->top == S->size-1)
    {
        printf("push(): stack full\n");
        return -1;
    }

    S->arrStack[++S->top] = data;
    return 0;
}

// Returns -1 if stack is full, 0 otherwise. poped element is returned in data as call-by-ref
int pop(myStack *S, int *data)
{
   if (S->top == -1)
   {
        printf("pop(): stack empty\n");
        return -1;
   }

   *data = S->arrStack[S->top--];
    return 0;
}

void destroyStack(myStack *S)
{
    free(S->arrStack);
}
