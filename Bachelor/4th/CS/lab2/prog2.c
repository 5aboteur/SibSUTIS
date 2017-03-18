// With repetition

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 1000000
#define n 10
#define m 4

int sum;
int mas[n] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
int hist[m][n];

void init(int *mas, double *p, double *pp)
{
	sum = 0;
	for (int i = 0; i < n; ++i) sum += mas[i];

	p[0] = pp[0] = mas[0] / (double)sum;
	for (int i = 1; i < n; ++i) {
		p[i] = mas[i] / (double)sum;
		pp[i] = pp[i - 1] + p[i];
	}
}

void print(int *mas, double *p, double *pp)
{
	printf("SUM: %d\n", sum);
	for (int i = 0; i < n; ++i)
		printf("m[%d]: %d\n", i, mas[i]);
	for (int i = 0; i < n; ++i)
		printf("p[%d]: %0.6lf\n", i, p[i]);
	for (int i = 0; i < n; ++i)
		printf("pp[%d]: %0.6lf\n", i, pp[i]);
}

int main()
{
	srand((unsigned)time(NULL));

	int masz[n];

	double p[n], pp[n], pz[n], ppz[n];

	for (int i = 0; i < N; ++i) {
		init(mas, p, pp);
		memcpy(masz, mas, sizeof(int) * n);
		memcpy(pz, p, sizeof(double) * n);
		memcpy(ppz, pp, sizeof(double) * n);

		if (i == 0) print(mas, p, pp);
		for (int k = 0; k < m; ++k) {
			double tmp = rand() / (double) RAND_MAX;
			for (int j = 0; j < n; ++j) {
				if (tmp < ppz[j]) {
					masz[j] = 0;
					init(masz, pz, ppz);
					++hist[k][j];
					break;
				}
			}
		}
	}

	for (int j = 0; j < m; ++j) {
		for (int i = 0; i < n; ++i) {
			printf("[%d][%d]: %d ", j, i, hist[j][i]);
		}
		printf("\n");
	}
	return 0;
}
