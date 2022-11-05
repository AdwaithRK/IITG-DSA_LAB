#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 * @brief The structure of the polynomial node
 */
struct PolyNode
{
    int coeff;
    int numVars;
    int *varPower;
    int varPowerNum;
    struct PolyNode *link;
};

// Type defining for ease of access
typedef struct PolyNode polyNode;
typedef struct PolyNode *polyNodePtr;

/**
 * @brief Gives the power of 10 with the number provided
 *
 * @param n - power
 * @return int - pow(10, n)
 */
int power10(int n)
{
    int power = 1;
    for (int i = 0; i < n; i++)
    {
        power *= 10;
    }
    return power;
}

/**
 * @brief Create a Polynomial List using the file provided in the argument
 *
 * @param head - The head pointer for the access
 * @param fileName - The name of the input file
 * @return int
 */
int createPolynomial(polyNodePtr *head, char *fileName)
{
    // Opening the file
    FILE *fptr = fopen(fileName, "r");

    // Reading the num of variables for only one time
    int num;
    fscanf(fptr, "%d", &num); 

    // pointer to the head
    polyNodePtr ptr = *head;

    // Reading file till end
    while (!feof(fptr))
    {   
        // Creating a new node to store the polynomial
        polyNodePtr temp = (polyNodePtr)malloc(sizeof(polyNode));
        temp->numVars = num;

        // Reading the coefficient
        fscanf(fptr, "%d", &(temp->coeff));

        if (!feof(fptr))
        {
            // Creating the array for storing the power of variables
            int *arr = (int *)malloc(num * sizeof(int));
            int curPower = 0;

            // Stores the power of variables
            for (int i = 0; i < num; i++)
            {
                int curInt;
                fscanf(fptr, "%d", &curInt);
                *(arr + i) = curInt;

                curPower += curInt * power10(num - i - 1);
            }

            temp->varPower = arr;
            temp->varPowerNum = curPower;

            temp->link = NULL;

            // If head is NULL then assugb the value to head
            if (ptr == NULL)
            {
                ptr = temp;
                (*head) = temp;
            }

            // Check for the correct place of the node in ascending order and add the node there
            else
            {
                polyNodePtr curPtr = (*head);
                polyNodePtr prevPtr = NULL;

                while (curPtr != NULL)
                {
                    if (curPower > curPtr->varPowerNum)
                    {
                        break;
                    }
                    prevPtr = curPtr;
                    curPtr = curPtr->link;
                }

                if (prevPtr == NULL)
                {
                    temp->link = curPtr;
                    (*head) = temp;
                }
                else if (curPtr == NULL)
                {
                    prevPtr->link = temp;
                }
                else
                {
                    prevPtr->link = temp;
                    temp->link = curPtr;
                }
            }
        }
    }

    return 0;
}

/**
 * @brief Prints the polynomial
 * @param head - the head pointer to the polynomial
 */
void printPolynomial(polyNodePtr head)
{
    polyNodePtr ptr = head;

    while (ptr != NULL)
    {
        printf("%d ", ptr->coeff);
        for (int i = 0; i < ptr->numVars; i++)
        {
            printf("%d ", ptr->varPower[i]);
        }
        // printf(" = %d -> ", ptr->varPowerNum);
        printf("-> ");

        ptr = ptr->link;
    }
    printf("\n");
}

/**
 * @brief It adds two polynomials and stores the result in the res polynomial
 *
 * @param P1 - The first input polynomial
 * @param P2 - The Second input polynomial
 * @param res - The resultant polynomial
 * @return int
 */
int addPolynomials(polyNodePtr P1, polyNodePtr P2, polyNodePtr *res)
{
    polyNodePtr ans = (*res);
    polyNodePtr p1 = P1;
    polyNodePtr p2 = P2;

    // Compare two polynomials and create node in 3rd
    while (p1 != NULL && p2 != NULL)
    {
        polyNodePtr temp = (polyNodePtr)malloc(sizeof(polyNode));

        // If the var powers are same then add them
        if (p1->varPowerNum == p2->varPowerNum)
        {
            temp->numVars = p1->numVars;
            temp->varPower = p1->varPower;
            temp->coeff = p1->coeff + p2->coeff;
            temp->varPowerNum = p1->varPowerNum;
            p1 = p1->link;
            p2 = p2->link;
        }

        // If p1 > p2 then add p1
        else if (p1->varPowerNum > p2->varPowerNum)
        {
            temp->numVars = p1->numVars;
            temp->varPower = p1->varPower;
            temp->coeff = p1->coeff;
            temp->varPowerNum = p1->varPowerNum;
            p1 = p1->link;
        }

        // If p2 > p1 then add p2
        else
        {
            temp->numVars = p2->numVars;
            temp->varPower = p2->varPower;
            temp->coeff = p2->coeff;
            temp->varPowerNum = p2->varPowerNum;
            p2 = p2->link;
        }

        // Update the resulatant polynomial list
        temp->link = NULL;
        if (ans == NULL)
        {
            ans = temp;
            (*res) = temp;
        }
        else
        {
            ans->link = temp;
            ans = temp;
        }
    }

    // take remaining nodes from P1
    while (p1 != NULL)
    {
        polyNodePtr temp = (polyNodePtr)malloc(sizeof(polyNode));

        temp->numVars = p1->numVars;

        temp->varPower = p1->varPower;
        temp->coeff = p1->coeff;
        temp->varPowerNum = p1->varPowerNum;
        p1 = p1->link;

        temp->link = NULL;
        if (ans == NULL)
        {
            ans = temp;
            (*res) = temp;
        }
        else
        {
            ans->link = temp;
            ans = temp;
        }
    }

    // take remaining nodes from P2
    while (p2 != NULL)
    {
        polyNodePtr temp = (polyNodePtr)malloc(sizeof(polyNode));

        temp->numVars = p2->numVars;

        temp->varPower = p2->varPower;
        temp->coeff = p2->coeff;
        temp->varPowerNum = p2->varPowerNum;
        p2 = p2->link;

        temp->link = NULL;
        if (ans == NULL)
        {
            ans = temp;
            (*res) = temp;
        }
        else
        {
            ans->link = temp;
            ans = temp;
        }
    }

    return 0;
}

/**
 * @brief Multiplies two polynomials
 *
 * @param P1 - input polynomial 1
 * @param P2 - input polynomial 2
 * @param res - resultant polynomial
 * @return int
 */
int multiplyPolynomials(polyNodePtr P1, polyNodePtr P2, polyNodePtr *res)
{
    polyNodePtr p1 = P1;
    polyNodePtr ans = (*res);

    // Multiply each node from P1 with the each node of p2
    while (p1 != NULL)
    {
        int num = p1->numVars;

        // Creating the new node
        polyNodePtr p2 = P2;

        // Loop through all the nodes of p2 for the taken node of p1
        while (p2 != NULL)
        {

            // Create a new node for storing the details
            polyNodePtr temp = (polyNodePtr)malloc(sizeof(polyNode));
            temp->numVars = p1->numVars > p2->numVars ? p1->numVars : p2->numVars;
            temp->coeff = p1->coeff * p2->coeff;

            // Allocate the space for the array
            int *arr = (int *)malloc(num * sizeof(int));
            int curPower = 0;

            // Add the power of the coefficients
            for (int i = 0; i < num; i++)
            {
                int curInt = p1->varPower[i] + p2->varPower[i];
                *(arr + i) = curInt;
                curPower += curInt * power10(num - i - 1);
            }

            temp->varPower = arr;
            temp->varPowerNum = curPower;


            // If head is null then point it there
            if (ans == NULL)
            {
                ans = temp;
                (*res) = temp;
            }
            // Check if there is already a node with same coeffs if yes then add it there only otherwise add a new node
            else
            {
                polyNodePtr findPtr = (*res);
                int found = 0;


                // Find the node with same coeff
                while (findPtr != NULL)
                {
                    if (findPtr->varPowerNum == temp->varPowerNum)
                    {
                        found = 1;
                        break;
                    }
                    findPtr = findPtr->link;
                }


                // Update the same node if found
                if (found)
                {
                    findPtr->coeff = findPtr->coeff + temp->coeff;
                    free(temp);
                }
                // Append the new node
                else
                {
                    ans->link = temp;
                    ans = temp;
                }
            }
            p2 = p2->link; // advance p2
        }
        p1 = p1->link; // advance p1
    }

    return 0;
}

/**
 * @brief The main function
 *
 * @param argc - count of arguments
 * @param argv - the arguments
 * @return int
 */
int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        printf("The input files are not provided\n");
        return 0;
    }

    polyNodePtr P1 = NULL;
    polyNodePtr P2 = NULL;

    createPolynomial(&P1, argv[1]);
    createPolynomial(&P2, argv[2]);

    polyNodePtr P3 = NULL;

    multiplyPolynomials(P1, P2, &P3);
    printPolynomial(P1);
    printPolynomial(P2);
    printf("The multiplication\n");
    printPolynomial(P3);

    polyNodePtr P4 = NULL;
    addPolynomials(P1, P2, &P4);

    printf("The Addition\n");
    printPolynomial(P4);

    return 0;
}