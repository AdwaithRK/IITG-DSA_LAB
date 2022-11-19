#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define SIZE 100

struct TreapNode 
{
    int key;
    int priority;
    int left;
    int right; 
};

typedef struct TreapNode TreapNode;

void push(int *stack,int *top,int n,int nodeIndex)
{
    if(*top >= n-1)
    {
        printf("\nStack is full");
    }
    else
    {
        (*top)++;
        stack[*top] = nodeIndex;
    }
}

int isStackEmpty(int top)
{
    if(top <= -1)
        return 1;
    return 0;
}

void pop(int *stack,int *top)
{
    if(*top <= -1)
    {
        printf("\nStack is empty");
    }
    else
    {
        (*top)--;
    }
}

int searchTreap(TreapNode *array, int key)
{
    int i = 0;

    while(i != -1)
    {
        if(array[i].key == key)
        {
            return 1;
        }
        else if(array[i].key > key)
        {
            i = array[i].left;
        }
        else
        {
            i = array[i].right;
        }
    }

    return 0;
}

void getNewNode(TreapNode *node,int key)
{
    node->key = key;
    node->left = node->right = -1;
    node->priority = rand()%100;
}

void swap(TreapNode *array,int i,int j)
{
    TreapNode temp;
    temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

void leftRotation(TreapNode *array,int index)
{
    int r = array[index].right;
    int rl = array[r].left;

    array[index].right = rl;
    swap(array,index,r);

    array[index].left = r;    
}

void rightRotation(TreapNode *array,int index)
{
    int l = array[index].left;
    int lr = array[l].right;

    array[index].left = lr;
    swap(array,index,l);

    array[index].right = l;    
}

int insertTreap(TreapNode *array,int *root,int *freePointer,int key)
{
    TreapNode newNode;
    
    if(*freePointer >= SIZE-1)
    {
        printf("\nNo free space");
        return 0;
    }

    getNewNode(&newNode,key);
    array[*freePointer] = newNode;

    if(*root == -1)
    {
        *root = *freePointer;
        (*freePointer)++;
        return 1;
    }
    else
    {
        int stack[SIZE];
        int top = -1;
        int i = *root;

        while(i != -1)
        {
            if(array[i].key <= key)
            {
                push(stack,&top,SIZE,i);
                i = array[i].right;
            }
            else
            {
                push(stack,&top,SIZE,i);
                i = array[i].left;
            }
        }

        bool isLeft;
        if(!isStackEmpty(top))
        {
            int index = stack[top];

            if(array[index].key >= key)
            {
                array[index].left = *freePointer;
                (*freePointer)++;
                isLeft = true;
            }
            else
            {
                array[index].right = *freePointer;
                (*freePointer)++;
                isLeft = false;
            } 
        }

        int prev;
        int index = stack[top];
        while(!isStackEmpty(top))
        {
            if(array[index].priority < array[array[index].left].priority && isLeft)
            {
                rightRotation(array,index);
            }
            else if(array[index].priority < array[array[index].right].priority && !isLeft)
            {
                leftRotation(array,index);  
            }
            else
            {
                break;
            }

            pop(stack,&top);
            prev = index;
            if(isStackEmpty(top))
                break;
            index = stack[top];

            if(array[index].left == prev)
                isLeft = true;
            else if(array[index].right == prev)
                isLeft = false;
        }

        return 1;
    }
    

}

void displayTreeHelper(TreapNode *array, FILE *fp, int index)
{
    if(index != -1)
    {
        if(array[index].left != -1)
        {
            fprintf(fp,"\n%d -> %d [color=green] ",array[index].key,array[array[index].left].key);
            fprintf(fp,"\n%d [label=\"%d, %d\"];",array[array[index].left].key,array[array[index].left].key,array[array[index].left].priority);
            displayTreeHelper(array,fp,array[index].left);
        }
        if(array[index].right != -1)
        {
            fprintf(fp,"\n%d -> %d [color=yellow]",array[index].key,array[array[index].right].key);
            fprintf(fp,"\n%d [label=\"%d, %d\"];",array[array[index].right].key,array[array[index].right].key,array[array[index].right].priority);
            displayTreeHelper(array,fp,array[index].right);
        }
    }
}

void printTreap(const char *filename, TreapNode *array, int root)
{
    FILE *fptr;
    fptr=fopen(filename,"w+");
    if(fptr == NULL)
    {
        printf("File doesn't exist");
    }
    else
    {
        fprintf(fptr,"digraph g{");
        if(root != -1)
        {
            fprintf(fptr,"\n%d [label=\"%d, %d\"];",array[root].key,array[root].key,array[root].priority);
            displayTreeHelper(array,fptr,root);
        }
        fprintf(fptr,"\n}\n");
        fclose(fptr);
    }   
}

int main()
{
    TreapNode array[SIZE];
    int root = -1;
    int freePointer = 0;
    insertTreap(array,&root,&freePointer,5);
    insertTreap(array,&root,&freePointer,4);
    insertTreap(array,&root,&freePointer,6);
    insertTreap(array,&root,&freePointer,7);
    insertTreap(array,&root,&freePointer,3);
    insertTreap(array,&root,&freePointer,2);
    insertTreap(array,&root,&freePointer,1);

    printTreap("Treap-array.dot",array,root);

    system("dot -Tpng Treap-array.dot -o Treap-array.png");

    return 0;
}