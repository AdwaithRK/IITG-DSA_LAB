#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int heap[100];
int heap_size = 0;
int capacity = 100;

int left(int i)
{
    return 2 * i + 1;
}

int right(int i)
{
    return 2 * i + 2;
}
int parent(int i)
{
    return (i - 1) / 2;
}

void min_heapify(int i)
{
    int l = left(i);
    int r = right(i);
    int temp;
    int smallest;

    if (l < heap_size && heap[i] > heap[l])
    {
        smallest = l;
    }
    else
    {
        smallest = i;
    }

    if (r < heap_size && heap[smallest] > heap[r])
    {
        smallest = r;
    }

    if (smallest != i)
    {
        temp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = temp;

        min_heapify(smallest);
    }
}

// Inserts a new key 'k'
void insertKey(int k)
{
    if (heap_size == capacity)
    {
        printf("\nOverflow: Could not insertKey\n");
        return;
    }

    // First insert the new key at the end
    heap_size++;
    int i = heap_size - 1;
    heap[i] = k;

    // Fix the min heap property if it is violated
    while (i != 0 && heap[parent(i)] > heap[i])
    {
        int temp = heap[i];
        heap[i] = heap[parent(i)];
        heap[parent(i)] = temp;
        i = parent(i);
    }
}

int main()
{
    insertKey(10);
    insertKey(19);
    insertKey(8);

    return 0;
}