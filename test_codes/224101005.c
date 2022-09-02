#include<stdio.h>
#include<stdlib.h>

struct polyNode{
    int coeff;
    int numVars;
    int *varPower;
    struct polyNode *link;
};

/*
used global variables
must change
*/

typedef struct polyNode polyNode;

typedef struct polyNode* polyNodePtr;

polyNode *head1 = NULL;
polyNode *head2 = NULL;
polyNode *resultadd = NULL;
int number_of_variables;
int addition_terms = 0;

/*
created the polynomial
*/

int createPolynomial(char *filename1, char *filename2){



    int coefficent;
    FILE *ptr = fopen(filename1, "r");

    fscanf(ptr, "%d", &number_of_variables);

    polyNode *last = NULL;


    while(fscanf(ptr, "%d", &coefficent) != EOF){

            // printf("\ncoefficent%d\n", coefficent);
            if(head1 == NULL){
                head1 = (polyNodePtr)malloc(sizeof(polyNode));
                head1 -> coeff = coefficent;
                last = head1;
            }
            else{
                last -> link = (polyNodePtr)malloc(sizeof(polyNode));
                (last -> link) -> coeff = coefficent;
                last = last -> link;
            }
                
            
            int *temp = (int *)malloc(number_of_variables*sizeof(int));
            int power;

            int counter = number_of_variables;
            int index = 0;
            while(index < counter){
                int power;
                fscanf(ptr, "%d", &power);
                // printf("\nvariable %d is %d\n", index+1, power);
                temp[index] = power;
                index++;
            }

            last -> numVars = number_of_variables;

            last -> varPower = temp;


    }

    fclose(ptr);


    FILE *ptr1 = fopen(filename2, "r");

    fscanf(ptr1, "%d", &number_of_variables);


    polyNode *last1 = NULL;


    while(fscanf(ptr1, "%d", &coefficent) != EOF){

            // printf("\ncoefficent%d\n", coefficent);
            if(head2 == NULL){
                head2 = (polyNodePtr)malloc(sizeof(polyNode));
                head2 -> coeff = coefficent;
                last1 = head2;
            }
            else{
                last1 -> link = (polyNodePtr)malloc(sizeof(polyNode));
                (last1 -> link) -> coeff = coefficent;
                last1 = last1 -> link;
            }
                
            
            int *temp = (int *)malloc(number_of_variables*sizeof(int));
            int power;

            int counter = number_of_variables;
            int index = 0;
            while(index < counter){
                int power;
                fscanf(ptr, "%d", &power);
                // printf("\nvariable %d is %d\n", index+1, power);
                temp[index] = power;
                index++;
            }

            last1 -> numVars = number_of_variables;

            last1 -> varPower = temp;


    }

    fclose(ptr);







    return 0;
}


/*
printing the polynomial
should change now working based on option
*/


void printPolynomial(int option){
    polyNode *temp;
    temp = (option == 0)? head1 : head2;
    if(option == 2) temp = resultadd;
    while(temp){
        printf("coeff: %d\t", temp->coeff);
        int counter = 0;
        while(counter < number_of_variables){
            printf("power of var %d: %d\t", counter +1 ,temp->varPower[counter]);
            counter++;
        }

        printf("\n");

        temp = temp -> link;

    }

    printf("\n\n");

}

/*
    adding polynomials - n^2 
    should change
*/


int addPlynomials(){
    polyNode *temp1 = head1;
    polyNode *temp2 = head2;
    polyNode *last = NULL;
    while (temp1)
    {
        int same = 1;
        while(temp2){
            for(int i = 0; i < number_of_variables; i++){
                if(temp1->varPower[i] != temp2->varPower[i]) {
                    same = 0;
                    break;
                }
            }

            if(same == 1){
                int *temp_array = (int *)malloc(number_of_variables*sizeof(int));

                printf("\nyay!! same!!\n");
                if(resultadd == NULL){
                    resultadd = (polyNodePtr)malloc(sizeof(polyNode));
                    resultadd -> coeff = temp1 -> coeff + temp2 -> coeff;
                    resultadd -> numVars = temp1 -> numVars;
                    last = resultadd;
                }else{
                    last -> link = (polyNodePtr)malloc(sizeof(polyNode));
                    (last -> link) -> coeff = temp1 -> coeff + temp2 -> coeff;
                    last -> numVars = temp1 -> numVars;
                    last = last -> link;
                }

                for(int i = 0; i < number_of_variables; i++){
                    temp_array[i] = temp1 -> varPower[i];
                }
                last -> varPower = temp_array;
                break;
            }

            temp2 = temp2 -> link;
            
        }


        if(temp2 == NULL){
            addition_terms++;
            int *temp_array = (int *)malloc(number_of_variables*sizeof(int));

            if(resultadd == NULL){
                    resultadd = (polyNodePtr)malloc(sizeof(polyNode));
                    resultadd -> coeff = temp1 -> coeff;
                    resultadd -> numVars = temp1 -> numVars;
                    last = resultadd;
            }else{
                    last -> link = (polyNodePtr)malloc(sizeof(polyNode));
                    (last -> link) -> coeff = temp1 -> coeff;
                    last -> numVars = temp1 -> numVars;
                    last = last -> link;
            }

            for(int i = 0; i < number_of_variables; i++){
                 temp_array[i] = temp1 -> varPower[i];
            }
            last -> varPower = temp_array;

        }





        
        temp2 = head2;
        temp1 = temp1 -> link;
    }





    polyNode *resultTemp = resultadd;
    polyNode *poly2 = head2;

    while(poly2 != NULL){
        last -> link = (polyNodePtr)malloc(sizeof(polyNode));
        (last ->link) -> coeff = poly2 -> coeff;
        (last->link) -> numVars = poly2 -> numVars;


        int *temp_array = (int *)malloc(number_of_variables*sizeof(int));
        for(int i = 0; i < number_of_variables; i++){
            temp_array[i] = poly2 -> varPower[i];
        }
        
        (last -> link) -> varPower = temp_array;
        last = last -> link;


        poly2 = poly2 -> link;
    }


}


int main(){

    createPolynomial("input.txt", "input1.txt");
    printf("\nTerm1:\n");
    printPolynomial(0);
    printf("\nTerm2:\n");
    printPolynomial(1);
    addPlynomials();

    printf("\nAddition Result:\n");
    printPolynomial(2);




    
    return 0;
}