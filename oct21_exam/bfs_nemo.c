#include <stdio.h>
#include <stdlib.h>

struct queue
{
    int size;
    int f;
    int r;
    int *arr;
};

struct node
{
    int vertex;
    struct node *next;
};

typedef struct node node;

struct graph
{
    int numNodes;
    struct node **adjList;
    int *visited;
};

typedef struct queue queue;
typedef struct graph graph;

struct queue *createQueue(int size)
{
    struct queue *q = (queue*)malloc(sizeof(struct queue));
    q->f = -1;
    q->r = -1;
    q->arr = (int *)malloc(sizeof(int) * size);
    return q;
}

int isEmpty(queue *q)
{
    if (q->f == q->r)
        return 1;
    return 0;
}

int isFull(queue *q)
{
    if (q->r == q->size - 1)
        return 1;
    return 0;
}

void enqueue(queue *q, int val)
{
    if (isFull(q))
        printf("Queue Full");

    else
    {
        q->r++;
        q->arr[q->r] = val;
    }
}

int dequeue(queue *q)
{
    int a = -1;
    if (isEmpty(q))
        printf("Queue empty");
    else
    {
        q->f++;
        a = q->arr[q->f++];
    }
    return a;
}

graph * createGraph(int size)
{
    graph *g = (graph *)malloc(sizeof(graph));
    g->numNodes = size;
    g->adjList = (node **)malloc(sizeof(node) * size);
    g->visited = (int *)malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++)
    {
        g->adjList[i] = NULL;
        g->visited[i] = 0;
    }

    return g;
}

node *createNode(int v)
{
    node *n = (node *)malloc(sizeof(node *));
    n->vertex = v;
    n->next = NULL;
    return n;
}

void addEdge(graph *g, int src, int dest)
{
    node *n = createNode(dest);
    n->next = g->adjList[src];
    g->adjList[src] = n;

    n = createNode(src);
    n->next = g->adjList[dest];
    g->adjList[dest] = n;
}

void bfs(graph *g, int startVertex)
{
    queue *q = createQueue(6);
    g->visited[startVertex] = 1;
    enqueue(q, startVertex);

    while (!isEmpty(q))
    {
        int currVertex = dequeue(q);

        node *temp = g->adjList[currVertex];

        while (temp)
        {
            int adjVertex = temp->vertex;

            if (g->visited[adjVertex] == 0)
            {
                g->visited[adjVertex] = 1;
                enqueue(q, adjVertex);
            }

            temp = temp -> next;
        }
    }
}

int main()
{
    graph *graph = createGraph(6);
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 2);
    addEdge(graph, 1, 4);
    addEdge(graph, 1, 3);
    addEdge(graph, 2, 4);
    addEdge(graph, 3, 4);

    bfs(graph, 0);

    return 0;
}