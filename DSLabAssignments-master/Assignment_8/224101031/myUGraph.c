#include <stdio.h>
#include <stdlib.h>
#include "myUGraph.h"
#include "myStack.h"
#include "myQueue.h"
#define BUFSIZE 256

/**
 * @brief Creates the graph with the number of provided vertices
 *
 * @param G
 * @param numOfVertices
 */
void createUGraph(myUGraph *G, int numOfVertices)
{
    // G = (myUGraph*) malloc(sizeof(myUGraph));
    G->numOfVertices = numOfVertices;
    G->numOfEdges = 0;
    G->adjList = (adjListEntry *)malloc(sizeof(adjListEntry) * numOfVertices);

    for (int i = 0; i < numOfVertices; i++)
    {
        G->adjList[i].bfsLevelNum = 0;
        G->adjList[i].dfsNum = 0;
        G->adjList[i].lowNum = 0;
        G->adjList[i].edgeListHeadPtr = NULL;
    }
}

/**
 * @brief Reading the input from file and creates the graph
 *
 * @param fileName
 * @param G
 */
void readUGraph(char *fileName, myUGraph *G)
{
    FILE *ipFilePtr;
    int i, u, v, n;
    char buf[BUFSIZE];

    ipFilePtr = fopen(fileName, "r+");
    if (ipFilePtr == NULL)
    {
        printf("readUGraph(): File %s does not exist.\n", fileName);
        exit(EXIT_FAILURE);
    }
    // First line of the file contains the number of vertices in the undirected graph
    fgets(buf, BUFSIZE, ipFilePtr);
    n = atoi(buf);

    createUGraph(G, n);

    // Read edges
    while (fgets(buf, BUFSIZE, ipFilePtr))
    {
        sscanf(buf, "%d %d", &u, &v);
        addUEdge(G, u, v);
    }
    fclose(ipFilePtr);
}

/**
 * @brief Create a Edge Node object
 *
 * @param vertex
 * @return edgeListNodePtr
 */
edgeListNodePtr createEdgeNode(int vertex)
{
    edgeListNodePtr newEdgeNode = (edgeListNodePtr)malloc(sizeof(edgeListNode));
    if (newEdgeNode == NULL)
    {
        printf("Error while allocating memory for the new edge node.");
        exit(EXIT_FAILURE);
    }
    newEdgeNode->vertex = vertex;
    newEdgeNode->edgeType = 1;
    newEdgeNode->linkPtr = NULL;
}

/**
 * @brief Adds the edge between u and v in the graph the edges are added between u and v and v and u as it is undirected graph
 *
 * @param G
 * @param u
 * @param v
 */
void addUEdge(myUGraph *G, int u, int v)
{
    G->numOfEdges++; // Updating the num of edges

    // u -> v
    edgeListNodePtr newEdgeNode = createEdgeNode(v);
    newEdgeNode->linkPtr = G->adjList[u].edgeListHeadPtr;
    G->adjList[u].edgeListHeadPtr = newEdgeNode;
    
    // v - > u
    edgeListNodePtr newEdgeNode1 = createEdgeNode(u);
    newEdgeNode1->linkPtr = G->adjList[v].edgeListHeadPtr;
    G->adjList[v].edgeListHeadPtr = newEdgeNode1;
}

/**
 * @brief prints the adjacency list
 *
 * @param G
 */
void printAdjList(myUGraph *G)
{
}

/**
 * @brief Prints the graph using dot
 * 
 * @param G 
 * @param opFileName 
 */
void printUGraph(myUGraph *G, char *opFileName)
{
    FILE *fp = fopen(opFileName, "w");

    fprintf(fp, "digraph {\n");

    for (int i = 0; i < G->numOfVertices; i++)
    {
        // printf("%d\n", i);
        fprintf(fp, "%d [label = \"  %d \"];\n", i, i);

        edgeListNodePtr temp = G->adjList[i].edgeListHeadPtr;

        while (temp)
        {
            fprintf(fp, "\"%d\"->\"%d\"[label = %d];\n", i, temp->vertex, temp->edgeType);
            temp = temp->linkPtr;
        }
    }

    fprintf(fp, "}\n");
    fclose(fp);
}

/**
 * @brief Destroys the graph and its dependent
 * 
 * @param G 
 */
void destroyUGraph(myUGraph *G)
{
    for (int i = 0; i < G->numOfVertices; i++) {
        edgeListNodePtr cur = G->adjList[i].edgeListHeadPtr;
        edgeListNodePtr temp;

        while (cur) {
            temp = cur;
            cur = cur->linkPtr;
            free(temp);
        }
    }

}

// Finds min between two numbers
int min(int a, int b)
{
    return a < b ? a : b;
}

/**
 * @brief DFS traversal
 * 
 * @param G 
 * @param v 
 */
void DFS(myUGraph *G, int v)
{
    // visited array
    int *visited = (int *)malloc(sizeof(int) * v);

    for (int i = 0; i < v; i++)
    {
        visited[i] = 0; // initializing the array
    }

    // To keep the dfsNum and visited time
    static int time = 0, cnt = 0;
    myStack S;
    createStack(&S, v); // Creates the stack

    int vertex;
    for (int i = 0; i < v; i++) // Checks for disconnected components
    {   
        // Pushes the vertex not visited and adjancent to vertex i
        if (visited[i] == 0) {
            push(&S, i);
        }
        
        // Unitll the stack is empty visit the nodes and update the visited and low number
        while (S.top != -1)
        {
            pop(&S, &vertex);
            visited[vertex] = ++cnt;
            G->adjList[vertex].dfsNum = ++time; // Updating dfsNum
            G->adjList[vertex].lowNum = time; // Low number

            // Traverse its edges
            edgeListNodePtr cur = G->adjList[vertex].edgeListHeadPtr;
            while (cur)
            {
                if (visited[cur->vertex] == 0)
                {
                    cur->edgeType = 1; // Tree edge
                    push(&S, cur->vertex);
                }
                else
                {

                    cur->edgeType = 2; // Back edge
                    G->adjList[vertex].lowNum = min(G->adjList[vertex].lowNum, G->adjList[cur->vertex].lowNum);
                }
                cur = cur->linkPtr;
            }
        }
    }

    // Creates the graph after performing DFS
    FILE *fp = fopen("graphAfterDFS.gv", "w");
    fprintf(fp, "digraph {\n");
    for (int i = 0; i < G->numOfVertices; i++)
    {
        // printf("%d\n", i);
        fprintf(fp, "%d [label = \"  %d  %d/%d\"];\n", i, i, G->adjList[i].dfsNum, G->adjList[i].lowNum);

        edgeListNodePtr temp = G->adjList[i].edgeListHeadPtr;

        while (temp)
        {
            fprintf(fp, "\"%d\"->\"%d\"[label = %d];\n", i, temp->vertex, temp->edgeType);
            temp = temp->linkPtr;
        }
    }

    fprintf(fp, "}\n");

    fclose(fp);

    system("dot -Tpng graphAfterDFS.gv -o graphAfterDFS.png");
    destroyStack(&S);
}

/**
 * @brief BFS traversal
 * 
 * @param G 
 * @param v 
 */
void BFS(myUGraph *G, int v)
{
    myQueue Q; // Creating queue
    createQueue(&Q, v);
    myQueue Q2;
    createQueue(&Q2, v);
    static int level = 0;

    int* visit = (int*) calloc (v, sizeof(int));
    int data, vertex;

    // For disconnected components 
    for (int i = 0; i < v; i++) {
        if (visit[i] == 0)
            pushQueue(&Q2, i);
        level = 0;

        // Add the level to the nodes
        while (!isQueueEmpty(&Q2)) {
            level++;
            while (!isQueueEmpty(&Q2)) {
                deleteQueue(&Q2, &data);
                pushQueue(&Q, data);
            }
            while (!isQueueEmpty(&Q)) {
                deleteQueue(&Q, &vertex);
                visit[vertex] = 1;
                G->adjList[vertex].bfsLevelNum = level;

                edgeListNodePtr cur = G->adjList[vertex].edgeListHeadPtr;

                while (cur) {
                    if (visit[cur->vertex] == 0) {
                        pushQueue(&Q2, cur->vertex);
                        visit[cur->vertex] = 1;
                    }
                    cur = cur->linkPtr;
                }
            }
        }
    }

    // Creating the graph after performing DFS
    FILE *fp = fopen("graphAfterBFS.gv", "w");
    fprintf(fp, "digraph {\n");
    for (int i = 0; i < G->numOfVertices; i++)
    {
        // printf("%d\n", i);
        fprintf(fp, "%d [label = \"  %d  %d\"];\n", i, i, G->adjList[i].bfsLevelNum);

        edgeListNodePtr temp = G->adjList[i].edgeListHeadPtr;

        while (temp)
        {
            fprintf(fp, "\"%d\"->\"%d\";\n", i, temp->vertex);
            temp = temp->linkPtr;
        }
    }

    fprintf(fp, "}\n");

    fclose(fp);

    system("dot -Tpng graphAfterBFS.gv -o graphAfterBFS.png");

}

/**
 * @brief Checks the odd cycle in the graph using bfs
 * 
 * @param G 
 * @param cycle 
 * @return int 
 */
int checkOddCycle(myUGraph *G, int *cycle)
{
    int cycleLevel = -1; // the level at which the cycle occurred
    int v1, v2; // The nodes involved in the cycle
    for (int i = 0; i < G->numOfVertices; i++) {
        edgeListNodePtr cur = G->adjList[i].edgeListHeadPtr;
        while (cur) {
            if (G->adjList[cur->vertex].bfsLevelNum == G->adjList[i].bfsLevelNum) {
                printf("Cycle detected among %d, %d\n", i, cur->vertex);
                cycleLevel = G->adjList[i].bfsLevelNum;
                v1 = i;
                v2 = cur->vertex;
                break;
            }
            cur = cur->linkPtr;
        }
        if (cycleLevel != -1)
            break;
    }

    // If it contains the cycle. Find the nodes participating in the cycle
    if (cycleLevel != -1) {
        int cycleLen = 0;
        cycle[cycleLen++] = v1;
        cycle[cycleLen++] = v2;
        while (v1 != v2) {
            edgeListNodePtr cur = G->adjList[v1].edgeListHeadPtr;
            while (cur) {
                if (G->adjList[cur->vertex].bfsLevelNum == (G->adjList[v1].bfsLevelNum - 1)) {
                    v1 = cur->vertex;
                    break;
                }
                cur = cur->linkPtr;
            }

            edgeListNodePtr cur2 = G->adjList[v2].edgeListHeadPtr;
            while (cur2) {

                if (G->adjList[cur2->vertex].bfsLevelNum == (G->adjList[v2].bfsLevelNum - 1)) {
                    v2 = cur2->vertex;
                    break;
                }
                cur2 = cur2->linkPtr;
            }


            if (v1 == v2) {
                cycle[cycleLen++] = v1;
            } else {
                cycle[cycleLen++] = v1;
                cycle[cycleLen++] = v2;
            }
        }

        printf("\nCycle is as below among the vertex");
        for(int i = 0; i < cycleLen; i++) {
            printf("%d - ", cycle[i]);
        }
        printf("\n");
    }

    return 0;
}
