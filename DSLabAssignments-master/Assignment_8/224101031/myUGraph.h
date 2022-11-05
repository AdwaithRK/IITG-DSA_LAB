#ifndef MYUGRAPH_H_
#define MYUGRAPH_H_

struct edgeListNode{
    int vertex;
    int edgeType; // 1 tree, 2 forward
    struct edgeListNode *linkPtr;
};
typedef struct edgeListNode edgeListNode;
typedef struct edgeListNode * edgeListNodePtr;

struct adjListEntry{
    int dfsNum;
    int lowNum;
    int bfsLevelNum; //level number assigned by bfs
    edgeListNodePtr edgeListHeadPtr;
};

typedef struct adjListEntry adjListEntry;

struct myUGraph{
    int numOfVertices; // Number of vertices
    int numOfEdges;  // Number of edges
    int numOfComp;  // Number of connected components
    adjListEntry *adjList; // Array
};

typedef struct myUGraph myUGraph;


//Allocate space for vertex array and initialize
void createUGraph(myUGraph *G, int numOfVertices);
void readUGraph(char *fileName, myUGraph *G);
// Add an undirected edge u -- v in G
void addUEdge(myUGraph *G, int u, int v);
void printAdjList(myUGraph *G);
void printUGraph(myUGraph *G, char *opFileName); // Visual output -- marks [10]
void destroyUGraph(myUGraph *G);
// Non-recursive DFS routine starting at vertex v. It also calculated the low number for each vertex
void DFS(myUGraph *G, int v);  //marks 20 + 10 (for computing low number)
void BFS(myUGraph *G, int v); // marks 20
// Finds whether graph has an odd cycle, if yes find one -- returns 0 if there is no cycle
// otherwise returns the length of cycle and cycle is stored in cycle
int checkOddCycle(myUGraph *G, int *cycle); // marks 30



#endif
