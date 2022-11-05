// Name: Kaushal Mistry
// Roll: 224101031

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief The structure required for the AVL tree with necessary elements
 */
struct AVLNode {
    int key;
    int bf;
    int size;
    struct AVLNode* leftChild;
    struct AVLNode* rightChild;
};

// Typedef for easier access
typedef struct AVLNode AVLNode;
typedef struct AVLNode* AVLNodePtr;

// ======================= Stack Helper functions using linked list ==========================
/**
 * @brief - The stack for the help in the insertion and deletion
 */
struct Node
{
    AVLNodePtr val; // stack of AVL tree node pointers
    struct Node *next;
};


/**
 * @brief The push operation on the stack
 * 
 * @param top - The stack
 * @param root - The TreeNode to push into
 * @return int - Returns -1 if any error or else 0
 */
int push(struct Node** top, AVLNodePtr root)
{
    struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
    // malloc check
    if (temp == NULL)
    {
        return -1; // Unable to create a new node
    }

    // Add new node in the begining
    temp->val = root;
    temp->next = *top;
    *top = temp; // Advance top

    return 0; // Successful
}

/**
 * @brief Pop operation for stack=
 * @param top Stack top
 * @return AVLNodePtr - Returns the node of the AVL tree
 */
AVLNodePtr pop(struct Node** top)
{
    // Checking for underflow condition
    if (*top == NULL)
    {
        printf("Stack Underflow");
        return NULL;
    }

    struct Node *tmp = *top;

    AVLNodePtr ptr = (*top)->val;
    *top = (*top)->next;

    free(tmp); // Freeing the memory to avoid memory leakage

    return ptr;
}

/**
 * @brief Checks if the Stack is empty or not
 *
 * @return int - 1 if not empty and 0 if empty
 */
int empty(struct Node** top)
{
    if (*top == NULL)
        return 1; // empty
    return 0; // not empty
}

/**
 * @brief Flushes the stack for a fresh start
 */
void flushStack(struct Node** top)
{
    while (!empty(top))
        pop(top);
}

/**
 * @brief The function gives the size of the node considering the size of it's left child and its right child.
 * 
 * @param node - The node under consideration
 * @return int - size of the node
 */
int size(AVLNodePtr node) {
    // It checks for the availability of the left child and the right child.
    // If both are there then returns sum of them + 1
    // If any one is not there then returns the size of available child + 1;
    // else returns 1 i.e. size of the leaf node.
    // Returns the size in O(1) time complexity 

    return node->leftChild ? (node->rightChild ? node->leftChild->size + node->rightChild->size + 1 : node->leftChild->size + 1) : (node->rightChild ? node->rightChild->size+1 : 1);
}

/**
 * @brief initialize the tree
 * @param root - The root of the tree
 */
void crateAVLTree(AVLNodePtr *root)
{
    *root = NULL;
}

/**
 * @brief Get the AVL tree Node object by allocating the memory for the new node
 * @return AVLNodePtr 
 */
AVLNodePtr getAVLTreeNode()
{
    AVLNodePtr newNode = (AVLNodePtr)malloc(sizeof(AVLNode));
    return newNode;
}

/**
 * @brief Get the Tree Node with basic initialization
 * @param data - The data with which the node to be created
 * @return AVLNodePtr - pointer to newly created node
 */
AVLNodePtr getTreeNode(int data)
{
    AVLNodePtr newNode = getAVLTreeNode(); // allocate memory

    // malloc condition check
    if (newNode == NULL)
    {
        return NULL; // Error in creating new node by allocating the memory
    }
    // Initializing the data
    newNode->key = data;
    newNode->size = 1;
    newNode->bf = 0;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;

    return newNode;
}

/**
 * @brief Helps in creating the dot file for the graph
 *
 * @param root - Root of the BST
 * @param fptr - File pointer to write the data in
 */
void displayAVLHelper(AVLNodePtr root, FILE *fptr)
{
    if (root != NULL)
    {
        // The below line is to show the balance factor and size along with the key of the node
        fprintf(fptr, "%d [label = \"%d, %d, %d\"];\n", root->key, root->key, root->size, root->bf);
        if (root->leftChild != NULL)
        {
            fprintf(fptr, "%d -> %d [color = red];\n", root->key, root->leftChild->key);
            displayAVLHelper(root->leftChild, fptr);
        }
        if (root->rightChild != NULL)
        {
            fprintf(fptr, "%d -> %d ;\n", root->key, root->rightChild->key);
            displayAVLHelper(root->rightChild, fptr);
        }
    }
}

/**
 * @brief Display the Tree graph
 * 
 * @param root - the root node of the tree
 * @param fileName - The file name 
 */
void displayAVLTree(AVLNodePtr root, char *fileName)
{
    FILE *fptr;

    fptr = fopen(fileName, "w+");
    if (fptr == NULL)
    {
        printf("Error while opening the file with name %s", fileName);
        return;
    }
    fprintf(fptr, "digraph bst{\n");
    displayAVLHelper(root, fptr);
    fprintf(fptr, "}\n");

    fclose(fptr);
}


/**
 * @brief Performs LL rotation starting with the node z
 * 
 * @param z - The node having the imbalance & under consideration for the rotation
 * @return AVLNodePtr - After rotation the node which comes in the place of z.
 */
AVLNodePtr LLrotation(AVLNodePtr z)
{
    AVLNodePtr y = z->leftChild;
    AVLNodePtr temp = y->rightChild;

    // Rotations
    y->rightChild = z;
    z->leftChild = temp;

    // Updating the balance factor
    z->bf = y->bf = 0;
    
    // Updating the size after the rotation
    z->size = size(z);
    y->size = size(y);

    return y;
}

/**
 * @brief Performs RR rotation starting with the node z
 * 
 * @param z - The node having the imbalance & under consideration for the rotation
 * @return AVLNodePtr - After rotation the node which comes in the place of z.
 */
AVLNodePtr RRrotation(AVLNodePtr z)
{
    AVLNodePtr y = z->rightChild;
    AVLNodePtr temp = y->leftChild;

    // Rotation performed here
    y->leftChild = z;
    z->rightChild = temp;

    // Updating the balance factor
    z->bf = y->bf = 0;

    // Updating the size
    z->size = size(z);
    y->size = size(y);

    return y;
}

/**
 * @brief Performs LR (double rotation) along with the node z;
 * 
 * @param z - The node having the imbalance & under consideration for the rotation
 * @return AVLNodePtr - The new root after the rotation
 */
AVLNodePtr LRrotation(AVLNodePtr z)
{
    // Taking y and x into consideration
    AVLNodePtr y = z->leftChild;
    AVLNodePtr x = y->rightChild;
    AVLNodePtr temp = x->rightChild;
    AVLNodePtr temp2 = x->leftChild;
    int check = x->bf;

    // Rotations
    x->leftChild = y;
    x->rightChild = z;

    z->leftChild = temp;
    y->rightChild = temp2;

    // Updating the size of the nodes affected by the rotation
    y->size = size(y);
    z->size = size(z);
    x->size = size(x);

    // Updating the balance factor based on the previous positions
    if (check == 1) // If the x was leaning towards right before rotation
    {
        x->bf = 0;
        y->bf = -1;
        z->bf = 0;
    }
    else if (check == -1) // If the x was leaning towards left before rotation
    {
        x->bf = 0;
        y->bf = 0;
        z->bf = 1;
    }
    else // If the x was balanced before
    {
        x->bf = y->bf = z->bf = 0;
    }

    return x;
}

/**
 * @brief Performs RL (double rotation) along with the node z;
 * 
 * @param z - The node having the imbalance & under consideration for the rotation
 * @return AVLNodePtr - The new root after the rotation
 */
AVLNodePtr RLrotation(AVLNodePtr z)
{
    // Taking y and x into consideration
    AVLNodePtr y = z->rightChild;
    AVLNodePtr x = y->leftChild;
    AVLNodePtr temp = x->rightChild;
    AVLNodePtr temp2 = x->leftChild;
    int check = x->bf;

    // Performing the rotation
    x->rightChild = y;
    x->leftChild = z;
    z->rightChild = temp2;
    y->leftChild = temp;

    // Updating the size of the nodes affected by the rotation
    y->size = size(y);
    z->size = size(z);
    x->size = size(x);

    // Updating the balance factor based on the previous positions
    if (check == 1) // If the x was leaning towards right before rotation
    {
        x->bf = 0;
        y->bf = 0;
        z->bf = -1;
    }
    else if (check == -1) // If the x was leaning towards left before rotation
    {
        x->bf = 0;
        y->bf = 1;
        z->bf = 0;
    }
    else // If the x was balanced before
    {
        x->bf = y->bf = z->bf = 0;
    }

    return x;
}

/**
 * @brief The function decides which rotation to be performed 
 * 
 * @param node - The node having imbalance
 * @return AVLNodePtr - The new root after the rotation
 */
AVLNodePtr rotation(AVLNodePtr node)
{   
    // The checks are based on the balance factor
    // 2, 1/0 -> RR
    // 2, -1 -> RL
    // -2, -1/0 -> LL
    // -2, 1 -> LR
    if (node->bf > 1 && node->rightChild->bf >= 0)
        return RRrotation(node);
    
    if (node->bf > 1 && node->rightChild->bf == -1)
        return RLrotation(node);

    if (node->bf < -1 && node->leftChild->bf <= 0)
        return LLrotation(node);

    if (node->bf < -1 && node->leftChild->bf == 1)
        return LRrotation(node);

    return NULL;
}

/**
 * @brief This helps in increasing the size of path nodes if the insertion is successful
 * Updates the balance factor of the node after insertion and checks whether the there is any imbalance or not.
 * After updating the bFactor, if the height is changed or not -> If changed then propagate the bFactor up above
 */
void AVLInsertHelper(AVLNodePtr *root, int key, struct Node** top)
{
    AVLNodePtr newRoot;

    // Helps in deciding whether to propagate the bFactor to parent or not
    int updateFlag = 1;

    // While stack has element and the bFactor needs to be updated to the parent
    while (!empty(top) && updateFlag)
    {
        AVLNodePtr temp = pop(top); // popping off the element

        temp->size = size(temp); // Updating the size

        // If the new node is added in the right subtree
        if (temp->key < key)
        {
            // Check the previous bFactor and update the bFactor accordingly
            if (temp->bf == 0)
            {
                temp->bf++;
                updateFlag = 1;
            }

            else if (temp->bf == -1)
            {
                temp->bf = 0;
                updateFlag = 0;
            }

            else // Imbalance occurs
            {
                updateFlag = 0;
                temp->bf++;
                // Rotate and get the new node
                newRoot = rotation(temp);
                // If there is no parent of the node assign it as root
                if (empty(top))
                    *root = newRoot;
                else
                {   
                    // Find the parent and update the pointer to new child root
                    AVLNodePtr parentTemp = pop(top);
                    if (parentTemp->leftChild == temp)
                        parentTemp->leftChild = newRoot;
                    else
                        parentTemp->rightChild = newRoot;

                    parentTemp->size = size(parentTemp); // Update the size of the parent
                    
                }
            }
        }
        // If the new node is added in the left subtree
        else
        {
            // Check the previous bFactor and update the bFactor accordingly
            if (temp->bf == 0)
            {
                temp->bf--;
                updateFlag = 1;
            }
            else if (temp->bf == 1)
            {
                temp->bf = 0;
                updateFlag = 0;
            }

            // Imbalance occurs
            else
            {
                updateFlag = 0;
                temp->bf--;
                // Rotate and get the new node
                newRoot = rotation(temp);
                // If there is no parent of the node assign it as root
                if (empty(top))
                    *root = newRoot;
                else
                {
                    // Find the parent and update the pointer to new child root
                    AVLNodePtr parentTemp = pop(top);
                    if (parentTemp->leftChild == temp)
                        parentTemp->leftChild = newRoot;
                    else
                        parentTemp->rightChild = newRoot;
                    
                    parentTemp->size = size(parentTemp);
                }
            }
        }
    }

    // If after updating the balance factors, there are any nodes in the stack then update the size of them
    while (!empty(top)) {
        AVLNodePtr node = pop(top);
        node->size = size(node);
    }
}


/**
 * @brief Insert a new node in the AVL tree
 * 
 * @param root - The root node
 * @param data - The data to be inserted
 * @return int - 0 if success -1 otherwise
 */
int AVLInsert(AVLNodePtr *root, int data)
{
    // Initializing the stack
    struct Node* top = NULL;
    // If root is empty then allocate root node
    if (*root == NULL)
    {
        *root = getTreeNode(data);
        return 0;
    }

    // Search for the place where the new node to be inserted
    AVLNodePtr cur = *root;
    AVLNodePtr prev = NULL;

    while (cur != NULL)
    {
        push(&top, cur);
        prev = cur;
        if (cur->key == data) // Duplicates not allowed in the BST
        {
            flushStack(&top);
            return -1; // The data already exist
        }
        // Data is in left subtree
        if (data < cur->key)
            cur = cur->leftChild;
        else // data is in right subtree
            cur = cur->rightChild;
    }

    // Allocate memory for new node
    AVLNodePtr temp = getTreeNode(data);
    // Successfully created the node then only update the Tree\

    // Checking the malloc condition
    if (temp)
    {
        if (data < prev->key)
            prev->leftChild = temp;
        else
            prev->rightChild = temp;
        AVLInsertHelper(root, data, &top);
        return 0;
    }
    
    flushStack(&top); // If any error in the malloc then remove elements from stack to avoid memory leakage

    return -1; // Error while adding the new node
}

/**
 * @brief Delete the AVL tree
 * 
 * @param root - The root of the tree
 */
void deleteTree(AVLNodePtr *root) {
    // If root is null then return
    if (*root == NULL)
        return;

    // Delete the nodes in the right subtree
    deleteTree(&((*root)->leftChild));
    // Delete the nodes in the left subtree
    deleteTree(&((*root)->rightChild));
    // Free the root
    AVLNodePtr temp = *root;
    *root = NULL;
    free(temp);
}

/**
 * @brief Inorder traversal
 * @param root - Root of the AVL tree
 */
void inorder(AVLNodePtr root) {
    if (!root)
        return;
    inorder(root->leftChild);
    printf(" %d ", root->key);
    inorder(root->rightChild);
}

int main(int argc, char* argv) {
    AVLNodePtr root;
    crateAVLTree(&root);

    int arr[] = {50, 30, 10, 5, 60, 70, 8};
    // for (int i = 0; i < sizeof(arr)/sizeof(int); i++) {
    for (int i = 0; i < 30; i++) {
        int n = rand() % 97;
        AVLInsert(&root, n);
    }

    printf("\n\nInorder\n");
    inorder(root);
    printf("\n");

    displayAVLTree(root, "AVLTree.gv");
    system("dot -Tpng AVLTree.gv -o AVLTree.png");

    int n = 0;
    int newData;
    while (n < 5) {
        printf("\nEnter Data: ");
        scanf("%d", &newData);
        AVLInsert(&root, newData);
        printf("\n\nInorder\n");
        inorder(root);
        printf("\n");
        displayAVLTree(root, "AVLTree.gv");
        system("dot -Tpng AVLTree.gv -o AVLTree.png");
        
    }
    // deleteTree(&root);

    // displayAVLTree(root, "AVLTree.gv");
    // system("dot -Tpng AVLTree.gv -o AVLTree.png");

    return 0;
}