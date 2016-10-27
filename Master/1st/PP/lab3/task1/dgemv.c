#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <omp.h>
#include <sys/time.h>

#include <emmintrin.h>
#include <immintrin.h>
#include <xmmintrin.h>
#include <pmmintrin.h>

/*
* Memory consumption: O(m * n + n + m)
*/

enum {
	m = 20000,
	n = 20000
};

void *xmalloc(size_t size)
{
	void *p = malloc(size);
	if (!p) {
		fprintf(stderr, "malloc failed\n");
		exit(EXIT_FAILURE);
	}
	return p;
}

double wtime()
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

/* dgemv: Compute matrix-vector product c[m] = a[m][n] * b[n] */
void dgemv(double *a, double *b, double *c, int m, int n)
{
	for (int i = 0; i < m; i++) {
		c[i] = 0.0;
		for (int j = 0; j < n; j++)
			c[i] += a[i * n + j] * b[j];
	}
}

/* dgemv_omp: Compute matrix-vector product c[m] = a[m][n] * b[n] */
void dgemv_omp(double *a, double *b, double *c, int m, int n)
{
	#pragma omp parallel for
	for (int i = 0; i < m; i++) {
		c[i] = 0.0;
		for (int j = 0; j < n; j++) {
			c[i] += a[i * n + j] * b[j];
		}
	}
}

void dgemv_omp_vec(double *a, double *b, double *c, int m, int n)
{
	__m128d *aa = (__m128d *)a;
	__m128d *bb = (__m128d *)b;
	__m128d *cc = (__m128d *)c;

	int k = m / 2;

	#pragma omp parallel for
	for (int i = 0; i < k; i++) {
		cc[i] = _mm_setzero_pd();
		for (int j = 0; j < n; j++) {
			__m128d tmp = _mm_setzero_pd();
			tmp = _mm_mul_pd(aa[i * n + j], bb[j]);
			cc[i] = _mm_add_pd(tmp, cc[i]);
		}
	}

	#pragma omp parallel for
	for (int i = k * 2; i < m; i++) {
		c[i] = 0.0;
		for (int j = 0; j < n; j++)
			c[i] += a[i * n + j] * b[j];
	}
}

double run_serial()
{
	double *a, *b, *c;

	// Allocate memory for 2-d array a[m, n]
	a = xmalloc(sizeof(*a) * m * n);
	b = xmalloc(sizeof(*b) * n);
	c = xmalloc(sizeof(*c) * m);

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			a[i * n + j] = i + j;
	}

	for (int j = 0; j < n; j++)
		b[j] = j;

	double t = wtime();
	dgemv(a, b, c, m, n);
	t = wtime() - t;

	printf("Elapsed time (serial): %.6f sec.\n", t);

	free(a);
	free(b);
	free(c);

	return t;
}

double run_parallel()
{
	double *a, *b, *c;

	// Allocate memory for 2-d array a[m, n]
	a = xmalloc(sizeof(*a) * m * n);
	b = xmalloc(sizeof(*b) * n);
	c = xmalloc(sizeof(*c) * m);

	#pragma omp parallel for
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			a[i * n + j] = i + j;
		}
	}

	#pragma omp for
	for (int j = 0; j < n; j++)
		b[j] = j;

	double t = wtime();
	dgemv_omp(a, b, c, m, n);
	t = wtime() - t;

	printf("Elapsed time (parallel): %.6f sec.\n", t);

	free(a);
	free(b);
	free(c);

	return t;
}

double run_parallel_vec()
{
	double *a, *b, *c;

	// Allocate memory for 2-d array a[m, n]
	a = _mm_malloc(sizeof(*a) * m * n, 16);
	b = _mm_malloc(sizeof(*b) * n, 16);
	c = _mm_malloc(sizeof(*c) * m, 16);

	#pragma omp parallel for
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			a[i * n + j] = i + j;
		}
	}

	#pragma omp for
	for (int j = 0; j < n; j++)
		b[j] = j;

	double t = wtime();
	dgemv_omp_vec(a, b, c, m, n);
	t = wtime() - t;

	printf("Elapsed time (parallel): %.6f sec.\n", t);

	_mm_free(a);
	_mm_free(b);
	_mm_free(c);

	return t;
}

int main(int argc, char **argv)
{
	printf("DGEMV: general matrix-vector multiplication\n (c[m] = a[m, n] * b[n]; m = %d, n = %d)\n", m, n);
	printf("Memory used: %" PRIu64 " MiB\n", (uint64_t)(((double)m * n + m + n) * sizeof(double)) >> 20);

	double tser = run_serial();
	double tpar = run_parallel();
	double tvec = run_parallel_vec();

	printf("Speedup: %.2f\n", tser / tpar);
	printf("Speedup: %.2f\n", tser / tvec);

	return 0;
}
