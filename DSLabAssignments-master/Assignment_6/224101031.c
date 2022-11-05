// Name: Kaushal Mistry
// Roll: 224101031

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Swap function to swap two elements
 * @param a element 1
 * @param b element 2
 */
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Max heapify: it heapifies the array with replacing largest element at root
 * 
 * @param A - Arr
 * @param i - Index of the root
 * @param arrLen - total size of the array
 */
void maxHeapify(int *A, int i, int arrLen) {
    int largest = i; // assumes the root to be the largest
    int l = 2*i + 1; // Left child
    int r = 2*i + 2; // Right child

    if (l < arrLen && A[l] > A[largest]) { // If left child is larger than root then update largest
        largest = l;
    }
    if (r < arrLen && A[r] > A[largest]) { // If right child is larger than root then update largest
        largest = r;
    }

    if (largest != i) { // If root is not largest then replace
        swap(&A[i], &A[largest]);
        maxHeapify(A, largest, arrLen); // Again do the maxHeapify with the largest element
    }
}

// Sorts in ascending order
/**
 * @brief Heap sort to sort the element in the ascending order
 * 
 * @param A - Array
 * @param arrLen - length of the array
 */
void heapSort(int* A, int arrLen)
{
	// Build max heap
	for (int i = arrLen / 2 - 1; i >= 0; i--)
		maxHeapify(A, i, arrLen);

	// Heap sort
	for (int i = arrLen - 1; i >= 0; i--) {
		swap(&A[0], &A[i]); // placing the max element to its right place everytime
		maxHeapify(A, 0, i); // heapifies the heap to get the max element on the top again
	}
}

/**
 * @brief Prints the array
 * 
 * @param A - Array
 * @param arrLen - the length of the array
 */
void printArray(int *A, int arrLen) {
    for (int i = 0; i < arrLen; i++) {
        printf("%d  ", A[i]);
    }
    printf("\n\n");
}

/**
 * @brief Merges 2 arrays of size n1 and n2 in O(n) time
 * 
 * @param A - Array 1
 * @param n1 - size of the array 1
 * @param B - Array 2
 * @param n2 - size of the array 2
 * @return int* - Returns the resultant array of size (n1 + n2)
 */
int* merge2Arrays(int* A, int n1, int* B, int n2) {

    // Allocates the space for the resultant array
    int *C = (int *) malloc(sizeof(int) * (n1+n2));

    int l1 = 0, l2 = 0, i = 0;

    // Linearly traversing through two arrays till one of them exceeds the size
    while (l1 < n1 && l2 < n2) {
        // if element in A is smaller then add that to resultant 
        if (A[l1] < B[l2]) {
            C[i] = A[l1];
            l1++; // Advance the pointer of Array A

        } else { // If the element in B is smaller than the element in A
            C[i] = B[l2]; // Assign B's element in the resultant array 
            l2++; // Advance the pointer of B
        }
        i++;
    }

    // Check for the remaining element in A
    while (l1 < n1) {
        C[i] = A[l1];
        l1++;
        i++;
    }

    // Check for the remaining element in B
    while (l2 < n2) {
        C[i] = B[l2];
        l2++;
        i++;
    }
    
    return C;
}

/**
 * @brief Using heapsort we are sorting the elements in all the k arrays
 * 
 * @param lists - The list contaning k input array
 * @param k - no of input arrays
 * @param sizes - array of sizes of every input array
 */
void sortArrayLists(int** lists, int k, int* sizes) {
    // Uses Heap sort for sorting the array
    for (int i = 0; i < k; i++) {
        heapSort(lists[i], sizes[i]);
    }
}

/**
 * @brief Merges k sorted arrays into one
 * 
 * @param lists - The input arrays 
 * @param sizes - Sizes of input arrays
 * @param l - start index
 * @param r - end index
 * @return int** Array containg the merged array and also the size of the resultant array
 * The resultant array 
 *  res[0] will contain the size of the resultant array
 *  res[1] will have the actual resultant array
 */
int** mergeKSortedLists(int** lists, int* sizes, int l, int r) {

    // Base to check if limits exceeds or not
    if ( l > r) {
        return NULL;
    }


    // Left with two arrays so return the merge of them
    if (l == r-1) {
        // Allocate space
        int** res = (int**) malloc(sizeof(int*) * 2);
        res[0] = (int*) malloc(sizeof(int)*1); 

        res[1] = merge2Arrays(lists[l], sizes[l], lists[r], sizes[r]); // resultant merged array
        res[0][0] = sizes[l] + sizes[r]; // size
        return res;
    }

    if (l == r) { // Left with only one array
        int** res = (int**) malloc(sizeof(int*) * 2); // return that array
        res[0] = (int*) malloc(sizeof(int)*1);
        res[0][0] = sizes[l];
        res[1] = (int*) malloc(sizeof(int)*sizes[l]);
        res[1] = lists[l];
        return res;
    }

    
    int mid = (l + r) / 2; // Divide into two

    int** A1 = mergeKSortedLists(lists, sizes, l, mid); // Merge 1st part
    int** A2 = mergeKSortedLists(lists, sizes, mid+1, r); // Merge 2nd part


    // Merge resulant two parts
    int** res = (int**) malloc(sizeof(int*) * 2); 
    res[0] = (int*) malloc(sizeof(int)*1);
    res[0][0] = A1[0][0] + A2[0][0];

    res[1] = merge2Arrays(A1[1], A1[0][0], A2[1], A2[0][0]);

    return res;
    return NULL;

}


int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Not enough arguments.\n");
        return 0;
    }

    char* fileName = argv[1];

    // Reading file
    FILE* fptr = fopen(fileName, "r");

    int k;
    fscanf(fptr, "%d", &k);
    int* numbers = (int*) malloc(sizeof(int) * k);
    int totalCount = 0;

    for (int i = 0; i < k; i++) {
        fscanf(fptr, "%d", &numbers[i]);
        totalCount += numbers[i];
    }

    int **arrayList = (int**) malloc(sizeof(int*) * k);

    for (int j = 0; j < k; j++) {
        arrayList[j] = (int *) malloc(sizeof(int) * numbers[j]);

        for (int x = 0; x < numbers[j]; x++) {
            fscanf(fptr, "%d", &arrayList[j][x]);
        }
    }


    sortArrayLists(arrayList, k, numbers);

    int **result = mergeKSortedLists(arrayList, numbers, 0, k-1);


    printf("\nFinal result: \n");
    printArray(result[1], result[0][0]);


    return 0;
}
