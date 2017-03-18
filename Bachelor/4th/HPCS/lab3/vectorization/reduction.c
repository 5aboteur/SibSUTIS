/*
* prog.c:
*
*/

#include <immintrin.h>
#include <stdio.h>

#include "hpctimer.h"

enum { N = 16 * 1024 * 1024, NREPS = 10 };

float reduction_sum(float *v, int n)
{
	int i;
	float sum = 0.0;

	for (i = 0; i < n; i++) {
		sum += v[i];
	}
	return sum;
}

float reduction_sum_sse(float *v, int n)
{
	int i, k;
	float sum = 0.0;
	__m128 *vv = (__m128 *)v;
	__m128 vsum = _mm_set1_ps(0.0f);

	k = n / 4;

	for (i = 0; i < k; ++i)
		vsum = _mm_add_ps(vsum, vv[i]);

	vsum = _mm_hadd_ps(vsum, vsum);
	vsum = _mm_hadd_ps(vsum, vsum);
	_mm_store_ss(&sum, vsum);

	return sum;
}

int main()
{
	int i;
	float *a;
	double sum;
	stopWatch t;

	a = (float *)_mm_malloc(sizeof(*a) * N, 16);
	for (i = 0; i < N; i++)
		a[i] = 1.0;

	startTimer(&t);
	for (i = 0; i < NREPS; i++) {
		//sum = reduction_sum(a, N);
		sum = reduction_sum_sse(a, N);
	}
	stopTimer(&t);
	printf("Reduction sum: %.4f (real %.4f)\n", sum, (float)N);
	printf("Elapsed time: %.6f sec.\n", getElapsedTime(&t) / NREPS);

	_mm_free(a);
	return 0;
}
