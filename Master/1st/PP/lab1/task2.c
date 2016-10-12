#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <emmintrin.h>
#include <immintrin.h>
#include <xmmintrin.h>
//#include <sys/time.h>

/************************************* WINDOWS **************************************/
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
/************************************* WINDOWS **************************************/

#define EPS 1E-6
#define REP 20

enum {
	n = 10000000
};

void init_particles(float *x, float *y, float *z, int n)
{
	for (int i = 0; i < n; ++i) {
		x[i] = cosf(i + 0.1f);
		y[i] = cosf(i + 0.2f);
		z[i] = cosf(i + 0.3f);
	}
}

void distance(float *x, float *y, float *z, float *d, int n)
{
	for (int i = 0; i < n; ++i) {
		d[i] = sqrtf(x[i] * x[i] + y[i] * y[i] + z[i] * z[i]);
	}
}

void distance_vec(float *x, float *y, float *z, float *d, int n)
{
	__m128 v1, v2, v3;

	__m128 *xx = (__m128 *)x;
	__m128 *yy = (__m128 *)y;
	__m128 *zz = (__m128 *)z;
	__m128 *dd = (__m128 *)d;

	// float -> 32bit
	int k = n / 4;

	for (int i = 0; i < k; ++i) {
		// powering components
		v1 = _mm_mul_ps(xx[i], xx[i]);
		v2 = _mm_mul_ps(yy[i], yy[i]);
		v3 = _mm_mul_ps(zz[i], zz[i]);

		// sum components
		v1 = _mm_add_ps(v1, v2);
		v1 = _mm_add_ps(v1, v3);

		// compute sqrt
		dd[i] = _mm_sqrt_ps(v1);
	}

	// compute tail (if it exist)
	for (int i = k * 4; i < n; ++i) {
		d[i] = sqrtf(x[i] * x[i] + y[i] * y[i] + z[i] * z[i]);
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

double run_scalar()
{
	float *d, *x, *y, *z;

	x = xmalloc(sizeof(*x) * n);
	y = xmalloc(sizeof(*y) * n);
	z = xmalloc(sizeof(*z) * n);
	d = xmalloc(sizeof(*d) * n);

	init_particles(x, y, z, n);

	double t = wtime();
	for (int iter = 0; iter < REP; ++iter) {
		distance(x, y, z, d, n);
	}
	t = wtime() - t;

	printf("Elapsed time (scalar): %.6f sec.\n", t);
	free(x);
	free(y);
	free(z);
	free(d);
	return t;
}

double run_vectorized()
{
	float *d, *x, *y, *z;

	x = _mm_malloc(sizeof(*x) * n, 32);
	y = _mm_malloc(sizeof(*y) * n, 32);
	z = _mm_malloc(sizeof(*z) * n, 32);
	d = _mm_malloc(sizeof(*d) * n, 32);

	init_particles(x, y, z, n);

	double t = wtime();
	for (int iter = 0; iter < REP; ++iter) {
		distance_vec(x, y, z, d, n);
	}
	t = wtime() - t;

	/* Verification */
	for (int i = 0; i < n; i++) {
		float x = cosf(i + 0.1f);
		float y = cosf(i + 0.2f);
		float z = cosf(i + 0.3f);
		float dist = sqrtf(x * x + y * y + z * z);
		if (fabs(d[i] - dist) > EPS) {
			fprintf(stderr, "Verification failed: d[%d] = %f != %f\n", i, d[i], dist);
			break;
		}
	}

	printf("Elapsed time (vectorized): %.6f sec.\n", t);
	_mm_free(x);
	_mm_free(y);
	_mm_free(z);
	_mm_free(d);
	return t;
}

int main(int argc, char **argv)
{
	printf("=========================================\n");
	printf("Particles: n = %d\n", n);
	double tscalar = run_scalar();
	double tvec = run_vectorized();

	printf("Speedup: %.2f\n", tscalar / tvec);
	printf("=========================================\n");

	return 0;
}
