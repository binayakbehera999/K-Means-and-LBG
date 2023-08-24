// K_Means.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#define delta 0.001

typedef struct Node Node;

long double codebook[8][12], distortion[8] = {0}, uni[6340][12], centroid[8][12] = {0},
							 tokura_wt[12] = {1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0}, distance[8] = {0},
							 dist_hist[1000] = {0.0};
int v_count[8] = {0};

//@brief - Generates 8 unique random no between (1, M) 
// M - size of universe
//@params - a to store the 8 random no
void generateRandNo(int *a)
{
	int count = 0, flag = 0;
	int temp;

	time_t t;

	srand((unsigned)time(&t));
	while (count != 8)
	{
		flag = 0;
		temp = (rand() % 6034) + 1;
		for (int i = 0; i < count; i++)
		{
			if (a[i] == temp)
				flag = 1;
		}
		if (flag == 0)
		{
			a[count] = temp;
			count++;
		}
	}
}

//@brief - finds the index of min tokura distance
//return - index of min tokura distance
int minIndex()
{
	int minIndex = 0;
	double min = distance[0];
	for (int i = 1; i < 8; i++)
	{
		if (distance[i] < min)
		{
			minIndex = i;
			min = distance[i];
		}
	}
	return minIndex;
}

//@brief - calculate tokura distance with codebook
//@parma - a - vector under consideration
void tokuraDist(long double a[])
{
	long double t;
	int index;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			t = (a[j] - centroid[i][j]) * (a[j] - centroid[i][j]);
			distance[i] = distance[i] + tokura_wt[j] * t;
		}
	}
}

//@brief - classifies the vectors in the univers (size n), into k buckets
//@params - in - file pointer to universe
//@params - k - size of codebook
//@params - n - size of universe
int K_means(FILE *in, int k, int n)
{
	int arr[8] = {0}, line = 0, m = 0, index;
	char str[256], *temp, *end;
	long double dist_sum = 0;
	generateRandNo(arr);

	for (int i = 0; i < 8; i++)
	{
		printf("%d\n", arr[i]);
	}

	for (int i = 0; i < 1000; i++)
	{
		dist_hist[i] = 0;
	}
	//input universe
	while (1)
	{
		fscanf(in, "%Lf, %Lf, %Lf, %Lf, %Lf, %Lf, %Lf, %Lf, %Lf, %Lf, %Lf, %Lf",
			   &uni[line][0], &uni[line][1], &uni[line][2], &uni[line][3], &uni[line][4], &uni[line][5],
			   &uni[line][6], &uni[line][7], &uni[line][8], &uni[line][9], &uni[line][10], &uni[line][11]);
		line++;
		if (line == n)
			break;
	}

	printf("Intial Codebook vectors\n");
	//set intial vectors
	for (int i = 0; i < k; i++)
	{
		printf("%d. ", i + 1);
		for (int j = 0; j < 12; j++)
		{
			codebook[i][j] = uni[arr[i]][j];
			printf("c%d = \'%.7Lf\' ", j + 1, codebook[i][j]);
		}
		printf("\n\n");
	}

	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			centroid[i][j] = codebook[i][j];
		}
	}

	for (int i = 0; i < k; i++)
	{
		distortion[i] = 0;
		v_count[i] = 0;
	}

	for (int i = 0; i < n; i++)
	{
		tokuraDist(uni[i]);
		index = minIndex();

		for (int j = 0; j < 12; j++)
		{
			codebook[index][j] += uni[i][j];
		}

		v_count[index]++;

		distortion[index] += distance[index];

		for (int j = 0; j < 8; j++)
		{
			distance[j] = 0;
		}
	}

	for (int i = 0; i < k; i++)
	{
		dist_sum += distortion[i];
	}

	dist_hist[m] = dist_sum / n;
	dist_sum = 0;
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			centroid[i][j] = codebook[i][j] / v_count[i];
		}
	}

	printf("Code book after iteration %d\n", m);
	for (int i = 0; i < k; i++)
	{
		printf("%d. ", i + 1);
		for (int j = 0; j < 12; j++)
		{
			printf("c%d = \'%.7Lf\' ", j + 1, centroid[i][j]);
		}
		printf("\n\n");
	}

	for (int i = 0; i < k; i++)
	{
		distortion[i] = 0;
		v_count[i] = 0;
	}

	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			codebook[i][j] = 0;
		}
	}
	printf("Iteration Result\n");
	printf("========================================\n");
	printf("Iteration\tDistortion\n");
	printf("%d\t\t\'%.7Lf\'\n\n", m, dist_hist[m]);
	do
	{
		m++;
		for (int i = 0; i < n; i++)
		{
			tokuraDist(uni[i]);
			index = minIndex();

			for (int j = 0; j < 12; j++)
			{
				codebook[index][j] += uni[i][j];
			}

			v_count[index]++;

			distortion[index] += distance[index];

			for (int j = 0; j < 8; j++)
			{
				distance[j] = 0;
			}
		}
		for (int i = 0; i < k; i++)
		{
			dist_sum += distortion[i];
		}

		dist_hist[m] = dist_sum / n;
		dist_sum = 0;

		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < 12; j++)
			{
				centroid[i][j] = codebook[i][j] / v_count[i];
			}
		}

		printf("Code book after iteration %d\n", m);
		for (int i = 0; i < k; i++)
		{
			printf("%d. ", i + 1);
			for (int j = 0; j < 12; j++)
			{
				printf("c%d = \'%.7Lf\' ", j + 1, centroid[i][j]);
			}
			printf("\n\n");
		}

		for (int i = 0; i < k; i++)
		{
			distortion[i] = 0;
			v_count[i] = 0;
		}

		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < 12; j++)
			{
				codebook[i][j] = 0;
			}
		}
		printf("Iteration Result\n");
		printf("========================================\n");
		printf("Iteration\tDistortion\tDistortion Diff\n");
		printf("%d\t\t%.7Lf\t%.7Lf\n\n", m, dist_hist[m], fabs(dist_hist[m] - dist_hist[m - 1]));
	} while (fabs(dist_hist[m] - dist_hist[m - 1]) > delta);
	printf("Final codebook\n");

	for (int i = 0; i < 8; i++)
	{
		printf("%d. ", i+1);
		for (int j = 0; j < 12; j++)
		{
			printf("c%d = \'%.7Lf\' ", j + 1, centroid[i][j]);
		}
		printf("\n\n");
	}

	return 1;
}

int _tmain(int argc, _TCHAR *argv[])
{
	FILE *un;
	un = fopen("Universe.csv", "r");

	if (!un)
		perror("Universe not accesible");
	else
		printf("Reading from Universe.csv\n");

	K_means(un, 8, 6340);

	return 0;
}
