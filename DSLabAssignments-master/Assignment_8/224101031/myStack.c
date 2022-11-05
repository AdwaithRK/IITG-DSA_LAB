#include "myStack.h"
#include<stdlib.h>
typedef struct myStack myStack;

int createStack(myStack *S, int stackSize){
	int *arr = (int *)malloc(stackSize*sizeof(int));
	if(arr == NULL)	return -1;
	S->arrStack = arr;
	S->size = stackSize;
	S->top = -1;
	return 0;
}

int push(myStack *S, int data){
	if(S->top == S->size - 1)	return -1;
	S->arrStack[++S->top] = data;
	return 0;
}

int pop(myStack *S, int *data){
	if(S->top == -1)	return -1;
	*data = S->arrStack[S->top--];
	return 0;
}

void destroyStack(myStack *S){
	free(S->arrStack);
	return;
}
