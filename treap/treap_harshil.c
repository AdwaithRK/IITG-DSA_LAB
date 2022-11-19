#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
// #include"myStack.h"
struct myStack{
    int *arrStack;
    int size;
    int top;
};

typedef struct myStack myStack;

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

struct treapNode{
    int key;
    int priority;
    int left;
    int right;
};
typedef struct treapNode treapNode;

void displayTreeHelper(treapNode* T, int root, FILE *fp)
{
    if (root!=-1)
    {
        fprintf(fp,"%d[label=\"%d|%d\"];\n",T[root].key, T[root].priority, T[root].key);
        if(T[root].left!=-1)
        {
            fprintf(fp, "%d -> %d [color = red];\n", T[root].key, T[T[root].left].key);
            displayTreeHelper(T, T[root].left, fp);
        }
        if(T[root].right!=-1)
        {
            fprintf(fp, "%d -> %d ;\n", T[root].key, T[T[root].right].key);
            displayTreeHelper(T, T[root].right, fp);
        }
    }
}

int displayTree(treapNode* T, int root, char* filename)
{
    FILE *fp;

    fp = fopen(filename, "w+");
    if (fp == NULL)
    {
        printf("displayTree(): Unable to open file %s", filename);
        return -1;
    }
    fprintf(fp, "digraph g{\n");
    displayTreeHelper(T, root, fp);
    fprintf(fp,"}\n");

    fclose(fp);

    return 0;
}

void print_arr(treapNode* arr, int n){
    for(int i=0;i<n;i++){
        printf("%d:\t%d\t%d\t%d\t%d\n",i,arr[i].left,arr[i].priority,arr[i].key,arr[i].right);
    }
}

void left_rotation(treapNode* arr, int x, int y){
    arr[x].right=arr[y].left;
    arr[y].left=x;
}

void right_rotation(treapNode* arr, int x, int y){
    arr[x].left=arr[y].right;
    arr[y].right=x;
}

void treap_insert(treapNode* arr, int n, int* top, int* root, int key, int priority){
    if(*top==-1){
        printf("treap_insert(): Space not available\n");
        return;
    }

    //allocate space for new node
    int newnode_index=(*top);
    (*top)=arr[(*top)].right;
    arr[newnode_index].key=key;
    arr[newnode_index].priority=priority;
    arr[newnode_index].left=-1;
    arr[newnode_index].right=-1;

    //empty
    if(*root==-1){
        *root=newnode_index;
        return;
    }

    myStack stack;
    createStack(&stack,n);
    //search
    int cur=*root;
    int curParent=-1,curGrandParent=-1;
    //loop until bst node inserted
    while(arr[cur].priority!=-1){
        //Key already exist
        if(arr[cur].key==key){
            printf("treap_insert() : Key=%d already exists!\n",key);
            return;
        }
        //Key is less than current node
        else if(key<arr[cur].key){
            push(&stack,cur);
            //found empty node
            if(arr[cur].left==-1){
                arr[cur].left=newnode_index;
                push(&stack,newnode_index);
                break;
            }
            cur=arr[cur].left;
        }
        //Key is greater than current node
        else{
            push(&stack,cur);
            //Found empty node
            if(arr[cur].right==-1){
                arr[cur].right=newnode_index;
                push(&stack,newnode_index);
                break;
            }
            cur=arr[cur].right;
        }
    }
    pop(&stack,&cur);
    while(stack.top!=-1){
        pop(&stack,&curParent);
        int gp=0;
        if(stack.top!=-1){
            pop(&stack,&curGrandParent);
        }
        else{
            gp=-1;
        }
        if(arr[curParent].priority>arr[cur].priority){
            //current is left child of parent
            //right rotation
            if(arr[curParent].left==cur){
                // right_rotation(arr,curParent,cur);
                arr[curParent].left=arr[cur].right;
                arr[cur].right=curParent;
                if(gp==-1){
                    *root=cur;
                }
                else{
                    if(arr[curGrandParent].left==curParent){
                        arr[curGrandParent].left=cur;
                    }
                    else{
                        arr[curGrandParent].right=cur;
                    }
                    push(&stack,curGrandParent);
                }
            }
            //current is right child of parent
            //left rotation
            else{
                // left_rotation(arr,curParent,cur);
                arr[curParent].right=arr[cur].left;
                arr[cur].left=curParent;
                if(gp==-1){
                    *root=cur;
                }
                else{
                    if(arr[curGrandParent].left==curParent){
                        arr[curGrandParent].left=cur;
                    }
                    else{
                        arr[curGrandParent].right=cur;
                    }
                    push(&stack,curGrandParent);
                }
            }
        }
        else{
            break;
        }
    }
    // print_arr(arr,n);
    // printf("-----------------------------------------------------------------\n");
    destroyStack(&stack);
}

void treap_delete(treapNode* arr, int n, int *top, int *root, int key){
    int cur=*root;
    int curParent=-1;
    while(arr[cur].priority!=-1){
        //found
        if(arr[cur].key==key){
            break;
        }
        else if(key<arr[cur].key){
            if(arr[cur].left==-1){
                printf("treap_delete(): %d not found!\n",key);
                return;
            }
            curParent=cur;
            cur=arr[cur].left;
        }
        else{
            if(arr[cur].right==-1){
                printf("treap_delete(): %d not found!\n",key);
                return;
            }
            curParent=cur;
            cur=arr[cur].right;
        }
    }
    arr[cur].priority=INT_MAX;
    while(!(arr[cur].left==-1 && arr[cur].right==-1)){
        int leftChild=arr[cur].left,rightChild=arr[cur].right;
        //no leftChild; only rightChild
        if(leftChild==-1){
            //left rotation
            arr[cur].right=arr[rightChild].left;
            arr[rightChild].left=cur;
            //update parent
            if(curParent==-1){
                *root=rightChild;
            }
            else{
                //left child
                if(cur==arr[curParent].left){
                    arr[curParent].left=rightChild;
                }
                //right child
                else{
                    arr[curParent].right=rightChild;
                }
            }
            //update curParent
            curParent=rightChild;
        }
        //No rightChild; only leftChild
        else if(rightChild==-1){
            //right rotation
            arr[cur].left=arr[leftChild].right;
            arr[leftChild].right=cur;
            //update parent
            if(curParent==-1){
                *root=leftChild;
            }
            else{
                //left child
                if(cur==arr[curParent].left){
                    arr[curParent].left=leftChild;
                }
                //right child
                else{
                    arr[curParent].right=leftChild;
                }
            }
            //update curParent
            curParent=leftChild;
        }
        //Both children
        else{
            if(arr[leftChild].priority<arr[rightChild].priority){
                //right rotation
                arr[cur].left=arr[leftChild].right;
                arr[leftChild].right=cur;
                //update parent
                if(curParent==-1){
                    *root=leftChild;
                }
                else{
                    //left child
                    if(cur==arr[curParent].left){
                        arr[curParent].left=leftChild;
                    }
                    //right child
                    else{
                        arr[curParent].right=leftChild;
                    }
                }
                //update curParent
                curParent=leftChild;
            }
            else{
                //left rotation
                arr[cur].right=arr[rightChild].left;
                arr[rightChild].left=cur;
                //update parent
                if(curParent==-1){
                    *root=rightChild;
                }
                else{
                    //left child
                    if(cur==arr[curParent].left){
                        arr[curParent].left=rightChild;
                    }
                    //right child
                    else{
                        arr[curParent].right=rightChild;
                    }
                }
                //update curParent
                curParent=rightChild;
            }
        }
    }
    //check deleting node is left child or right child
    //left child
    if(cur==arr[curParent].left){
        arr[curParent].left=-1;
    }
    //right child
    else{
        arr[curParent].right=-1;
    }
    //delete and update top
    arr[cur].left=-1;
    arr[cur].priority=-1;
    arr[cur].right=*top;
    *top=cur;
}

int main(){
    FILE* fp=fopen("input.txt","r");
    if(fp==NULL){
        printf("main(): Error opening file input.txt\n");
        return -1;
    }
    int n;
    fscanf(fp,"%d",&n);
    // printf("Initial Size:");
    // scanf("%d",&n);
    treapNode *arr=(treapNode*)malloc(n*sizeof(treapNode));
    if(arr==NULL){
        printf("main: malloc failure!\n");
        exit(EXIT_FAILURE);
    }
    int top=0;
    int root=-1;
    for(int i=0;i<n;i++){
        arr[i].left=-1;
        arr[i].priority=-1;
        arr[i].right=i+1;
        if(i==n-1){
            arr[i].right=-1;
        }
    }
    for(int i=0;i<n;i++){
        int k,p;
        // printf("Priority:");
        // scanf("%d",&p);
        // printf("Key:");
        // scanf("%d",&k);
        fscanf(fp,"%d%d",&p,&k);
        treap_insert(arr,n,&top,&root,k,p);
       // displayTree(arr, root, "treap_bst.dot");
       // system("dot -Tpng treap_bst.dot -o treap_bst.png");

    }
    print_arr(arr,n);
    printf("Root:%d\tTop:%d\n",root,top);
    treap_delete(arr,n,&top,&root,17);
    print_arr(arr,n);
    printf("Root:%d\tTop:%d\n",root,top);
    displayTree(arr, root, "treap_delete.dot");
    system("dot -Tpng treap_delete.dot -o treap_delete.png");
}
