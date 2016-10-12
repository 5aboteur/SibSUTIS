#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <xmmintrin.h>
#include <emmintrin.h>
//#include <sys/time.h>

/************************************* WINDOWS /*************************************/
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdint.h> // portable: uint64_t   MSVC: __int64 

// MSVC defines this in winsock2.h!?
typedef struct timeval {
	long tv_sec;
	long tv_usec;
} timeval;

int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
	// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
	static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

	SYSTEMTIME  system_time;
	FILETIME    file_time;
	uint64_t    time;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	time = ((uint64_t)file_time.dwLowDateTime);
	time += ((uint64_t)file_time.dwHighDateTime) << 32;

	tp->tv_sec = (long)((time - EPOCH) / 10000000L);
	tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
	return 0;
}
/************************************* WINDOWS /*************************************/

#define EPS 1E-6
#define REP 20

enum {
	n = 1000000
};

void saxpy(float *x, float *y, float a, int n)
{
	for (int i = 0; i < n; ++i)
		y[i] = a * x[i] + y[i];
}

void saxpy_sse(float * __restrict x, float * __restrict y, float a, int n)
{
	__m128 *xx = (__m128 *)x;
	__m128 *yy = (__m128 *)y;

	int k = n / 4;							// float -> 32bit

	__m128 aa = _mm_set1_ps(a);				// [a, a, a, a]

	for (int i = 0; i < k; ++i) {
		__m128 z = _mm_mul_ps(aa, xx[i]);	// [aa0 * xx0, ..]
		yy[i] = _mm_add_ps(z, yy[i]);		// [z0 + yy0, ..]
	}

	/* Loop reminder (n % 4 != 0) ? */
	if (n % 4) {
		for (int i = k * 4; i < n; ++i)
			y[i] = a * x[i] + y[i];
	}
}

void daxpy(double *x, double *y, double a, int n)
{
	for (int i = 0; i < n; ++i)
		y[i] = a * x[i] + y[i];
}

void daxpy_sse(double * __restrict x, double * __restrict y, double a, int n)
{
	__m128d *xx = (__m128d *)x;
	__m128d *yy = (__m128d *)y;

	int k = n / 2;							// double -> 64bit

	__m128d aa = _mm_set1_pd(a);			// [a, a, a, a]

	for (int i = 0; i < k; ++i) {
		__m128d z = _mm_mul_pd(aa, xx[i]);	// [aa0 * xx0, ..]
		yy[i] = _mm_add_pd(z, yy[i]);		// [z0 + yy0, ..] 
	}

	/* Loop reminder (n % 4 != 0) ? */
	if (n % 2) {
		for (int i = k * 2; i < n; ++i)
			y[i] = a * x[i] + y[i];
	}
}

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

double run_scalar_saxpy()
{
	float *x, *y, a = 2.0;

	x = xmalloc(sizeof(*x) * n);
	y = xmalloc(sizeof(*y) * n);

	for (int i = 0; i < n; ++i) {
		x[i] = i * 2 + 1.0f;
		y[i] = (float)i;
	}

	saxpy(x, y, a, n);

	double t = wtime();
	for (int i = 0; i < REP; ++i)
		saxpy(x, y, a, n);
	t = (wtime() - t) / REP;

	/* Verification */
	for (int i = 0; i < n; ++i) {
		float xx = i * 2 + 1.0f;
		float yy = a * xx + i;
		if (fabs(y[i] - yy) > EPS) {
			fprintf(stderr, "run_scalar: verification failed (y[%d] = %f != %f)\n", i, y[i], yy);
			break;
		}
	}

	printf("Elapsed time (scalar): %.6f sec.\n", t);
	free(x);
	free(y);
	return t;
}

double run_vectorized_saxpy()
{
	float *x, *y, a = 2.0;

	x = _mm_malloc(sizeof(*x) * n, 16);
	y = _mm_malloc(sizeof(*y) * n, 16);

	for (int i = 0; i < n; ++i) {
		x[i] = i * 2 + 1.0f;
		y[i] = (float)i;
	}

	saxpy_sse(x, y, a, n);

	double t = wtime();
	for (int i = 0; i < REP; ++i)
		saxpy_sse(x, y, a, n);
	t = (wtime() - t) / REP;

	/* Verification */
	for (int i = 0; i < n; ++i) {
		float xx = i * 2 + 1.0f;
		float yy = a * xx + i;
		if (fabs(y[i] - yy) > EPS) {
			fprintf(stderr, "run_vectorized: verification failed (y[%d] = %f != %f)\n", i, y[i], yy);
			break;
		}
	}

	printf("Elapsed time (vectorized): %.6f sec.\n", t);
	_mm_free(x);
	_mm_free(y);
	return t;
}
double run_scalar_daxpy()
{
	double *x, *y, a = 2.0;

	x = xmalloc(sizeof(*x) * n);
	y = xmalloc(sizeof(*y) * n);

	for (int i = 0; i < n; ++i) {
		x[i] = i * 2 + 1.0;
		y[i] = i;
	}

	daxpy(x, y, a, n);

	double t = wtime();
	for (int i = 0; i < REP; ++i)
		daxpy(x, y, a, n);
	t = (wtime() - t) / REP;

	/* Verification */
	for (int i = 0; i < n; ++i) {
		double xx = i * 2 + 1.0;
		double yy = a * xx + i;
		if (fabs(y[i] - yy) > EPS) {
			fprintf(stderr, "run_scalar: verification failed (y[%d] = %lf != %lf)\n", i, y[i], yy);
			break;
		}
	}

	printf("Elapsed time (scalar): %.6f sec.\n", t);
	free(x);
	free(y);
	return t;
}

double run_vectorized_daxpy()
{
	double *x, *y, a = 2.0;

	x = _mm_malloc(sizeof(*x) * n, 16);
	y = _mm_malloc(sizeof(*y) * n, 16);

	for (int i = 0; i < n; ++i) {
		x[i] = i * 2 + 1.0;
		y[i] = i;
	}

	daxpy_sse(x, y, a, n);

	double t = wtime();
	for (int i = 0; i < REP; ++i)
		daxpy_sse(x, y, a, n);
	t = (wtime() - t) / REP;

	/* Verification */
	for (int i = 0; i < n; ++i) {
		double xx = i * 2 + 1.0;
		double yy = a * xx + i;
		if (fabs(y[i] - yy) > EPS) {
			fprintf(stderr, "run_vectorized: verification failed (y[%d] = %lf != %lf)\n", i, y[i], yy);
			break;
		}
	}

	printf("Elapsed time (vectorized): %.6f sec.\n", t);
	_mm_free(x);
	_mm_free(y);
	return t;
}

int main(int argc, char **argv)
{
	printf("=========================================\n");
	printf("SAXPY (y[i] = a * x[i] + y[i]; n = %d)\n", n);

	double tscalar_saxpy = run_scalar_saxpy();
	double tvec_saxpy = run_vectorized_saxpy();

	printf("Speedup: %.2f\n", tscalar_saxpy / tvec_saxpy);
	printf("=========================================\n");
	printf("DAXPY (y[i] = a * x[i] + y[i]; n = %d)\n", n);

	double tscalar_daxpy = run_scalar_daxpy();
	double tvec_daxpy = run_vectorized_daxpy();

	printf("Speedup: %.2f\n", tscalar_daxpy / tvec_daxpy);
	printf("=========================================\n");

	return 0;
}
