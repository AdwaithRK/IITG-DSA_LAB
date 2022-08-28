#include <stdio.h>
#include <stdlib.h>

struct avlNode
{
    int value;
    struct avlNode *left;
    struct avlNode *right;
    int height;
    int balance_factor;
};

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int compute_height(struct avlNode *root)
{
    if (root == NULL)
        return 0;
    int left_height = 0, right_height = 0;
    if (root->left)
    {
        left_height = root->left->height;
    }

    if (root->right)
    {
        right_height = root->right->height;
    }
    return 1 + max(left_height, right_height);
}

int get_balance_factor(struct avlNode *root)
{
    if (root == NULL)
        return 0;
    return root->balance_factor;
}

struct avlNode *create_node(int value)
{
    struct avlNode *node = (struct avlNode *)malloc(sizeof(struct avlNode));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    node->balance_factor = 0;
    return node;
}

struct avlNode *update_balance_factor(struct avlNode *node)
{
    if (node == NULL)
        return node;
    int left_height = 0, right_height = 0;
    if (node->left)
    {
        left_height = node->left->height;
    }

    if (node->right)
    {
        right_height = node->right->height;
    }
    node->balance_factor = left_height - right_height;
    node->height = 1 + max(left_height, right_height);

    return node;
}

struct avlNode *right_rotate(struct avlNode *y)
{
    struct avlNode *x = y->left;
    y->left = x->right;
    x->right = y;

    y = update_balance_factor(y);
    x = update_balance_factor(x);

    return x;
}

struct avlNode *left_rotate(struct avlNode *x)
{
    struct avlNode *y = x->right;

    x->right = y->left;
    y->left = x;

    y = update_balance_factor(y);
    x = update_balance_factor(x);

    return y;
}

struct avlNode *insert_node_into_avl(int value, struct avlNode *root)
{
    if (root == NULL)
    {
        return create_node(value);
    }

    if (value < root->value)
    {
        root->left = insert_node_into_avl(value, root->left);
    }

    if (value > root->value)
    {
        root->right = insert_node_into_avl(value, root->right);
    }

    // balancing factor
    root = update_balance_factor(root);

    int balance_factor = root->balance_factor;

    if (balance_factor > 1 && value < root->left->value)
        return right_rotate(root);

    if (balance_factor > 1 && value > root->left->value)
    {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    if (balance_factor < -1 && value > root->right->value)
        return left_rotate(root);

    if (balance_factor < -1 && value < root->right->value)
    {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

struct avlNode *find_inorder_successor(struct avlNode *current)
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

struct avlNode *delete_avl_node(int value, struct avlNode *root)
{

    if (root == NULL)
        return root;

    if (root->value > value)
        root->left = delete_avl_node(value, root->left);

    else if (root->value < value)
        root->right = delete_avl_node(value, root->right);
    else
    {
        if (root->left == NULL || root->right == NULL)
        {
            struct avlNode *temp = root->left ? root->left : root->right;

            if (temp == NULL)
            {
                root = temp;
            }
            else
            {
                *root = *temp;
            }
            free(temp);
        }
        else
        {
            struct avlNode *temp = find_inorder_successor(root);
            root->value = temp->value;

            root->right = delete_avl_node(temp->value, root->right);
        }
    }

    if (root == NULL)
        return root;

    // balancing factor
    root = update_balance_factor(root);

    int balance_factor = root->balance_factor;

    if (balance_factor > 1 && get_balance_factor(root->left) >= 0)
        return right_rotate(root);

    // Left Right Case
    if (balance_factor > 1 && get_balance_factor(root->left) < 0)
    {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    // Right Right Case
    if (balance_factor < -1 && get_balance_factor(root->right) <= 0)
        return left_rotate(root);

    // Right Left Case
    if (balance_factor < -1 && get_balance_factor(root->right) > 0)
    {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

void displayTreeHelper(struct avlNode *T, FILE *fp)
{
    if (T != NULL)
    {

        fprintf(fp, "%d[label=\"%d, bf:%d, h:%d\"];", T->value, T->value, get_balance_factor(T), T->height);
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

int displayTree(struct avlNode *T, char *filename)
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
    struct avlNode *root = NULL;
    root = insert_node_into_avl(8, root);

    root = insert_node_into_avl(3, root);

    root = insert_node_into_avl(5, root);
    root = insert_node_into_avl(10, root);
    root = insert_node_into_avl(12, root);
    root = insert_node_into_avl(6, root);
    root = insert_node_into_avl(2, root);
    root = insert_node_into_avl(1, root);
    root = insert_node_into_avl(0, root);

    root = delete_avl_node(8, root);

    displayTree(root, "tree.dot");
    system("dot -Tpng tree.dot -o tree.png");
}