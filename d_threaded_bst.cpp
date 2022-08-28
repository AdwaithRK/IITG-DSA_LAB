#include <bits/stdc++.h>
using namespace std;

struct Node
{
    int data;
    Node *left, *right;

    // False if the left pointer points to its in-order predecessor
    bool leftThread;

    // False if the right pointer points to its in-order successor
    bool rightThread;
};

// Insertion of a Node in a Threaded Binary Tree
struct Node *insert(struct Node *root, int key)
{
    // Searching for a Node with given value
    Node *ptr = root;
    Node *parent = NULL; // Parent of key to be inserted
    while (ptr != NULL)
    {
        // If the key already exists, return it
        if (key == (ptr->data))
        {
            printf("Duplicate Key !\n");
            return root;
        }

        parent = ptr; // Update parent pointer

        // Moving on to the left subtree.
        if (key < ptr->data)
        {
            if (ptr->leftThread == false)
                ptr = ptr->left;
            else
                break;
        }

        // Moving on to the right subtree.
        else
        {
            if (ptr->rightThread == false)
                ptr = ptr->right;
            else
                break;
        }
    }

    // Create a new node
    Node *value = new Node;
    value->data = key;
    value->leftThread = true;
    value->rightThread = true;

    if (parent == NULL)
    {
        root = value;
        value->left = NULL;
        value->right = NULL;
    }
    else if (key < (parent->data))
    {
        value->left = parent->left;
        value->right = parent;
        parent->leftThread = false;
        parent->left = value;
    }
    else
    {
        value->left = parent;
        value->right = parent->right;
        parent->rightThread = false;
        parent->right = value;
    }

    return root;
}

// Here 'parent ' is a pointer referring to the parent node
// and 'current' is a pointer to the current node.

struct Node *case1(struct Node *root, struct Node *parent, struct Node *current)

{
    // If root node has to be deleted
    if (parent == NULL)
        root = NULL;

    // If the node to be deleted is left child of its parent
    else if (current == parent->left)
    {
        parent->leftThread = true;
        parent->left = current->left;
    }

    // If the node to be deleted is the right child of its parent
    else
    {
        parent->rightThread = true;
        parent->right = current->right;
    }

    // Now memory is freed and new root is returned
    free(current);
    return root;
}

// Here 'parent ' is a pointer referring to the parent node
// and 'current' is a pointer to the current node.

struct Node *case2(struct Node *root, struct Node *parent, struct Node *current)
{
    struct Node *child;

    // First initialize whether the child Node to be deleted has left child.
    if (current->leftThread == false)
        child = current->left;

    // or a right child
    else
        child = current->right;

    // If root node has to be deleted
    if (parent == NULL)
        root = child;

    // If the node to be deleted is left child of its parent
    else if (current == parent->left)
        parent->left = child;

    // If the node to be deleted is right child of its parent
    else
        parent->right = child;

    // Find successor and predecessor
    Node *s = inSucc(current);
    Node *p = inPred(current);

    // If current node has left subtree
    if (current->leftThread == false)
        p->right = s;

    // If current node has right subtree.
    else
    {
        if (current->rightThread == false)
            s->left = p;
    }

    // Now memory is freed and new root is returned
    free(current);
    return root;
}

// Here 'parent' is a pointer referring to the parent node
// and 'current' is a pointer to the current node
struct Node *case3(struct Node *root, struct Node *parent, struct Node *current)
{
    // Find the inorder successor
    struct Node *parsucc = current;
    struct Node *succ = current->right;

    // Find the leftmost child
    while (succ->leftThread == false)
    {
        parsucc = succ;
        succ = succ->left;
    }

    current->data = succ->data;

    if (succ->leftThread == true && succ->rightThread == true)
        root = case1(root, parsucc, succ);
    else
        root = case2(root, parsucc, succ);

    return root;
}

// For deleting the value from threaded BST with the given root
// and returning a new root of BST

struct Node *dThreadedBST(struct Node *root, int value)
{
    // Function for initializing the parent node to NULL and current node to root.
    struct Node *parent = NULL, *current = root;

    // Set true if value is found
    int found = 0;

    // Now, we will search value in BST
    while (current != NULL)
    {
        if (value == current->data)
        {
            found = 1;
            break;
        }
        parent = current;
        if (value < current->data)
        {
            if (current->leftThread == false)
                current = current->left;
            else
                break;
        }
        else
        {
            if (current->rightThread == false)
                current = current->right;
            else
                break;
        }
    }

    if (found == 0)
        printf("The value is not present in tree\n");

    // When the node to be deleted has two children
    else if (current->leftThread == false && current->rightThread == false)
        root = case3(root, parent, current);

    // When the node to be deleted has only left child
    else if (current->leftThread == false)
        root = case2(root, parent, current);

    // When the node to be deleted has only right child
    else if (current->rightThread == false)
        root = case2(root, parent, current);

    // When a leaf node needs to be deleted
    else
        root = case1(root, parent, current);

    return root;
}

// Returns the inorder successor using rightThread
struct Node *inorderSuccessor(struct Node *ptr)
{
    // If rightThread is set, we can quickly find
    if (ptr->rightThread == true)
        return ptr->right;

    // Else return leftmost child of right subtree
    ptr = ptr->right;
    while (ptr->leftThread == false)
        ptr = ptr->left;
    return ptr;
}

// Printing of the threaded tree
void inorder(struct Node *root)
{
    if (root == NULL)
        printf("Tree is empty");

    // In leftmost node
    struct Node *ptr = root;
    while (ptr->leftThread == false)
        ptr = ptr->left;

    // One by one print successors
    while (ptr != NULL)
    {
        cout << ptr->data << "  ";
        ptr = inorderSuccessor(ptr);
    }
}

// Driver Program
int main()
{
    struct Node *root = NULL;

    root = insert(root, 23);
    root = insert(root, 4);
    root = insert(root, 30);
    root = insert(root, 11);
    root = insert(root, 7);
    root = insert(root, 34);
    root = insert(root, 20);
    root = insert(root, 24);
    root = insert(root, 1);

    inorder(root);

    return 0;
}
