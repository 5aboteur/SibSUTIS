/*
* vec.c: Example of SSE usage.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <emmintrin.h>

#include "hpctimer.h"

enum {
	N = 1024 * 1024,
	NREPS = 10
};

void fun_def(float *a, float *b, float *c, int n)
{
	int i;

	for (i = 0; i < n; i++) {
		c[i] = sqrt(a[i] * a[i] + b[i] * b[i]) + 0.5f;
	}
}

void fun_sse(float *a, float *b, float *c, int n)
{
	int i, k;
	__m128d x, y, z;
	__m128d *aa = (__m128d *)a;
	__m128d *bb = (__m128d *)b;
	__m128d *cc = (__m128d *)c;

	k = n / 2;
	z = _mm_set1_pd(0.5f);

	for (i = 0; i < k; i++) {
		x = _mm_mul_pd(*aa, *aa);
		y = _mm_mul_pd(*bb, *bb);
		x = _mm_add_pd(x, y);
		x = _mm_sqrt_pd(x);
		*cc = _mm_add_pd(x, z);
		aa++;
		bb++;
		cc++;
	}
}

void print_vec(float *a, int n)
{
	int i;

	for (i = 0; i < n; i++) {
		printf("%14.6f ", a[i]);
	}
	printf("\n");
}

int main(int argc, char **argv)
{
	int i;
	float *a, *b, *c;
	stopWatch t;

	a = (float *)_mm_malloc(sizeof(float) * N, 16);
	b = (float *)_mm_malloc(sizeof(float) * N, 16);
	c = (float *)_mm_malloc(sizeof(float) * N, 16);
	for (i = 0; i < N; i++) {
		a[i] = 1.0;
		b[i] = 2.0;
	}

	startTimer(&t);
	for (i = 0; i < NREPS; i++) {
		fun_sse(a, b, c, N);
		//fun_def(a, b, c, N);
	}
	stopTimer(&t);

	//print_vec(c, N);

	printf("Elapsed time: %.6f sec.\n", getElapsedTime(&t) / NREPS);

	_mm_free(a);
	_mm_free(b);
	_mm_free(c);

	return 0;
}

