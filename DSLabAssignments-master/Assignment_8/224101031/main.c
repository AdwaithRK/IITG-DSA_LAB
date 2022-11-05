#include <stdio.h>
#include <stdlib.h>
#include "myStack.h"
#include "myQueue.h"
#include "myUGraph.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {

	myUGraph G;

	readUGraph("Test cases/tinyG.txt", &G);

	// printf("G->vertex = %d", );

	printUGraph(&G, "initialGraph.gv");

	system("dot -Tpng initialGraph.gv -o initialGraph.png");


	DFS(&G, G.numOfVertices);

	BFS(&G, G.numOfVertices);

	int* cycle = (int*) malloc (sizeof(int) * G.numOfVertices); 
	checkOddCycle(&G, cycle);

	destroyUGraph(&G);

	return 0;
}
