#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef unsigned long intL;

intL fact(intL f)
{
	if (!f) return 1;
	return (f * fact(f - 1));
}

void shift(char *a, int sz)
{
	int i, j;
	char first = a[0];

	for (i = 1, j = 0; i < sz; ++i, ++j)
		a[j] = a[i];

	a[j] = first;
}

int permutate(char *a, int sz, intL *cnt)
{
	static intL fact;

	if (sz > 1) {
		if (permutate(a + 1, sz - 1, cnt))
			shift(a, sz);

		fact *= sz;
	} else {
		(*cnt)++;
		fact = 1;
	}

	return !(*cnt % fact);
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Input N, man..\n");
		return -1;
	}

	srand((unsigned)time(NULL));

	FILE *fp;

	char alpha[12] = "ABCDEFGHIJKL";
	intL N = atoi(argv[1]);
	intL M = 0;
	intL i, j, s;

	/* Connectivity matrix */
	int **mtx = (int **)malloc(N * sizeof(int *));
	for (i = 0; i < N; ++i)
		mtx[i] = (int *)malloc(N * sizeof(int));

	for (i = 0; i < N; ++i) {
		for (j = i; j < N; ++j) {
			if (j == i)
				mtx[i][j] = 0;
			else {
				mtx[i][j] = mtx[j][i] = 1 + rand() % 100;
			}
		}
	}

	printf("  | ");
	for (i = 0; i < N - 1; ++i)
		printf(" %c |", alpha[i]);
	printf(" %c\n", alpha[N - 1]);

	printf("---");
	for (i = 0; i < N - 1; ++i)
		printf("-----");

	putchar('\n');

	for (i = 0; i < N; ++i) {
		printf("%c |", alpha[i]);
		for (j = 0; j < N; ++j)
			printf("%3d ", mtx[i][j]);
		putchar('\n');
	}

	M = fact(N - 1);

	printf("\nAll possible ways: %lu\n", M);

	/* Possible ways */
	char **wayz = (char **)malloc(M * sizeof(char *));
	for (i = 0; i < M; ++i) {
		wayz[i] = (char *)malloc((N + 1) * sizeof(char));
		memset(wayz[i], '\0', (N + 1) * sizeof(char));
	}

	putchar('\n');

	i = 0, M = 0;

	intL min = INT_MAX, min_i = 0;

	clock_t start = clock();

	do {
		for (j = 0; j < N; ++j)
			wayz[i][j] = alpha[j];
		wayz[i][N] = alpha[0];

//		printf("[%d]: %s, ", i, wayz[i]);

		s = 0;

		for (j = 0; j < N; ++j) {
//				printf("%c%c\n", wayz[i][j],wayz[i][j+1]);
				s += mtx[(int)wayz[i][j] - 65][(int)wayz[i][j + 1] - 65];
		}
//		printf("length: %d\n", s);

		if (min > s) {
			min = s;
			min_i = i;
		}

		++i;

	} while (!permutate(alpha + 1, N - 1, &M));
/*
	for (i = 0; i < M; ++i) {
		for (j = 0; j < N + 1; ++j)
			printf(" %c ", wayz[i][j]);
		putchar('\n');
	}
*/
	clock_t elapsed = clock() - start;

	fp = fopen("result", "a+");
	fprintf(fp, "%lu %f\n", N, (float)elapsed / CLOCKS_PER_SEC * 1000.0f);
	fclose(fp);

	printf("Optimal path is: %s (%lu)\n", wayz[min_i], min);
	printf("Elapsed time: %.6f ms.\n", (float)elapsed / CLOCKS_PER_SEC * 1000.0f);

	return 0;
}
