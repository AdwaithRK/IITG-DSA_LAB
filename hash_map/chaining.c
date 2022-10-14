#include <stdio.h>
#include <stdlib.h>
#define size 10

struct node
{
    int data;
    struct node *next_node;
};

int hashing_fn(int value)
{
    return value % size;
}

void initialize_table(struct node ***table)
{
    *table = (struct node **)malloc(size * sizeof(struct node *));
    for (int i = 0; i < size; i++)
    {
        table[i] = NULL;
    }
}

struct node *create_node(int data)
{
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    if (new_node == NULL)
    {
        printf("\nMemory cannot be allocated\n");
        return NULL;
    }
    new_node->data = data;
    new_node->next_node = NULL;
    return new_node;
}

void insert_node(struct node **table, int value)
{
    int key = hashing_fn(value);

    if (table[key] == NULL)
    {
        table[key] = create_node(value);
    }
    else
    {
        struct node *temp = table[key];
        while (temp->next_node)
        {
            temp = temp->next_node;
        }

        temp->next_node = create_node(value);
    }
}

void print_hash_table(struct node **table)
{

    for (int i = 0; i < size; i++)
    {
        struct node *temp = table[i];
        printf("table[%d]-->", i);
        while (temp)
        {
            printf("%d -->", temp->data);
            temp = temp->next_node;
        }
        printf("NULL\n");
    }
}

int search_table(int value, struct node **table)
{
    struct node *temp = table[hashing_fn(value)];
    while (temp)
    {
        if (temp->data == value)
            return 1;
        temp = temp->next_node;
    }
    return 0;
}

int delete_node(int value, struct node **table)
{
    struct node *temp = table[hashing_fn(value)], *dealloc;
    if (temp != NULL)
    {
        if (temp->data == value)
        {
            dealloc = temp;
            temp = temp->next_node;
            free(dealloc);
            return 1;
        }
        else
        {
            while (temp->next_node)
            {
                if (temp->next_node->data == value)
                {
                    dealloc = temp->next_node;
                    temp->next_node = temp->next_node->next_node;
                    free(dealloc);
                    return 1;
                }
                temp = temp->next_node;
            }
        }
    }

    return 0;
}

int main()
{
    struct node **table;
    // initialize_table(&table);
    table = (struct node **)malloc(size * sizeof(struct node *));
    for (int i = 0; i < size; i++)
    {
        table[i] = NULL;
    }

    for (int i = 0; i < 50; i++)
    {
        insert_node(table, i);
    }

    print_hash_table(table);

    if (search_table(48, table))
        printf("48 - it is found!!\n");

    if (search_table(30, table))
        printf("30 - it is found!!\n");

    if (delete_node(30, table))
        printf("30- deleted\n");

    print_hash_table(table);

    if (!search_table(30, table))
        printf("30 - it is not found!!\n");

    return 0;
}