#include <stdlib.h>
#include <stdio.h>
int time = 1;
struct Node
{
    int value;
    struct Node *next;
};

struct Node **adjacency_list;
int *in_time;
int *out_time;
int *visited;
int number_of_nodes;

void attach_node(int index, int node_no)
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

void DFS(int current_node_index)
{
    printf("%d\n", current_node_index);
    struct Node *temp;
    temp = adjacency_list[current_node_index];
    while (temp != NULL)
    {
        if (visited[temp->value] == -1)
        {
            in_time[temp->value] = time++;
            visited[temp->value] = 0;
            DFS(temp->value);
            visited[temp->value] = 1;
            out_time[temp->value] = time++;
        }
        temp = temp->next;
    }
    return;
}

void Initial_DFS()
{
    int component_count = 0;
    for (int i = 0; i < number_of_nodes; i++)
    {
        if (visited[i] == -1)
        {
            in_time[i] = time++;
            visited[i] = 0;
            printf("Component count now: %d \n", ++component_count);
            DFS(i);
            visited[i] = 1;
            out_time[i] = time++;
        }
    }
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
        attach_node(start_node, end_node);
    }

    fclose(ptr);

    return number_of_nodes;
}
int main()
{

    in_time = (int *)calloc(number_of_nodes, sizeof(int));
    out_time = (int *)calloc(number_of_nodes, sizeof(int));
    visited = (int *)calloc(number_of_nodes, sizeof(int));
    adjacency_list = (struct Node **)malloc(number_of_nodes * sizeof(struct Node *));

    create_adjacency_list();

    // Initial_DFS();

    return 0;
}