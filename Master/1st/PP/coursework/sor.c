#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define EPS	        1e-4
#define GRID_SZ	 	1024
#define MAXITERS 	2048
#define NTHR	 	8
#define OMEGA	  	0.337

#define HEIGHT	 	GRID_SZ/NTHR
#define CHECK_VALUE	256

double grid[GRID_SZ + 2][GRID_SZ + 2];
double oldgrid[GRID_SZ + 2][GRID_SZ + 2];

double dabs(double a) { return a > 0 ? a : -a; }
double ddmax(double a, double b) { return a > b ? a : b; }
//int is_converged(double diff) { return diff < EPS ? 1 : 0; }

double wtime();
void info();
void copy();
void print();
void red(void *__val);
void black(void *__val);

/* <-------- MAIN STARTS HERE --------> */

int main(int argc, char **argv)
{
	info();

	/*** GRID INITIALIZATION ***/

	for (int i = 0; i < GRID_SZ + 2; ++i) {
		for (int j = 0; j < GRID_SZ + 2; ++j) {
			if (i == 0 || i == GRID_SZ + 1 ||
				j == 0 || j == GRID_SZ + 1)
				grid[i][j] = 1.0;
			else
				grid[i][j] = 0.03;
		}
	}

//	print();

	/*** COMPUTATION STAGE ***/

	double t, maxdiff = 0.0;
	pthread_t thr[NTHR];

	t = wtime();

	for (int l = 1; l <= MAXITERS; ++l) {
		// we need to copy current values to another array
		// because we need them on the next iteration
		if (!(l % CHECK_VALUE))
			copy();

		// compute the red dots
		for (int j = 0; j < NTHR; ++j) {
			int *__val = malloc(sizeof(*__val));
			*__val = j;
			pthread_create(&thr[j], NULL, (void *) red, __val);
		}

		// wait til the job is done
		for (int j = 0; j < NTHR; ++j)
			pthread_join(thr[j], NULL);

		// compute the black dots
		for (int j = 0; j < NTHR; ++j) {
			int *__val = malloc(sizeof(*__val));
			*__val = j;
			pthread_create(&thr[j], NULL, (void *) black, __val);
		}

		// wait til the job is done
		for (int j = 0; j < NTHR; ++j)
			pthread_join(thr[j], NULL);

		// check for a convergence
		if (!(l % CHECK_VALUE)) {
			print();
			maxdiff = 0.0;
			for (int i = 0; i < GRID_SZ + 2; ++i) {
				for (int j = 0; j < GRID_SZ + 2; ++j) {
					maxdiff = (ddmax(dabs(oldgrid[i][j] - grid[i][j]), maxdiff));
//					printf("diff: %lf\n", dabs(oldgrid[i][j] - grid[i][j]));
				}
			}

			// if the difference between new and previous relevant value
			// less than EPS then break, else - continue our computations
			if (maxdiff < EPS) {
				printf("\033[1;32mIt`s converged!\033[0m\nmaxdiff=%.6lf\n", maxdiff);
				break;
			} else printf("Iteration [%d], maxdiff=%.6lf\n", l, maxdiff);
		}
	}

	t = wtime() - t;

	printf("\033[1;33mElapsed time:\033[0m %.6lf \033[1;33msec.\033[0m\n", t);

	return 0;
}

double wtime()
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

void info()
{
	printf("\n\n");
	printf(" ************ Red-Black SOR ************\n");
	printf(" > Size of grid:\t\t%dx%d\n", GRID_SZ, GRID_SZ);
	printf(" > Number of iterations:\t%d\n", MAXITERS);
	printf(" > Number of threads:\t\t%d\n", NTHR);
	printf(" > -------------------------------------\n");
	printf(" > Tolerance value (EPS):\t%lf\n", EPS);
	printf(" > Relaxation value (OMEGA):\t%.3lf\n", OMEGA);
	printf(" ***************************************\n");
	printf("\n\n");
}

void copy()
{
	for (int i = 0; i < GRID_SZ + 2; ++i) {
		for (int j = 0; j < GRID_SZ + 2; ++j)
			oldgrid[i][j] = grid[i][j];
	}
}

void print()
{
	for (int i = 0; i < GRID_SZ + 2; ++i) {
		for (int j = 0; j < GRID_SZ + 2; ++j)
			printf("%.5lf ", grid[i][j]);
		printf("\n");
	}
	printf("\n\n");
}

void red(void *__val)
{
	int val = *((int *) __val);

	int istart = val * HEIGHT + 1;
	int iend = (val + 1) * HEIGHT;
	int jstart;

//	printf("istart: %d, iend: %d\n", istart, iend);

	for (int i = istart; i <= iend; ++i) {
		if (i % 2 == 1)
			jstart = 1;	// odd row
		else
			jstart = 2;	// even row

		for (int j = jstart; j <= GRID_SZ; j += 2) {
			grid[i][j] = OMEGA * (grid[i - 1][j] + grid[i][j - 1]
				+ grid[i + 1][j] + grid[i][j + 1]) * 0.25
				+ (1 - OMEGA) * grid[i][j];
		}
	}
}

void black(void *__val)
{
	int val = *((int *) __val);

	int istart = val * HEIGHT + 1;
	int iend = (val + 1) * HEIGHT;
	int jstart;

//	printf("istart: %d, iend: %d\n", istart, iend);

	for (int i = istart; i <= iend; ++i) {
		if (i % 2 == 1)
			jstart = 2;	// odd row
		else
			jstart = 1;	// even row

		for (int j = jstart; j <= GRID_SZ; j += 2) {
			grid[i][j] = OMEGA * (grid[i - 1][j] + grid[i][j - 1]
				+ grid[i + 1][j] + grid[i][j + 1]) * 0.25
				+ (1 - OMEGA) * grid[i][j];
		}
	}
}
