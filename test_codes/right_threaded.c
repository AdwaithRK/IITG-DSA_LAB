#include <stdlib.h>
#include <stdio.h>

struct rightThreadedBSTNode
{
    int key;
    struct rightThreadedBSTNode *left;
    struct rightThreadedBSTNode *right;
    int size;
    int rightThread;
};

typedef struct rightThreadedBSTNode rTBSTNode;
typedef struct rightThreadedBSTNode *rTBSTNodePtr;

void createRightThreadedBST(rTBSTNodePtr *root)
{
    *root = NULL;
}

rTBSTNodePtr getRightThreadedBST(int value)
{
    rTBSTNodePtr node = (rTBSTNodePtr)malloc(sizeof(rTBSTNode));
    node->key = value;
    // node->right = NULL;
    // node->left = NULL;
    node->rightThread = 1;
    node->size = 1;

    return node;
}

int rtbstInsert(rTBSTNodePtr *root, int data)
{
    rTBSTNodePtr ptr = *root;
    rTBSTNodePtr parent = NULL;

    while (ptr != NULL)
    {
        if (data == (ptr->key))
        {
            printf("Duplicate Key !\n");
            return 0;
        }

        parent = ptr;

        if (data < ptr->key)
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

    rTBSTNodePtr element = getRightThreadedBST(data);

    if (parent == NULL)
    {
        // printf("\ncreating root\n");
        *root = element;
        element->left = NULL;
        element->right = NULL;
    }
    else if (data < (parent->key))
    {
        element->right = parent;
        parent->left = element;
        element->rightThread = 1;
    }
    else
    {
        element->right = parent->right;
        parent->right = element;
        parent->rightThread = 0;
    }

    return 1;
}

rTBSTNodePtr inorderSuccessor(rTBSTNodePtr node)
{
    if (node == NULL)
        return node;
    if (node->rightThread == 1)
        return node->right;

    node = node->right;

    while (node->left != NULL)
        node = node->left;

    return node;
}

rTBSTNodePtr inorderPred(rTBSTNodePtr current)
{
    if (current == NULL)
        return NULL;

    current = current->left;
    while (current->rightThread == 0)
        current = current->right;
    return current;
}

rTBSTNodePtr find_node(rTBSTNodePtr root, int value)
{
    // struct Node *parent = NULL;
    // printf("here pow\n");
    rTBSTNodePtr current = root;

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

int rtbstDelete(rTBSTNodePtr *root, int data)
{

    if (find_node(*root, data) == NULL)
    {
        return 0;
    }

    int found = 0;

    rTBSTNodePtr temp = *root, parent = NULL;

    while (temp != NULL)
    {
        if (temp->key == data)
        {
            found = 1;
            break;
        }

        parent = temp;
        if (temp->key > data)
        {
            // printf("temp value %d\n", temp->key);
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
        // printf("\nFound!!!\n");

        if (parent == NULL && ((*root)->right == NULL || (*root)->left == NULL))
        {
            // printf("here in the new part!!!");
            if ((*root)->right == NULL)
            {
                rTBSTNodePtr predecessor = inorderPred(*root);
                *root = (*root)->left;
                predecessor->right = NULL;
                predecessor->rightThread = 0;
            }
            else
            {
                *root = (*root)->right;
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
                    rTBSTNodePtr successor = inorderSuccessor(temp);
                    rTBSTNodePtr predecessor = inorderPred(temp);
                    rTBSTNodePtr child = temp->left;

                    predecessor->right = successor;
                    predecessor->rightThread = 1;

                    if (parent->left == temp)
                        parent->left = child;
                    else
                        parent->right = child;
                }
                else
                {
                    rTBSTNodePtr child = temp->right;

                    if (parent->left == temp)
                        parent->left = child;
                    else
                        parent->right = child;
                }
            }
            else
            {
                rTBSTNodePtr succ = inorderSuccessor(temp);
                int k = succ->key;
                rtbstDelete(root, k);
                temp->key = k;
            }
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

void displayTreeHelper(rTBSTNodePtr T, FILE *fp)
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

int displayRightThreadedTree(rTBSTNodePtr T, char *filename)
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

void inorder(rTBSTNodePtr *root, int *List)
{
    rTBSTNodePtr temp = *root;
    int index = 0;

    while (temp->left != NULL)
        temp = temp->left;

    while (temp != NULL)
    {
        List[index++] = temp->key;

        temp = inorderSuccessor(temp);
    }
    return;
}

int sizeOfTree(rTBSTNodePtr root)
{
    if (!root)
        return 0;
    return root->size;
}

rTBSTNodePtr kthElement(rTBSTNodePtr *root, int k)
{
    if (!(*root))
        return NULL;
    int size = sizeOfTree((*root)) - sizeOfTree((*root)->right);
    //  printf("%d %d %d\n", root->key, size, root->size);

    if (size == k)
        return *root;
    if (size < k && (*root)->rightThread != 1)
        return kthElement(&((*root)->right), k - size);
    else
        return kthElement(&((*root)->left), k);

    return NULL;
}

int main()
{
    rTBSTNodePtr root;
    createRightThreadedBST(&root);
    rtbstInsert(&root, 55);
    rtbstInsert(&root, 20);
    rtbstInsert(&root, 75);
    rtbstInsert(&root, 19);
    rtbstInsert(&root, 30);
    rtbstInsert(&root, 65);
    rtbstInsert(&root, 90);
    rtbstInsert(&root, 100);
    rtbstInsert(&root, 95);

    rtbstDelete(&root, 55);

    rtbstDelete(&root, 19);

    rtbstDelete(&root, 90);

    int *inorder_array = (int *)malloc((root->size + 1) * sizeof(int));
    printf("Inorder traversal:");
    inorder(&root, inorder_array);

    for (int i = 0; i < root->size; i++)
    {
        printf("%d\t", inorder_array[i]);
    }

    printf("\n");

    rTBSTNodePtr ele = kthElement(&root, 4);
    if (ele)
    {
        printf("\nvalue found is : %d\n", ele->key);
    }
    else
    {
        printf("\n no program found\n");
    }

    displayRightThreadedTree(root, "tree.dot");
    system("dot -Tpng tree.dot -o tree.png");
}