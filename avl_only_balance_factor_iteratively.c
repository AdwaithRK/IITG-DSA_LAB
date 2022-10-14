#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct AVLNode
{
    int key;
    int bf; // left - right
    int size;
    struct AVLNode *leftChild;
    struct AVLNode *rightChild;
};

typedef struct AVLNode AVLNode;
typedef struct AVLNode *AVLNodePtr;

void createAVLTree(AVLNodePtr *root) { *root = NULL; }

struct StackRecord
{
    AVLNodePtr val;
    struct StackRecord *next;
};

struct StackRecord *top = NULL;

int push(AVLNodePtr root)
{
    struct StackRecord *newRecord = (struct StackRecord *)malloc(sizeof(struct StackRecord));
    if (newRecord == NULL)
    {
        return 0;
    }

    newRecord->val = root;
    newRecord->next = top;
    top = newRecord;

    return 1;
}

AVLNodePtr pop()
{
    if (top == NULL)
    {
        return NULL;
    }

    struct StackRecord *newRecord = top;

    AVLNodePtr ptr = top->val;
    top = top->next;

    free(newRecord);

    return ptr;
}

int empty()
{
    if (top == NULL)
    {
        return 1;
    }
    return 0;
}

void emptyStack()
{
    while (!empty())
    {
        pop();
    }
}

AVLNodePtr top_of_stack()
{
    if (top == NULL)
        return NULL;
    return top->val;
}

AVLNodePtr getAVLTreeNode()
{
    AVLNodePtr node = (AVLNodePtr)malloc(sizeof(AVLNode));
    node->bf = 0;
    node->leftChild = NULL;
    node->rightChild = NULL;
    node->size = 1;
    return node;
}

AVLNodePtr AVLTreeSearch(AVLNodePtr root, int value)
{
    if (root == NULL)
    {
        return NULL; // end of path, node not found
    }
    // search in right tree
    if (value > root->key)
    {
        return AVLTreeSearch(root->rightChild, value);
    }
    // search at left
    else if (value < root->key)
    {
        return AVLTreeSearch(root->leftChild, value);
    }
    else
        return root;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a > b ? b : a;
}

AVLNodePtr rightRotation(AVLNodePtr *root)
{
    AVLNodePtr parent = *root;
    AVLNodePtr child = parent->leftChild;
    parent->leftChild = child->rightChild;
    child->rightChild = parent;
    *root = child;

    parent->bf = parent->bf + 1 - min(child->bf, 0);
    child->bf = child->bf + 1 + max(parent->bf, 0);
    return child;
}

AVLNodePtr leftRotation(AVLNodePtr *root)
{
    AVLNodePtr parent = *root;
    AVLNodePtr child = parent->rightChild;
    parent->rightChild = child->leftChild;
    child->leftChild = parent;
    *root = child;

    parent->bf = parent->bf - 1 - max(child->bf, 0);
    child->bf = child->bf - 1 + min(parent->bf, 0);
    return child;
}

void AVLInsert(AVLNodePtr *root, int data)
{
    if (*root == NULL)
    {
        AVLNodePtr newNode = getAVLTreeNode();
        newNode->key = data;
        *root = newNode;
        return;
    }

    AVLNodePtr cur = *root;
    AVLNodePtr prev = NULL;

    int flag = 0;

    while (cur != NULL)
    {
        push(cur);
        prev = cur;
        if (cur->key == data)
        {
            emptyStack();
            return; // The data already exist
        }
        if (data < cur->key)
        {
            cur = cur->leftChild;
        }
        else
        {
            cur = cur->rightChild;
        }
    }

    AVLNodePtr temp = getAVLTreeNode();
    temp->key = data;

    if (temp)
    {
        if (data < prev->key)
        {
            prev->leftChild = temp;
        }
        else
        {
            prev->rightChild = temp;
        }
    }

    bool heightIncreased = true;

    while (!empty())
    {
        AVLNodePtr parent = pop();               // current
        AVLNodePtr grandParent = top_of_stack(); // parent of current

        if (heightIncreased == false)
            continue;

        if (data < parent->key)
        {
            if (parent->key == 1)
            {
                parent->bf = 0;
                heightIncreased = false;
                break;
            }
            else if (parent->bf == -1)
            {
                parent->bf = -2;
                if (data < grandParent->key)
                {
                    grandParent->leftChild = rightRotation(&parent);
                }
                else
                { // parents right child
                    grandParent->rightChild = leftBalance(&anc);
                }
            }
        }
    }
}

void displayTreeHelper(AVLNodePtr *T, FILE *fp)
{
    if (T != NULL)
    {

        fprintf(fp, "%d[label=\"%d, bf:%d\"];", (*T)->key, (*T)->key, (*T)->bf);
        if ((*T)->leftChild != NULL)
        {
            fprintf(fp, "%d -> %d [color = red, style=dotted];\n", (*T)->key, (*T)->leftChild->key);
            displayTreeHelper(&(*T)->leftChild, fp);
        }
        if ((*T)->rightChild != NULL)
        {
            fprintf(fp, "%d -> %d ;\n", (*T)->key, (*T)->rightChild->key);
            displayTreeHelper(&(*T)->rightChild, fp);
        }
    }
}

int displayTree(AVLNodePtr root)
{
    FILE *fpointer;
    fpointer = fopen("tree.dot", "w+");
    if (!fpointer)
    {
        printf("Unble to open file\n");
        return 1;
    }
    fprintf(fpointer, "digraph g {\n");
    displayTreeHelper(&root, fpointer);
    fprintf(fpointer, "}");
    fclose(fpointer);
    system("dot -Tpng tree.dot -o tree.png");
    return 0;
}

int main()
{

    AVLNodePtr root;
    createAVLTree(&root);

    AVLInsert(&root, 24);
    AVLInsert(&root, 12);
    AVLInsert(&root, 42);
    AVLInsert(&root, 47);

    displayTree(root);
}
