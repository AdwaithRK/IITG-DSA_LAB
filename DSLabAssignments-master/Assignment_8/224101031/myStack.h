#ifndef MYSTACK_H_
#define MYSTACK_H_

struct myStack{
    int *arrStack;
    int size;
    int top;
};

typedef struct myStack myStack;


int createStack(myStack *S, int stackSize);
// Returns -1 if stack is full, 0 otherwise
int push(myStack *S, int data);
// Returns -1 if stack is full, 0 otherwise. poped element is returned in data as call-by-ref
int pop(myStack *S, int *data);
void destroyStack(myStack *S);

#endif
