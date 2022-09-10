#include <stdio.h>
#include <stdlib.h>

#define RED 1
#define BLACK 0

struct bstNode
{
    int key;
    struct bstNode *left;
    struct bstNode *right;
    int size;
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
    newRedNode->size = 1;
    return newRedNode;
}

void update_size(BSTNodePtr *node)
{
    int left_size = (*node)->right ? (*node)->right->size : 0;
    int right_size = (*node)->left ? (*node)->left->size : 0;

    (*node)->size = left_size + right_size + 1;
}

BSTNodePtr RightRotation(BSTNodePtr parent)
{
    BSTNodePtr child = parent->left;
    BSTNodePtr temp = child->right;

    child->right = parent;
    parent->left = temp;

    update_size(&parent);
    update_size(&child);

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

    update_size(&parent);
    update_size(&child);

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
        if (root->color == RED)
        {
            fprintf(fptr, "%d [label = \"%d bf:%d\" ,color = red];\n", root->key, root->key, root->size);
        }
        else
        {
            fprintf(fptr, "%d [label = \" %d bf:%d\", color = black];\n", root->key, root->key, root->size);
        }
        if (root->left)
        {
            fprintf(fptr, "%d -> %d [color = red, style=dotted];\n", root->key, root->left->key);
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

void rotateAndAdjust(int *updateFlag, BSTNodePtr *grandParent, int data, BSTNodePtr *root)
{
    *updateFlag = 0;                                   // nothing need to be done after one
    BSTNodePtr newRoot = rotation(*grandParent, data); // rotate based on key value of inserted node and grandparent
    if (empty())
    {
        *root = newRoot; // if node is root, make root after rotation the root
    }
    else
    {
        BSTNodePtr greatGrandParent = pop();        // take out great grandparent (or simply the parent of node)
        if (greatGrandParent->left == *grandParent) // assign it the right position
            greatGrandParent->left = newRoot;
        else
            greatGrandParent->right = newRoot;
    }
}

int RBTreeSearch(BSTNodePtr root, int value)
{
    if (root == NULL)
    {
        return 0; // end of path, node not found
    }
    // search in right tree
    if (value > root->key)
    {
        return RBTreeSearch(root->right, value);
    }
    // search at left
    else if (value < root->key)
    {
        return RBTreeSearch(root->left, value);
    }
    else
        return 1;
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

    if (RBTreeSearch(*root, data))
        return 0;

    while (cur != NULL)
    {
        cur->size++;
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

    int updateFlag = 1; // A flag for checking if the further rotations are needed.

    while (!empty() && updateFlag)
    {
        BSTNodePtr parent = pop(); // Parent of the inserted node

        if (parent->color == RED)
        {
            BSTNodePtr grandParent = pop(); // grand parent

            if (data < grandParent->key) // key inserted is to the Left subtree of grandparent
            {
                if (grandParent->right && grandParent->right->color == RED) // sibling of parent is red
                {
                    grandParent->color = RED;          // make grandparent red
                    grandParent->right->color = BLACK; // make sibling of parent black
                    grandParent->left->color = BLACK;  // make parent black
                }
                else
                {

                    rotateAndAdjust(&updateFlag, &grandParent, data, root);
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
                    rotateAndAdjust(&updateFlag, &grandParent, data, root);
                }
            }
        }
        else
            break;
    }

    (*root)->color = 0; // color root black

    emptyStack();
    return 1;
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

    // RedBlackInsert(&root, 95);

    // RedBlackInsert(&root, 105);

    // for (int i = 90; i < 120; i++)
    //     RedBlackInsert(&root, i);

    displayBST(root, "RBtree.gv");
    system("dot -Tpng RBtree.gv -o RBtree.png");
}