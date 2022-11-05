#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "directedGraph.h"


//Allocate space for vertex array and initialize
void createGraph(myGraph *G, int numOfVertices)
{
    assert(numOfVertices > 0);
    G->numOfVertices = numOfVertices;
    G->numOfEdges = 0;
    G->numOfComp = -1;

    G->adjList = (adjListEntry *)malloc(numOfVertices*sizeof(adjListEntry));
    if (G->adjList == NULL)
    {
        printf("createGraph: malloc failure\n");
        exit(EXIT_FAILURE);
    }

    // Initilize
    for (int v = 0; v < numOfVertices; v++)
    {
        G->adjList[v].dfsNum = G->adjList[v].lowNum = G->adjList[v].compNum = -1;
        G->adjList[v].edgeListHeadPtr = NULL;
    }

}

// Add a directed edge u --> v in G
// Note: same edge can be inserted multiple times
void addEdge(myGraph *G, int u, int v)
{
    assert(u >= 0 && u < G->numOfVertices);
    assert(v >= 0 && v < G->numOfVertices);

    edgeListNodePtr edgeNodePtr;
    edgeNodePtr = (edgeListNodePtr)malloc(sizeof(edgeListNode));
    if (edgeNodePtr == NULL)
    {
        printf("addEdge(): malloc failure.\n");
        exit(EXIT_FAILURE);
    }

    //Add edge u --> v
    G->numOfEdges++;
    edgeNodePtr->vertex = v;
    edgeNodePtr->edgeType  = -1;
    // Insert node at the beginning of the edge list (G[u])
    edgeNodePtr->linkPtr = G->adjList[u].edgeListHeadPtr;
    G->adjList[u].edgeListHeadPtr = edgeNodePtr;
}


//--------------------------------------------------------------------------------------------------------------------------------
// readDirectedGraph reads the directed graph from a text file with following format
// n -- number of vertices (0, 1, 2,..., n-1)
// 0 1 -- an edge 0 --> 1
// ...
// Note: input file should not contain any blank line (don't put enter after last line, it will take 0->0 as an edge)
//
// It reads the graph and creates an adjacency list
void readDirectedGraph(char *fileName, myGraph *G)
{
    FILE *ipFilePtr;
    int i, u, v, n;
    char buf[BUFSIZE];

    ipFilePtr = fopen(fileName, "r+");
    if (ipFilePtr == NULL)
    {
        printf("readDirectedGraph(): File %s does not exist.\n", fileName);
        exit(EXIT_FAILURE);
    }
    // First line of the file contains the number of vertices in the directed graph
    fgets(buf, BUFSIZE, ipFilePtr);
    n = atoi(buf);

    createGraph(G, n);

    //Read edges
    while(fgets(buf, BUFSIZE, ipFilePtr))
    {
        sscanf(buf,"%d %d", &u, &v);

        // Add an edge (u --> v)
        addEdge(G, u, v);
    }
    fclose(ipFilePtr);

}

void printAdjList(myGraph *G)
{
    int v;
    edgeListNodePtr edgeNodePtr;

    for (v = 0; v < G->numOfVertices; v++)
    {
        printf("[%d] : ", v);
        edgeNodePtr = G->adjList[v].edgeListHeadPtr;
        while(edgeNodePtr)
        {
            printf("%d -> ", edgeNodePtr->vertex);
            edgeNodePtr = edgeNodePtr->linkPtr;
        }
        printf("\n");
    }
}

void printGraph(myGraph *G, char *opDotFileName)
{
    FILE *opFilePtr;
    int v;
    
    edgeListNodePtr edgeNodePtr;

    opFilePtr = fopen(opDotFileName, "w");
    if (opFilePtr == NULL)
    {
        printf("printGraph(): can't create file %s\n", opDotFileName);
        exit(EXIT_FAILURE);
    }

    fprintf(opFilePtr, "digraph g{\n");

    for (v = 0; v < G->numOfVertices; v++)
    {
        fprintf(opFilePtr, "%d [label = \"%d \\n(%d,%d)\"]\n", v, v, G->adjList[v].dfsNum, G->adjList[v].lowNum);
        edgeNodePtr = G->adjList[v].edgeListHeadPtr;
        while(edgeNodePtr)
        {
            //printf("%d %d\n",i, edgeNodePtr->vertex);
            switch(edgeNodePtr->edgeType) // 1 tree, 2 forward, 3 back, 4 cross
            {
                case 1:
                    fprintf(opFilePtr, "%d -> %d [label = \"T\"]\n", v, edgeNodePtr->vertex);
                    break;
                case 2:
                    fprintf(opFilePtr, "%d -> %d [label = \"F\"]\n", v, edgeNodePtr->vertex);
                    break;
                case 3:
                    fprintf(opFilePtr, "%d -> %d [label = \"B\"]\n", v, edgeNodePtr->vertex);
                    break;
                case 4:
                    fprintf(opFilePtr, "%d -> %d [label = \"C\"]\n", v, edgeNodePtr->vertex);
                    break;
                default:
                    fprintf(opFilePtr, "%d -> %d \n", v, edgeNodePtr->vertex);
                    break;
            }

            edgeNodePtr = edgeNodePtr->linkPtr;
        }

    }

    fprintf(opFilePtr,"}\n");

    fclose(opFilePtr);
}

void deleteGraph(myGraph *G)
{
    int v;
    edgeListNodePtr edgeNodePtr, p;

    for (v = 0; v < G->numOfVertices; v++)
    {
        edgeNodePtr = G->adjList[v].edgeListHeadPtr;
        while(edgeNodePtr)
        {
            p = edgeNodePtr;
            edgeNodePtr = edgeNodePtr->linkPtr;
            free(p);
        }
    }
    free(G->adjList);
}
