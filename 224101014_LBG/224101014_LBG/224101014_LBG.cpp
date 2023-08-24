// LBG.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#define delta 0.001
#define epsilon 0.03

typedef struct Node Node;

long double codebook[8][12], distortion[8] = {0}, uni[6340][12], centroid[8][12] = {0},
							 tokura_wt[12] = {1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0}, distance[8] = {0},
							 dist_hist[1000] = {0.0};
int v_count[8] = {0}, l_index = 0;

//@brief - finds the index of min tokura distance
//return - index of min tokura distance
int minIndex(int k)
{
	int minIndex = 0;
	double min = distance[0];
	for (int i = 1; i < k; i++)
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
void tokuraDist(long double a[], int k)
{
	long double t;
	int index;
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			t = (a[j] - centroid[i][j]) * (a[j] - centroid[i][j]);
			distance[i] = distance[i] + tokura_wt[j] * t;
		}
	}
}

//@brief - reads universe
//@params - in - file pointer to universe
//@params - n - size of universe
void inputUniverse(FILE *in, int n){
	int line = 0;
	while (1)
	{
		fscanf(in, "%Lf, %Lf, %Lf, %Lf, %Lf, %Lf, %Lf, %Lf, %Lf, %Lf, %Lf, %Lf",
			   &uni[line][0], &uni[line][1], &uni[line][2], &uni[line][3], &uni[line][4], &uni[line][5],
			   &uni[line][6], &uni[line][7], &uni[line][8], &uni[line][9], &uni[line][10], &uni[line][11]);
		line++;
		if (line == n)
			break;
	}
}


//@brief - classifies the vectors in the univers (size n), into k buckets
//@params - k - size of codebook
//@params - n - size of universe
int K_means(int k, int n)
{
	int m = 0, index;
	long double dist_sum = 0;

	for (int i = 0; i < 1000; i++)
	{
		dist_hist[i] = 0;
	}

	for (int i = 0; i < k; i++)
	{
		distortion[i] = 0;
		v_count[i] = 0;
	}

	for (int i = 0; i < n; i++)
	{
		tokuraDist(uni[i], k);
		index = minIndex(k);

		for (int j = 0; j < 12; j++)
		{
			codebook[index][j] += uni[i][j];
		}

		v_count[index]++;

		distortion[index] += distance[index];

		for (int j = 0; j < k; j++)
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
	printf("%d\t\t%.7Lf\n\n", m, dist_hist[m]);
	do
	{
		m++;
		for (int i = 0; i < n; i++)
		{
			tokuraDist(uni[i], k);
			index = minIndex(k);

			for (int j = 0; j < 12; j++)
			{
				codebook[index][j] += uni[i][j];
			}

			v_count[index]++;

			distortion[index] += distance[index];

			for (int j = 0; j < k; j++)
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

	return 1;
}

//@brief - sets intial codebook
//@params - n - size of universe
void intialisation(int n){

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			centroid[l_index][j] += uni[i][j];
		}
	}
	printf("Intial Centroid\n\n");
	for (int i = 0; i < 12; i++)
	{
		centroid[l_index][i] /= n;
		printf("c%d = \'%.7Lf\' ", i + 1, centroid[l_index][i]);
	}
	printf("\n\n");
}

int LBG(int n, int k){
	intialisation(n);

	int x = 1;

	while(x < k){
		printf("Code Book Size %d\n\n", x * 2);
		
		//splits codebook
		for (int i = 0; i < x; i++)
		{	
			l_index++;
			for (int j = 0; j < 12; j++)
			{
				centroid[l_index][j] = centroid[i][j] + epsilon;
			}
			for (int j = 0; j < 12; j++)
			{
				centroid[i][j] = centroid[i][j] - epsilon;
			}
		}
		x *= 2;

		printf("K Means Results\n\n");
		K_means(x, n);
		printf("Centroids after K Means converges\n\n");
		for (int i = 0; i <= l_index; i++)
		{	
			printf("%d.", i + 1);
			for (int j = 0; j < 12; j++)
			{
				printf("c%d = \'%.7Lf\' ",j+1, centroid[i][j]);
			}
			printf("\n\n");			
		}		
	}

	
	return 1;
}


int _tmain(int argc, _TCHAR* argv[])
{	
	FILE *un;
	un = fopen("Universe.csv", "r");

	if (!un)
		perror("Universe not accesible");
	else
		printf("Reading from Universe.csv\n");
	inputUniverse(un, 6340);
	LBG(6340, 8);
	return 0;
}

