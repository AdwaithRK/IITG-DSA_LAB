// assignment_5.cpp : Defines the entry point for the console application.

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#define P 12

// Tokura's weight given by sir, after years of experimentation

long double W[13] = {0.0, 1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};

long double code_book[8][13];
long double vectors[6340][13];
long double distorsion[8];
long double new_centroids[8][13];
int classified_vectors[8];
int m = 0;

long double measure_tokuras_distance(long double Cr[13], long double Ct[13])
{
    long double distance = 0;
    for (int i = 1; i <= P; i++)
    {
        distance += W[i] * ((Cr[i] - Ct[i]) * (Cr[i] - Ct[i]));
    }
    return distance;
}

int number_of_vectors_in_file(char *input_file)
{
    int count = 0;
    FILE *fp = fopen(input_file, "r");

    // Check if file exists
    if (fp == NULL)
    {
        printf("Could not open file %s", input_file);
        return 0;
    }

    // Extract characters from file and store in character c
    for (char c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;

    fclose(fp);

    return count;
}

int generate_number_within_zero_and_n(int n)
{
    srand(time(0));

    return rand() % n;
}

int initialise_buckets(char *input_file, int number_of_vectors)
{
    FILE *fp = fopen(input_file, "r");
    long double k = 0;

    // Check if file exists
    if (fp == NULL)
    {
        printf("Could not open file %s", input_file);
        return 0;
    }

    // initialize the code book

    for (int i = 0; i < 8; i++)
    {
        code_book[i][0] = 0;
        for (int j = 1; j <= 11; j++)
        {
            fscanf(fp, "%Lf,", &k);
            code_book[i][j] = k;
        }
        fscanf(fp, "%Lf\n", &k);
        code_book[i][12] = k;
    }

    // initialized the codebook by taking the initial 8 vectors
}

int read_vectors_into_array(int number_of_vectors, char *input_file)
{

    long double k;

    FILE *fp = fopen(input_file, "r");

    for (int i = 0; i < number_of_vectors; i++)
    {

        vectors[i][0] = 0;

        for (int j = 1; j <= 11; j++)
        {
            fscanf(fp, "%Lf,", &k);
            printf("%Lf,", k);
            vectors[i][j] = k;
        }
        fscanf(fp, "%Lf\n", &k);
        printf("%Lf\n", k);
        vectors[i][12] = k;
    }

    return 0;
}

/*
Finding the index which has minimum distance
*/
int find_minimum_index(long double *distance, int size)
{
    int minimum_index = 0;
    long double minimum = distance[0];
    for (int i = 1; i < size; i++)
    {
        if (distance[i] < minimum)
        {
            minimum = distance[i];
            minimum_index = i;
        }
    }
    return minimum_index;
}

int classify_into_buckets(int number_of_vector_in_universe)
{

    for (int i = 0; i < 8; i++)
    {
        distorsion[i] = 0;
        classified_vectors[i] = 0;
    }

    for (int i = 0; i < number_of_vector_in_universe; i++)
    {
        long double temp[8];

        for (int j = 0; j < 8; j++)
        {

            temp[j] = measure_tokuras_distance(code_book[j], vectors[i]);
        }

        int bucket_index = find_minimum_index(temp, 8);

        distorsion[bucket_index] += temp[bucket_index];

        new_centroids[bucket_index][0] = 0;

        for (int k = 1; k <= 12; k++)
        {
            new_centroids[bucket_index][k] += vectors[i][k];
        }

        classified_vectors[bucket_index]++;

        // printf("\nThe bucket index: %d\n", bucket_index);
    }

    for (int i = 0; i < 8; i++)
    {
        for (int k = 1; k <= 12; k++)
        {
            new_centroids[i][k] = new_centroids[i][k] / classified_vectors[i];
        }
    }
}

int update_code_book()
{
    for (int i = 0; i < 8; i++)
    {
        for (int k = 1; k <= 12; k++)
        {
            code_book[i][k] = new_centroids[i][k];
        }
    }
    return 0;
}

long double get_distortion()
{
    long double total_distortion = 0;
    for (int i = 0; i < 8; i++)
    {
        total_distortion += distorsion[i];
    }
    return total_distortion;
}

void print_code_book()
{
    printf("\n\nThe code book is :\n");
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j <= 12; j++)
        {
            printf("%Lf,", code_book[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    long double total_distortion = 0, prev_distortion = 0;

    int number_of_vector_in_universe = number_of_vectors_in_file("Universe.csv");

    initialise_buckets("Universe.csv", number_of_vector_in_universe);

    read_vectors_into_array(number_of_vector_in_universe, "Universe.csv");

    do
    {
        prev_distortion = total_distortion;

        classify_into_buckets(number_of_vector_in_universe);

        update_code_book();

        total_distortion = get_distortion();

        m++;
    } while (fabsl(prev_distortion - total_distortion) > 0.0001);

    print_code_book();

    printf("\nTotal iterations: %d\n", m);

    return 0;
}
