#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define TIME_INTERVAL 	10
#define EXPERIMENTS 	10
#define MAX_TERMINALS 	3
#define TERMINALS 	5
#define L_COLUMNS 	14
#define M_COLUMNS 	10

const float step = 0.001;

const int Lambda[TERMINALS][L_COLUMNS] = {
	{  4,  5,  4,  1,  5,  5,  5,  3,  5,  3,  9,  7,  7,  3  },
	{  4, 10,  6,  5,  7,  4,  5,  5,  8,  8,  8, 11,  8,  4  },
	{  9, 14,  6, 12,  8, 11, 18, 17,  7, 15, 16,  7, 16, 15  },
	{ 11,  8,  9, 13, 15, 13,  7,  8,  3,  9, 12,  8,  5, 10  },
	{  8,  7,  8,  9,  7,  9,  7,  7,  9, 18,  6,  3,  8,  7  }
};

const float Mu[TERMINALS][M_COLUMNS] = {
	{0.39, 0.09, 0.60, 1.07, 0.72, 2.20, 1.84, 2.50, 4.54, 0.40},
	{1.14, 0.10, 1.87, 0.46, 0.15, 3.08, 0.14, 1.44, 0.64, 1.40},
	{0.59, 3.29, 0.22, 0.76, 0.64, 0.95, 0.20, 0.41, 0.60, 0.86},
	{0.47, 3.99, 1.66, 2.28, 1.48, 0.91, 0.17, 1.33, 0.89, 0.45},
	{0.26, 0.21, 0.70, 0.66, 0.07, 1.02, 1.05, 1.14, 0.40, 0.10}
};

float l[TERMINALS], m[TERMINALS];


// gets accurate values of the intensities
void intensities() {
	int i, j;
	float sum1, sum2;
	for (i = 0; i < TERMINALS; ++i) {
		sum1 = 0.0; sum2 = 0.0;
		for (j = 0; j < L_COLUMNS; ++j)
			sum1 += Lambda[i][j];
		for (j = 0; j < M_COLUMNS; ++j)
			sum2 += Mu[i][j];
		l[i] = sum1 / L_COLUMNS;
		m[i] = M_COLUMNS / sum2;
	}
}


// monte carlo method
void experiment(int *good, int *bad) {
	int i, j;
	float time = 0.0, x = 0.0;
	int good_try = 0, bad_try = 0, w_terms = 0;
	int prev[TERMINALS] = { 0 }, curr[TERMINALS] = { 0 };
	for (; time < TIME_INTERVAL; time += step) {
		for (i = 0; i < TERMINALS; ++i) {
			x = (float) rand() / RAND_MAX;
			if (prev[i] == 0) {
//				printf("step*li: %f\n", step*l[i]);
				if (x <= step * l[i]) {
					++good_try;
					w_terms = 0;
					for (j = i - 1; j >= 0; --j)
						if (curr[j] == 1)
							++w_terms;
					for (j = i + 1; j < TERMINALS; ++j)
						if (prev[j] == 1)
							++w_terms;
					if (w_terms >= MAX_TERMINALS) {
						++bad_try;
						curr[i] = 0;
					} else
						curr[i] = 1;
				} else
					curr[i] = 0;
			} else {
				if (x <= step * m[i])
					curr[i] = 0;
				else
					curr[i] = 1;
			}
/*			int z;
			printf("x: %f\n", x);
			printf("prev: ");
			for (z = 0; z < TERMINALS; ++z)
				printf("%d ", prev[z]);
			printf("\n");

			printf("cur: ");
			for (z = 0; z < TERMINALS; ++z)
				printf("%d ", prev[z]);
			printf("\n");
			getchar();
*/		}
		for (i = 0; i < TERMINALS; ++i)
			prev[i] = curr[i];
	}
	*good = good_try;
	*bad = bad_try;
}

// main
int main() {
	int i = 0, good = 0, bad = 0, sum_good = 0, sum_bad = 0;
	float p = 0.0;
	srand(time(NULL));
	intensities();
	printf("\n===========================================\n");
	// dump intensities
	printf("\n Intensities:\n\n__#__|____l____|____m____\n");
	for (i = 0; i < TERMINALS; ++i)
		printf("  %d  | %7.3f | %7.3f \n", i + 1, l[i], m[i]);
	printf("\n");
	// dump results of experiments
	printf("\n Experiments:\n\n__#__|___+__|___-__\n");
	for (i = 0; i < EXPERIMENTS; ++i) {
		experiment(&good, &bad);
		if (i == EXPERIMENTS - 1)
			printf("_%3d_|_%4d_|_%4d_\n", i + 1, good, bad);
		else
			printf(" %3d | %4d | %4d\n", i + 1, good, bad);
		sum_good += good; sum_bad += bad;
	}
	printf("TOTAL| %4d | %4d\n\n", sum_good, sum_bad);
	printf("\n");
	// calc & dump probability
	p = (float) sum_bad / sum_good;
	printf("Probability of failure: %.3f\n\n", p);
	printf("===========================================\n\n");

	return 0;
}
