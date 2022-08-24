#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int value;
    struct Node *left;
    struct Node *right;
};

struct Node *root = NULL;

struct Hash_table_node
{
    int value;
    struct Hash_table_node *next;
};

struct Hash_table_node *table[100] = {NULL};

int hashCode(int value)
{
    return (value % 100);
}

void insert_into_hash_map(int value)
{
    int key = hashCode(value);

    struct Hash_table_node *hash_node = (struct Hash_table_node *)malloc(sizeof(struct Hash_table_node));
}

void displayTreeHelper(struct Node *T, FILE *fp)
{
    if (T != NULL)
    {
        if (T->left != NULL)
        {
            fprintf(fp, "%d -> %d [color = red, style=dotted];\n", T->value, T->left->value);
            displayTreeHelper(T->left, fp);
        }
        if (T->right != NULL)
        {
            fprintf(fp, "%d -> %d ;\n", T->value, T->right->value);
            displayTreeHelper(T->right, fp);
        }
    }
}

int displayTree(struct Node *T, char *filename)
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

struct Node *createNode(int value)
{
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    node->value = value;
    return node;
}

struct Node *insertNode(struct Node *parent, int value, char direction)
{
    if (parent == NULL)
    {
        printf("The parent is NULL!!!");
        return NULL;
    }

    struct Node *element = createNode(value);
    if (direction == 'r')
    {
        parent->right = element;
    }

    if (direction == 'l')
    {
        parent->left = element;
    }

    return element;
}

void insertRoot(int value)
{
    struct Node *element = createNode(value);
    root = element;
}

int DFS_Traversal(struct Node *root, int level)
{
    if (root == NULL)
    {
    }
}

int BFS_Traversal(struct Node *root)
{
    struct Node *queue[10];
    int front, rear;
    front = rear = -1;

    front++;
    rear++;
    queue[rear] = root;
    int queue_size = 0;
    int level = 0;

    while (!(front == -1 && rear == -1))
    {

        queue_size = rear - front + 1;

        // compelete one level

        printf("\nElements of level %d =", level);

        while (queue_size--)
        {

            struct Node *ele = queue[front];
            printf("\t%d\t", ele->value);
            if (front == rear)
            {
                front = rear = -1;
            }
            else
            {
                front++;
            }

            if (ele->left != NULL)
            {
                if (front == -1 && rear == -1)
                {
                    front = rear = 0;
                }
                else
                    rear++;
                queue[rear] = ele->left;
            }

            if (ele->right != NULL)
            {
                if (front == -1 && rear == -1)
                {
                    front = rear = 0;
                }
                else
                    rear++;
                queue[rear] = ele->right;
            }
        }
        level++;
    }

    return level;
}

int main()
{
    insertRoot(8);
    struct Node *rootright = insertNode(root, 10, 'r');
    struct Node *rootleft = insertNode(root, 11, 'l');

    insertNode(rootright, 7, 'l');
    struct Node *rightmost = insertNode(rootright, 13, 'r');

    insertNode(rootleft, 14, 'l');
    insertNode(rootleft, 16, 'r');

    insertNode(rightmost, 12, 'l');

    displayTree(root, "tree.dot");

    system("dot -Tpng tree.dot -o tree.png");

    int level = BFS_Traversal(root);

    printf("\nSo the number of levels are %d \n", level);

    return 0;
}