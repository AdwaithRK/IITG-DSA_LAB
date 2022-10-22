// A C program to demonstrate linked list based
// implementation of queue
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Node **adjacency_list = NULL;

struct Node
{
    int value;
    struct Node *next;
};

// A linked list (LL) node to store a queue entry
struct QNode
{
    int key;
    struct QNode *next;
};

// The queue, front stores the front node of LL and rear
// stores the last node of LL
struct Queue
{
    struct QNode *front, *rear;
};

// A utility function to create a new linked list node.
struct QNode *newNode(int k)
{
    struct QNode *temp = (struct QNode *)malloc(sizeof(struct QNode));
    temp->key = k;
    temp->next = NULL;
    return temp;
}

// A utility function to create an empty queue
struct Queue *createQueue()
{
    struct Queue *q = (struct Queue *)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

// The function to add a key k to q
void enQueue(struct Queue *q, int k)
{
    // Create a new LL node
    struct QNode *temp = newNode(k);

    // If queue is empty, then new node is front and rear
    // both
    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
        return;
    }

    // Add the new node at the end of queue and change rear
    q->rear->next = temp;
    q->rear = temp;
}

// Function to remove a key from given queue q
void deQueue(struct Queue *q)
{
    // If queue is empty, return NULL.
    if (q->front == NULL)
        return;

    // Store previous front and move front one node ahead
    struct QNode *temp = q->front;

    q->front = q->front->next;

    // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL)
        q->rear = NULL;

    free(temp);
}

int isEmpty(struct Queue *q)
{
    if (q->front == NULL)
        return 1;

    return 0;
}

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

void BFS(int *visited, int *predecessor, int *distance, int total_number_nodes)
{
    for (int i = 0; i < total_number_nodes; i++)
    {

        visited[i] = -1;
        predecessor[i] = -1;
        distance[i] = INT_MAX;
    }

    visited[0] = 0;
    predecessor[0] = -1;
    distance[0] = 0;
    struct Queue *q = createQueue();
    enQueue(q, 0);
    printf("visited: %d\n", 0);
    struct Node *temp_ele;

    while (!isEmpty(q))
    {
        int front_key = q->front->key;
        deQueue(q);
        temp_ele = adjacency_list[front_key];

        while (temp_ele != NULL)
        {
            if (visited[temp_ele->value] == -1)
            {
                printf("visited: %d\n", temp_ele->value);
                visited[temp_ele->value] = 0;
                predecessor[temp_ele->value] = front_key;
                enQueue(q, temp_ele->value);
            }

            temp_ele = temp_ele->next;
        }
        visited[front_key] = 1;
    }
}

// Driver Program to test above functions
int main()
{
    // struct Queue *q = createQueue();

    int total_number_nodes = create_adjacency_list();

    int *visited = (int *)malloc(total_number_nodes * sizeof(int));
    int *predecessor = (int *)malloc(total_number_nodes * sizeof(int));
    int *distance = (int *)malloc(total_number_nodes * sizeof(int));

    BFS(visited, predecessor, distance, total_number_nodes);
    // int *color  = (int *)malloc(total_number_nodes * sizeof(int));

    return 0;
}
