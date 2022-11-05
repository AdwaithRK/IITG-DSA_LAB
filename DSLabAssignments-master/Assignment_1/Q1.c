#include<stdio.h>

/**
 * @brief Stack push operation
 * 
 * @param arr 
 * @param top 
 * @param c 
 * @return int 
 */
int push(char *arr, int top, char c) {
    if (top == 100) {
        printf("Stack overflow");
        return -1;
    }
    arr[top] = c;

    return top+1;

}

/**
 * @brief Stack pop operation
 * 
 * @param arr 
 * @param top 
 * @return char 
 */
char pop(char *arr, int top) {
    if (top == 0) {
        printf("Stack is empty");
        return '0';
    }
    top--;
    return arr[top];
}

int main(int argc, char *argv[]) {

    // Opening the file by reading the name from Command line
    FILE* ipFile = fopen(argv[1], "r");
    char o1 = '{', o2 = '(', o3 ='[';
    char c1 = '}', c2 = ')', c3 = ']';

    char arr[100];
    int top = 0;
    int flag = 1;

    char curChar;
    while (!feof(ipFile) && flag == 1) {
        curChar = fgetc(ipFile);

        // Check for the parenthesis
        if (curChar == o1 || curChar == o2 || curChar == o3) {
            top = push(arr, top, curChar);
            if (top == -1) {
                flag = 0;
            }
        } else if (curChar == c1)
        {
            if (pop(arr, top) != o1)
                flag = 0;
            top--;
        } else if (curChar == c2)
        {
            if (pop(arr, top) != o2)
                flag = 0;
            top--;
        } else if (curChar == c3) {
            if (pop(arr, top) != o3)
                flag = 0;
            top--;
        }
    }

    // Checking overll code if the input file is valid or not
    if (flag == 0 || top > 0) {
        printf("Input file is invalid\n");
    } else {
        printf("Input file is valid\n");
    }

}