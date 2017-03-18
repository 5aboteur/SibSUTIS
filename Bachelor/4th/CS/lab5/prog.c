#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 10
#define DEG 4
 
int drand(int from, int to)
{
	return from + rand() % (to - from + 1);
}
 
void swap(int *a, int *b)
{
    int tmp = *a;
	*a = *b;
	*b = tmp;
}
 
int main()
{
	printf("\nMax degree: %d\nNumber of vertices: %d\n\n", DEG, N);
	srand((unsigned)time(NULL));
 
	int k = 0;
	int I[N], Degree[N];
	int io[N - 1], ou[N - 1];
 
	for (int i = 0; i < N; ++i) {
    	I[i] = i;
    	Degree[i] = 0;
	}
 
	int root = rand() % (N - 1);
 
	printf("Root: %d\n\n", root);
 
	swap(&I[N - 1], &I[root]);
 
	for (int i = 0; i < N - 1; ++i) {
    	int s = drand(N - 1 - i, N - 1 - k);
    	int t = drand(0, N - 2 - i);
 
    	io[i] = I[t];
    	ou[i] = I[s];
 
    	++Degree[I[t]];
    	++Degree[I[s]];
 
    	if (Degree[I[s]] == DEG) {
        	I[s] = I[N - 1 - k];
        	++k;
    	}
    	swap(&I[N - 2 - i], &I[t]);
	}
 
	printf("DEGREE\n");
	for (int i = 0; i < N; ++i) printf("[%d]: %d ", i, Degree[i]);
	printf("\nOU\n");
	for (int i = 0; i < N - 1; ++i) printf("[%d]: %d ", i, ou[i]);
	printf("\nIO\n");
	for (int i = 0; i < N - 1; ++i) printf("[%d]: %d ", i, io[i]);
	printf("\n");

	return 0;
}
