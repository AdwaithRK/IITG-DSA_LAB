#include <stdio.h>
#include <stdlib.h>

struct node
{
    int vertex;
    struct node *next;
};

typedef struct node node;

struct graph{
    int numVt;
    node **adjList;
    int *color; // 0 => white, 1 => gray, 2 => black
    int *parent;
    int *d;
    int *f;
};

typedef struct graph graph;


node *createNode(int v){
    node *newNode = (node *)malloc(sizeof(node));
    if(!newNode) return NULL;
    newNode -> vertex = v;
    newNode -> next = NULL;
    return newNode;
}

void addEdge(graph *g, int src, int dest){
    node *n = createNode((dest));
    n -> next = g -> adjList[src];
    g -> adjList[src] = n;
}

graph *createGraph(int numVertex){
    graph *g = (graph *)malloc((sizeof(graph)));
    if(!g) return NULL;
    g -> numVt = numVertex;
    g -> adjList = (node **)malloc(sizeof(node) * numVertex);
    g -> color = (int *)malloc(sizeof(int) * numVertex);
    g -> parent = (int *)malloc(sizeof(int) * numVertex);
    g -> d = (int *)malloc(sizeof(int) * numVertex);
    g -> f = (int *)malloc(sizeof(int) * numVertex);
    for (int i = 0; i < numVertex; i++)
    {
        g -> adjList[i] = NULL;
        g -> color[i] = 0;
        g -> parent[i] = -1;
        g -> d[i] = 0;
        g -> f[i] = 0;
    }
    return g;
}

void dfsVisit(graph *g, int u, int *time){
    g -> d[u] = ++(*time);
    g -> color[u] = 1;
    node *t = g -> adjList[u];
    while(t){
        if(g -> color[t -> vertex] == 0){
            g -> parent[t -> vertex] = u;
            dfsVisit(g, t -> vertex, time);
        }
        t = t -> next;
    }
    g -> color[u] = 2;
    g -> f[u] = ++(*time);
}

int dfs(graph *g){
    int *time, t = 0;
    time = &t;
    for (int i = 0; i < g -> numVt ; i++)
    {
        if(g -> color[i] == 0){
            dfsVisit(g, i, time);
        }
    }
    return 1;
}

void printGraph(graph *g, char *filename){
    FILE *fp;
    fp = fopen(filename, "w+");
    if (!fp)
    {
        printf("Unable to open the file\n");
        return;
    }

    fprintf(fp, "digraph g{\n");
    for (int i = 0; i < g -> numVt; i++)
    {   
        node *t = g -> adjList[i];
        while(t){
            fprintf(fp, "%d -> %d;\n", i, t -> vertex);
            t = t -> next;
        }
    }
    fprintf(fp, "}\n");
}

void printDFSTree(graph *g, char *filename){
    FILE *fp;
    fp = fopen(filename, "w+");
    if (!fp)
    {
        printf("Unable to open the file\n");
        return;
    }

    fprintf(fp, "digraph g{\n");
    for (int i = 0; i < g -> numVt; i++)
    {   
        if(g -> parent[i] != -1)
            fprintf(fp, "%d -> %d;\n", g -> parent[i], i);
        fprintf(fp, "%d [label = \" %d, %d, %d\"];\n", i, i, g -> d[i], g -> f[i]);
    }
    fprintf(fp, "}\n");
}

int main(int argc, char const *argv[])
{
    graph *g = createGraph(5);
    addEdge(g, 0, 1);
    addEdge(g, 0, 3);
    addEdge(g, 0, 2);
    addEdge(g, 2, 4);
    addEdge(g, 4, 2);
    printGraph(g, "graph.dot");

    dfs(g);
    printDFSTree(g, "dfs.dot");
    return 0;
}
