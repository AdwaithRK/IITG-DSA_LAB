#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#define WHITE 0
#define GRAY 1
#define BLACK 2
#define TREE 1
#define FORWARD 2
#define BACK 3
#define CROSS 4

int min(int a, int b){
    return (a<b) ?  a : b;
}
// queue
struct queueNode{
    int data;
    struct queueNode* next;
};
typedef struct queueNode queueNode;
typedef struct queueNode* queueNodePtr;

struct queue{
    queueNodePtr front, rear;
};
typedef struct queue queue;
typedef struct queue* queuePtr;

queuePtr createQueue(){
    queuePtr qPtr = (queuePtr)malloc(sizeof(queue));
    qPtr->front = qPtr->rear = NULL;
    return qPtr;
}

queueNodePtr getQueueNode(int data){
    queueNodePtr p = (queueNodePtr) malloc(sizeof(queueNode));
    if(!p){
        fprintf(stderr, "getQueueNode(): malloc failed\n");
        exit(EXIT_FAILURE);
    }
    p->data = data;
    p->next = NULL;
    return p;
}

void enqueue(queuePtr q, int data){
    if(!(q->front)){
        q->front = q->rear = getQueueNode(data);
        return;
    }

    q->rear->next = getQueueNode(data);
    q->rear = q->rear->next;
}

int front(queuePtr q){
    if(q->front == 0)
        return -1;
    return q->front->data;
} 

void dequeue(queuePtr q){
    if(q->front == NULL)
        return;
    q->front = q->front->next;
    if(q->front == NULL)
        q->rear = NULL;
}

int isEmpty(queuePtr q){
    return (q->front == NULL) ? 1 : 0;
}

// linked list
struct llNode{
    int data;
    struct llNode* next;
};
typedef struct llNode llNode;
typedef llNode* llNodePtr;

llNodePtr getllNode(int data){
    llNodePtr p = (llNodePtr)malloc(sizeof(llNode));
    if(!p){
        fprintf(stderr, "getllNode(): malloc failed\n");
        exit(EXIT_FAILURE);
    }
    p->data = data;
    p->next = NULL;
    return p;
}

void insertAtFront(llNodePtr* head, int data){
    if((*head)==NULL){
//        printf("inserted");
        *head = getllNode(data);
        return;
    }
    llNodePtr p = getllNode(data);
    p->next = *head;
    *head = p;
}

void printll(llNodePtr head){
    while(head){
        printf("\n%d", head->data);
        head = head->next;
    }
    printf("\n");
}

//graph
struct adjNode{
    int vertex;
    int type;
    struct adjNode* next;
};

typedef struct adjNode adjNode;
typedef adjNode* adjNodePtr;

struct Graph{
    int n;
    adjNodePtr* adjList;
};
typedef struct Graph Graph;
typedef struct Graph* GraphPtr;

adjNodePtr getAdjNode(int vertex){
    adjNodePtr p = (adjNodePtr) malloc(sizeof(adjNode));
    if(!p){
        fprintf(stderr, "getAdjNode(): malloc failed\n");
        exit(EXIT_FAILURE);
    }
    p->vertex = vertex;
    p->type = 0;
    p->next = NULL;
    return p;
}

void insertEdge(GraphPtr g, int u, int v){
    if(g->adjList[u] == NULL)
        g->adjList[u] = getAdjNode(v);
    else{
        adjNodePtr newNode = getAdjNode(v);
        newNode->next = g->adjList[u];
        g->adjList[u] = newNode;
    }
}


GraphPtr createGraph(char* fileName){
    FILE* fp = fopen(fileName, "r");
    if(!fp){
        fprintf(stderr, "createGraph(): can't open file\n");
        exit(EXIT_FAILURE);
    }
    int n;
    fscanf(fp, "%d", &n);
    GraphPtr g = (GraphPtr) malloc(sizeof(Graph));
    if(!g){
        fprintf(stderr, "createGraph(): malloc failed\n");
        exit(EXIT_FAILURE);
    }

    g->n = n;
    g->adjList = (adjNodePtr*) malloc(sizeof(adjNodePtr)*n);
    for(int i = 0; i < n; i++)
        g->adjList[i] = NULL;
    
    while(!feof(fp)){
        int u, v;
        fscanf(fp, "%d", &u);
        fscanf(fp, "%d", &v);
        insertEdge(g, u, v);
        //insertEdge(g, v, u);
    }
    return g;
}

void displayGraph(GraphPtr g, char* fileName){
    FILE* fp = fopen(fileName, "w");
    if(!fp){
        fprintf(stderr, "displayGraph(): can't open file\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "digraph{\n");
    for(int i = 0; i < g->n; i++){
        adjNodePtr p = g->adjList[i];
        fprintf(fp, "%d ;\n", i);
        
        while(p){
            fprintf(fp, "%d -> %d ;\n", i, p->vertex);
            p = p->next;
        }
    }
    fprintf(fp, "}\n");
    fclose(fp);
}

struct BFSresult{
    int n;
    int* color;
    int* pred;
    int* dist;
    int* traversal;
};
typedef struct BFSresult BFSresult;

BFSresult* BFS(GraphPtr g, int s){
    int* color = (int*)malloc(g->n * sizeof(int));
    int* pred = (int*)malloc(g->n * sizeof(int));
    int* dist = (int*)malloc(g->n * sizeof(int));
    int* traversal = (int*)malloc(g->n * sizeof(int));

    color[s] = GRAY;
    dist[s] = 0;
    pred[s] = -1;

    for(int i = 0; i < g->n; i++){
        if(i != s){
            color[i] = WHITE;
            dist[i] = INT_MAX;
            pred[i] = -1; 
        }
    }

    int k = 0;
    queuePtr q = createQueue();
    enqueue(q, s);
    while(!isEmpty(q)){
        int u = front(q);
        dequeue(q);
        traversal[k++] = u;
        adjNodePtr ptr = g->adjList[u];
        while(ptr){
            int v = ptr->vertex;
            if(color[v] == WHITE){
                color[v] = GRAY;
                dist[v] = dist[u]+1;
                pred[v] = u;
                enqueue(q, v);
            }
            ptr = ptr->next;
        }
        color[u] = BLACK;
    }

    BFSresult* res = (BFSresult*) malloc(sizeof(BFSresult));
    res->n = g->n;
    res->pred = pred;
    res->dist = dist;
    res->traversal = traversal;
    res->color = color;
    return res;
}

struct DFSresult{
    int* d;
    int* f;
    int* pred;
    int cycleFound;
    llNodePtr topologicalSort;
};
typedef struct DFSresult DFSresult;
void DFS_Visit(GraphPtr g, int u, int* color, int* d, int* f, int* pred, int* time, int* cycleFound, llNodePtr* topSort, int* low, int print){
    if(print)    
        printf("%d ", u);
    *time = *time + 1;
    color[u] = GRAY;
    d[u] = low[u] = *time;

    adjNodePtr p = g->adjList[u];
    while(p){
        int v = p->vertex;
        if(color[v] == WHITE){
            p->type = TREE;
            pred[v] = u;
            DFS_Visit(g, v, color, d, f, pred, time, cycleFound, topSort, print);
            low[u] = min(low[u], low[v]);
            if (pred[i] != -1 && low[v] >= d[u])
                isAP[u] = true;
        }
        else if(color[v] == GRAY){
            p->type = BACK;
            *cycleFound = 1;
        }
        else{
            if(f[v] < d[u])
                p->type = CROSS;
            else
                p->type = FORWARD;
        }
        p = p->next;
    }
    color[u] = BLACK;
    insertAtFront(topSort, u);
    *time = *time + 1;
    f[u] = *time;
}

DFSresult* DFS(GraphPtr g, llNodePtr order){
    int* d = (int*)malloc(g->n * sizeof(int));
    int* f = (int*)malloc(g->n * sizeof(int));
    int* pred = (int*)malloc(g->n * sizeof(int));
    int* traversal = (int*)malloc(g->n * sizeof(int));
    int* color = (int*)malloc(g->n * sizeof(int));
    llNodePtr topSort = NULL;
    int* low = (int*)malloc(g->n * sizeof(int));

    for(int i = 0; i < g->n; i++){
        color[i] = WHITE;
        pred[i] = -1;
    }

    int* time = (int*) malloc(sizeof(int));
    int* cycleFound = (int*)malloc(sizeof(int));
    *cycleFound = 0;

    if(!order){
        for(int u = 0 ; u < g->n; u++){
            if(color[u] == WHITE)
                DFS_Visit(g, u, color, d, f, pred, time, cycleFound, &topSort, low,  0);
        }
    }
    else{
        while(order){
            if(color[order->data] == WHITE){
                DFS_Visit(g, order->data, color, d, f, pred, time, cycleFound, &topSort, low, 1);
                printf("\n");
            }
            //printf("%d ", order->data);
            order = order->next;
        }
    }

    DFSresult* res = (DFSresult*) malloc(sizeof(DFSresult));
    res->d = d;
    res->f = f;
    res->pred = pred;
    res->cycleFound = *cycleFound;
    res->topologicalSort = topSort;
    return res;
}

GraphPtr getTranspose(GraphPtr g){
    GraphPtr gt = (GraphPtr) malloc(sizeof(Graph));
    gt->n = g->n;
    gt->adjList = (adjNodePtr*) malloc(g->n * sizeof(adjNodePtr));
    int i;
    for(i = 0; i < g->n; i++){
        gt->adjList[i] = NULL;
    }

    int u;
    for(u = 0; u < g->n; u++){
        adjNodePtr p = g->adjList[u];
        while(p){
            insertEdge(gt, p->vertex, u);
            p = p->next;
        }
    }
    return gt;
}

void displayDFS(GraphPtr g, char* fileName){
    DFS(g, 0);
    FILE* fp = fopen(fileName, "w");
    if(!fp){
        fprintf(stderr, "displayDFS(): can't open file\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "digraph{\n");
    for(int i = 0; i < g->n; i++){
        adjNodePtr p = g->adjList[i];
        fprintf(fp, "%d ;\n", i);
        while(p){
            if(p->type == TREE)
                fprintf(fp, "%d -> %d ;\n", i, p->vertex);
            else    
                fprintf(fp, "%d -> %d [style=dotted] ;\n", i, p->vertex);
            p = p->next;
        }
    }
    fprintf(fp, "}\n");
    fclose(fp);
}




int* getLow(GraphPtr g, int* d){
    int* low = (int*)malloc(g->n * sizeof(int));
    for(int i = 0; i < n; i++){
        int minimum = INT_MAX;
        adjNodePtr p = g->adjList[i];
        while(p){
            minimum = min(minimum, d[p->vertex])
            p = p->next;
        }
    }
}
int main(int argc, char *argv[])
{
    if(argc < 2){
        fprintf(stderr, "provide ip file\n");
        exit(EXIT_FAILURE);
    }
    
    GraphPtr g = createGraph(argv[1]);
    displayGraph(g, argv[2]);

    BFSresult* res = BFS(g, 0);
    //DFSresult* res2 = DFS(g, NULL);
    GraphPtr gt = getTranspose(g);
    //printll(res2->topologicalSort);
    //displayGraph(gt, argv[3]);
    printf("%s\n", argv[3]);
    displayDFS(g, argv[3]);

    //SCC(g);

    return 0;
}
