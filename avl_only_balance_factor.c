#include <stdio.h>
#include <stdlib.h>

struct node
{
    int key, balance;
    struct node *left;
    struct node *right;
};

typedef struct node node;
typedef struct node *nodePtr;

void createBST(nodePtr *root)
{
    *root = NULL;
}

// simple function to create a node
nodePtr getTreeNode()
{
    nodePtr newNode = (nodePtr)malloc(sizeof(struct node));
    newNode->balance = 0;
    newNode->left = NULL;
    newNode->right = NULL;
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
                if ((*root)->left->balance >= 0)
                {
                    rightRotation(root);
                }
                else
                {
                    leftRotation(&(*root)->right);
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
                checkBalance = 0;
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
        }
    }
}

void displayTreeHelper(nodePtr T, FILE *fp)
{
    if (T != NULL)
    {
        fprintf(fp, "%d[label=\"%d, bf: %d\"];", T->key, T->key, T->balance);
        if (T->left != NULL)
        {
            fprintf(fp, "%d -> %d [color = red, style=dotted];\n", T->key, T->left->key);
            displayTreeHelper(T->left, fp);
        }
        if (T->right != NULL)
        {
            fprintf(fp, "%d-> %d ;\n", T->key, T->right->key);
            displayTreeHelper(T->right, fp);
        }
    }
}

int displayTree(nodePtr T, char *filename)
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
    fprintf(fp, "}\n");

    fclose(fp);

    return 0;
}

int main()
{
    nodePtr root;
    createBST(&root);
    for (int i = 0; i < 20; i++)
        insert(&root, i);

    insert(&root, 20);
    insert(&root, 21);

    delete_node(&root, 7);
    delete_node(&root, 15);
    delete_node(&root, 17);

    // delete_node(&root, 9);
    // delete_node(&root, 8);

    // for (int i = 0; i < 20; i++)
    //     delete_node(&root, i);

    displayTree(root, "tree.dot");
    system("dot -Tpng tree.dot -o tree.png");

    return 0;
}