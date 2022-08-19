#include<stdio.h>
#include<stdlib.h>
#define MAX 100

struct Node{
    int data;
    struct Node *left;
    struct Node *right;
} *root;
typedef struct Node Node;

Node * stack[MAX];
int top = -1;

void push(Node *node){
    top++;
    stack[top] = node;
}

Node* pop(){
   if(top == -1) return NULL;
   return stack[top--];
}

Node* createNode(int data){
    Node *newNode = (Node *)malloc(sizeof(Node));

    newNode -> data = data;
    newNode -> left = NULL;
    newNode -> right = NULL;

    return newNode;
}

void inorder(Node *root){
    if(root == NULL) return;
    inorder(root -> left);
    printf("%d \t", root -> data);
    inorder(root -> right);
}

void inorderWithoutRecursion(Node *root){
    Node *curr = root;
    int done = 0;
    while(!done){
        if(curr) {
            push(curr);
            curr = curr -> left;
        }

        else{
            if(top == -1){
                done = 1;
            }
            else{
                curr = pop();
                printf("%d\t", curr -> data);
                curr = curr -> right;
            }
        }
    }

    printf("\n");
}

void preorderWithoutRecursion(Node *root){
    Node *curr = root;
    int done = 0;
    while(!done){
        if(curr) {
            printf("%d\t", curr -> data);
            push(curr);
            curr = curr -> left;
        }

        else{
            if(top == -1){
                done = 1;
            }
            else{
                curr = pop();
                curr = curr -> right;
            }
        }
    }
    printf("\n");
}

void postorderWithoutRecursion(Node *root){
    Node *curr = root, *next;
    int done = 0;
    while(!done){
        if(curr){
            if(curr -> right) push(curr -> right);
            push(curr);
            curr = curr -> left;
        }
        else{
            curr = pop();
            next = pop();
            if(curr-> right && curr -> right == next){
                push(curr);
                curr = curr -> right;
            }

            else{
                printf("%d\t", curr -> data);
                if(top != -1)
                    push(next);
                else{
                    done = 1;
                    printf("%d", next -> data);
                }
                curr = NULL;
            }
        }
    }
    printf("\n");
}




int main(){
    root = createNode(7);
    root -> left = createNode(6);
    root -> right = createNode(8);
    root -> right -> left = createNode(9);
    root -> right -> right = createNode(5);
    root -> right -> right -> left = createNode(4);    
    root -> left -> left = createNode(2);
    root -> left -> right = createNode(3);
    // inorderWithoutRecursion(root);
    // preorderWithoutRecursion(root);
    postorderWithoutRecursion(root);
    return 0;
}