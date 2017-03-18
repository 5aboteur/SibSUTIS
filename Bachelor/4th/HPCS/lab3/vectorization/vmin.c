/*
* prog.c:
*
*/

#include <immintrin.h>
#include <stdio.h>
#include <float.h>

#include "hpctimer.h"

enum { N = 64 * 1024 * 1024, NREPS = 10 };

float vmin(float *v, int n)
{
	int i;
	float minval = FLT_MAX;

	for (i = 0; i < n; i++) {
		if (v[i] < minval)
			minval = v[i];
	}
	return minval;
}

float vmin_sse(float *v, int n)
{
	int i, k;
	float res;
	__m128 minval = _mm_set_ps1(FLT_MAX);
	__m128 *vv = (__m128 *)v;

	k = n / 4;

	for (i = 0; i < k; i++)
		minval = _mm_min_ps(minval, vv[i]);

	minval = _mm_min_ps(minval, _mm_shuffle_ps(minval, minval, 0x93));
	minval = _mm_min_ps(minval, _mm_shuffle_ps(minval, minval, 0x93));
	minval = _mm_min_ps(minval, _mm_shuffle_ps(minval, minval, 0x93));
	_mm_store_ss(&res, minval);

	return res;
}

int main()
{
	int i;
	float *a, val;
	stopWatch t;

	a = (float *)_mm_malloc(sizeof(*a) * N, 16);
	for (i = 0; i < N; i++)
		a[i] = (float)(N - i);

	startTimer(&t);
	for (i = 0; i < NREPS; i++) {
		//val = vmin(a, N);
		val = vmin_sse(a, N);
	}
	stopTimer(&t);
	printf("Min value: %.4f (real %.4f)\n", val, a[N - 1]);
	printf("Elapsed time: %.6f sec.\n", getElapsedTime(&t) / NREPS);

	_mm_free(a);
	return 0;
}

