#include <stdlib.h>
#include <stdio.h>

struct bstNode
{
    int key;
    int size;
    struct bstNode *leftChild;
    struct bstNode *rightChild;
};

int total_count = 0;

void displayTreeHelper(struct bstNode *T, FILE *fp)
{
    if (T != NULL)
    {
       // fprintf(fp, "%d[label=\"%d, %d\"];", T->key ,T->key, T->size);
        if(T->leftChild != NULL)
        {
            fprintf(fp, "%d -> %d [color = red, style=dotted];\n", T->key ,T->leftChild->key);
            displayTreeHelper(T->leftChild, fp);
        }
        if(T->rightChild != NULL)
        {
            fprintf(fp, "%d-> %d ;\n", T->key ,T->rightChild->key);
            displayTreeHelper(T->rightChild, fp);
        }
    }
}

int displayTree(struct bstNode *T, char* filename)
{
    FILE *fp;

    fp = fopen(filename, "w+");
    if (fp == NULL)
    {
        printf("displayTree(): Unable to open file %s", filename);
        return -1;
    }
    fprintf(fp, "digraph g{\n");
    displayTreeHelper(T, fp);
    fprintf(fp,"}\n");

    fclose(fp);

    return 0;
}



/*
    For inserting into a binary tree
    If the root is NULL create the bstNode and return
*/

struct bstNode* bstInsert(struct bstNode *root, int key)
{
    if (root == NULL)
    {
        struct bstNode *current = (struct bstNode *)malloc(sizeof(struct bstNode));
        current->key = key;
        current -> size = 0;
        current->leftChild = NULL;
        current->rightChild = NULL;
        return current;
    }

    (root -> size)++;

    if (root->key > key)
    {
        
        root->leftChild = bstInsert(root->leftChild, key);
    }
    else if (root->key < key)
    {
        root->rightChild = bstInsert(root->rightChild, key);
    }
    return root;
}

/*
    Inorder traversal of a tree using recursion
*/

void inorderTraversal(struct bstNode *root){
    if(root == NULL) return;

    inorderTraversal(root -> leftChild);
    total_count++;
    printf("%d\t", root->key);
    inorderTraversal(root -> rightChild);
}

/*
    Finding the Kth largest element using inorder traversal
*/

struct bstNode* kthElement(struct bstNode *root, int k)
{
    struct bstNode *stack[100];
    int top = -1;
    int kth_count = 0;

    struct bstNode *current = root;


    while(current != NULL || top != -1){
        while(current != NULL){
            stack[++top] = current;
            current = current -> leftChild;
        }

        current = stack[top--];
        if(++kth_count == (total_count - k + 1)) return current;

        current = current -> rightChild;
    }

    return NULL;
}



/*
    Search through the tree recursively using BST property to find the node
*/
struct bstNode* bstSearch(struct bstNode* root, int data)
{
    if(root == NULL) return root;
    if(root -> key == data){
        return root;
    }

    if(root -> key < data) return bstSearch(root -> rightChild, data);

    if(root -> key > data) return bstSearch(root -> leftChild, data);

}

/*
    Function to find inorder successor
*/

struct bstNode *find_inorder_successor(struct bstNode *current)
{
    if (current->rightChild != NULL)
    {
        current = current->rightChild;

        while (current->leftChild != NULL)
        {
            current = current->leftChild;
        }

        return current;
    }
    return NULL;
}


/*
    Attach parent to child
*/

void attach_child_to_parent(struct bstNode *parent, struct bstNode *current)
{
    struct bstNode *child;
    if (current->leftChild != NULL)
    {
        child = current->leftChild;
    }
    else
    {
        child = current->rightChild;
    }

    if (parent->leftChild == current)
    {
        parent->leftChild = child;
    }
    else
    {
        parent->rightChild = child;
    }
}


/*
    Remove leaf Node
*/
void remove_leaf(struct bstNode *parent, struct bstNode *current)
{
    if (parent->leftChild == current)
    {
        parent->leftChild = NULL;
    }
    else if (parent->rightChild == current)
    {
        parent->rightChild = NULL;
    }
}

/*
    Finding the Node along with it's parent for deletion
*/
struct bstNode *find_node(struct bstNode *root, int value, struct bstNode **parent)
{
    struct bstNode *current = root;
    (root -> size)--;

    while (current != NULL)
    {
        if (value == current->key)
        {
            return current;
        }
        *parent = current;
        (current -> size)--;
        if (current->key < value)
        {
            current = current-> rightChild;
        }
        else
        {
            current = current->leftChild;
        }
    }
    return NULL;
}

/*
    In case of deletion three cases have to be handled separately
        1. Leaf Node deletion
            -> make the pointer to the child NULL for the parent
            -> free the memory
        2. Deleting a node with a single child
            -> make the parent pointer point to the child
        3. Deleting a node with two children
            -> find the inorder successor
            -> copy it's value to the node
            -> delete the Inorder successor with the deletion logic(Inorder successor should be leaf or a node with one child)
*/
int bstDelete(struct bstNode* root, int value)
{
    struct bstNode* parent = NULL;
    struct bstNode* found = find_node(root, value, &parent);
    if (parent != NULL)
        printf("checking for parent %d", parent->key);

    if (found != NULL)
    {
        if (found->leftChild != NULL && found->rightChild != NULL)
        {
            struct bstNode* inorder_successor = find_inorder_successor(found);
            int temp = inorder_successor-> key;
            bstDelete(root, temp);
            found->key = temp;
        }
        else if (found->leftChild != NULL || found->rightChild != NULL)
        {
            printf("\nOnly one child - attach to parent!!\n");
            if(root -> key == value){
                printf("\nSpecial case root with one child is deleted\n");
                return 1;
            }
            attach_child_to_parent(parent, found);
        }
        else
        {
            printf("Leaf node!!");
            remove_leaf(parent, found);
            free(found);
        }
    }
    return 0;
}



int main()
{
    struct bstNode* root = NULL;
    int element,input;
    while(1){
        printf("\nEnter the option\n");
        scanf("%d", &input);
        switch (input)
        {
        case 1: 
            printf("\n\nEnter element\n\n");
            scanf("%d", &element);
            if(root == NULL){
                root = bstInsert(root, element);
            }else{
                bstInsert(root, element);
            }
        break;

        case 2:
            displayTree(root, "tree.dot");
            system("dot -Tpng tree.dot -o tree.png");
        break;

        case 3:
            printf("\nEnter the element to be deleted\n");
            scanf("%d", &element);
                if(bstDelete(root, element)){
                    if(root -> leftChild != NULL)
                        root = root -> leftChild;
                    else
                        root = root -> rightChild;
                }   
        break;
        
        case 4:
            total_count = 0;
            inorderTraversal(root);
            printf("\nFind the kth largest\n");
            scanf("%d", &element);
            printf("bow bow");
            struct bstNode *kthLargest = kthElement(root, element);
            if(kthLargest != NULL)
                printf("\n kth largest: %d\n", kthLargest -> key);
        break;

        case 5:
            printf("\nEnter element to find\n");
            scanf("%d", &element);
            if(bstSearch(root, element) != NULL){
                printf("!!Found!!");
            }else{
                printf("!!Not Found!!");
            }
        break;
        }
    }



    return 0;

}