// Name: Kaushal Mistry
// Roll No: 224101031
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// ======================= Stack Helper functoins using linked list ==========================
/**
 * @brief 
 * 
 */
struct Node
{
    int val;
    struct Node *next;
};

struct Node *top = NULL;

/**
 * @brief The push operation on the stack
 *
 * @param root - The TreeNode to push into
 * @return int - Returns 1 if any error or else 0
 */
int push(int val)
{
    struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
    if (temp == NULL)
    {
        return -1; // Unable to create a new node
    }

    temp->val = val;
    temp->next = top;
    top = temp;

    return 0; // Successful
}

/**
 * @brief Pop operation for stack
 * @return struct bstNode*
 */
int pop()
{
    if (top == NULL)
    {
        printf("Stack Underflow");
        return -1;
    }

    struct Node *tmp = top;

    int val = top->val;
    top = top->next;

    free(tmp);

    return val;
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
    while(!empty()) {
        pop();
    }
}



// Hash table
struct hashNode {
    char* cName;
    int value;
};

typedef struct hashNode hNode;

struct hashTable {
    hNode** items;
    int size;
};

typedef struct hashTable hTable;

hTable* createHashTable(int capacity) {
    hTable* table = (hTable*) malloc (sizeof(hTable));
    table->size = capacity;

    table->items = (hNode**) calloc (capacity, sizeof(hNode*));

    for (int i = 0; i < capacity; i++) {
        (table->items)[i] = NULL;
    }

    return table;
}

int power(int base, int power) {
    int res = 1;
    for (int i = 0; i < power; i++) {
        res *= base;
    }
    return res;
}

int hashFunction(char str[], hTable* table) {
    int p = 13;
    int value = 0;

    for (int i = 0; str[i]; i++) {
        value += (str[i]);
    }

    int pos = value % table->size;
    return pos;
}


hNode* createHashNode(char key[], int value) {
    hNode* temp = (hNode*) malloc(sizeof(hNode));
    char* s1 = (char*) malloc(sizeof(char)*15);
    strcpy(s1, key);
    temp->cName = s1;
    temp->value = value;

    return temp;

}


void insertIntoHashTable(char key[], int value, hTable* table) {
    int pos = hashFunction(key, table);

    hNode* newNode = createHashNode(key, value);

    // Found an empty slot
    if ((table->items)[pos] == NULL) {
        table->items[pos] = newNode;
    }

    // Collision: Resolution using Linear probing
    else {
        while (table->items[pos] != NULL)
        {
            pos = (pos+1) % table->size;
        }

        table->items[pos] = newNode;
    }
}


void printHashTable(hTable* table) {
    printf("\n-------------------\n");
    for (int i=0; i<table->size; i++) {
        if (table->items[i]) {
            printf("Index:%d, Key:%s, Value:%d", i, table->items[i]->cName, table->items[i]->value);
            printf("\n");
        }
    }
    printf("-------------------\n");
}


hNode* search(char key[], hTable* table) {
    int pos = hashFunction(key, table);
    
    // Key found
    if (!strcmp(table->items[pos]->cName, key)) {
        return table->items[pos];
    } 
    // It is not found at its hashed position
    else {
        while (strcmp(table->items[pos]->cName, key)) {
            pos = (pos+1) % table->size;
        }

        return table->items[pos];
    }

    return NULL;
}

int** createAdjancencyMatrix(int n) {
    int** adjMat = (int**) malloc (sizeof(int*));

    for (int i = 0; i < n; i++) {
        adjMat[i] = (int*) calloc (n, sizeof(int));
    }
}


struct ListNode {
    char* key;
    int value;
    struct ListNode* next;
};

typedef struct ListNode listNode;

struct Array {
    char* key;
    int value;
    listNode* next;
};

typedef struct Array graph;

graph* creaeGraphNode(hNode* hnode) {
    graph* newGraphNode = (graph*) malloc (sizeof(graph));
    newGraphNode->key = hnode->cName;
    newGraphNode->value = hnode->value;
    newGraphNode->next = NULL;
}

listNode* createListNode(hNode* hnode) {
    listNode* newGraphNode = (listNode*) malloc (sizeof(listNode));
    newGraphNode->key = hnode->cName;
    newGraphNode->value = hnode->value;
    newGraphNode->next = NULL;
}

graph** createAdjacencyList(int* inDeg, char* fileName, hTable* table) {
    graph** adjList = (graph**) malloc(sizeof(graph*) * table->size);

    for (int i = 0; i < table->size; i++) {
        adjList[i] = NULL;
    }

    FILE* fptr = fopen(fileName, "r");

    int n;
    fscanf(fptr, "%d", &n);

    hNode* tmp;
    char type[20];
    char course[20];
    int deg = 0;
    char curCourse[20] = "";
    fscanf(fptr, "%s %s", type, curCourse);
    while (!feof(fptr)) {
        fscanf(fptr, "%s %s", type, course);
        if (!strcmp(type, "CLASS")) {

            tmp = search(curCourse, table);

            if (adjList[tmp->value - 1] == NULL)
                adjList[tmp->value - 1] = creaeGraphNode(tmp);

            inDeg[tmp->value - 1] = deg;
            strcpy(curCourse, course);
            deg = 0;
        } else {
            tmp = search(course, table);


            if (adjList[tmp->value - 1] == NULL) {
                adjList[tmp->value - 1] = creaeGraphNode(tmp);
            } else {
                listNode* head = adjList[tmp->value - 1]->next;
                adjList[tmp->value - 1]->next = createListNode(search(curCourse, table));
                adjList[tmp->value - 1]->next->next = head;
            }
            
            deg++;
        }
    }

    tmp = search(curCourse, table);

    inDeg[tmp->value - 1] = deg;



    if (adjList[tmp->value - 1] == NULL)
        adjList[tmp->value - 1] = creaeGraphNode(tmp);

    return adjList;
}


/**
 * @brief Display the Tree graph
 * 
 * @param root - the root node of the tree
 * @param fileName - The file name 
 */
void displayGraph(graph** adjList, char *fileName, hTable* table)
{
    FILE *fptr;

    fptr = fopen(fileName, "w+");
    if (fptr == NULL)
    {
        printf("Error while opening the file with name %s", fileName);
        return;
    }
    fprintf(fptr, "digraph dep{\n");

    for (int i = 0; i < table->size; i++) {
        fprintf(fptr, "%s;\n", adjList[i]->key);

        listNode* tmp = adjList[i]->next;
        while (tmp) {
            fprintf(fptr, "%s -> %s;\n", adjList[i]->key, tmp->key);
            tmp = tmp->next;
        }
    }

    fprintf(fptr, "}\n");

    fclose(fptr);
}

void printAdjList(graph** adjList, hTable* table) {

    for (int i = 0; i < table->size; i++) {

        listNode* tmp = adjList[i]->next;
        while (tmp) {
            tmp = tmp->next;
        }
    }
}


void topoLogical(int* inDeg, graph** adjList, hTable* table) {
    int sem = 1;
    int cycleFlag = 0;
    while (!cycleFlag) {
        printf("\n\nThe following subjects can be taken in sem %d\n", sem);
        int foundZero = 0;
        int completeFlag = 0;
        


        for (int i = 0 ; i < table->size; i++) {
            if (inDeg[i] == 0) {
                foundZero = 1;
                inDeg[i] = -1;
                printf("Subject = %s\n", adjList[i]->key);
                push(i);
            } else if (inDeg[i] == -1) {
                completeFlag++;
            }
        }

        while (!empty())
        {
            int x = pop();
            listNode* tmp = adjList[x]->next;

            while(tmp) {
                inDeg[tmp->value - 1] -= 1;
                
                tmp = tmp->next;
            }
        }
        flushStack();
        

        if (!foundZero) {
            if (completeFlag < table->size)
                printf("Cycle found and can not move furthur");
            cycleFlag = 1;
        }
        sem++;
    }

    printf("Total sem = %d", sem - 1);

}


int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("The input file is not provided\n");
        return -1;
    }

    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Error in opening the input file");
        return -1;
    }

    // Number of courses
    int n;
    fscanf(fp, "%d", &n);


    hTable* table = createHashTable(n);


    char type[20];
    char course[20];

    int curKey = 1;

    while(!feof(fp)) {
        fscanf(fp, "%s %s", type, course);

        if (!strcmp(type, "CLASS")) {
            insertIntoHashTable(course, curKey, table);
            curKey++;
        }
    }

    fclose(fp);

    printHashTable(table);

    int* inDeg = (int*) calloc (n, sizeof(int));
    
    graph** adjacencyList = createAdjacencyList(inDeg, argv[1], table);


    printAdjList(adjacencyList, table);


    displayGraph(adjacencyList, "depGraph.gv", table);


    system("dot -Tpng depGraph.gv -o depGraph.png");

    topoLogical(inDeg, adjacencyList, table);

    return 0;
}