#include<stdio.h>
#include<stdlib.h>
#include "myQueue.h"
int createQueue(myQueue *Q, int queueSize){
	int *arr = (int *)malloc(queueSize*sizeof(int));
	if(arr == NULL)	return -1;
	Q->arrQueue=arr;
	Q->front = -1;
	Q->rear = 0;
	Q->size=queueSize;
	return 0;
}

int pushQueue(myQueue *Q, int data){
	if(Q->rear == Q->size){
		return -1;
	}
	if(Q->front == -1)	Q->front++;
	Q->arrQueue[Q->rear++] = data;
	return 0;
}

int deleteQueue(myQueue *Q, int *data){
	// if(Q->front == Q->rear)	return -1;
	if(Q->front == Q->size)	return -1;
	*data = Q->arrQueue[Q->front++];
	if(Q->front == Q->rear) {	
		Q->front = -1;
		Q->rear =0;
	}
	return 0;
}

int isQueueEmpty(myQueue *Q){
	return Q->front == Q->rear || Q->front == -1? 1 : 0;
}

void destroyQueue(myQueue *Q){
	free(Q->arrQueue);
	return ;
}
