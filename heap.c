#include <stdlib.h>
#include <stdio.h>
int heap_size = 9;

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

int max_heapify(int *heap, int index, int heap_size)
{

    int largest, temp;

    if (index >= heap_size || left_child(index) >= heap_size || right_child(index) >= heap_size)
        return 0;

    if (heap[left_child(index)] > heap[index])
        largest = left_child(index);
    else
        largest = index;

    if (heap[right_child(index)] > heap[largest])
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

void construct_heap(int *heap, int heap_size)
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
    int heap[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    construct_heap(heap, heap_size);
    print_heap(heap, heap_size);

    printf("\nAdding new element: %d\n", 10);

    heap[heap_size] = 10;
    heap_size++;
    max_heapify_reverse(heap, 9, heap_size);
    print_heap(heap, heap_size);

    printf("\n deleting element %d \n", heap[0]);
    heap[0] = heap[9];

    max_heapify(heap, 0, --heap_size);
    print_heap(heap, heap_size);

    return 0;
}