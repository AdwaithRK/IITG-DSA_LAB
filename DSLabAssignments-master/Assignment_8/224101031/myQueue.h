#ifndef MYQUEUE_H_
#define MYQUEUE_H_

struct myQueue{
	int *arrQueue;
    int front, rear,size;
};

typedef struct myQueue myQueue;

int createQueue(myQueue *Q, int queueSize);
int pushQueue(myQueue *Q, int data);
int deleteQueue(myQueue *Q, int *data);
int isQueueEmpty(myQueue *Q);
void destroyQueue(myQueue *Q);

#endif
