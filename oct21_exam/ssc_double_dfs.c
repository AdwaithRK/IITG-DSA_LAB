// A C program to demonstrate linked list based
// implementation of queue
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Node
{
    int value;
    struct Node *next;
};

struct Node **adjacency_list = NULL;
struct Node **adjacency_list_rev = NULL;

void attach_node(int index, int node_no, struct Node **adjacency_list)
{
    if (adjacency_list[index] == NULL)
    {
        adjacency_list[index] = (struct Node *)malloc(sizeof(struct Node));
        (*adjacency_list[index]).value = node_no;
    }
    else
    {
        struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
        temp->value = node_no;
        temp->next = adjacency_list[index];
        adjacency_list[index] = temp;
    }
    return;
}

int create_adjacency_list()
{
    int number_of_nodes;
    int start_node = 0, end_node = 0;
    FILE *ptr = fopen("input.txt", "r");

    fscanf(ptr, "%d", &number_of_nodes);
    printf("\nNumber of nodes %d\n", number_of_nodes);

    adjacency_list = (struct Node **)malloc(number_of_nodes * sizeof(struct Node *));

    while (fscanf(ptr, "%d<%d", &start_node, &end_node) != EOF)
    {
        printf("\nstart:%d, end: %d\n", start_node, end_node);
        attach_node(start_node, end_node, adjacency_list);
    }

    fclose(ptr);

    return number_of_nodes;
}

int create_rev_adjacency_list(int number_of_nodes)
{
    for (int i = 0; i < number_of_nodes; i++)
    {
        struct Node *temp = adjacency_list[i];
        while(temp != NULL){
            
        }
    }
}

// Driver Program to test above functions
int main()
{
    // struct Queue *q = createQueue();

    int total_number_nodes = create_adjacency_list();

    // int *visited = (int *)malloc(total_number_nodes * sizeof(int));
    // int *predecessor = (int *)malloc(total_number_nodes * sizeof(int));
    // int *distance = (int *)malloc(total_number_nodes * sizeof(int));

    // BFS(visited, predecessor, distance, total_number_nodes);
    // int *color  = (int *)malloc(total_number_nodes * sizeof(int));

    return 0;
}