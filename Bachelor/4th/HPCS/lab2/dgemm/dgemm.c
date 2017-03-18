/*
* dgemm.c: DGEMM - Double-precision General Matrix Multiply.
*
*/

#include <stdio.h>
#include <stdlib.h>

#define BS 32

#include "hpctimer.h"

enum {
	N = 512,
	NREPS = 3
};

double A[N * N], B[N * N], C[N * N];

void dgemm_def(double *a, double *b, double *c, int n)
{
	int i, j, k;

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			for (k = 0; k < n; k++) {
				*(c + i * n + j) += *(a + i * n + k) * *(b + k * n + j);
			}
		}
	}
}

void dgemm_transpose(double *a, double *b, double *c, int n)
{
	int i, j, k;

	for (i = 0; i < n; i++) {
		for (k = 0; k < n; k++) {
			for (j = 0; j < n; j++) {
				*(c + i * n + j) += *(a + i * n + k) * *(b + k * n + j);
			}
		}
	}
}

void dgemm_transpose2(double *a, double *b, double *c, int n)
{
	int i, j, k;

	for (i = 0; i < n; i++) {
		for (k = 0; k < n; k++) {
			for (j = 0; j < n; j += 8) {
				*(c + i * n + j) += *(a + i * n + k) * *(b + k * n + j);
				*(c + i * n + j + 1) += *(a + i * n + k) * *(b + k * n + j + 1);
				*(c + i * n + j + 2) += *(a + i * n + k) * *(b + k * n + j + 2);
				*(c + i * n + j + 3) += *(a + i * n + k) * *(b + k * n + j + 3);
				*(c + i * n + j + 4) += *(a + i * n + k) * *(b + k * n + j + 4);
				*(c + i * n + j + 5) += *(a + i * n + k) * *(b + k * n + j + 5);
				*(c + i * n + j + 6) += *(a + i * n + k) * *(b + k * n + j + 6);
				*(c + i * n + j + 7) += *(a + i * n + k) * *(b + k * n + j + 7);
			}
		}
	}
}

void dgemm_block(double *a, double *b, double *c, int n)
{
	int i, j, k, i0, j0, k0;

	double a0[BS], b0[BS], c0[BS];

	for (i = 0; i < n; i += BS) {
		for (j = 0; j < n; j += BS) {
			for (k = 0; k < n; k += BS) {
				for (i0 = 0, *c0 = *(c + i * n + j), *a0 = *(a + i * n + k);
					i0 < BS; ++i0, *c0 += n, *a0 += n) {
					for (k0 = 0, *b0 = *(b + k * n + j);
						k0 < BS; ++k0, *b0 += n) {
						for (j0 = 0; j0 < BS; ++j0) {
							*(c0 + j0) += *(a0 + k0) * (*b0 + j0);
						}
					}
				}
			}
		}
	}
}

void dgemm_block2(double *a, double *b, double *c, int n)
{
	int i, j, k, i0, j0, k0;

	double a0[BS], b0[BS], c0[BS];

	for (i = 0; i < n; i += BS) {
		for (j = 0; j < n; j += BS) {
			for (k = 0; k < n; k += BS) {
				for (i0 = 0, *c0 = *(c + i * n + j), *a0 = *(a + i * n + k);
				i0 < BS; ++i0, *c0 += n, *a0 += n) {
					for (k0 = 0, *b0 = *(b + k * n + j);
					k0 < BS; ++k0, *b0 += n) {
						for (j0 = 0; j0 < BS; j0 += 8) {
							*(c0 + j0) += *(a0 + k0) * (*b0 + j0);
							*(c0 + j0 + 1) += *(a0 + k0) * (*b0 + j0 + 1);
							*(c0 + j0 + 2) += *(a0 + k0) * (*b0 + j0 + 2);
							*(c0 + j0 + 3) += *(a0 + k0) * (*b0 + j0 + 3);
							*(c0 + j0 + 4) += *(a0 + k0) * (*b0 + j0 + 4);
							*(c0 + j0 + 5) += *(a0 + k0) * (*b0 + j0 + 5);
							*(c0 + j0 + 6) += *(a0 + k0) * (*b0 + j0 + 6);
							*(c0 + j0 + 7) += *(a0 + k0) * (*b0 + j0 + 7);
						}
					}
				}
			}
		}
	}
}

void init_matrix(double *a, double *b, double *c, int n)
{
	int i, j, k;

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			for (k = 0; k < n; k++) {
				*(a + i * n + j) = 1.0;
				*(b + i * n + j) = 2.0;
				*(c + i * n + j) = 0.0;
			}
		}
	}
}

void print_matrix(double *a, int n)
{
	int i, j;

	printf("Matrix:\n");
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			printf("%12.2f", *(a + i * n + j));
		}
		printf("\n");
	}
}

int main(int argc, char **argv)
{
	int i;
        double t;

        init_matrix(A, B, C, N);

        t = hpctimer_getwtime();
        for (i = 0; i < NREPS; i++) {
                //dgemm_def(A, B, C, N);
        	//dgemm_transpose(A, B, C, N);
        	//dgemm_transpose2(A, B, C, N);
        	//dgemm_block(A, B, C, N);
        	dgemm_block2(A, B, C, N);
    	}
    	t = hpctimer_getwtime() - t;
    	t = t / NREPS;

    	print_matrix(C, N);

    	printf("Elapsed time: %.6f sec.\n", t);

	return 0;
}
