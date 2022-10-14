#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

struct AVLNode
{
    int key;
    int bf;
    int size;
    struct AVLNode *leftchild;
    struct AVLNode *rightchild;
};

typedef struct AVLNode AVLNode;
typedef struct AVLNode *AVLNodePtr;

void createAVLTree(AVLNodePtr *root)
{
    *root = NULL;
}

AVLNodePtr getAVLTreeNode()
{
}

/*
    A Helper function for printing all the nodes of the  AVL tree
    Takes input root and the file pointer to write into
    to be visualized through graphviz
    */

void printnode(AVLNodePtr root, FILE *fp)
{
    // print the tree structure through graphviz
    if (root != NULL)
    {
        printf("Node %d with balance factor %d and size %d \n", root->key, root->bf, root->size);
        if (root->leftchild)
        {
            // if leftchild is not null, then traverse it
            fprintf(fp, "%d%s%d%s", root->key, "->", root->leftchild->key, ";\n");
            printnode(root->leftchild, fp);
        }
        if (root->rightchild)
        {
            // if rightchild is not null, then traverse it
            fprintf(fp, "%d%s%d%s", root->key, "->", root->rightchild->key, ";\n");
            printnode(root->rightchild, fp);
        }
    }
}

/*
    An AVL tree function to display the tree through graphviz
    Takes help of a helper function printnode to traverse the nodes.
    */

void displayAVLTree(AVLNodePtr *root, char *filename)
{

    FILE *fp;
    char command[50];
    fp = fopen("graph.gv", "w");

    fprintf(fp, "%s", "digraph G { \n");
    printnode(*root, fp);
    fprintf(fp, "%s", "}");

    strcpy(command, "dot -Tpng graph.gv -o ");
    strcat(command, filename);
    printf("%s\n", command);
    fclose(fp);
    // Execute the command to generate the png file reperesenting the graph
    system(command);
}

/*
    A helper function to help in left rotating a particular node
              parent
         /
        y = imbalance node
           / \
          x  T2
         / \
        T1  z
    After rotation it becomes
          parent
         /
        x
           / \
          T1  y
             / \
            z  T2
    Refer to this structure to understand the logic
    */

void leftrotate(AVLNodePtr *n, AVLNodePtr *root, AVLNodePtr stack[], int *i)
{

    int val;
    AVLNodePtr x, y, z, n1 = *n, parent = NULL;
    x = n1->rightchild;
    y = n1;
    z = x->leftchild;
    if ((*i) >= 0)
        // extract the parent of the current node
        //  present on the top of the stack
        parent = stack[*i];
    if (parent)
    {
        // rearranging the parent's pointers
        if (n1 == parent->leftchild)
            parent->leftchild = x;
        else
            parent->rightchild = x;
    }
    else
    {
        printf("Root is getting rotated \n");
        // if a node has no parent then it is the root
        *root = x;
        printf("New root data is %d\n", (*root)->key);
    }
    // rearrange the pointers
    x->leftchild = y;
    y->rightchild = z;

    // update the balance factors
    y->bf = y->bf - 1 - max(0, x->bf);
    x->bf = x->bf - 1 + min(0, y->bf);

    if (z)
        val = z->size;
    else
        val = 0;

    // Refer to the structure above to understand the logic
    //  size(y)=size(x)+size(T2)+1, size(y')=size(z)+size(T2)+1
    //  size(x)=size(z)+size(T1)+1, size(x')=size(y')+size(T1)+1
    //  Now express size(y') and size(x') in terms of size(x) and size(y) respectively
    y->size += (val - x->size);
    x->size += (y->size - val);
}

/*
    A helper function to help in right rotating a particular node
        parent
        \
        y = imbalance node
           / \
          T2  x
         / \
        z  T1
    after rotation it becomes
       parent
           \
        x
           / \
          y   T1
         / \
        T2  z
    */
void rightrotate(AVLNodePtr *n, AVLNodePtr *root, AVLNodePtr stack[], int *i)
{

    int val;
    AVLNodePtr x, y, z, n1 = *n, parent = NULL;
    x = n1->leftchild;
    y = n1;
    z = x->rightchild;
    if ((*i) >= 0)
        // extract the parent of the current node
        //  present on the top of the stack
        parent = stack[*i];
    if (parent)
    {
        // rearranging the parent's pointers
        if (n1 == parent->leftchild)
            parent->leftchild = x;
        else
            parent->rightchild = x;
    }
    else
    {
        printf("Root is getting rotated \n");
        // if a node has no parent then it is the root
        *root = x;
        printf("New root data is %d\n", (*root)->key);
    }

    // rearrange the pointers
    x->rightchild = y;
    y->leftchild = z;

    // update the balance factors
    y->bf = y->bf + 1 - min(0, x->bf);
    x->bf = x->bf + 1 + max(0, y->bf);

    if (z)
        val = z->size;
    else
        val = 0;

    // Refer to the structure above to understand the logic
    //  size(y)=size(x)+size(T2)+1, size(y')=size(z)+size(T2)+1
    //  size(x)=size(z)+size(T1)+1, size(x')=size(y')+size(T1)+1
    //  Now express size(y') and size(x') in terms of size(x) and size(y) respectively
    y->size += (val - x->size);
    x->size += (y->size - val);
}

/*
    An helper function to identigy and rotate nodes in case of imbalance
    takes help of leftrotate and rightrotate functions
    */

void rotate(AVLNodePtr *n, AVLNodePtr *root, AVLNodePtr stack[], int *i)
{

    AVLNodePtr x = *n;
    stack[++(*i)] = x;
    if (x->bf > 0)
    {
        // New node inserted in right subtree of x
        if (x->rightchild->bf < 0)
        {
            // new node inserted in left subtree of right child of x
            printf("Performing R-L rotation \n");
            rightrotate(&x->rightchild, root, stack, i);
            (*i)--;
            leftrotate(&x, root, stack, i);
        }
        else
        {
            // new node inserted in right subtree of right child of x
            printf("Performing L-L rotation \n");
            (*i)--;
            leftrotate(&x, root, stack, i);
        }
    }
    else
    {
        // New node inserted in left subtree of x
        if (x->leftchild->bf > 0)
        {
            // new node inserted in right subtree of left child of x
            printf("Performing L-R rotation \n");
            leftrotate(&x->leftchild, root, stack, i);
            (*i)--;
            rightrotate(&x, root, stack, i);
        }
        else
        {
            // new node inserted in left subtree of left child of x
            printf("Performing R-R rotation \n");
            (*i)--;
            rightrotate(&x, root, stack, i);
        }
    }
}

/*
    A helper function to update the balance factors in case of insertion
    and also check if balance factor is in range -1 to 1
    else rotate
    takes help of rotate function
    */

void balance(AVLNodePtr *n, AVLNodePtr *root, AVLNodePtr stack[], int *i)
{

    AVLNodePtr x = *n, parent = NULL;
    while ((*i) >= 0)
    {
        if (x->bf < -1 || x->bf > 1)
        {
            // There is an imbalance
            printf("The unbalanced node is %d with bf as %d\n", x->key, x->bf);
            rotate(&x, root, stack, i);
            break;
        }
        // Extracting the parent of current node present in the top of stack
        parent = stack[*i];
        (*i)--;

        // updating the parent's balance factors
        if (x == parent->leftchild)
            parent->bf -= 1;
        else
            parent->bf += 1;
        printf("New BF of %d the parent of %d is %d \n", parent->key, x->key, parent->bf);
        if (parent->bf)
            x = parent;
        else
            break;
    }
    if (x->bf < -1 || x->bf > 1)
    {
        rotate(&x, root, stack, i);
    }
}

void AVLInsert(AVLNodePtr *root, int data)
{

    AVLNodePtr stack[100];
    int i = -1;

    // create a newnode
    AVLNodePtr newnode = (AVLNodePtr)malloc(sizeof(AVLNode));
    if (newnode == NULL)
    {
        printf("The new node couldn't be created\n");
        return;
    }
    newnode->key = data;
    newnode->bf = 0;
    newnode->size = 1;
    newnode->leftchild = NULL;
    newnode->rightchild = NULL;

    AVLNodePtr x = *root, y = NULL;

    while (x != NULL)
    {
        // store the potential ancestors of newnode in stack
        stack[++i] = x;
        y = x;
        // printf("The data inserted in stack is %d \n", stack[i]->key);
        // If newnode data is less than curret key, it should be inserted in left subtree
        // else right subtree
        if (data < x->key)
            x = x->leftchild;
        else
            x = x->rightchild;
    }
    if (y == NULL)
        // if root is null then this is the root
        *root = newnode;
    else
    {
        // initialize parents pointers
        if (newnode->key < y->key)
            y->leftchild = newnode;
        else
            y->rightchild = newnode;
    }

    if (y)
        printf("%d node is inserted  with parent %d and i is %d\n", newnode->key, y->key, i);

    // printf("The contents of the stack are :\n");
    for (int j = 0; j <= i; j++)
    {
        // update the size of all its ancestors
        stack[j]->size++;
        // printf("%d \t", stack[j]->key);
    }
    printf("\n");
    balance(&newnode, root, stack, &i);
}

/*
    An AVL tree function to delete and free the memory of all the nodes of the tree
    */

void deleteTree(AVLNodePtr *root)
{

    AVLNodePtr temp = *root;
    if (temp != NULL)
    {
        if (temp->leftchild)
            deleteTree(&temp->leftchild);
        if (temp->rightchild)
            deleteTree(&temp->rightchild);
        free(temp);
    }
    *root = NULL;
}

int main()
{
    // struct Node *root=(struct Node*)malloc(sizeof(struct Node));
    AVLNodePtr root;
    char filename[50] = "image.png";
    createAVLTree(&root);
    AVLInsert(&root, 5);
    // printf("The root after inserting 5 is %d \n", root->key);
    AVLInsert(&root, 10);
    // printf("The root after inserting 10 is %d \n", root->key);
    AVLInsert(&root, 15);
    // printf("The root after inserting 15 is %d \n", root->key);
    // displayAVLTree(&root, filename);
    AVLInsert(&root, 20);
    // displayAVLTree(&root, filename);
    AVLNodePtr stack[100];
    int i = -1;
    // printf("The root we r getting in main is %d\n", root->key);
    AVLInsert(&root, 25);
    // displayAVLTree(&root, filename);
    AVLInsert(&root, 30);
    // displayAVLTree(&root, filename);
    AVLInsert(&root, 2);
    // displayAVLTree(&root, filename);
    AVLInsert(&root, 1);
    // displayAVLTree(&root, filename);
    printf("\n\n\n");
    AVLInsert(&root, 3);
    AVLInsert(&root, 8);
    AVLInsert(&root, 45);

    displayAVLTree(&root, filename);

    deleteTree(&root);
    displayAVLTree(&root, "image3.png");
    return 0;
}