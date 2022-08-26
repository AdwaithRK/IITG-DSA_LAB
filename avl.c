#include <stdio.h>
#include <stdlib.h>

struct avlNode
{
    int value;
    struct avlNode *left;
    struct avlNode *right;
    int height;
};

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int height(struct avlNode *N)
{
    if (N == NULL)
        return 0;
    return 1 + max(height(N->left), height(N->right));
}

int getBalance(struct avlNode *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

struct avlNode *create_node(int value)
{
    struct avlNode *node = (struct avlNode *)malloc(sizeof(struct avlNode));

    node->left = NULL;
    node->right = NULL;
    node->height = 0;
    return node;
}

struct avlNode *right_rotate(struct avlNode *y)
{
    struct avlNode *x = y->left;
    y->left = x->right;
    x->right = y;

    y->height = height(y);
    x->height = height(x);

    return x;
}

struct avlNode *left_rotate(struct avlNode *x)
{
    struct avlNode *y = x->right;

    x->right = y->left;
    y->left = x;

    y->height = height(y);
    x->height = height(x);

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

    int balance_factor = getBalance(root);

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
        root = delete_avl_node(value, root->left);

    else if (root->value < value)
        root = delete_avl_node(value, root->right);
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

    root->height = 1 + max(height(root->left),
                           height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return right_rotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return left_rotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

int main() {}