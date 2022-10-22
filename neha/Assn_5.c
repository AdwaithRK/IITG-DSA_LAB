// Assn_5.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <math.h>
#define M 6340

long double universe_cepstrals[M][12];
long double k_means_code_book[8][12];
long double temp_code_book[8][12];
long double code_book_vector[12], test_vector[12];
long double tokuras_wieght[12] = {1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};
long double temperory_distance[8];
int count_of_classification[8];
long double total_distorsion = 0;

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
	for (int i = 1; i < 8; i++)
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
	int m = 0;

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

	for (int l = 0; l < 8; l++)
		for (int p = 0; p < 12; p++)
		{
			k_means_code_book[l][p] = universe_cepstrals[l][p];
		}

	do
	{
		prev_distorsion = total_distorsion; // setting total distortion as previous distortion
		total_distorsion = 0;				// initializing current distortion as zero

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

			for (int p = 0; p < 8; p++)
			{
				// copying the current code book vector
				for (int i = 0; i < 12; i++)
				{
					code_book_vector[i] = k_means_code_book[p][i];
				}

				temperory_distance[p] = distance();
			}
			// finding the minimum index among the temporary distances
			int minimum_index = find_minimum_distance();
			// incrementing count of classification
				count_of_classification[minimum_index]++;
			// adding to new temporary code book
			add_to_new_code_book(universe_cepstrals[u], minimum_index);
		}

		update_code_book();
		m++;
	} while (fabsl(prev_distorsion - total_distorsion) > 0.0001);

	printf("\nIt took %d iterations of k means\n", m);

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
