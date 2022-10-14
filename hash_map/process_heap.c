#include <stdlib.h>
#include <stdio.h>
// int heap_size = 9;

int process_ids[5] = {128, 456, 123, 78, 987};
int priority_ids[5] = {44, 99, 98, 89, 90};

struct process_node
{
    int process_id;
    int priority;
};

int left_child(int index)
{
    return 2 * index + 1;
}

int right_child(int index)
{
    return 2 * index + 2;
}

int parent(int index)
{
    return ((index - 1) / 2);
}

int max_heapify(struct process_node *heap, int index, int heap_size)
{

    int largest;
    struct process_node temp;

    if (index >= heap_size || left_child(index) >= heap_size || right_child(index) >= heap_size)
        return 0;

    if (heap[left_child(index)].priority > heap[index].priority)
        largest = left_child(index);
    else
        largest = index;

    if (heap[right_child(index)].priority > heap[largest].priority)
        largest = right_child(index);

    if (largest != index)
    {
        printf("\nThe largest is: %d \n", largest);
        temp = heap[largest];
        heap[largest] = heap[index];
        heap[index] = temp;

        max_heapify(heap, largest, heap_size);
    }
}

void max_heapify_reverse(int *heap, int index, int heap_size)
{
    if (index <= 0)
        return;

    if (heap[parent(index)] < heap[index])
    {
        int temp = heap[parent(index)];
        heap[parent(index)] = heap[index];
        heap[index] = temp;

        max_heapify_reverse(heap, parent(index), heap_size);
    }
}

void construct_heap(struct process_node *heap, int heap_size)
{
    for (int index = heap_size / 2; index >= 0; index--)
        max_heapify(heap, index, heap_size);
}

void print_heap(int *heap, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("\n %d \n", heap[i]);
    }
}

int main()
{
    // int heap[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    // construct_heap(heap, heap_size);
    // print_heap(heap, heap_size);

    // printf("\nAdding new element: %d\n", 10);

    // heap[heap_size] = 10;
    // heap_size++;
    // max_heapify_reverse(heap, 9, heap_size);
    // print_heap(heap, heap_size);

    // printf("\n deleting element %d \n", heap[0]);
    // heap[0] = heap[9];

    // max_heapify(heap, 0, --heap_size);
    // print_heap(heap, heap_size);

    // return 0;

    struct process_node *heap = (struct process_node *)malloc(10 * sizeof(struct process_node));
    int heap_size = 5;

    for (int i = 0; i < heap_size; i++)
    {
        printf("The process no %d values (process_id and priority) is:\n", i);
        heap[i].process_id = process_ids[i];
        heap[i].priority = priority_ids[i];
    }

    construct_heap(heap, heap_size);

    return 0;
}