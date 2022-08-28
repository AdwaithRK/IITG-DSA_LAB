#include <stdio.h>
#include <stdlib.h>

struct ThBSTNode
{
    int key;
    struct ThBSTNode *left;
    struct ThBSTNode *right;
    int size;
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
            ptr->size++;
            ptr = ptr->left;
        }
        else
        {
            if (ptr->rightThread != 1)
            {
                ptr->size++;
                ptr = ptr->right;
            }
            else
            {
                ptr->size++;
                break;
            }
        }
    }

    struct ThBSTNode *element = (struct ThBSTNode *)malloc(sizeof(struct ThBSTNode));
    element->key = value;
    element->rightThread = 1;
    element->size = 0;
    printf("here here");

    if (parent == NULL)
    {
        printf("\ncreating root\n");
        root = element;
        element->left = NULL;
        element->right = NULL;
        element->rightThread = 0;
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
        fprintf(fp, "%d[label=\"%d, size:%d\"];", T->key, T->key, T->size);
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

int sizeOfTree(struct ThBSTNode *root)
{
    if (!root)
        return 0;
    return root->size;
}

struct ThBSTNode *kthElement(struct ThBSTNode *root, int k)
{
    if (!root)
        return NULL;

    int size_of_right = sizeOfTree(root) - sizeOfTree(root->left);
    // printf("%d %d %d\n", root->key, s, root->size);

    if (size_of_right == k)
        return root;
    if (size_of_right > k && root->rightThread != 0)
        return kthElement(root->right, k);
    else
        return kthElement(root->left, k - size_of_right);
}

struct ThBSTNode *find_node(struct ThBSTNode *root, int value)
{
    // struct Node *parent = NULL;
    printf("here pow\n");
    struct ThBSTNode *current = root;

    while (current != NULL)
    {
        if (value == current->key)
        {
            return current;
        }
        if (current->key < value)
        {
            current = current->right;
        }
        else
        {
            current = current->left;
        }
    }
    return NULL;
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

    if (find_node(root, value) == NULL)
    {
        return;
    }

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
            temp->size--;
            temp = temp->left;
        }
        else
        {
            if (temp->rightThread == 0)
            {
                temp->size--;
                temp = temp->right;
            }

            else
                break;
        }
    }

    if (found == 1)
    {
        printf("\nFound!!!\n");

        if (parent == NULL && (root->right == NULL || root->left == NULL))
        {
            printf("here in the new part!!!");
            if (root->right == NULL)
            {
                printf("bye");
                struct ThBSTNode *predecessor = inorderPred(root);
                root = root->left;
                predecessor->right = NULL;
                predecessor->rightThread = 0;
            }
            else
            {
                root = root->right;
            }
        }
        else
        {

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

    insertThBSTNode(20);
    insertThBSTNode(15);
    insertThBSTNode(12);
    insertThBSTNode(11);
    insertThBSTNode(14);

    deleteThBinaryTree(15);
    // deleteThBinaryTree(19);
    // deleteThBinaryTree(22);
    // deleteThBinaryTree(17);

    displayTree(root, "tree.dot");
    system("dot -Tpng tree.dot -o tree.png");
}