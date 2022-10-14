#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LEFT_HEAVY 1
#define RIGHT_HEAVY -1
#define EVEN_HEAVY 0

/*

*/

struct bstNode {
    int key;
    int size;
    int balance;
    struct bstNode * left;
    struct bstNode * right;
};

typedef struct bstNode bstNode;
typedef struct bstNode* bstNodePtr;

void rightRotation(bstNodePtr * root, bool isDelete);
void leftRotation(bstNodePtr * root, bool isDelete);
void LR(bstNodePtr * root);
void RL(bstNodePtr * root);

void createBST(bstNodePtr * root) {
    *root = NULL;
}

/*
  function - Function that return pointer to newly created node with given key. 
  Params = key
*/
bstNodePtr getTreeNode(int key) {
    bstNodePtr newNode = (bstNodePtr) malloc(sizeof(bstNode));
    newNode->size = 1;
    newNode->balance = 0;
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}


/*
  function - Helper dunction that takes root pointer and file as input and generates tree by using Inorder. 
  Params = root, filepointer
*/
void generateGraph(bstNodePtr root, FILE * fp) {
    if(root == NULL) return;
    if(root->left) {
        fprintf(fp, "%d [label = \"%d, %d\"];\n", root->key, root->key, root->balance);
        fprintf(fp, "%d [label = \"%d, %d\"];\n", root->left->key, root->left->key, root->left->balance); 
        fprintf(fp, "%d -> %d [color = blue];\n", root->key, root->left->key);
        generateGraph(root->left, fp);
    }

    printf("[%d,%d] ", root->key, root->balance);
    
    if(root->right) {
        fprintf(fp, "%d [label = \"%d, %d\"];\n", root->key, root->key, root->balance);
        fprintf(fp, "%d [label = \"%d, %d\"];\n", root->right->key, root->right->key, root->right->balance);
        fprintf(fp, "%d -> %d [color = red];\n", root->key, root->right->key);
        generateGraph(root->right, fp);
    }
}

/*
  function - takes root pointer and file as input and generates tree by using Inorder. It stores output in png format
  Params = root, filepointer
*/
void displayBST(bstNodePtr root, char * filename) {
    FILE * fp = fopen(filename, "w");
    static int ind = 0;
    printf("Inorder - ");

    fprintf(fp, "digraph G {\n");
    generateGraph(root, fp);
    fprintf(fp, "}\n");

    printf("\n");

    fclose(fp);
    char cmd[50] = "dot -Tpng output.dot -o tree .png";
    cmd[28] = ind?'2':'1'; 
    printf("Cmd = %s\n", cmd);
    system(cmd);
    ind = ind^1; 
}


/*
  function - Function searches the given data in given tree. 
  Params = root, data
*/
bstNodePtr bstSearch(bstNodePtr root, int data) {
    if(root == NULL) return NULL;
    if(data < root->key) return bstSearch(root->left, data);
    if(data > root->key) return bstSearch(root->right, data);
    return root; 
}

/*
  function - Function that return min node in given tree. 
  Params = root
*/
bstNodePtr getMinNode(bstNodePtr root) {
    if(root == NULL) return NULL;

    bstNodePtr ptr = root;
    while(ptr->left) {
        ptr = ptr->left;
    }

    return ptr;
}


void rightRotation(bstNodePtr * root, bool isDelete) {
    
    bstNodePtr y = *root;
    bstNodePtr x = y->left;

    y->left = x->right;
    x->right = y;

    printf("Rotating - y = [%d,%d], x = [%d, %d]", y->key, y->balance, x->key, x->balance);
    

    if(isDelete && x->balance == 0) {
        y->balance = 1;
        x->balance = -1;
    }
    else {
        y->balance = 0;
        x->balance = 0;
    }

    *root = x;
}

void leftRotation(bstNodePtr * root, bool isDelete) {
    
    bstNodePtr y = *root;
    bstNodePtr x = y->right;

    y->right = x->left;
    x->left = y;

    if(isDelete && x->balance == 0) {
        y->balance = -1;
        x->balance = 1;
    }
    else {
        y->balance = 0;
        x->balance = 0;
    }

    *root = x;
}

void LR(bstNodePtr * root) {
    bstNodePtr z = *root;
    bstNodePtr y = z->left;
    bstNodePtr x = y->right;

    z->left = x->right;
    y->right = x->left;
    x->left = y;
    x->right = z;
    
    if(x->balance == 1) {
        y->balance = 0;
        x->balance = 0;
        z->balance = -1;
    }
    else if(x->balance == -1) {
        y->balance = 1;
        x->balance = 0;
        z->balance = 0;
    }
    else if(x->balance == 0) {
        y->balance = 0;
        x->balance = 0;
        z->balance = 0;
    }

    *root = x;
}

void RL(bstNodePtr * root) {
    bstNodePtr z = *root;
    bstNodePtr y = z->right;
    bstNodePtr x = y->left;

    z->right = x->left;
    y->left = x->right;
    x->left = z;
    x->right = y;
    
    if(x->balance == 1) {
        z->balance = 0;
        x->balance = 0;
        y->balance = -1;
    }
    else if(x->balance == -1) {
        z->balance = 1;
        x->balance = 0;
        y->balance = 0;
    }
    else if(x->balance == 0) {
        z->balance = 0;
        x->balance = 0;
        y->balance = 0;
    }

    *root = x;
}


void leftBalance(bstNodePtr* root) {
    if((*root)->left->balance == 1) {
        rightRotation(root, false);
    }
    else {
        LR(root);
    }
}

void rightBalance(bstNodePtr* root) {
    if((*root)->right->balance == -1) {
        leftRotation(root, false);
    }
    else {
        RL(root);
    }
}


/*
  function - Function that inserts given node in tree
  param  - root, data (to insert)
*/
int bstInsert(bstNodePtr * root, int data, bool* heightIncreased) {
    if(*root == NULL) {
        *root = getTreeNode(data);
        if(*root == NULL) return -1;
        return 1;
    }

    (*root)->size++;
    int rv;
    // Left Insertion
    if(data < (*root)->key) {
        
        if((*root)->left) {
            rv =  bstInsert(&(*root)->left, data, heightIncreased);
        }
        else {
            (*root)->left = getTreeNode(data);
            *heightIncreased = true;
            rv = 1;
        }
        if(*heightIncreased) {
            switch ((*root)->balance) {
                case LEFT_HEAVY: 
                    leftBalance(root);
                    *heightIncreased = false;
                    break;
                
                case RIGHT_HEAVY: 
                    (*root)->balance = EVEN_HEAVY;
                    *heightIncreased = false;
                    break;
                
                case EVEN_HEAVY: 
                    (*root)->balance = LEFT_HEAVY;
                    break;
                
                default:
                printf("INValid Balance factor\n");
                    break;
            }
        }
    }
    // Right Insertion
    else if(data > (*root)->key) {
        if((*root)->right) {
            rv = bstInsert(&(*root)->right, data, heightIncreased);
        }
        else {
            (*root)->right = getTreeNode(data);
            *heightIncreased = true;
            rv = 1;
        }
        if(*heightIncreased) {
            switch ((*root)->balance) {
                case LEFT_HEAVY: 
                    (*root)->balance = EVEN_HEAVY;
                    *heightIncreased = false;
                    break;
                
                case RIGHT_HEAVY: 
                    rightBalance(root);
                    *heightIncreased = false;
                    break;
                
                case EVEN_HEAVY: 
                    (*root)->balance = RIGHT_HEAVY;
                    break;
                
                default:
                    printf("INValid Balance factor\n");
                    break;
            }
        }
    } 
    else
        return -1;
    
    return rv;
}

void leftDeleteBalance(bstNodePtr* root) {
    if((*root)->right->balance != 1) {
        leftRotation(root, true);
    }
    else {
        RL(root);
    }
}

void rightDeleteBalance(bstNodePtr* root) {
    if((*root)->left->balance != -1) {
        printf("Del - Left Rotation\n");
        rightRotation(root, true);
    }
    else {
        LR(root);
        printf("LR roartion\n");
    }
}

/*
  function - Function that deletes given node in tree
  params - root, data to be deleted
*/
int bstDelete(bstNodePtr * root, int data, bool * heightDecreased) {
    if(*root == NULL) { return -1; }
    bstNodePtr node = *root;
    
    (*root)->size--;
    printf("Entered Delete\n");
    if(data < node->key) {
        printf("node - %d, left\n", node->key);
        bstDelete(&node->left, data, heightDecreased);   
        if(*heightDecreased) {
            switch ((*root)->balance) {
                case LEFT_HEAVY: 
                    (*root)->balance = EVEN_HEAVY;
                    break;
                
                case RIGHT_HEAVY: 
                    leftDeleteBalance(root);
                    break;
                
                case EVEN_HEAVY: 
                    (*root)->balance = RIGHT_HEAVY;
                    *heightDecreased = false;
                    break;
                
                default:
                printf("INValid Balance factor\n");
                    break;
            }
        } 
    }
    else if(data > node->key) {
        bstDelete(&node->right, data, heightDecreased);
        printf("node - %d, right\n", node->key);
        if(*heightDecreased) {
            switch ((*root)->balance) {
                case LEFT_HEAVY: 
                    rightDeleteBalance(root);
                    break;
                
                case RIGHT_HEAVY: 
                    (*root)->balance = EVEN_HEAVY;
                    break;
                
                case EVEN_HEAVY: 
                    (*root)->balance = LEFT_HEAVY;
                    *heightDecreased = false;
                    break;
                
                default:
                    printf("INValid Balance factor\n");
                    break;
            }
        }    
    }
    else {
        bstNodePtr temp = *root;
        if(node->left == NULL) {
            *root = node->right;
            *heightDecreased = true;
            free(temp);
            printf("Del - Left NULL\n");
        }
        else if(node->right == NULL) {
            *root = node->left;
            *heightDecreased = true;
            free(temp);
            printf("Del - Right NULL\n");
        } 
        else {
            bstNodePtr successor = getMinNode(node->right);
            node->key = successor->key;
            bstDelete(&node->right, successor->key, heightDecreased);
            printf("Del - No NULL\n");
            if(*heightDecreased ==  true) {
            // printf("node = %d, balance = %d\n", (*root)->key, (*root)->balance);
            switch ((*root)->balance) {
                case LEFT_HEAVY: 
                    rightDeleteBalance(root);
                    break;
                
                case RIGHT_HEAVY: 
                    (*root)->balance = EVEN_HEAVY;
                    break;
                
                case EVEN_HEAVY: 
                    (*root)->balance = LEFT_HEAVY;
                    *heightDecreased = false;
                    break;
                
                default:
                    printf("INValid Balance factor\n");
                    break;
            }
        }
        }
        
    }
    return 1;
}

/*
  function - Function that return kth min node in node in given tree
  params - root, k 
*/
bstNodePtr kthMinElement(bstNodePtr root, int k) {
    if(k==1) return getMinNode(root);

    if(root->left == NULL) {
        if(k == 1) return root;
        return kthMinElement(root->right, k-1);
    }
    else {
        if(k <= root->left->size) {
            return kthMinElement(root->left, k);
        }
        if(root->left && k > root->left->size) {
            // printf("node %d\n", root->key);
            int leftSize = root->left->size;
            if(k - leftSize == 1) return root;
            return kthMinElement(root->right, k - leftSize - 1);
        }
    }
}

/*
  function - Function that return kth maxn node in node in given tree.It use kthMIn as helper function
  params - root, k 
*/
bstNodePtr kthMaxElement(bstNodePtr root, int k) {
    if(k > root->size) return NULL;
    if(root == NULL) return NULL;
    k = root->size - k + 1;
    return kthMinElement(root, k);
}

/*
void kthMin(bstNodePtr root, int *k, int * ans) {
    if(root == NULL) return;
    kthMin(root->left, k, ans);
    (*k)--;
    // printf("k = %d\n", *k);
    if(*k == 0) {
        *ans = root->key; 
        return;
    }
    kthMin(root->right, k, ans);
}


void KthMax(bstNodePtr root, int *k, int * ans) {
    if(k > root->size) return;
    if(root == NULL) return;
    (*k) = root->size - *k + 1;
    kthMin(root, k, ans);
}
*/

int main(int argc, char const *argv[])
{
    bstNodePtr root;
    createBST(&root);
    /*
    // bstInsert(&root, 10);
    // bstInsert(&root, 3);
    // bstInsert(&root, 5);
    // bstInsert(&root, 1);
    // bstInsert(&root, 17);
    // bstInsert(&root, 19);
    // bstInsert(&root, 16);
    // bstInsert(&root, 2);
    */
    while(1) {
        printf("\n\nChoose operation:\n");
        printf("1. Insert\n");
        printf("2. Delete\n");
        printf("3. Print\n");
        printf("4. Search\n");
        printf("5. KthMax\n");
        printf("6. Press any other num for exit\n\n");
        int opt;
        scanf("%d", &opt);
        int flag = 0, data;
        int n, ans, k;
        bstNodePtr kthmin;
        bool hi = false, hd = false;
        switch (opt)
        {
            case 1:
                scanf("%d", &data);
                bstInsert(&root, data, &hi);
                break;
            case 2:
                scanf("%d", &data);
                bstDelete(&root, data, &hd);
                break;
            case 3:
                displayBST(root, "output.dot");
                break;
            case 4:
                scanf("%d", &data);
                bstNodePtr s = bstSearch(root, data);
                if(s) printf("\nNode found = %d", data);
                else printf("\nNode %d not found", data);
                break;
            case 5:
                scanf("%d", &k);
                kthmin = kthMaxElement(root, k);
                if(kthmin == NULL) printf("Invalid k - %d\n", k);
                else printf("%dth Max  = %d\n", k, kthmin->key);
                break;
            default:
                flag = 1;
                break;
        }

        if(flag == 1) break;

    }
    return 0;
}


