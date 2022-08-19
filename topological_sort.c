#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int value;
    struct Node *next;
};

struct Node **adjacency_list;
int *indegree;
int *queue;
int front, rear;

void attach_node(int index, int node_no)
{
    if (adjacency_list[index - 1] == NULL)
    {
        adjacency_list[index - 1] = (struct Node *)malloc(sizeof(struct Node));
        (*adjacency_list[index - 1]).value = node_no - 1;
    }
    else
    {
        struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
        temp->value = node_no - 1;
        temp->next = adjacency_list[index - 1];
        adjacency_list[index - 1] = temp;
    }
    return;
}

int create_adjacency_list()
{
    int number_of_nodes;
    int start_node = 0, end_node = 0;
    int i = 0;
    FILE *ptr = fopen("input.txt", "r");

    fscanf(ptr, "%d", &number_of_nodes);
    printf("\nNumber of nodes %d\n", number_of_nodes);

    adjacency_list = (struct Node **)malloc(number_of_nodes * sizeof(struct Node *));
    queue = (int *)malloc(number_of_nodes * sizeof(int));
    indegree = (int *)calloc(number_of_nodes, sizeof(int));
    front = rear = -1;

    while (fscanf(ptr, "%d<%d", &start_node, &end_node) != EOF)
    {
        printf("\nstart:%d, end: %d\n", start_node, end_node);
        indegree[end_node - 1]++;
        attach_node(start_node, end_node);
    }

    fclose(ptr);
    printf("\n\n");

    while (adjacency_list && i < number_of_nodes)
    {
        printf("Node : %d, Indegree:%d Adjacents:", i + 1, indegree[i]);
        struct Node *temp = adjacency_list[i];
        while (temp)
        {
            printf("\t%d -> ", (*temp).value + 1);
            temp = temp->next;
        }
        printf("\tNULL\n");
        i++;
    }

    return number_of_nodes;
}

void topological_sort(int number_of_nodes)
{
    for (int i = 0; i < number_of_nodes; i++)
    {
        if (indegree[i] == 0)
        {
            if (front == -1 && rear == -1)
                front = rear = 0;
            else
                rear++;
            queue[rear] = indegree[i];
        }
    }

    while (front != -1 && rear != -1)
    {
        int current = queue[front];
        printf("\t%d\t", current + 1);
        if (front == rear)
            front = rear = -1;
        else
            front++;

        struct Node *temp = adjacency_list[current];
        while (temp != NULL)
        {
            int value = temp->value;
            indegree[value]--;
            if (indegree[value] == 0)
            {
                if (front == -1 && rear == -1)
                    front = rear = 0;
                else
                    rear++;
                queue[rear] = value;
            }
            temp = temp->next;
        }
    }
}

int main()
{
    int number_nodes = create_adjacency_list();
    topological_sort(number_nodes);
    return 0;
}
