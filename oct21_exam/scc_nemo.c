
#include <stdio.h>
#include <stdlib.h>

struct stackNode
{
    int data;
    struct stackNode *next;
};

typedef struct stackNode stackNode;

struct node
{
    int vertex;
    struct node *next;
};

typedef struct node node;

struct graph
{
    int numVt;
    node **adjList;
    int *color; // 0 => white, 1 => gray, 2 => black
    int *id;
    int *low;
    int *onStack;
};

typedef struct graph graph;

int min(int a, int b){
    return a > b? b: a;
}

stackNode *createStackNode(int val)
{
    return (stackNode *)malloc(sizeof(int));
}

void createStack(stackNode **head){
    *head = NULL;
}

int isEmpty(stackNode *head){
    if(!head) return 1;
    return 0;
}

int push(stackNode *head, int val)
{
    stackNode *newNode = createStackNode(val);
    if (!newNode)
        return -1;
    newNode->next = head;
    head = newNode;
    return 1;
}

int pop(stackNode *head)
{
    if (!head)
        return -1;
    stackNode *t = head;
    int vertex = t->data;
    head = t->next;
    free(t);
    return vertex;
}

node *createNode(int v)
{
    node *newNode = (node *)malloc(sizeof(node));
    if (!newNode)
        return NULL;
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

graph *createGraph(int numVertex)
{
    graph *g = (graph *)malloc((sizeof(graph)));
    if (!g)
        return NULL;
    g->numVt = numVertex;
    g->adjList = (node **)malloc(sizeof(node) * numVertex);
    g->color = (int *)malloc(sizeof(int) * numVertex);
    g->low = (int *)malloc(sizeof(int) * numVertex);
    g->id = (int *)malloc(sizeof(int) * numVertex);
    g -> onStack = (int *)malloc(sizeof(int) * numVertex);
    for (int i = 0; i < numVertex; i++)
    {
        g->adjList[i] = NULL;
        g->color[i] = 0;
        g->id[i] = -1;
        g->low[i] = -1;
        g -> onStack[i] = 0;
    }
    return g;
}

void addEdge(graph *g, int src, int dest){
    node *n = createNode((dest));
    n -> next = g -> adjList[src];
    g -> adjList[src] = n;
}

void dfs(graph *g, int currVertex, int *time, int *sscCount, stackNode *head){
    push(head, currVertex);
    g -> onStack[currVertex] = 1;
    g -> id[currVertex] = g -> low[currVertex] = (*time)++;

    node *t = g -> adjList[currVertex];
    int adjVertex;
    while(t){
        adjVertex = t -> vertex;
        if(g -> adjList[adjVertex]){
            dfs(g, adjVertex, time, sscCount, head);
        }
        if(g -> onStack[adjVertex]){
            g -> low[currVertex] = min(g -> low[currVertex], g -> low[adjVertex]);
        }
        t = t -> next;
    }
    int stackVertex;
    if(g -> id[currVertex] == g -> low[currVertex]){
        while (isEmpty(head))
        {
            stackVertex = pop(head);
            g -> onStack[stackVertex] = 0;
            g -> low[stackVertex] = g -> low[currVertex];
            if(stackVertex == currVertex) break;
        }
        (*sscCount)++;
    }
}

void findSCC(graph *g){
    int time = 0, sscCount = 0;
    int *t, *s;
    t = &time;
    s = &sscCount;
    stackNode *head;
    createStack(&head);
    for (int i = 0; i < g -> numVt ; i++)
    {
        if(g -> id[i] == -1){
            dfs(g, i, t, s, head);
        }
    }
}

int main(int argc, char const *argv[])
{
    
    return 0;
}
