#include <stdio.h>
#include <stdlib.h>

#define RED 1
#define BLACK 0

struct bstNode
{
    int key;
    struct bstNode *left;
    struct bstNode *right;
    int color;
};

typedef struct bstNode BSTNode;
typedef struct bstNode *BSTNodePtr;

struct StackRecord
{
    struct bstNode *val;
    struct StackRecord *next;
};

struct StackRecord *top = NULL;

int push(struct bstNode *root)
{
    struct StackRecord *newRecord = (struct StackRecord *)malloc(sizeof(struct StackRecord));
    if (newRecord == NULL)
    {
        return 0;
    }

    newRecord->val = root;
    newRecord->next = top;
    top = newRecord;

    return 1;
}

struct bstNode *pop()
{
    if (top == NULL)
    {
        return NULL;
    }

    struct StackRecord *newRecord = top;

    struct bstNode *ptr = top->val;
    top = top->next;

    free(newRecord);

    return ptr;
}

int empty()
{
    if (top == NULL)
    {
        return 1;
    }
    return 0;
}

void emptyStack()
{
    while (!empty())
    {
        pop();
    }
}

void createBST(BSTNodePtr *root)
{
    *root = NULL;
}

BSTNodePtr createNode(int key)
{
    BSTNodePtr newRedNode = (BSTNodePtr)malloc(sizeof(BSTNode));
    if (!newRedNode)
        return 0;
    newRedNode->key = key;
    newRedNode->color = RED; // every node added is RED
    newRedNode->left = NULL;
    newRedNode->right = NULL;
    return newRedNode;
}

BSTNodePtr RightRotation(BSTNodePtr parent)
{
    BSTNodePtr child = parent->left;
    BSTNodePtr temp = child->right;

    child->right = parent;
    parent->left = temp;

    parent->color = 1;
    child->color = 0;

    return child;
}

BSTNodePtr LeftRotation(BSTNodePtr parent)
{
    BSTNodePtr child = parent->right;
    BSTNodePtr temp = child->left;

    child->left = parent;
    parent->right = temp;

    parent->color = 1;
    child->color = 0;

    return child;
}

BSTNodePtr LeftRightRotation(BSTNodePtr parent)
{

    BSTNodePtr child = parent->left;
    parent->left = LeftRotation(child);
    return RightRotation(parent);
}

BSTNodePtr RightLeftRotation(BSTNodePtr parent)
{
    BSTNodePtr child = parent->right;
    parent->right = RightRotation(child);
    return LeftRotation(parent);
}

BSTNodePtr rotation(BSTNodePtr node, int key)
{
    if (key < node->key)
    { // Left rotation
        // Check for LL or LR
        if (key < node->left->key) // LL
            return RightRotation(node);
        else // LR
            return LeftRightRotation(node);
    }
    else
    {                               // Right rotation
        if (key < node->right->key) // RL
            return RightLeftRotation(node);
        else // RR
            return LeftRotation(node);
    }

    return NULL;
}

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

int RedBlackInsert(BSTNodePtr *root, int data)
{
    if (*root == NULL)
    {
        *root = createNode(data);
        (*root)->color = 0; // root color is alway black
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
            emptyStack();
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

    if (!temp)
    {
        printf("\nOut of memory!!!\tcan't create new node\n");
        return -1;
    }

    if (data < prev->key)
    {
        prev->left = temp;
    }
    else
    {
        prev->right = temp;
    }

    BSTNodePtr parent = pop(); // Parent of the inserted node

    if (parent->color == BLACK) // i.e parent is black and the added node is red. So no changes needed.
    {
        emptyStack();
        return 1;
    }

    int updateFlag = 1; // A flag for checking if the further rotations are needed.

    while (!empty() && updateFlag)
    {
        BSTNodePtr grandParent = pop(); // grand parent
        if (data < grandParent->key)    // key inserted is to the Left subtree of grandparent
        {
            if (grandParent->right && grandParent->right->color == RED) // sibling of parent is red
            {
                grandParent->color = RED;          // make grandparent red
                grandParent->right->color = BLACK; // make sibling of parent black
                grandParent->left->color = BLACK;  // make parent black
            }
            else
            {
                // rotate    RedBlackInsert(&root, 7);

                updateFlag = 0;                                   // nothing need to be done after one
                BSTNodePtr newRoot = rotation(grandParent, data); // rotate based on key value of inserted node and grandparent
                if (empty())
                {
                    *root = newRoot; // if node is root, make root after rotation the root
                }
                else
                {
                    BSTNodePtr greatGrandParent = pop();       // take out great grandparent (or simply the parent of node)
                    if (greatGrandParent->left == grandParent) // assign it the right position
                        greatGrandParent->left = newRoot;
                    else
                        greatGrandParent->right = newRoot;
                }
            }
        }
        else // key inserted is to the right of the subtree of grandparent
        {
            if (grandParent->left && grandParent->left->color == RED) // sibling of parent is red
            {
                grandParent->color = RED;
                grandParent->right->color = BLACK; // same as the above logic
                grandParent->left->color = BLACK;
            }
            else
            {
                updateFlag = 0;
                BSTNodePtr newRoot = rotation(grandParent, data);
                if (empty())
                {
                    *root = newRoot;
                }
                else
                {
                    BSTNodePtr greatGrandParent = pop();
                    if (greatGrandParent->left == grandParent)
                        greatGrandParent->left = newRoot;
                    else
                        greatGrandParent->right = newRoot;
                }
            }
        }
    }
}

int main()
{
    BSTNodePtr root;
    createBST(&root);

    RedBlackInsert(&root, 10);

    RedBlackInsert(&root, 18);

    RedBlackInsert(&root, 7);

    RedBlackInsert(&root, 15);

    RedBlackInsert(&root, 16);

    RedBlackInsert(&root, 30);

    RedBlackInsert(&root, 25);

    RedBlackInsert(&root, 40);

    RedBlackInsert(&root, 60);

    RedBlackInsert(&root, 2);

    RedBlackInsert(&root, 1);

    RedBlackInsert(&root, 70);

    displayBST(root, "RBtree.gv");
    system("dot -Tpng RBtree.gv -o RBtree.png");
}