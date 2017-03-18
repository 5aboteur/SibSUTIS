// Without repetition

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000000
#define n 10

int mas[n] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
int hist[n] = { 0 };

int main()
{
	srand((unsigned)time(NULL));

	int msum = 0;
	double p[n], pp[n];

	for (int i = 0; i < n; ++i) msum += mas[i];

	printf("> Sum: %d\n", msum);

	for (int i = 0; i < n; ++i) {
		p[i] = mas[i] / (double)msum;
		printf(" p[%d]: %0.6lf\n", i, p[i]);
	}

	pp[0] = p[0];
	printf(" pp[0]: %0.6lf\n", pp[0]);
	for (int i = 1; i < n; ++i) {
		pp[i] = pp[i - 1] + p[i];
		printf(" pp[%d]: %0.6lf\n", i, pp[i]);
	}

	for (int i = 0; i < N; ++i) {
		double tmp = rand() / (double) RAND_MAX;
		for (int j = 0; j < n; ++j) {
			if (tmp < pp[j]) { ++hist[j]; break; }
		}
	}

	printf("> HIST: ");
	for (int i = 0; i < n; ++i) printf("[%d]: %d ", i, hist[i]);
	printf("\n");

	return 0;
}
