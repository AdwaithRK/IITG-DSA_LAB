#include <stdio.h>
#include <stdlib.h>

struct node
{
    int key, balance;
    struct node *left, *right;
};

typedef struct node node;
typedef struct node *nodePtr;

void createBST(nodePtr *root)
{
    *root = NULL;
}

nodePtr getTreeNode()
{
    nodePtr newNode = (nodePtr)malloc(sizeof(struct node));
    newNode->balance = 0;
    newNode->left = NULL;
    newNode->right = NULL;
}

void displayTreeHelper(nodePtr root, FILE *fpointer)
{
    if (!root)
        return;
    if (root->left)
    {
        fprintf(fpointer, "%d [label = \"%d, %d\"]\n", root->key, root->key, root->balance);
        fprintf(fpointer, "%d [label = \"%d, %d\"]\n", root->left->key, root->left->key, root->left->balance);
        fprintf(fpointer, "%d -> %d [style=dotted, color=green]\n", root->key, root->left->key);
        displayTreeHelper(root->left, fpointer);
    }
    if (root->right)
    {
        fprintf(fpointer, "%d [label = \"%d, %d\"]\n", root->key, root->key, root->balance);
        fprintf(fpointer, "%d [label = \"%d, %d\"]\n", root->right->key, root->right->key, root->right->balance);
        fprintf(fpointer, "%d -> %d [color=red]\n", root->key, root->right->key);
        displayTreeHelper(root->right, fpointer);
    }
}

int displayTree(nodePtr root)
{
    FILE *fpointer;
    fpointer = fopen("tree.dot", "w+");
    if (!fpointer)
    {
        printf("Unble to open file\n");
        return 1;
    }
    fprintf(fpointer, "digraph g {\n");
    displayTreeHelper(root, fpointer);
    fprintf(fpointer, "}");
    fclose(fpointer);
    system("dot -Tpng tree.dot -o tree.png");
    return 0;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a > b ? b : a;
}

void leftRotation(nodePtr *root)
{
    nodePtr x, y;
    x = *root;
    y = x->right;
    x->right = y->left;
    y->left = x;
    *root = y;
    x->balance = x->balance - 1 - max(y->balance, 0);
    y->balance = y->balance - 1 + min(x->balance, 0);
}

void rightRotation(nodePtr *root)
{
    nodePtr x, y;
    x = *root;
    y = x->left;
    x->left = y->right;
    y->right = x;
    *root = y;
    x->balance = x->balance + 1 - min(y->balance, 0);
    y->balance = y->balance + 1 + max(x->balance, 0);
}

void insert(nodePtr *root, int data)
{
    static int checkBalance = 0;
    nodePtr x, y;
    if (!(*root))
    {
        *root = getTreeNode();
        (*root)->key = data;
        return;
    }
    else if (data < (*root)->key)
    {
        if ((*root)->left)
        {
            insert(&(*root)->left, data);
        }
        else
        {
            (*root)->left = getTreeNode();
            (*root)->left->key = data;
            checkBalance = 1;
        }
        if (checkBalance)
        {
            if ((*root)->balance == 1)
            {
                (*root)->balance = 0;
                checkBalance = 0;
            }
            else if ((*root)->balance == -1)
            {
                (*root)->balance = -2;
                if ((*root)->left->balance == -1)
                    rightRotation(root);
                else if ((*root)->left->balance == 1)
                {
                    leftRotation(&(*root)->left);
                    rightRotation(root);
                }
                checkBalance = 0;
            }
            else if ((*root)->balance == 0)
            {
                (*root)->balance = -1;
            }
        }
    }
    else if (data > (*root)->key)
    {
        if ((*root)->right)
        {
            insert(&(*root)->right, data);
        }
        else
        {
            (*root)->right = getTreeNode();
            (*root)->right->key = data;
            checkBalance = 1;
        }
        if (checkBalance)
        {
            if ((*root)->balance == -1)
            {
                (*root)->balance = 0;
                checkBalance = 0;
            }
            else if ((*root)->balance == 1)
            {
                (*root)->balance = 2;
                if ((*root)->right->balance == 1)
                    leftRotation(root);
                else if ((*root)->right->balance == -1)
                {
                    rightRotation(&(*root)->right);
                    leftRotation(root);
                }
                checkBalance = 0;
            }
            else if ((*root)->balance == 0)
            {
                (*root)->balance = 1;
            }
        }
    }
}

nodePtr find_inorder_successor(nodePtr current)
{
    if (current->right != NULL)
    {
        current = current->right;

        while (current->left != NULL)
        {
            current = current->left;
        }

        return current;
    }
    return NULL;
}

void delete_node(nodePtr *root, int data)
{

    static int checkBalance = 0;

    if (*root == NULL)
    {
        return;
    }

    // printf("Entered Delete\n");
    if (data < (*root)->key)
    {
        delete_node(&(*root)->left, data);
        if (checkBalance)
        {
            if ((*root)->balance == 1)
            {
                (*root)->balance++;
                if ((*root)->right->balance >= 0)
                {
                    leftRotation(root);
                }
                else
                {
                    rightRotation(&(*root)->right);
                    leftRotation(root);
                }
            }
            else if ((*root)->balance == -1)
            {
                (*root)->balance = 0;
            }
            else if ((*root)->balance == 0)
            {
                (*root)->balance = 1;
                checkBalance = 0;
            }
        }
    }
    else if (data > (*root)->key)
    {
        delete_node(&(*root)->right, data);
        if (checkBalance)
        {
            if ((*root)->balance == -1)
            {
                (*root)->balance--;
                if ((*root)->left->balance >= 0)
                {
                    leftRotation(&(*root)->left);
                    rightRotation(root);
                }
                else
                {
                    rightRotation(root);
                }
            }
            else if ((*root)->balance == 1)
            {
                (*root)->balance = 0;
            }
            else if ((*root)->balance == 0)
            {
                (*root)->balance = 1;
            }
        }
    }
    else
    {
        if ((*root)->left == NULL || (*root)->right == NULL)
        {
            checkBalance = 1;
            nodePtr temp = (*root)->left ? (*root)->left : (*root)->right;

            if (temp == NULL)
            {
                *root = temp;
            }
            else
            {
                **root = *temp;
            }
            free(temp);
        }
        else
        {
            nodePtr temp = find_inorder_successor(*root);
            (*root)->key = temp->key;

            delete_node(&(*root)->right, temp->key);
            if (checkBalance == 1)
            {

                if ((*root)->balance == -1)
                {
                    (*root)->balance = -2;
                    if ((*root)->left->balance >= 0)
                    {
                        leftRotation(&(*root)->right);
                        rightRotation(root);
                    }
                    else
                    {
                        rightRotation(root);
                    }
                }
                else if ((*root)->balance == 1)
                {
                    (*root)->balance = 0;
                }
                else if ((*root)->balance == 0)
                {
                    (*root)->balance = -1;
                    checkBalance = 0;
                }
            }
        }
    }
}

void inorder(nodePtr root)
{
    if (!root)
        return;
    inorder(root->left);
    printf("Key: %d, Balance: %d\n", root->key, root->balance);
    inorder(root->right);
}

int main()
{
    nodePtr root;
    createBST(&root);
    for (int i = 0; i < 20; i++)
        insert(&root, i);

    // insert(&root, 24);
    // insert(&root, 12);
    // insert(&root, 42);
    // insert(&root, 41);

    // delete_node(&root, 11);
    // delete_node(&root, 15);
    // delete_node(&root, 17);

    // delete_node(&root, 9);
    delete_node(&root, 18);

    // for (int i = 0; i < 20; i++)
    //     delete_node(&root, i);

    displayTree(root);

    return 0;
}