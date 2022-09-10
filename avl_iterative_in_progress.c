#include <stdio.h>
#include <stdlib.h>

struct AVLNode
{
    int key;
    struct AVLNode *left;
    struct AVLNode *right;
    int bf;
    int size;
};

struct Stack
{
    struct AVLNode *AVLNode;
    struct Stack *next;
};

typedef struct Stack Stack;

typedef struct AVLNode AVLNode;
typedef struct AVLNode *AVLNodePtr;

int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

void createStack(Stack **head)
{
    *head = NULL;
}

int push(Stack **head, AVLNodePtr AVLNode)
{
    Stack *newNode = (Stack *)malloc(sizeof(AVLNode));
    if (!newNode)
        return -1;
    newNode->AVLNode = AVLNode;
    newNode->next = *head;
    *head = newNode;
    return 1;
}

AVLNodePtr pop(Stack **head)
{
    if (!*head)
        return NULL;

    Stack *curr = *head;
    AVLNode *currNode = curr->AVLNode;
    *head = (*head)->next;
    free(curr);
    return currNode;
}

int isEmpty(Stack *head)
{
    if (head == NULL)
        return 1;
    else
        return 0;
}

AVLNodePtr top(Stack *head)
{
    return head->AVLNode;
}

void createAVL(AVLNodePtr *root)
{
    *root = NULL;
}

AVLNodePtr getNode()
{
    return (AVLNodePtr)malloc(sizeof(AVLNode));
}

void displayHelper(FILE *fp, AVLNodePtr root)
{
    if (root)
    {
        if (root->left)
        {
            fprintf(fp, "%d -> %d [color = red];\n", root->key, root->left->key);
            fprintf(fp, "%d [label = \"%d, %d\"];\n", root->key, root->key, root->bf);

            displayHelper(fp, root->left);
        }
        if (root->right)
        {
            fprintf(fp, "%d -> %d;\n", root->key, root->right->key);
            fprintf(fp, "%d [label = \"%d, %d, %d\"];\n", root->key, root->key, root->bf, root->size);
            displayHelper(fp, root->right);
        }
        if (!root->right && !root->left)
        {
            fprintf(fp, "%d [label = \"%d, %d\"];\n", root->key, root->key, root->bf);
        }
    }
}

int displayBST(AVLNodePtr *root, char *filename)
{
    FILE *fp;
    fp = fopen(filename, "w+");
    if (!fp)
    {
        printf("Unable to open the file\n");
        return -1;
    }

    fprintf(fp, "digraph g{\n");
    displayHelper(fp, *root);
    fprintf(fp, "}\n");
    return 1;
}

AVLNodePtr leftRotate(AVLNodePtr parent, AVLNodePtr *root, Stack *head)
{
    AVLNodePtr y, z;
    y = parent;
    z = y->right;
    y->right = z->left;
    z->left = y;

    y->bf = y->bf - 1 - max(z->bf, 0);
    z->bf = z->bf - 1 + min(y->bf, 0);

    AVLNodePtr ancestor = pop(&head);

    if (ancestor)
    {
        if (ancestor->left == y)
        {
            ancestor->left = z;
        }
        else
        {
            ancestor->right = z;
        }
    }
    else
    {
        *root = z;
    }

    int x;

    if (y->left)
        x = y->left->size;
    else
        x = 0;

    y->size = y->size + x - z->size;
    z->size = z->left->size + z->right->size;
    return z;
}

AVLNodePtr rightRotate(AVLNodePtr parent, AVLNodePtr *root, Stack *head)
{
    AVLNodePtr y, z;
    y = *root;
    z = y->left;
    y->left = z->right;
    z->right = y;

    AVLNodePtr ancestor = pop(&head);

    if (ancestor)
    {
        if (ancestor->left == y)
        {
            ancestor->left = z;
        }
        else
        {
            ancestor->right = z;
        }
    }
    else
    {
        *root = z;
    }

    y->bf = y->bf + 1 - min(z->bf, 0);
    z->bf = z->bf + 1 + max(y->bf, 0);

    int x;

    if (y->right)
        x = y->left->size;
    else
        x = 0;

    y->size = y->size + x - z->size;
    z->size = z->left->size + z->right->size;
    return z;
}

int insertAVL(AVLNodePtr *root, int value)
{
    AVLNodePtr newNode = getNode();
    newNode->key = value;
    newNode->left = newNode->right = NULL;
    newNode->size = 1;
    if (*root = NULL)
    {
        *root = newNode;
        return 1;
    }
    Stack *head;
    createStack(&head);
    AVLNodePtr currNode = *root;
    while (currNode != NULL)
    {
        if (currNode->key > value)
        {
            if (currNode->left)
            {
                push(&head, currNode);
                currNode->size++;
                currNode = currNode->left;
            }
            else
            {
                currNode->left = newNode;
                currNode->bf--;
                break;
            }
        }
        else
        {
            if (currNode->left)
            {
                push(&head, currNode);
                currNode->size++;
                currNode = currNode->right;
            }
            else
            {
                currNode->right = newNode;
                currNode->bf++;
                break;
            }
        }
    }

    if (isEmpty(head))
        return 1;

    while (!isEmpty(head))
    {
        AVLNodePtr parent = pop(&head);
        if (parent->left == currNode)
            parent->bf--;
        else
            parent->bf++;

        if (parent->bf == -2)
        {
            if (currNode->bf == -1)
            {
                rightRotate(parent, root, head);
                return 1;
            }
            else
            {
                leftRotate(parent->left, root, head);
                rightRotate(parent, root, head);
                return 1;
            }
        }
        if (parent->bf == 2)
        {
            if (currNode->bf == 1)
            {
                leftRotate(parent, root, head);
                return 1;
            }
            else
            {
                rightRotate(parent->right, root, head);
                leftRotate(parent, root, head);
                return 1;
            }
        }
        if (!parent->bf)
            return 1;
        currNode = parent;
    }
    return 1;
}

AVLNodePtr findMin(AVLNodePtr root)
{
    if (!root->left)
        return root;
    return findMin(root->left);
}

int delete (AVLNodePtr *root, int value)
{
    AVLNodePtr curr = *root;
    Stack *head;
    createStack(&head);

    while (curr != NULL)
    {
        curr->size--;
        if (curr->key > value)
        {
            push(&head, curr);
            curr = curr->left;
        }
        else
        {
            if (curr->key > value)
            {
                push(&head, curr);

                curr = curr->right;
            }
            else
            {
                break;
            }
        }
    }

    AVLNodePtr parent = pop(&head);
    if (parent->left == curr)
        parent->bf--;
    else
        parent->bf++;

    if (!curr->left && !curr->right)
    {
        if (parent->left == curr)
            parent->left = NULL;
        else
            parent->right = NULL;
        free(curr);
        return 1;
    }

    if (!curr->left)
    {
        if (parent->left == curr)
            parent->left = curr->right;
        else
            parent->right = curr->right;
        free(curr);
        return 1;
    }

    if (!curr->right)
    {
        if (parent->left == curr)
            parent->left = curr->left;
        else
            parent->right = curr->left;
        free(curr);
        return 1;
    }

    AVLNodePtr inorder = findMin(curr->right);
    delete (&(curr->right), inorder->key);

    AVLNodePtr ancestor;

    while (isEmpty(head))
    {
        ancestor = pop(&head);
        if (ancestor->left == parent)
            ancestor->bf++;
        else
            ancestor->bf--;

        if (ancestor->bf == -2)
        {
            if (ancestor->left <= 0)
            {
                parent = rightRotate(ancestor, root, head);
            }
            else
            {
                leftRotate(ancestor->left, root, head);
                parent = rightRotate(ancestor, root, head);
            }
        }
        else
        {
            if (ancestor->bf == 2)
            {
                if (ancestor->right >= 0)
                {
                    parent = leftRotate(ancestor, root, head);
                }
                else
                {
                    rightRotate(ancestor->right, root, head);
                    parent = leftRotate(ancestor, root, head);
                }
            }
            else
            {
                if (ancestor->bf = 0)
                    return 1;
                else
                    parent = ancestor;
            }
        }
    }
    return 1;
}

int main()
{
    AVLNodePtr root;
    Stack *head;
    createAVL(&root);
    createStack(&head);

    displayBST(root, "tree.dot");

    system("dot -Tpng tree.dot -o tree.png");

    return 0;
}
