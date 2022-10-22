#include <stdlib.h>
#include <stdio.h>

struct Node **adjacency_list = NULL;
int *predecessor = NULL;

struct vertex
{
    int color;
    int d;
    int f;
    int pi;
};

struct Node
{
    int value;
    struct Node *next;
};

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

void DFS(int current_node_index, struct Node **adjacency_list, int *visited, int *in_time, int *out_time, int *time, int previous)
{
    printf("%d\n", current_node_index);
    predecessor[current_node_index] = previous;
    struct Node *temp;
    temp = adjacency_list[current_node_index];
    while (temp != NULL)
    {
        if (visited[temp->value] == -1)
        {
            in_time[temp->value] = (*time)++;
            visited[temp->value] = 0;
            DFS(temp->value, adjacency_list, visited, in_time, out_time, time, current_node_index);
            visited[temp->value] = 1;
            out_time[temp->value] = (*time)++;
        }
        temp = temp->next;
    }
    return;
}

void Initial_DFS(int number_of_nodes, int *time, int *visited, int *in_time, int *out_time, struct Node **adjacency_list)
{
    int component_count = 0;

    for (int i = 0; i < number_of_nodes; i++)
    {
        if (visited[i] == -1)
        {
            in_time[i] = (*time)++;
            visited[i] = 0;
            printf("Component count now: %d \n", ++component_count);
            DFS(i, adjacency_list, visited, in_time, out_time, time, -1);
            visited[i] = 1;
            out_time[i] = (*time)++;
        }
    }
}

void printDFSTree(int number_of_nodes, int *in_time, int *out_time)
{
    FILE *fp = fopen("dfstree.dot", "w");
    fprintf(fp, "digraph{\n");
    for (int i = 0; i < number_of_nodes; i++)
    {
        fprintf(fp, "%d[label=\"vertex name:%d, d:%d f:%d\"] ;\n", i, i, in_time[i], out_time[i]);
    }
    for (int i = 0; i < number_of_nodes; i++)
    {
        if (predecessor[i] != -1)
        {
            fprintf(fp, "%d -> %d ;\n", predecessor[i], i);
        }
    }
    fprintf(fp, "}\n");
    fclose(fp);
}

int return_edge_type(int u, int v, int *in_time, int *out_time)
{
    if (in_time[u] < in_time[v] && in_time[v] < out_time[v] && out_time[v] < out_time[u])
        return 0; // tree edge or forward edge
    if (in_time[v] <= in_time[u] && in_time[u] < out_time[u] && out_time[u] <= out_time[v])
        return 1; // back edge
    if (in_time[v] < out_time[v] && out_time[v] < in_time[u] && in_time[u] < out_time[u])
        return 2; // cross edge
}

void edge_classification(int number_of_nodes, int *in_time, int *out_time)
{
    for (int i = 0; i < number_of_nodes; i++)
    {
        struct Node *temp = adjacency_list[i];
        while (temp != NULL)
        {
            int k = return_edge_type(i, temp->value, in_time, out_time);
            printf("%d -> %d", i, temp->value);
            if (k == 0)
            {
                printf("tree edge or forward edge\n");
            }
            else if (k == 1)
            {
                printf("back edge\n");
            }
            else if (k == 2)
            {
                printf("cross edge\n");
            }
            temp = temp->next;
        }
    }
}

int main()
{
    int time = 0;

    int total_number_nodes = create_adjacency_list();

    struct vertex *vertex_list = (struct vertex *)malloc(total_number_nodes * sizeof(struct vertex));

    int *visited = (int *)malloc(total_number_nodes * sizeof(int));

    for (int i = 0; i < total_number_nodes; i++)
        visited[i] = -1;

    int *in_time = (int *)calloc(total_number_nodes, sizeof(int));
    int *out_time = (int *)calloc(total_number_nodes, sizeof(int));
    predecessor = (int *)calloc(total_number_nodes, sizeof(int));

    Initial_DFS(total_number_nodes, &time, visited, in_time, out_time, adjacency_list);

    printDFSTree(total_number_nodes, in_time, out_time);

    system("dot -Tpng dfstree.dot -o dfstree.png");

    edge_classification(total_number_nodes, in_time, out_time);

    return 0;
}