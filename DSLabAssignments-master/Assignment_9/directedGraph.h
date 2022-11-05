#ifndef DIRECTEDGRAPH_H_INCLUDED
#define DIRECTEDGRAPH_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUFSIZE 256

struct edgeListNode{
    int vertex;
    int edgeType; // 1 tree, 2 forward, 3 back, 4 cross
    struct edgeListNode *linkPtr;
};

typedef struct edgeListNode edgeListNode;
typedef struct edgeListNode * edgeListNodePtr;

struct adjListEntry{
    int dfsNum;
    int lowNum;
    int compNum; //Indicates which component the vertex belong
    edgeListNodePtr edgeListHeadPtr;
};

typedef struct adjListEntry adjListEntry;

struct myGraph{
    int numOfVertices; //Number of vertices
    int numOfEdges;  // Number of edges
    int numOfComp; // Number of strongly connected components
    adjListEntry *adjList; // Array
};

typedef struct myGraph myGraph;

//Allocate space for vertex array and initialize
void createGraph(myGraph *G, int numOfVertices);
// Add a directed edge u --> v in G
// Note: same edge can be inserted multiple times
void addEdge(myGraph *G, int u, int v);
// readDirectedGraph reads the directed graph from a text file with following format
// n -- number of vertices (0, 1, 2,..., n-1)
// 0 1 -- an edge 0 --> 1
// ...
// Note: input file should not contain any blank line (don't put enter after last line, it will take 0->0 as an edge)
//
// It reads the graph and creates an adjacency list
void readDirectedGraph(char *fileName, myGraph *G);
void printAdjList(myGraph *G);
void printGraph(myGraph *G, char *opFileName);
void deleteGraph(myGraph *G);
#endif // DIRECTEDGRAPH_H_INCLUDED
