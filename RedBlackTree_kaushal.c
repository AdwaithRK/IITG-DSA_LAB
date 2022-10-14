#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct bstNode
{
    int key;
    struct bstNode *left;
    struct bstNode *right;
    int color; // 1 - Red and 0 - Black
};

typedef struct bstNode BSTNode;
typedef struct bstNode *BSTNodePtr;

/*The stack implementation to store the pointers and posiyion*/
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
    while (!empty())
    {
        pop();
    }
}
/*End of stack functions*/

/*Display function starts*/
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
        if (root->color)
            fprintf(fptr, "%d [label = \"%d\", color = red];\n", root->key, root->key);
        else
            fprintf(fptr, "%d [label = \"%d\", color = black];\n", root->key, root->key);
        if (root->left)
        {
            fprintf(fptr, "%d -> %d [color = red];\n", root->key, root->left->key);
            displayBSTHelper(root->left, fptr);
        }
        if (root->right)
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

/*Display function ends*/

int max(int a, int b)
{
    return a > b ? a : b;
}

void createBST(BSTNodePtr *root)
{
    *root = NULL;
}

BSTNodePtr createNode(int key)
{
    BSTNodePtr newNode = (BSTNodePtr)malloc(sizeof(BSTNode));
    newNode->key = key;
    newNode->color = 1;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

BSTNodePtr LLrotation(BSTNodePtr z)
{
    BSTNodePtr y = z->left;
    BSTNodePtr temp = y->right;

    y->right = z;
    z->left = temp;

    z->color = 1;
    y->color = 0;

    return y;
}

BSTNodePtr RRrotation(BSTNodePtr z)
{
    BSTNodePtr y = z->right;
    BSTNodePtr temp = y->left;

    y->left = z;
    z->right = temp;

    z->color = 1;
    y->color = 0;

    return y;
}

BSTNodePtr LRrotation(BSTNodePtr z)
{
    BSTNodePtr y = z->left;
    BSTNodePtr x = y->right;
    BSTNodePtr temp = x->right;
    BSTNodePtr temp2 = x->left;

    x->left = y;
    x->right = z;
    z->left = temp;
    y->right = temp2;

    x->color = 0;
    y->color = z->color = 1;

    return x;
}

BSTNodePtr RLrotation(BSTNodePtr z)
{
    BSTNodePtr y = z->right;
    BSTNodePtr x = y->left;
    BSTNodePtr temp = x->right;
    BSTNodePtr temp2 = x->left;

    x->right = y;

    x->left = z;
    z->right = temp2;
    y->left = temp;

    x->color = 0;
    y->color = z->color = 1;

    return x;
}

BSTNodePtr rotation(BSTNodePtr node, int key)
{
    if (key < node->key)
    { // Left rotation
        // Check for LL or LR
        if (key < node->left->key) // LL
            return LLrotation(node);
        else // LR
            return LRrotation(node);
    }
    else
    {                               // Right rotation
        if (key < node->right->key) // RL
            return RLrotation(node);
        else // RR
            return RRrotation(node);
    }

    return NULL;
}

/**
 * @brief This helps in increasing the size of path nodes if the insertion is successful
 */
void BSTInsertHelper(BSTNodePtr *root, int key)
{
    BSTNodePtr newRoot;
    int updateFlag = 1;
    while (!empty() && updateFlag)
    {
        BSTNodePtr temp = pop(); // Parent

        if (temp->color)
        {                            // if parent color is red
            BSTNodePtr node = pop(); // grand parent
            if (key < node->key)
            { // key inserted is to the Left subtree of grandparent
                if (node->right && node->right->color)
                {                           // sibling of parent is red
                    node->color = 1;        // make grandparent red
                    node->right->color = 0; // make sibling of parent black
                    node->left->color = 0;  // make parent black
                }
                else
                {
                    // rotate
                    updateFlag = 0;                // nothing need after one rotation?
                    newRoot = rotation(node, key); // rotate based on key value of inserted node and grandparent
                    if (empty())
                    {
                        *root = newRoot; // if node is root, make root after rotation the root
                    }
                    else
                    {
                        BSTNodePtr parentTemp = pop(); // take out great grandparent (or simply the parent of node)
                        if (parentTemp->left == node)  // assign it the right position
                            parentTemp->left = newRoot;
                        else
                            parentTemp->right = newRoot;
                    }
                }
            }
            else
            { // key inserted is to the right of the subtree of grandparent
                if (node->left && node->left->color)
                { // sibling of parent is red
                    node->color = 1;
                    node->right->color = 0; // same as the above logic
                    node->left->color = 0;
                }
                else
                {
                    // rotate
                    updateFlag = 0;
                    newRoot = rotation(node, key);
                    if (empty())
                    {
                        *root = newRoot;
                    }
                    else
                    {
                        BSTNodePtr parentTemp = pop();
                        if (parentTemp->left == node)
                            parentTemp->left = newRoot;
                        else
                            parentTemp->right = newRoot;
                    }
                }
            }
        }
        else // if parent color is black then do nothing?
            break;
    }

    flushStack();
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
        *root = createNode(data);
        (*root)->color = 0;
        return 0;
    }

    BSTNodePtr cur = *root;
    BSTNodePtr prev = NULL;

    int flag = 0;

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

    BSTNodePtr temp = createNode(data);
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
        BSTInsertHelper(root, data);
        (*root)->color = 0;
    }
    else
    {
        return -1; // Error while allocating memory for creating the new node
    }
}

int main()
{
    BSTNodePtr root;
    createBST(&root);

    bstInsert(&root, 10);

    bstInsert(&root, 18);

    bstInsert(&root, 7);

    bstInsert(&root, 15);

    bstInsert(&root, 16);

    bstInsert(&root, 30);

    bstInsert(&root, 25);

    bstInsert(&root, 40);

    bstInsert(&root, 60);

    bstInsert(&root, 2);

    bstInsert(&root, 1);

    bstInsert(&root, 70);

    displayBST(root, "tree.gv");
    system("dot -Tpng tree.gv -o tree.png");

    return 0;
}