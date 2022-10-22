#include <stdio.h>
#include <math.h>
#define M 6340
#define file_path "Universe.csv"

long double universe_cepstrals[M][12];
long double universe_centroid[12];
long double code_book_vector[12], test_vector[12];
long double k_means_code_book[8][12];
long double temp_k_means_code_book[8][12]; // for splitting
long double temp_code_book[8][12];         // for k means
long double temperory_distance[8];
long double total_distorsion = 0;
int count_of_classification[8];

int code_book_size = 1;

long double tokuras_wieght[12] = {1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};

long double distance()
{
    long double distance = 0;
    for (int i = 0; i < 12; i++)
    {
        distance += tokuras_wieght[i] * (code_book_vector[i] - test_vector[i]) * (code_book_vector[i] - test_vector[i]);
    }
    return distance;
}

int find_minimum_distance()
{
    long double minimum = temperory_distance[0];
    int minimum_index = 0;
    for (int i = 1; i < code_book_size; i++)
    {
        if (minimum > temperory_distance[i])
        {
            minimum = temperory_distance[i];
            minimum_index = i;
        }
    }

    total_distorsion += minimum;

    return minimum_index;
}

int add_to_new_code_book(long double *vector, int index)
{
    for (int i = 0; i < 12; i++)
    {
        temp_code_book[index][i] = temp_code_book[index][i] + vector[i];
    }
}

int update_code_book()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            temp_code_book[i][j] /= count_of_classification[i];
        }
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            k_means_code_book[i][j] = temp_code_book[i][j];
        }
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            temp_code_book[i][j] = 0;
        }
    }
}

int main()
{
    FILE *ptr = fopen("Universe.csv", "r");
    long double k;
    long double prev_distorsion = 0;

    for (int i = 0; i < 12; i++)
        universe_centroid[i] = 0;

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 12; j++)
            k_means_code_book[i][j] = 0;

    for (int p = 0; p < M; p++)
    {
        for (int b = 0; b < 11; b++)
        {
            fscanf(ptr, "%Lf,", &k);
            universe_cepstrals[p][b] = k;
        }
        fscanf(ptr, "%Lf\n", &k);
        universe_cepstrals[p][11] = k;
    }

    for (int k = 0; k < M; k++)
    {
        for (int j = 0; j < 12; j++)
        {
            universe_centroid[j] += universe_cepstrals[k][j];
        }
    }

    for (int l = 0; l < 12; l++)
    {
        universe_centroid[l] /= M;
    }

    for (int i = 0; i < 12; i++)
    {
        k_means_code_book[0][i] = universe_centroid[i];
    }

    while (1)
    {
        // part of splitting code book

        int x = 0;
        for (int i = 0; i < code_book_size; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                temp_k_means_code_book[x][j] = k_means_code_book[i][j] + 0.03;
                temp_k_means_code_book[x + 1][j] = k_means_code_book[i][j] - 0.03;
            }
            x = x + 2;
        }
        code_book_size = code_book_size * 2;

        for (int i = 0; i < code_book_size; i++)
        {
            for (int k = 0; k < 12; k++)
            {
                k_means_code_book[i][k] = temp_k_means_code_book[i][k];
            }
        }

        do
        {
            // -----------start--------------------------

            prev_distorsion = total_distorsion; // setting total distortion as previous distortion
            total_distorsion = 0;               // initializing current distortion as zero

            // setting classification count as 0
            for (int i = 0; i < 8; i++)
                count_of_classification[i] = 0;

            for (int u = 0; u < M; u++)
            {
                // taking a test vector
                for (int i = 0; i < 12; i++)
                {
                    test_vector[i] = universe_cepstrals[u][i];
                }

                for (int p = 0; p < code_book_size; p++)
                {
                    // copying the current code book vector
                    for (int i = 0; i < 12; i++)
                    {
                        code_book_vector[i] = k_means_code_book[p][i];
                    }

                    temperory_distance[p] = distance();
                }

                int minimum_index = find_minimum_distance();

                count_of_classification[minimum_index]++;

                add_to_new_code_book(universe_cepstrals[u], minimum_index);
            }

            update_code_book();

            // --------------------end ------------------------
        } while (fabsl(prev_distorsion - total_distorsion) > 0.0001);

        if (code_book_size == 8)
            break;
    }

    printf("\n The final code book is:\n");

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            printf("%Lf,", k_means_code_book[i][j]);
        }
        printf("%Lf\n", k_means_code_book[i][11]);
    }

    return 0;
}