#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>


struct TreapNode
{
    int key;
    int priority;
    struct TreapNode *left;
    struct TreapNode *right;
};

typedef struct TreapNode TreapNode;

struct Stack 
{
    TreapNode *treapNode;
    bool toLeft;
    struct Stack *next;
};

typedef struct Stack Stack;

Stack *getNewStackNode()
{
    return (Stack*)malloc(sizeof(Stack));
}

void push(Stack **S,TreapNode *node,bool toLeft)
{
    Stack *temp = getNewStackNode();

    if(temp == NULL)
    {
        printf("\nMemory not allocated");
        return;
    }

    temp->treapNode = node;
    temp->toLeft = toLeft;
    temp->next = NULL;

    if(*S == NULL)
    {
        *S = temp;
    }
    else
    {
        temp->next = *S;
        *S = temp;
    }
}

Stack *pop(Stack **S)
{
    if(*S == NULL)
    {
        printf("\nStack is empty");
        return NULL;
    }

    Stack *temp = *S;
    *S = (*S)->next;

    return temp;
}

int searchTreap(TreapNode *root, int key)
{
    TreapNode *temp = root;

    while(temp != NULL)
    {
        if(temp->key == key)
        {
            return 1;
        }
        else if(temp->key > key)
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }

    return 0;
}

TreapNode *getNewNode(int key)
{
    TreapNode *node = (TreapNode *)malloc(sizeof(TreapNode));
    node->left = NULL;
    node->right = NULL;
    node->priority = rand()%100;
    node->key = key;

    return node;
}

TreapNode *leftRotation(TreapNode *node)
{
    TreapNode *r = node->right;
    TreapNode *rl = r->left;

    node->right = rl;
    r->left = node;
    
    return r;
}

TreapNode *rightRotation(TreapNode* node)
{
    TreapNode *l = node->left;
    TreapNode *lr = l->right;

    node->left = lr;
    l->right = node;
    
    return l;
}

int insertTreap(TreapNode **root, int key)
{
    TreapNode *temp = *root;
    TreapNode *newNode = getNewNode(key);

    if(temp == NULL)
    {
        *root = newNode;

        return 1;
    }

    Stack *S = NULL;

    while(temp != NULL)
    {
        if(temp->key <= key)
        {
            push(&S,temp,false);
            temp = temp->right;
        }
        else
        {
            push(&S,temp,true);
            temp = temp->left;
        }
    }

    while(S != NULL)
    {
        Stack *stemp = pop(&S);

        if(stemp->toLeft)
        {
            stemp->treapNode->left = newNode;
        }
        else
        {
            stemp->treapNode->right = newNode;
        }

        if(stemp->treapNode->priority < newNode->priority && stemp->toLeft)
        {
            newNode = rightRotation(stemp->treapNode);
        }
        else if(stemp->treapNode->priority < newNode->priority && !stemp->toLeft)
        {
            newNode = leftRotation(stemp->treapNode);  
        }
        else
        {
            newNode = stemp->treapNode;
        }
        free(stemp);
    }
    *root = newNode;

    return 1;
}

void displayTreeHelper(TreapNode *T, FILE *fp)
{
    if(T != NULL)
    {
        if(T->left != NULL)
        {
            fprintf(fp,"\n%d -> %d [color=green] ",T->key,T->left->key);
            fprintf(fp,"\n%d [label=\"%d, %d\"];",T->left->key,T->left->key,T->left->priority);
            displayTreeHelper(T->left,fp);
        }
        if(T->right != NULL)
        {
            fprintf(fp,"\n%d -> %d [color=yellow]",T->key,T->right->key);
            fprintf(fp,"\n%d [label=\"%d, %d\"];",T->right->key,T->right->key,T->right->priority);
            displayTreeHelper(T->right,fp);
        }
    }
}

void printTreap(const char *filename, TreapNode *root)
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
        if(root != NULL)
        {
            fprintf(fptr,"\n%d [label=\"%d, %d\"];",root->key,root->key,root->priority);
            displayTreeHelper(root,fptr);
        }
        fprintf(fptr,"\n}\n");
        fclose(fptr);
    }   
}

int deleteTreap(TreapNode **root,int key)
{
    TreapNode *temp = *root;

    if(temp == NULL)
        return 0;
    if(!searchTreap(temp,key))
        return 0;

    Stack *S = NULL;

    while(temp->key != key)
    {
        if(temp->key < key)
        {
            push(&S,temp,false);
            temp = temp->right;
        }
        else
        {
            push(&S,temp,true);
            temp = temp->left;
        }
    }
    
    while(S != NULL)
    {
        Stack *stemp = pop(&S);
        if(temp->key == key && temp->left == NULL && temp->right == NULL)
        {
            if(stemp->toLeft)
                stemp->treapNode->left = NULL;
            else
                stemp->treapNode->right = NULL;
            free(temp);
        }
        else 
        {
            if(temp->key == key)
                temp->priority = INT16_MIN;
            if(temp->left != NULL && temp->right != NULL)
            {
                if(temp->left->priority > temp->priority) 
                {
                    if(temp->right->priority > temp->left->priority)
                    {
                        temp = leftRotation(temp);
                    }
                    else
                    {
                        temp = rightRotation(temp);
                    }
                }
                else if(temp->right->priority > temp->priority)
                {
                    temp = rightRotation(temp);
                }
            }
            else if(temp->left != NULL)
            {
                if(temp->left->priority > temp->priority)
                {
                    temp = rightRotation(temp);
                }
            }
            else if(temp->right != NULL)
            {
                if(temp->right->priority > temp->priority)
                {
                    temp = leftRotation(temp);
                }
            }

            if(stemp->toLeft)
            {
                stemp->treapNode->left = temp; 
            }
            else
            {
                stemp->treapNode->right = temp;
            }
        } 
        temp = stemp->treapNode;
        free(stemp);  
    }

    return 1;
}

int main()
{
    TreapNode *root = NULL;

    insertTreap(&root,5);
    insertTreap(&root,4);
    insertTreap(&root,6);
    insertTreap(&root,7);
    insertTreap(&root,3);
    insertTreap(&root,1);
    insertTreap(&root,2);

    //deleteTreap(&root,4);
    deleteTreap(&root,4);
    
    printTreap("Treap.dot",root);

    system("dot -Tpng Treap.dot -o Treap.png");
}