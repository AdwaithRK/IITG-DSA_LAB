#include <stdlib.h>
#include <stdio.h>

struct Node
{
    int value;
    struct Node *left;
    struct Node *right;
};

struct Node *stack[100];
int top = -1;


void bst_print_dot_null(int key, int nullcount, FILE* stream)
{
    fprintf(stream, "    null%d [shape=point];\n", nullcount);
    fprintf(stream, "    %d -> null%d;\n", key, nullcount);
}

void bst_print_dot_aux(struct Node* node, FILE* stream)
{
    static int nullcount = 0;

    if (node->left)
    {
        fprintf(stream, "    %d -> %d;\n", node->value, node->left->value);
        bst_print_dot_aux(node->left, stream);
    }
    else
        bst_print_dot_null(node->value, nullcount++, stream);

    if (node->right)
    {
        fprintf(stream, "    %d -> %d;\n", node->value, node->right->value);
        bst_print_dot_aux(node->right, stream);
    }
    else
        bst_print_dot_null(node->value, nullcount++, stream);
}

void bst_print_dot(struct Node* tree, FILE* stream)
{
    fprintf(stream, "digraph BST {\n");
    fprintf(stream, "    node [fontname=\"Arial\"];\n");

    if (!tree)
        fprintf(stream, "\n");
    else if (!tree->right && !tree->left)
        fprintf(stream, "    %d;\n", tree->value);
    else
        bst_print_dot_aux(tree, stream);

    fprintf(stream, "}\n");
}

struct Node *insert_node(struct Node *root, int value)
{
    if (root == NULL)
    {
        struct Node *current = (struct Node *)malloc(sizeof(struct Node));
        current->value = value;
        current->left = NULL;
        current->right = NULL;
        return current;
    }

    if (root->value > value)
    {
        root->left = insert_node(root->left, value);
    }
    else if (root->value < value)
    {
        root->right = insert_node(root->right, value);
    }
    return root;
}

void inorder_traversal(struct Node *root)
{
    if (root == NULL)
        return;

    inorder_traversal(root->left);
    printf("%d\t", root->value);
    inorder_traversal(root->right);
}

void inorder_traversal_with_out_recursion(struct Node *root)
{
    struct Node *current = root;
    while (current != NULL || top != -1)
    {

        while (current != NULL)
        {
            stack[++top] = current;
            current = current->left;
        }

        current = stack[top--];

        printf("\t%d\t", current->value);

        current = current->right;
    }
}

void preorder_traversal_with_out_recursion(struct Node *root)
{
    struct Node *current = root;
    while (current != NULL || top != -1)
    {

        while (current != NULL)
        {
            printf("\t%d\t", current->value);
            stack[++top] = current;
            current = current->left;
        }

        current = stack[top--];

        current = current->right;
    }
}

int kth_smallest_element(struct Node *root, int k)
{

    int temp_array[100];
    int elements_count = -1;

    top = -1;

    struct Node *current = root;

    while (top != -1 || current != NULL)
    {

        while (current != NULL)
        {
            stack[++top] = current;
            current = current->left;
        }

        current = stack[top--];
        temp_array[++elements_count] = current->value;
        if (elements_count == k - 1)
        {
            return temp_array[elements_count];
        }

        current = current->right;
    }
}

struct Node *find_inorder_successor(struct Node *current, struct Node **parent)
{
    if (current->right != NULL)
    {
        *parent = current;
        current = current->right;

        while (current->left != NULL)
        {
            *parent = current;
            current = current->left;
        }

        return current;
    }
    return NULL;
}

void attach_child_to_parent(struct Node *parent, struct Node *current)
{
    struct Node *child;
    if (current->left != NULL)
    {
        child = current->left;
    }
    else
    {
        child = current->right;
    }

    if (parent->left == current)
    {
        parent->left = child;
    }
    else
    {
        parent->right = child;
    }
}

void remove_leaf(struct Node *parent, struct Node *current)
{
    if (parent->left == current)
    {
        parent->left = NULL;
    }
    else if (parent->right == current)
    {
        parent->right = NULL;
    }
}

struct Node *find_node(struct Node *root, int value, struct Node **parent)
{
    // struct Node *parent = NULL;
    printf("here pow\n");
    struct Node *current = root;

    while (current != NULL)
    {
        if (value == current->value)
        {
            return current;
        }
        *parent = current;
        if (current->value < value)
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

void delete_node(struct Node *root, int value)
{
    struct Node *parent = NULL;
    struct Node *found = find_node(root, value, &parent);
    if (parent != NULL)
        printf("checking for parent %d", parent->value);

    if (found != NULL)
    {
        if (found->left != NULL && found->right != NULL)
        {
            struct Node *inorder_parent = NULL;
            struct Node *inorder_successor = find_inorder_successor(found, &inorder_parent);
            int temp = inorder_successor -> value;
            delete_node(root, temp);
            found -> value = temp;
        }
        else if (found->left != NULL || found->right != NULL)
        {
            printf("\nOnly one child - attach to parent!!\n");
            attach_child_to_parent(parent, found);
        }
        else
        {
            printf("Leaf node!!");
            remove_leaf(parent, found);
        }
    }
    return;
}

int main()
{
    struct Node *root = NULL;
    root = insert_node(root, 6);
    insert_node(root, 4);
    insert_node(root, 9);
    insert_node(root, 2);
    insert_node(root, 5);
    insert_node(root, 8);
    insert_node(root, 10);
    insert_node(root, 11);
    inorder_traversal(root);
    // printf("\n-----experiment -------\n");
    // inorder_traversal_with_out_recursion(root);
    // printf("\n----experiment -------\n");
    // preorder_traversal_with_out_recursion(root);

   // printf("\n3th smallest element : %d\n", kth_smallest_element(root, 3));

    delete_node(root, 9);
    printf("\nInorder traversal\n");
    inorder_traversal(root);

    FILE *ptr = fopen("adwaith.dot", "w+");

    bst_print_dot(root, ptr);

    system("dot -Tpng adwaith.dot -o tree.png");



    return 0;
}