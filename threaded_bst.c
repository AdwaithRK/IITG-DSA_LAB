#include <stdio.h>
#include <stdlib.h>

struct ThBSTNode
{
    int key;
    struct ThBSTNode *left;
    struct ThBSTNode *right;
    int rightThread;
};

struct ThBSTNode *root = NULL;

struct ThBSTNode *insertThBSTNode(int value)
{
    struct ThBSTNode *ptr = root;
    struct ThBSTNode *parent = NULL;

    while (ptr != NULL)
    {
        if (value == (ptr->key))
        {
            printf("Duplicate Key !\n");
            return root;
        }

        parent = ptr;

        if (value < ptr->key)
        {
            ptr = ptr->left;
        }
        else
        {
            if (ptr->rightThread != 1)
            {
                ptr = ptr->right;
            }
            else
                break;
        }
    }

    struct ThBSTNode *element = (struct ThBSTNode *)malloc(sizeof(struct ThBSTNode));
    element->key = value;
    element->rightThread = 1;
    printf("here here");

    if (parent == NULL)
    {
        printf("\ncreating root\n");
        root = element;
        element->left = NULL;
        element->right = NULL;
    }
    else if (value < (parent->key))
    {
        element->right = parent;
        parent->left = element;
    }
    else
    {
        element->right = parent->right;
        parent->right = element;
        parent->rightThread = 0;
    }

    return root;
}

void displayTreeHelper(struct ThBSTNode *T, FILE *fp)
{
    if (T != NULL)
    {
        if (T->left != NULL)
        {
            fprintf(fp, "%d -> %d [color = red, style=dotted];\n", T->key, T->left->key);
            displayTreeHelper(T->left, fp);
        }
        if (T->right != NULL && (T->rightThread != 1))
        {
            fprintf(fp, "%d -> %d ;\n", T->key, T->right->key);
            displayTreeHelper(T->right, fp);
        }
        if (T->right != NULL && (T->rightThread == 1))
        {
            fprintf(fp, "%d -> %d [color = blue, style=dotted];\n", T->key, T->right->key);
        }
    }
}

int displayTree(struct ThBSTNode *T, char *filename)
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

struct ThBSTNode *inorderSuccessor(struct ThBSTNode *node)
{
    if (node->rightThread == 1)
        return node->right;

    node = node->right;

    while (node->left != NULL)
        node = node->left;

    return node;
}

struct ThBSTNode *inorderPred(struct ThBSTNode *current)
{
    if (current == NULL)
        return NULL;

    current = current->left;
    while (current->rightThread == 0)
        current = current->right;
    return current;
}

void printInorderTraversal()
{
    struct ThBSTNode *temp = root;
    while (temp->left != NULL)
        temp = temp->left;

    while (temp->rightThread != 1 && temp->right != NULL)
    {
        printf("\n%d\t", temp->key);
        temp = inorderSuccessor(temp);
    }
}

void deleteThBinaryTree(int value)
{

    int found = 0;

    struct ThBSTNode *temp = root, *parent = NULL;

    while (temp != NULL)
    {
        if (temp->key == value)
        {
            found = 1;
            break;
        }

        parent = temp;
        if (temp->key > value)
        {
            printf("temp value %d\n", temp->key);
            temp = temp->left;
        }
        else
        {
            if (temp->rightThread == 0)
            {
                temp = temp->right;
            }

            else
                break;
        }
    }

    if (found == 1)
    {
        printf("\nFound!!!\n");
        if (temp->left == NULL && temp->rightThread == 1)
        {
            if (parent->left == temp)
            {
                parent->left = NULL;
                free(temp);
            }
            else
            {
                parent->right = temp->right;
                parent->rightThread = 1;
                free(temp);
            }
        }
        else if (temp->left == NULL || temp->rightThread == 1)
        {
            if (temp->rightThread == 1)
            {
                struct ThBSTNode *successor = inorderSuccessor(temp);
                struct ThBSTNode *predecessor = inorderPred(temp);
                struct ThBSTNode *child = temp->left;

                predecessor->right = successor;
                predecessor->rightThread = 1;

                if (parent->left == temp)
                    parent->left = child;
                else
                    parent->right = child;
            }
            else
            {
                printf("bow bow");

                struct ThBSTNode *child = temp->right;

                if (parent->left == temp)
                    parent->left = child;
                else
                    parent->right = child;
            }
        }
        else
        {
            struct ThBSTNode *succ = inorderSuccessor(temp);
            int k = succ->key;
            deleteThBinaryTree(k);
            temp->key = k;
        }
    }
}

int main()
{
    // insertThBSTNode(8);
    // insertThBSTNode(5);
    // insertThBSTNode(10);
    // insertThBSTNode(6);
    // insertThBSTNode(4);
    // insertThBSTNode(9);
    // deleteThBinaryTree(10);

    insertThBSTNode(16);
    insertThBSTNode(10);
    insertThBSTNode(14);
    insertThBSTNode(15);
    insertThBSTNode(13);
    deleteThBinaryTree(14);
    displayTree(root, "tree.dot");
    system("dot -Tpng tree.dot -o tree.png");
}