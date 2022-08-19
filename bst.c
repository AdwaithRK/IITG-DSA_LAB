#include <stdio.h>
#include <stdlib.h>

struct bstNode{
       int key;
       struct bstNode *leftChild;
       struct bstNode *rightChild;
};

typedef struct bstNode bstNode;
typedef struct bstNode *bstNodePtr;

void createBST(bstNodePtr *root)
{
     *root = NULL;
}

/* Returns NULL in case of allocation faliure*/
bstNodePtr getTreeNode()
{
     bstNodePtr p;
     p = (bstNodePtr)malloc(sizeof(bstNode));
     if (p != NULL)
     {
           p->leftChild = NULL;
           p->rightChild = NULL;
     }
     return p;
}

/* Function returns -1 if duplicate exists; otherwise return 0*/
int bstInsert(bstNodePtr *root, int data)
{
    bstNodePtr p, currPtr, currPtrParent;
    /* Insert into an empty tree */
    if (*root == NULL)
    {
        p = getTreeNode();
        if (p == NULL)
           return -2; /* Allocation faliure*/
        p->key = data;
        *root = p;
        return 0;
    }

    currPtr = *root;
    currPtrParent = NULL;
    while (currPtr != NULL)
    {
        if (currPtr->key == data)
            return -1; /*Duplicate exists*/
        else if (currPtr->key > data)
        {
            currPtrParent = currPtr;
            currPtr = currPtr->leftChild;
        }
        else
        {
            currPtrParent = currPtr;
            currPtr = currPtr->rightChild;
        }
    }
    p = getTreeNode();
    if (p == NULL)
       return -2; /* Allocation faliure*/
    p->key = data;

    if (currPtrParent->key > data)
       currPtrParent->leftChild = p;
    else
       currPtrParent->rightChild = p;

    return 0;
}

void displayTreeHelper(bstNodePtr T, FILE *fp)
{
    if (T != NULL)
    {
        if(T->leftChild != NULL)
        {
            fprintf(fp, "%d -> %d [color = red, style=dotted];\n", T->key, T->leftChild->key);
            displayTreeHelper(T->leftChild, fp);
        }
        if(T->rightChild != NULL)
        {
            fprintf(fp, "%d -> %d ;\n", T->key, T->rightChild->key);
            displayTreeHelper(T->rightChild, fp);
        }
    }
}

int displayTree(bstNodePtr T, char* filename)
{
    FILE *fp;

    fp = fopen(filename, "w+");
    if (fp == NULL)
    {
        printf("displayTree(): Unable to open file %s", filename);
        return -1;
    }
    fprintf(fp, "digraph g{\n");
    displayTreeHelper(T, fp);
    fprintf(fp,"}\n");

    fclose(fp);

    return 0;
}

void inorder(bstNodePtr T)
{
    if (T != NULL)
    {
        inorder(T->leftChild);
        printf("%d ", T->key);
        inorder(T->rightChild);
    }

}


int main(int argc, char *argv[])
{
    bstNodePtr T;

    createBST(&T);

    bstInsert(&T, 100);
    bstInsert(&T, 110);
    bstInsert(&T, 50);
    bstInsert(&T, 70);
    bstInsert(&T, 200);
    bstInsert(&T, 20);
    bstInsert(&T, 105);
    bstInsert(&T, 115);

    inorder(T);

    displayTree(T, "tree.dot");

    system("dot -Tpng tree.dot -o tree.png");

    //system("PAUSE");
    return 0;
}



