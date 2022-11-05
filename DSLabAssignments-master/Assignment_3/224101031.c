// BST Operations

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief The structure to store the BST Tree Node
 *
 */
struct bstNode
{
    int key;               // The value
    int size;              // Size of the tree including the root
    struct bstNode *left;  // left child
    struct bstNode *right; // right child
};

// ======================= Stack Helper functoins using linked list ==========================
/**
 * @brief 
 * 
 */
struct Node
{
    struct bstNode *val;
    struct Node *next;
};

struct Node *top = NULL;

/**
 * @brief The push operation on the stack
 *
 * @param root - The TreeNode to push into
 * @return int - Returns 1 if any error or else 0
 */
int push(struct bstNode *root)
{
    struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
    if (temp == NULL)
    {
        return -1; // Unable to create a new node
    }

    temp->val = root;
    temp->next = top;
    top = temp;

    return 0; // Successful
}

/**
 * @brief Pop operation for stack
 * @return struct bstNode*
 */
struct bstNode *pop()
{
    if (top == NULL)
    {
        printf("Stack Underflow");
        return NULL;
    }

    struct Node *tmp = top;

    struct bstNode *ptr = top->val;
    top = top->next;

    free(tmp);

    return ptr;
}

/**
 * @brief Checks if the Stack is empty or not
 *
 * @return int - 1 if not empty and 0 if empty
 */
int empty()
{
    if (top == NULL)
    {
        return 1; // empty
    }
    return 0; // not empty
}

/**
 * @brief Flushes the stack for a fresh start
 *
 */
void flushStack()
{
    while(!empty()) {
        pop();
    }
}

// Typedefs for the easy use
typedef struct bstNode BSTNode;
typedef struct bstNode* BSTNodePtr;

/**
 * @brief Initializes the BST root with NULL value
 * @param root - The root of the BST
 */
void createBST(BSTNodePtr *root)
{
    *root = NULL;
}

/**
 * @brief Get the Tree Node object
 *
 * @param data - The data with which the node to be created
 * @return BSTNodePtr
 */
BSTNodePtr getTreeNode(int data)
{
    BSTNodePtr newNode = (BSTNodePtr)malloc(sizeof(BSTNode));

    if (newNode == NULL)
    {
        return NULL; // Error in creating new node by allocating the memory
    }
    newNode->key = data;
    newNode->size = 1;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

/**
 * @brief Helps in creating the dot file for the graph
 *
 * @param root - Root of the BST
 * @param fptr - File pointer to write the data in
 */
void displayBSTHelper(BSTNodePtr root, FILE *fptr)
{
    if (root != NULL)
    {
        fprintf(fptr, "%d [label = \"%d, %d\"];\n", root->key, root->key, root->size);
        if(root->left != NULL)
        {
            fprintf(fptr, "%d -> %d [color = red];\n", root->key, root->left->key);
            displayBSTHelper(root->left, fptr);
        }
        if (root->right != NULL)
        {
            fprintf(fptr, "%d -> %d ;\n", root->key, root->right->key);
            displayBSTHelper(root->right, fptr);
        }
    }
}

/**
 * @brief Creates the dot file and create the BST
 *
 * @param root - The root of the BST
 * @param filename - The file name for graph values
 * @return int - If error then returns -1 or else 0
 */
int displayBST(BSTNodePtr root, char *filename)
{
    FILE *fptr;

    fptr = fopen(filename, "w+");
    if (fptr == NULL)
    {
        printf("Error while opening the file with name %s", filename);
        return -1;
    }
    fprintf(fptr, "digraph bst{\n");
    displayBSTHelper(root, fptr);
    fprintf(fptr, "}\n");

    fclose(fptr);

    return 0;
}

/**
 * @brief Search for the key
 *
 * @param root
 * @param data
 * @return BSTNodePtr
 */
BSTNodePtr bstSearch(BSTNodePtr *root, int data)
{
    BSTNodePtr ptr = *root;
    while (ptr != NULL)
    {
        if (ptr->key == data)
        {
            break;
        }
        if (data < ptr->key)
        {
            ptr = ptr->left;
        }
        else
        {
            ptr = ptr->right;
        }
    }

    return ptr;
}

/**
 * @brief This helps in increasing the size of path nodes if the insertion is successful
 */
void BSTInsertHelper()
{
    while (!empty())
    {
        BSTNodePtr temp = pop();
        temp->size = (temp->size) + 1;
    }
}

/**
 * @brief Insert the new node in the BST (Iterative fashion)
 *
 * @param root - The address of root pointer
 * @param data - The data to be added in the BST
 * @return int - -1 if the key already exists 0 otherwise
 */
int bstInsert(BSTNodePtr *root, int data)
{

    // If root is empty then allocate root node
    if (*root == NULL)
    {
        *root = getTreeNode(data);
        return 0;
    }


    BSTNodePtr cur = *root;
    BSTNodePtr prev = NULL;

    while (cur != NULL)
    {
        push(cur);
        prev = cur;
        if (cur->key == data)
        {
            flushStack();
            return -1; // The data already exist
        }
        if (data < cur->key)
        {
            cur = cur->left;
        }
        else
        {
            cur = cur->right;
        }
    }

    BSTNodePtr temp = getTreeNode(data);
    // Successfully created the node then only
    if (temp)
    {
        if (data < prev->key)
        {
            prev->left = temp;
        }
        else
        {
            prev->right = temp;
        }
        BSTInsertHelper();
    }
}

/**
 * @brief Helps in decresing the size if the element is deleted successfully
 *
 */
void BSTDeleteHelper()
{
    BSTNodePtr temp;
    while (top != NULL)
    {
        temp = pop();
        temp->size = (temp->size) - 1;
    }
}

/**
 * @brief Delete an element from the BST - Iterative
 *
 * @param root - The root of the tree
 * @param data - The data to be deleted
 * @return int - -1 if the key doesn't exists 0 otherwise
 */
int bstDelete(BSTNodePtr *root, int data)
{

    if ((*root) == NULL)
    {
        return -1; // The tree doesn't exist
    }

    BSTNodePtr cur = *root;
    BSTNodePtr prev = NULL;

    while (cur != NULL)
    {
        push(cur);
        if (cur->key == data)
        {
            break;
        }
        prev = cur;
        if (data < cur->key)
        {
            cur = cur->left;
        }
        else
        {
            cur = cur->right;
        }
    }

    // The key doesn't exist
    if (cur == NULL)
    {
        flushStack();
        return -1; // The data doesn't exists in the tree;
    }


    // Case 0 : Delete Root
    if (!prev && (!cur->left || !cur->right)) {
        if (!cur->left && !cur->right) {
            *root = NULL;
        } else if (!cur->left) {
            *root = cur->right;
        } else {
            *root = cur->left;
        }
    }

    // Case 1 - If the left side is null then assign the right to the parent
    else if (cur->left == NULL)
    {
        if (prev->left == cur)
        {
            prev->left = cur->right;
        }
        else
        {
            prev->right = cur->right;
        }
        free(cur);
    }

    // Case 2 - If right is null then assign left to the parent
    else if (cur->right == NULL)
    {
        if (prev->left == cur)
        {
            prev->left = cur->left;
        }
        else
        {
            prev->right = cur->left;
        }
        free(cur);
    }

    // Case 3 - If both are not null - Then find inorder successor and assign that in place of that
    else
    {
        // find inorder successor

        BSTNodePtr p = NULL;
        BSTNodePtr c = cur->right;

        while (c->left != NULL)
        {
            push(c);
            p = c;
            c = c->left;
        }

        if (p == NULL)
        {
            cur->right = c->right;
        }
        else
        {
            p->left = c->right;
        }

        cur->key = c->key;

        free(c);
    }

    // Helps in adjustment of the size
    BSTDeleteHelper();
}

/**
 * @brief Find the kth largest element
 *
 * @param root - Root
 * @param k - The position of the element
 * @return BSTNodePtr - The node with kth element (starting with index 1)
 */
BSTNodePtr kthElement(BSTNodePtr *root, int k)
{
    int n = (*root)->size - k + 1;
    int lcount;
    BSTNodePtr ptr = *root;


    while (n>0) {
        lcount = ptr->left?ptr->left->size:0;

        if (lcount + 1 == n)
            return ptr;
        if (lcount + 1 > n)
            ptr = ptr->left;
        else {
            ptr = ptr->right;
            n -= lcount+1;
        }
    }

    return ptr;
}

/**
 * @brief Inorder traversal
 *
 * @param root - BST root
 */
void BSTInorder(BSTNodePtr root)
{
    if (root == NULL)
    {
        return;
    }
    BSTInorder(root->left);
    printf("%d [%d] -> ", root->key, root->size);
    BSTInorder(root->right);
}

void searchHelper(BSTNodePtr* root, int data)
{
    BSTNodePtr res = bstSearch(root, data);
    if (res != NULL)
    {
        printf("\nThe key found = %d\n", res->key);
    }
    else
    {
        printf("\nThe key not found\n");
    }
}

int main(int agrv, char *argv[])
{
    BSTNodePtr root;
    createBST(&root);

    bstInsert(&root, 10);
    bstInsert(&root, 5);
    bstInsert(&root, 15);
    bstInsert(&root, 3);
    bstInsert(&root, 7);
    bstInsert(&root, 12);
    bstInsert(&root, 12);
    bstInsert(&root, 20);
    bstInsert(&root, 10);
    bstInsert(&root, 14);
    bstInsert(&root, 17);
    bstInsert(&root, 13);
    bstInsert(&root, 19);
    bstInsert(&root, 4);
    bstInsert(&root, 60);
    bstInsert(&root, 2);
    bstInsert(&root, 1);

    bstDelete(&root, 4);
    bstDelete(&root, 2);
    bstDelete(&root, 12);
    bstDelete(&root, 10);
    bstDelete(&root, 20);
    bstDelete(&root, 60);
    bstDelete(&root, 15);
    bstDelete(&root, 15);
    bstDelete(&root, 3);
    bstDelete(&root, 1);
    bstDelete(&root, 1);
    bstDelete(&root, 19);
    bstDelete(&root, 17);
    bstDelete(&root, 14);
    bstDelete(&root, 13);
    bstDelete(&root, 5);
    bstDelete(&root, 7);

    // searchHelper(&root, 4);
    displayBST(root, "tree.gv");

    system("dot -Tpng tree.gv -o tree.png");

}