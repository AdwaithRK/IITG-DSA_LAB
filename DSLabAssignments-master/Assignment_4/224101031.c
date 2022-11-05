// Name: Kaushal Mistry
// Roll No: 224101031

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/**
 * @brief Structure of the tree node
 */
struct rightThreadedBSTNode
{
    int key;                                 // The element
    int size;                                // size of the tree rooted at that element
    int rightThread;                         // the bool indicating if the right thread is there or not - 1 / 0
    struct rightThreadedBSTNode *leftChild;  // left child
    struct rightThreadedBSTNode *rightChild; // right child
};

// typedef for easier access
typedef struct rightThreadedBSTNode rTBSTNode;
typedef struct rightThreadedBSTNode *rTBSTNodePtr;

// ======================= Stack Helper functoins using linked list ==========================
/**
 * @brief - The stack for the help in the insertion and deletion
 */
struct Node
{
    rTBSTNodePtr val; // stack of tree node pointers
    struct Node *next;
};

// Initializing stack for
struct Node *top = NULL;

/**
 * @brief The push operation on the stack
 *
 * @param root - The TreeNode to push into
 * @return int - Returns 1 if any error or else 0
 */
int push(rTBSTNodePtr root)
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
rTBSTNodePtr pop()
{
    if (top == NULL)
    {
        printf("Stack Underflow");
        return NULL;
    }

    struct Node *tmp = top;

    rTBSTNodePtr ptr = top->val;
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

/**
 * @brief initialize the tree
 * @param root - The root of the tree
 */
void crateRightThreadedBST(rTBSTNodePtr *root)
{
    *root = NULL;
}

/**
 * @brief Get the Right Threaded Tree Node object by allocating the memory for the new node
 * @return rTBSTNodePtr 
 */
rTBSTNodePtr getRightThreadedTreeNode()
{
    rTBSTNodePtr newNode = (rTBSTNodePtr)malloc(sizeof(rTBSTNode));
    return newNode;
}

/**
 * @brief Get the Tree Node with basic initialization
 * @param data - The data with which the node to be created
 * @return rTBSTNodePtr - pointer to newly created node
 */
rTBSTNodePtr getTreeNode(int data)
{
    rTBSTNodePtr newNode = getRightThreadedTreeNode(); // allocate memory

    if (newNode == NULL)
    {
        return NULL; // Error in creating new node by allocating the memory
    }
    newNode->key = data;
    newNode->size = 1;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;
    newNode->rightThread = 1;

    return newNode;
}

/**
 * @brief Helps in creating the dot file for the graph
 *
 * @param root - Root of the BST
 * @param fptr - File pointer to write the data in
 */
void displayBSTHelper(rTBSTNodePtr root, FILE *fptr)
{
    if (root != NULL)
    {
        fprintf(fptr, "%d [label = \"%d, %d\"];\n", root->key, root->key, root->size);
        if (root->leftChild != NULL)
        {
            fprintf(fptr, "%d -> %d [color = red];\n", root->key, root->leftChild->key);
            displayBSTHelper(root->leftChild, fptr);
        }
        if (root->rightChild != NULL)
        {
            // If the thread is there then use diff color and style
            if (root->rightThread)
            {
                fprintf(fptr, "%d -> %d [color=brown, style=dotted];\n", root->key, root->rightChild->key);
            }
            else
            {
                fprintf(fptr, "%d -> %d ;\n", root->key, root->rightChild->key);
                displayBSTHelper(root->rightChild, fptr);
            }
        }
    }
}

/**
 * @brief Display the Tree graph
 * 
 * @param root - the root node of the tree
 * @param fileName - The file name 
 */
void displayRightThreadedBST(rTBSTNodePtr root, char *fileName)
{
    FILE *fptr;

    fptr = fopen(fileName, "w+");
    if (fptr == NULL)
    {
        printf("Error while opening the file with name %s", fileName);
        return;
    }
    fprintf(fptr, "digraph bst{\n");
    displayBSTHelper(root, fptr);
    fprintf(fptr, "}\n");

    fclose(fptr);
}

/**
 * @brief Searches for the key in the Tree
 * @param root - Root node of the tree
 * @param data - The data to be searched
 * @return rTBSTNodePtr - the pointer to the node with the data as key if found NULL otherwise
 */
rTBSTNodePtr rtbstSearch(rTBSTNodePtr *root, int data)
{
    rTBSTNodePtr ptr = *root;
    // Searches iteratively
    while (ptr != NULL)
    {
        if (ptr->key == data)
        {
            break;
        }
        if (data < ptr->key)
        {
            ptr = ptr->leftChild;
        }
        else
        {
            if (ptr->rightThread)
                return NULL; // The key not found
            ptr = ptr->rightChild;
        }
    }

    return ptr;
}

/**
 * @brief Helps in analyzing the search result
 * @param root 
 * @param data 
 */
void searchHelper(rTBSTNodePtr *root, int data)
{
    rTBSTNodePtr res = rtbstSearch(root, data);
    if (res)
    {
        printf("\nThe key found = %d\n", res->key);
    }
    else
    {
        printf("\nThe key not found\n");
    }
}


/**
 * @brief Finds the left most node of a given node
 * 
 * @param node - The tree node
 * @return rTBSTNodePtr - left most node of the given node
 */
rTBSTNodePtr leftMost(rTBSTNodePtr node)
{
    rTBSTNodePtr ptr = node;

    while (ptr->leftChild)
    {
        ptr = ptr->leftChild;
    }

    return ptr;
}

/**
 * @brief Performs inorder traversal of the tree without recursion and without stack
 * 
 * @param root - The root node of the tree
 * @param List - The array to store the values of the key of the nodes in inorder fashion
 */
void inorder(rTBSTNodePtr root, int *List)
{
    if (root == NULL)
    {
        return; // The tree is empty
    }
    int i = 0;

    int flag = 1;
    rTBSTNodePtr ptr = leftMost(root);

    while (ptr)
    {
        List[i] = ptr->key;
        i++;

        if (ptr->rightThread)
            ptr = ptr->rightChild;
        else
            ptr = leftMost(ptr->rightChild);
    }
}

/**
 * @brief Prints the inorder by reading the int array
 * @param root - Root for the size
 * @param list - The array
 */
void printInorder(rTBSTNodePtr root, int *list)
{
    if (sizeof(list) < 1)
    {
        return; // The list is empty
    }
    printf("Inorder list = ");
    for (int i = 0; i < root->size; i++)
    {
        printf("%d -> ", list[i]);
    }
    printf("\n");
}

/**
 * @brief This helps in increasing the size of path nodes if the insertion is successful
 */
void BSTInsertHelper()
{
    while (!empty())
    {
        rTBSTNodePtr temp = pop();
        temp->size++;
    }
}

/**
 * @brief Insert a new node in the BST 
 * 
 * @param root - The root node
 * @param data - The data to be inserted
 * @return int - 0 if success -1 otherwise
 */
int rtbstInsert(rTBSTNodePtr *root, int data)
{
    // If root is empty then allocate root node
    if (*root == NULL)
    {
        *root = getTreeNode(data);
        return 0;
    }

    // Search for the place where the new node to be inserted
    rTBSTNodePtr cur = *root;
    rTBSTNodePtr prev = NULL;

    while (cur != NULL)
    {
        push(cur);
        prev = cur;
        if (cur->key == data) // Duplicates not allowed in the BST
        {
            flushStack();
            return -1; // The data already exist
        }
        if (data < cur->key)
        {
            cur = cur->leftChild;
        }
        else
        {
            if (cur->rightThread)
                break;
            cur = cur->rightChild;
        }
    }

    rTBSTNodePtr temp = getTreeNode(data);
    // Successfully created the node then only update the Tree
    if (temp)
    {
        if (data < prev->key)
        {
            prev->leftChild = temp;
            temp->rightChild = prev;
        }
        else
        {
            // Update the right thread
            temp->rightChild = prev->rightChild;
            prev->rightChild = temp;
            prev->rightThread = 0;
        }
        BSTInsertHelper();
        return 0;
    }

    return -1; // Error while adding the new node
}

/**
 * @brief Helps in decresing the size if the element is deleted successfully
 */
void BSTDeleteHelper()
{
    rTBSTNodePtr temp;
    while (top != NULL)
    {
        temp = pop();
        temp->size = (temp->size) - 1;
    }
}

/**
 * @brief Delete the element from the BST
 * @param root - Root node of the Tree
 * @param data - The data to be deleted
 * @return int - 0 if success -1 otherwise
 */
int rtbstDelete(rTBSTNodePtr *root, int data)
{
    if ((*root) == NULL)
    {
        return -1; // The tree doesn't exist
    }

    // Find the node to be deleted
    rTBSTNodePtr cur = *root;
    rTBSTNodePtr prev = NULL;

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
            cur = cur->leftChild;
        }
        else
        {
            if (cur->rightThread)
            {
                cur = NULL;
                break;
            }
            cur = cur->rightChild;
        }
    }

    // The key doesn't exist
    if (cur == NULL)
    {
        flushStack();
        return -1; // The data doesn't exists in the tree;
    }

    // Case 0 : Delete Root
    if (!prev && (!cur->leftChild || !cur->rightChild))
    {
        // Root is the only element
        if (!cur->leftChild && !cur->rightChild)
        {
            *root = NULL;
        }
        // Root without left
        else if (!cur->leftChild)
        {
            *root = cur->rightChild;
        }
        // Root without right
        else
        {
            *root = cur->leftChild;
            rTBSTNodePtr predec = cur->leftChild;

            while (predec->rightChild && !predec->rightThread)
                predec = predec->rightChild;
            predec->rightChild = NULL;
        }
    }

    // Case 1 - If the leftChild side is null then assign the rightChild to the parent
    else if (!cur->leftChild)
    {
        if (prev->leftChild == cur)
        {
            if (cur->rightThread)
            {
                prev->leftChild = NULL;
            }
            else
            {
                prev->leftChild = cur->rightChild;
            }
        }
        else
        {
            prev->rightChild = cur->rightChild;
            prev->rightThread = cur->rightThread;
        }
        free(cur);
    }

    // Case 2 - If rightChild is null then assign leftChild to the parent
    else if (cur->rightThread)
    {
        if (prev->leftChild == cur)
        {
            prev->leftChild = cur->leftChild;
            rTBSTNodePtr pred = cur->leftChild;
            while (pred->rightChild && !pred->rightThread)
            {
                pred = pred->rightChild;
            }
            pred->rightChild = prev;
        }
        else
        {
            prev->rightChild = cur->rightChild;
            prev->rightThread = cur->rightThread;
        }
        free(cur);
    }

    // Case 3 - If both are not null - Then find inorder successor and assign that in place of that
    else
    {
        // find inorder successor
        rTBSTNodePtr p = NULL;
        rTBSTNodePtr c = cur->rightChild;

        while (c->leftChild)
        {
            push(c);
            p = c;
            c = c->leftChild;
        }

        if (!p)
        {
            cur->rightChild = c->rightChild;
            cur->rightThread = c->rightThread;
        }
        else
        {
            if (c->rightThread)
                p->leftChild = NULL;
            else
                p->leftChild = c->rightChild;
        }

        cur->key = c->key;

        free(c);
    }

    // Helps in adjustment of the size
    BSTDeleteHelper();
    return 0;
}

/**
 * @brief Find the kth smallest element in O(h) time complexity
 * @param root - Root node of the tree
 * @param k - The number of element
 * @return rTBSTNodePtr - The node with the element
 */
rTBSTNodePtr kthElement(rTBSTNodePtr *root, int k)
{
    if (*root == NULL)
    {
        return NULL; // The tree is empty
    }
    int noOfNodes = (*root)->size;

    if (k < 1 || k > noOfNodes)
    {
        printf("The requested element is out of the range of the maximum tree nodes.");
        return NULL;
    }

    int lcount;
    rTBSTNodePtr ptr = *root;

    while (k > 0)
    {
        // Calculates the left count
        lcount = ptr->leftChild ? ptr->leftChild->size : 0;

        if (lcount + 1 == k)
            return ptr;
        if (lcount + 1 > k)
            ptr = ptr->leftChild;
        else
        {
            ptr = ptr->rightChild;
            k -= lcount + 1;
        }
    }

    return ptr;
}

int main(int argc, char **argv)
{
    rTBSTNodePtr root;
    crateRightThreadedBST(&root);

    int choice;
    printf("================================================================");
    printf("\n1 - Insert");
    printf("\n2 - Delete");
    printf("\n3 - Display");
    printf("\n4 - Search");
    printf("\n5 - Kth Element");
    printf("\n6 - Print Inorder");
    printf("\n-1 - Exit");

    int data;
    int *arr;

    while (1)
    {
        printf("\n\nEnter the choice from the above list: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            /* code */
            printf("Enter data to be inserted: ");
            scanf("%d", &data);
            rtbstInsert(&root, data);
            break;

        case 2:
            /* code */
            printf("Enter data to be deleted: ");
            scanf("%d", &data);
            rtbstDelete(&root, data);
            break;

        case 3:
            displayRightThreadedBST(root, "ThreadedTree.gv");
            system("dot -Tpng ThreadedTree.gv -o ThreadedTree.png");
            break;

        case 4:
            printf("Enter key for searching: ");
            scanf("%d", &data);
            searchHelper(&root, data);
            break;

        case 5:
            printf("Enter k for kth smalled element: ");
            scanf("%d", &data);
            rTBSTNodePtr res = kthElement(&root, data);
            if (res)
            {
                printf("Kth element is %d", res->key);
            }
            break;

        case 6:
            arr = (int *)malloc(sizeof(int) * (root)->size);
            inorder(root, arr);
            printInorder(root, arr);
            break;

        case -1:
            break;

        default:
            break;
        }
        if (choice == -1)
            break;
    }
    return 0;
}