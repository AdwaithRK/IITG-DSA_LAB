/**
 * @file    : 224101019.c
 * @author  : Gandhar Deshpande
 * @roll_no : 224101019
 * @date    : 2022-09-03
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct AVLNode
{
    int key;
    int bf; // left - right
    int size;
    struct AVLNode *leftChild;
    struct AVLNode *rightChild;
};

typedef struct AVLNode AVLNode;
typedef struct AVLNode *AVLNodePtr;

// stack node
typedef struct stNode
{
    AVLNodePtr node;
    struct stNode *next;
} stNode;

/**
 * @brief stack that stores top pointer to stackNode & size
 */
typedef struct Stack
{
    int size;
    stNode *top;
} Stack;

void createAVLTree(AVLNodePtr *root);
AVLNodePtr getAVLTreeNode();
void displayAVLTree(AVLNodePtr root, char *filename);
AVLNodePtr AVLTreeSearch(AVLNodePtr root, int value);
void AVLInsert(AVLNodePtr *root, int data);
AVLNodePtr rightRotation(AVLNodePtr *root);
AVLNodePtr leftRotation(AVLNodePtr *root);
AVLNodePtr LR(AVLNodePtr *root);
AVLNodePtr RL(AVLNodePtr *root);
AVLNodePtr leftBalance(AVLNodePtr *root);
AVLNodePtr rightBalance(AVLNodePtr *root);
void deleteTree(AVLNodePtr *root);

/**
 * @brief Intialize stack with size 0 and set top of stack NULL.
 * @param st - pointer to stack
 */
void init(Stack *st)
{
    st->size = 0;
    st->top = NULL;
}

/**
 * @brief - Push AVLNodePtr to stack
 * @param st : stack
 * @param value : AVLNodePtr to be pushed
 * @return : -1 --> error
 *            1 --> succesfully performed push
 */
int push(Stack *st, AVLNodePtr value)
{
    stNode *newNode = (stNode *)malloc(sizeof(stNode));
    if (!newNode)
        return -1;

    newNode->node = value;
    newNode->next = NULL;
    if (st->top)
    {
        newNode->next = st->top;
    }
    st->top = newNode;
    st->size++;
    return 1;
}

/**
 * @brief - pop AVLNodePtr from stack
 * @param st : stack
 * @return : -1 --> error
 *            1 --> succesfully performed pop
 */
AVLNodePtr pop(Stack *st)
{
    if (!st->top)
        return NULL;
    AVLNodePtr rv = st->top->node;

    stNode *temp = st->top;
    st->top = st->top->next;
    free(temp);
    st->size--;
    return rv;
}

/**
 * @brief - return top AVLNodePtr in stack
 * @param st : stack
 * @return : NULL --> stack empty
 *           otherwise --> return top AVLNodePtr
 */
AVLNodePtr top(Stack st)
{
    if (st.top == NULL)
        return NULL;
    return st.top->node;
}

/**
 * @brief - return whether stack is empty or not
 * @param st : stack
 * @return : 1 --> stack empty
 *           0 --> not empty
 */
int isEmpty(Stack st)
{
    if (st.top)
        return 0;
    return 1;
}

/**
 * @brief - prints values in stack from top of stack
 * @param st : stack
 */
void printStack(Stack st)
{
    stNode *itr = st.top;
    printf("top -> [");
    while (itr)
    {
        printf("%d -> ", itr->node->key);
        itr = itr->next;
    }
    printf("NULL]\n");
}

/**
 * @brief - Generated the tree in png format
 *
 * @param root - root of tree
 * @param fp - filename for writing dot file
 */
void generateGraph(AVLNodePtr root, FILE *fp)
{
    if (root == NULL)
        return;
    fprintf(fp, "%d [label = \"%d, %d, (%d)\"];\n", root->key, root->key, root->bf, root->size);
    if (root->leftChild)
    {
        fprintf(fp, "%d [label = \"%d, %d, (%d)\"];\n", root->leftChild->key, root->leftChild->key, root->leftChild->bf, root->leftChild->size);
        fprintf(fp, "%d -> %d [color = blue];\n", root->key, root->leftChild->key);
        generateGraph(root->leftChild, fp);
    }

    printf("%d ", root->key);

    if (root->rightChild)
    {
        // fprintf(fp, "%d [label = \"%d, %d\"];\n", root->key, root->key, root->size);
        fprintf(fp, "%d [label = \"%d, %d, (%d)\"];\n", root->rightChild->key, root->rightChild->key, root->rightChild->bf, root->rightChild->size);
        fprintf(fp, "%d -> %d [color = red];\n", root->key, root->rightChild->key);
        generateGraph(root->rightChild, fp);
    }
}

/**
 * @brief - takes root pointer and file as input and generates tree by using Inorder. It stores output in png format
 * @param root - root of tree
 * @param filename - dot filename
 */
void displayAVLTree(AVLNodePtr root, char *filename)
{
    static int ind = 0;
    FILE *fp = fopen("AVLTree.dot", "w");

    char cmd[100] = "dot -Tpng ";

    strcat(cmd, filename);
    if (ind)
        strcat(cmd, " -o AVLTree1.png");
    else
        strcat(cmd, " -o AVLTree2.png");

    fprintf(fp, "digraph G {\n");
    generateGraph(root, fp);
    fprintf(fp, "}\n");

    printf("\n");

    fclose(fp);

    system(cmd);

    // for two alternating images
    ind = ind ^ 1;
}

/**
 * @brief - takes root pointer intialize it to NULL.
 * @param root : pointer to rootPtr
 */
void createAVLTree(AVLNodePtr *root) { *root = NULL; }

/**
 * @brief - generates a node, initialize it and return pointer to node
 * @return AVLNodePtr
 */
AVLNodePtr getAVLTreeNode()
{
    AVLNodePtr node = (AVLNodePtr)malloc(sizeof(AVLNode));
    node->bf = 0;
    node->leftChild = NULL;
    node->rightChild = NULL;
    node->size = 1;
    return node;
}

/**
 * @brief Search avl tree for given value & return pointer to node
 * @param root - root of tree
 * @param value  - value to search
 * @return AVLNodePtr  - if succesful search
 *         NULL - if not found
 */
AVLNodePtr AVLTreeSearch(AVLNodePtr root, int value)
{
    if (root == NULL)
    {
        return NULL; // end of path, node not found
    }
    // search in right tree
    if (value > root->key)
    {
        return AVLTreeSearch(root->rightChild, value);
    }
    // search at left
    else if (value < root->key)
    {
        return AVLTreeSearch(root->leftChild, value);
    }
    else
        return root;
}

/**
 * @brief Get the Size of tree rooted at node
 * @param node
 * @return int size of tree
 */
int getSize(AVLNodePtr node)
{
    if (node == NULL)
        return 0;
    return node->size;
}

/**
 * @brief return root after right roatating tree, also changes balance factors.
 * @param root - node at which inbalance occurs
 * @return AVLNodePtr of rotated tree
 */
AVLNodePtr rightRotation(AVLNodePtr *root)
{
    AVLNodePtr y = *root;
    AVLNodePtr x = y->leftChild;

    // changing pointers for right rotation
    y->leftChild = x->rightChild;
    x->rightChild = y;

    printf("(LL) - y = [%d,%d], x = [%d, %d]\n", y->key, y->bf, x->key, x->bf);

    // After rotation both nodes have bf of 0
    y->bf = 0;
    x->bf = 0;

    // after rotation size also needs update
    // we can use this because child's size does not change
    y->size = getSize(y->leftChild) + getSize(y->rightChild) + 1;
    x->size = getSize(x->leftChild) + getSize(x->rightChild) + 1;

    return x;
}

/**
 * @brief return root after left roatating tree, also changes balance factors.
 * @param root - node at which inbalance occurs
 * @return AVLNodePtr of rotated tree
 */
AVLNodePtr leftRotation(AVLNodePtr *root)
{
    AVLNodePtr y = *root;
    AVLNodePtr x = y->rightChild;

    // changing pointers for left rotation
    y->rightChild = x->leftChild;
    x->leftChild = y;

    printf("(RR) - y = [%d,%d], x = [%d, %d]\n", y->key, y->bf, x->key, x->bf);

    // After rotation both nodes have bf of 0
    y->bf = 0;
    x->bf = 0;

    // after rotation size also needs update
    // we can use this because child's size does not change
    y->size = getSize(y->leftChild) + getSize(y->rightChild) + 1;
    x->size = getSize(x->leftChild) + getSize(x->rightChild) + 1;
    // *root = x;
    return x;
}

/**
 * @brief return root after left roatating tree, also changes balance factors.
 * @param root - node at which inbalance occurs
 * @return AVLNodePtr of rotated tree
 */
AVLNodePtr LR(AVLNodePtr *root)
{
    AVLNodePtr z = *root;
    AVLNodePtr y = z->leftChild;
    AVLNodePtr x = y->rightChild;

    z->leftChild = x->rightChild;
    y->rightChild = x->leftChild;
    x->leftChild = y;
    x->rightChild = z;
    printf("(LR) - z = [%d,%d], y = [%d,%d], x = [%d, %d]\n", z->key, z->bf, y->key, y->bf, x->key, x->bf);

    //        z                  x
    //       /                 /   \
    //      y       --->      y     z
    //       \
    //        x
    // bf after rotation depends upon x's bf
    if (x->bf == 1)
    {
        y->bf = 0;
        x->bf = 0;
        z->bf = -1;
    }
    else if (x->bf == -1)
    {
        y->bf = 1;
        x->bf = 0;
        z->bf = 0;
    }
    else if (x->bf == 0)
    {
        y->bf = 0;
        x->bf = 0;
        z->bf = 0;
    }

    // after rotation size also needs update
    // we can use this because child's size does not change
    // but we need to update in order -> y, z, x as y & z are children of x
    y->size = getSize(y->leftChild) + getSize(y->rightChild) + 1;
    z->size = getSize(z->leftChild) + getSize(z->rightChild) + 1;
    x->size = getSize(x->leftChild) + getSize(x->rightChild) + 1;
    // *root = x;
    return x;
}

/**
 * @brief return root after left roatating tree, also changes balance factors.
 * @param root - node at which inbalance occurs
 * @return AVLNodePtr of rotated tree
 */
AVLNodePtr RL(AVLNodePtr *root)
{
    AVLNodePtr z = *root;
    AVLNodePtr y = z->rightChild;
    AVLNodePtr x = y->leftChild;

    z->rightChild = x->leftChild;
    y->leftChild = x->rightChild;
    x->leftChild = z;
    x->rightChild = y;
    printf("(RL) - z = [%d,%d], y = [%d,%d], x = [%d, %d]\n", z->key, z->bf, y->key, y->bf, x->key, x->bf);

    //        z                    x
    //         \                 /   \
    //          y   --->        z     y
    //         /
    //        x
    // bf after rotation depends upon x's bf
    if (x->bf == 1)
    {
        z->bf = 0;
        x->bf = 0;
        y->bf = -1;
    }
    else if (x->bf == -1)
    {
        z->bf = 1;
        x->bf = 0;
        y->bf = 0;
    }
    else if (x->bf == 0)
    {
        z->bf = 0;
        x->bf = 0;
        y->bf = 0;
    }

    // after rotation size also needs update
    // we can use this because child's size does not change
    // but we need to update in order -> y, z, x as y & z are children of x
    y->size = getSize(y->leftChild) + getSize(y->rightChild) + 1;
    z->size = getSize(z->leftChild) + getSize(z->rightChild) + 1;
    x->size = getSize(x->leftChild) + getSize(x->rightChild) + 1;

    // *root = x;
    return x;
}

/**
 * @brief - selects appropriate roation according to leftchild's bf.
 * @param root - root at which left inbalance occurred (Left Insert)
 * @return AVLNodePtr after rotation
 */
AVLNodePtr leftBalance(AVLNodePtr *root)
{
    // Left of left is heavy so LL inbalance
    if ((*root)->leftChild->bf == 1)
    {
        return rightRotation(root);
    }
    // Right of left is heavy so RL
    // otherwise LL inbalance
    else
    {
        return LR(root);
    }
}

/**
 * @brief - selects appropriate roation according to rightchild's bf.
 * @param root - root at which right inbalance occurred (Right Insert)
 * @return AVLNodePtr after rotation
 */
AVLNodePtr rightBalance(AVLNodePtr *root)
{
    // Right of Right is heavy so RR inbalance
    if ((*root)->rightChild->bf == -1)
    {
        return leftRotation(root);
    }
    // left of right is heavy so RL
    else
    {
        return RL(root);
    }
}

/**
 * @brief Inserts node in iterative fashion
 *
 * @param root - root of tree
 * @param data - data to be inserted
 */
void AVLInsert(AVLNodePtr *root, int data)
{
    // first node insertion
    if (*root == NULL)
    {
        AVLNodePtr newNode = getAVLTreeNode();
        newNode->key = data;
        *root = newNode;
        return;
    }

    // if alreday inserted then return
    if (AVLTreeSearch(*root, data) != NULL)
        return;

    // stack for storing ancestors along the path of insertion
    Stack st;
    init(&st);

    AVLNodePtr node = *root, parent = NULL;
    // find spot for insertion
    while (1)
    {
        if (node == NULL)
        { // spot for insertion found
            break;
        }
        push(&st, node); // pushing nodes along the path
        parent = node;

        if (data < node->key)
        { // left search
            node = node->leftChild;
        }

        else
        { // Right search
            node = node->rightChild;
        }
    }

    // generate newNode for insertion
    AVLNodePtr newNode = getAVLTreeNode();
    newNode->key = data;

    // Insert at appropriate child
    if (data > parent->key)
    {
        parent->rightChild = newNode;
        // printf("%d Right Inserted at - %d\n", data, parent->data);
    }
    else
    {
        parent->leftChild = newNode;
        // printf("%d Left Inserted at - %d\n", data, parent->data);
    }

    // *************  Now check for balance of tree in reverse way *********************

    // as inserted set heightInserted = true
    bool heightIncreased = true;

    // update all ancestors in path upto root (ancestors stored in stack st)
    // once balance is achieved heightincreased is set to false, so after that only size is updated not the bf.
    while (!isEmpty(st))
    {
        AVLNodePtr anc = pop(&st);       // current
        AVLNodePtr anc_parent = top(st); // parent of current

        // printf("anc = [%d, %d]\n", anc->key, anc->bf);

        // increase size of all ancestors
        anc->size++;

        // If height is balanced , do not check for inbalance
        if (heightIncreased == false)
            continue;

        // left Inserted at this node
        if (data < anc->key)
        {
            // updation based on old bf
            switch (anc->bf)
            {
            case 0:          // Even Heavy
                anc->bf = 1; // after left insertion becomes Left Heavy
                break;

            case 1: // Left Heavy
                // after left insertion creates inbalance
                if (anc_parent)
                { // Parent of root to be rotated exists
                    // parents left child
                    if (data < anc_parent->key)
                    {
                        anc_parent->leftChild = leftBalance(&anc);
                    }
                    else
                    { // parents right child
                        anc_parent->rightChild = leftBalance(&anc);
                    }
                }
                else
                { // root does not have parent
                    *root = leftBalance(&anc);
                }

                // After rotation height is balanced
                heightIncreased = false;
                break;

            case -1:                     // Right Heavy
                anc->bf = 0;             // after left Insertion becomes Even Heavy
                heightIncreased = false; // so height is also balanced.
                break;

            default:
                break;
            } // end switch
        }     // end if
        else
        { // Right Inserted at this node
            switch (anc->bf)
            {
            case 0:           // Even Heavy
                anc->bf = -1; // after right insertion becomes Right Heavy
                break;

            case 1:                      // Left Heavy
                anc->bf = 0;             // after right Insertion becomes Even Heavy
                heightIncreased = false; // so height is also balanced.
                break;

            case -1: // Right Heavy
                // after right insertion creates inbalance
                if (anc_parent)
                { // Parent of root to be rotated exists
                    if (data < anc_parent->key)
                    { // parents left child
                        anc_parent->leftChild = rightBalance(&anc);
                    }
                    else
                    { // parents right child
                        anc_parent->rightChild = rightBalance(&anc);
                    }
                }
                else
                { // root does not have parent
                    *root = rightBalance(&anc);
                }
                // After rotation height is balanced
                heightIncreased = false;
                break;

            default:
                break;
            } // end switch
        }     // end else
    }         // end while
}

/**
 * @brief - deletes tree recursively
 * @param root - root of tree
 */
void deleteTree(AVLNodePtr *root)
{
    if ((*root) == NULL)
        return;                       // return if nothing to delete
    deleteTree(&(*root)->leftChild);  // deletes all left nodes
    deleteTree(&(*root)->rightChild); // deletes all right nodes
    AVLNodePtr temp = *root;          // stores cur node
    free(temp);                       // free cur node
    *root = NULL;                     // set this node to NULL
}

int main(int argc, char const *argv[])
{
    /* code */
    AVLNodePtr root;
    createAVLTree(&root);
    while (1)
    {
        printf("\n\nChoose operation:\n");
        printf("1. Insert\n");
        printf("2. Delete\n");
        printf("3. Print\n");
        printf("4. Search\n");
        printf("5. Press any other num for exit\n\n");
        int opt;
        scanf("%d", &opt);
        int flag = 0, key;
        int n, ans, k;
        AVLNodePtr kthmin;

        switch (opt)
        {
        case 1:
            scanf("%d", &key);
            AVLInsert(&root, key);
            displayAVLTree(root, "AVLTree.dot");
            break;
        case 2:
            deleteTree(&root);
            displayAVLTree(root, "AVLTree.dot");
            break;
        case 3:
            displayAVLTree(root, "AVLTree.dot");
            break;
        case 4:
            scanf("%d", &key);
            AVLNodePtr s = AVLTreeSearch(root, key);
            if (s)
                printf("\nNode found = %d", key);
            else
                printf("\nNode %d not found", key);
            break;
        default:
            flag = 1;
            break;
        }

        if (flag == 1)
            break;
    }
    return 0;
    return 0;
}
