#include <stdio.h>
#include <stdlib.h>

// Initializing the set
struct Set
{
    int value;
    struct Set *next;
};

// Initializing the head pointer to Null for the fresh Linked list
int makeSet(struct Set** SetA)
{
    *SetA = NULL;
}

/**
 * @brief Inserts an element in the Set
 * @param SetA - The Set
 * @param value - Value to be inserted
 * @return int 
 */
int Insert(struct Set **SetA, int value)
{
    struct Set *ptr = (*SetA); // traversing pointer

    int foundFlag = 0;
    // check whether the value is already in or not
    while (ptr != NULL)
    {
        if (ptr->value == value)
        {
            foundFlag = 1;
        }
        ptr = ptr->next;
    }

    // if value is already in the set
    if (foundFlag)
        return 0;

    // If value is not already in then create a node and set the value to specified value
    struct Set *newNode = (struct Set *)malloc(sizeof(struct Set));
    newNode->value = value;
    newNode->next = (*SetA);
    (*SetA) = newNode; // Chaning the head pointer to newly added node
    return 1;
}

/**
 * @brief Delete an element from the set if it is present
 * @param SetA - Set from which it needs to be deleted
 * @param value - Value to be deleted
 * @return int 
 */
int Delete(struct Set *SetA, int value)
{
    struct Set *curPtr = SetA; // Cur and prev pointer to traverse and delete the element
    struct Set *prevPtr = NULL; // 
    int foundFlag = 0;

    while (curPtr->next != NULL)
    {
        if (curPtr->value == value)
        {
            foundFlag = 1;
            break;
        }
        prevPtr = curPtr;
        curPtr = curPtr->next;
    }

    //If the value node found then delete
    if (foundFlag)
    {
        if (prevPtr == NULL)
        {
            SetA = curPtr->next;
        }
        else
        {
            prevPtr->next = curPtr->next;
        }
        free(curPtr); // Free the memory allocated
        return 1;
    }
    return 0;
}

/**
 * @brief Prints the set
 * 
 * @param SetA - The set to be printed
 * @return int 
 */
int printSet(struct Set *SetA)
{ // Print all the element of the SetA, where SetA is headpointer of link list
  // Write your code
    struct Set *ptr = SetA;

    while (ptr != NULL)
    {
        printf("%d ", ptr->value);
        ptr = ptr->next;
    }
    printf("\n");
}

/**
 * @brief Union of two Sets and stores in the 3rd set
 * 
 * @param SetA 
 * @param SetB 
 * @param SetC 
 * @return int 
 */
int Union(struct Set *SetA, struct Set *SetB, struct Set **SetC)
{
    struct Set *ptr1 = SetA;
    struct Set *ptr2 = SetB;
    while (ptr1 != NULL)
    {
        Insert(SetC, ptr1->value);
        ptr1 = ptr1->next;
    }
    while (ptr2 != NULL)
    {
        Insert(SetC, ptr2->value);
        ptr2 = ptr2->next;
    }

    return 1;
}

/**
 * @brief Helping function to check if the value is there in the set or not
 * 
 * @param SetX 
 * @param value 
 * @return int 
 */
int findValue(struct Set *SetX, int value)
{
    struct Set *ptr = SetX;
    while (ptr != NULL)
    {
        if (ptr->value == value)
        {
            return 1;
        }
        ptr = ptr->next;
    }
    return 0;
}

/**
 * @brief Intersection of first 2 sets and stores the result in the 3rd set
 * 
 * @param SetA - input set 1
 * @param SetB - Input set 2
 * @param SetC - Result set
 * @return int 
 */
int intersection(struct Set *SetA, struct Set *SetB, struct Set **SetC)
{ // To perform intersection between two sets SetA, SatB and
  // store the result in SetC, , where SetA,SetB,SetC is the head pointer of the link list
  // Write your code

    struct Set *ptr = SetA;
    while (ptr != NULL)
    {
        if (findValue(SetB, ptr->value))
        {
            Insert(SetC, ptr->value);
        }
        ptr = ptr->next;
    }

    return 1;
}

/**
 * @brief Difference of Set 1 and Set 2 and result in Set 3
 * 
 * @param SetA 
 * @param SetB 
 * @param SetC 
 * @return int 
 */
int difference(struct Set *SetA, struct Set *SetB, struct Set **SetC)
{ // To perform difference between two sets SetA, SatB and store the
  // result in SetC, where SetA,SetB,SetC is the head pointer of the link list
  // Write your code
    struct Set *ptr = SetA;
    while (ptr != NULL)
    {
        if (findValue(SetB, ptr->value) == 0)
        {
            Insert(SetC, ptr->value);
        }
        ptr = ptr->next;
    }
    return 1;
}

/**
 * @brief Deleting the set by freeing the space
 * 
 * @param SetA 
 * @return int 
 */
int deleteSet(struct Set **SetA)
{ // Take headpointer of link list SetA and Delete all the node from heap.
    // Write your code
    *SetA = NULL;
    return 1;
}

int main()
{
    // Read text file
    // Call accordingly each function
    // For each line, first number represent the operation, and remaining are parameter

    struct Set *SetA;
    makeSet(&SetA);
    struct Set *SetB;
    makeSet(&SetB);

    Insert(&SetA, 1);
    Insert(&SetA, 1);
    Insert(&SetA, 1);
    Insert(&SetA, 10);
    Insert(&SetA, 1);
    Insert(&SetA, 3);
    Insert(&SetA, 4);
    Insert(&SetA, 4);
    Insert(&SetA, 2);

    Insert(&SetB, 1);
    Insert(&SetB, 2);
    Insert(&SetB, 6);
    Insert(&SetB, 7);
    Insert(&SetB, 8);

    printSet(SetA);
    printSet(SetB);

    Delete(SetA, 3);
    Delete(SetA, 7);
    Delete(SetB, 3);
    Delete(SetB, 7);
    printSet(SetA);
    printSet(SetB);

    struct Set *SetC;
    makeSet(&SetC);
    Union(SetA, SetB, &SetC);
    printSet(SetC);

    struct Set *SetD;
    makeSet(&SetD);
    intersection(SetA, SetB, &SetD);
    printSet(SetD);

    struct Set *SetE;
    makeSet(&SetE);
    struct Set *SetF;
    makeSet(&SetF);

    difference(SetA, SetB, &SetE);
    difference(SetB, SetA, &SetF);

    printSet(SetE);
    printSet(SetF);

    deleteSet(&SetA);
    // deleteSet(SetB);

    printSet(SetA);
}