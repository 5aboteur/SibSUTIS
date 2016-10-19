#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <emmintrin.h>
#include <immintrin.h>
#include <xmmintrin.h>
#include <pmmintrin.h>
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

#define REP 20

enum { n = 100003 };

float sdot(float *x, float *y, int n)
{
	float s = 0;
	for (int i = 0; i < n; ++i)
		s += x[i] * y[i];
	return s;
}

float sdot_sse(float *x, float *y, int n)
{
	__declspec(align(16)) float res = 0;

	__m128 *xx = (__m128 *)x;
	__m128 *yy = (__m128 *)y;

	// init vecs w/ zeroes
	__m128 s = _mm_setzero_ps();
	__m128 tmp = _mm_setzero_ps();

	// float -> 32bit 
	int k = n / 4;

	// mul & sum
	for (int i = 0; i < k; ++i) {
		tmp = _mm_mul_ps(xx[i], yy[i]);
		s = _mm_add_ps(tmp, s);
	}

	s = _mm_hadd_ps(s, s);	// [s3 + s2, ..]
	s = _mm_hadd_ps(s, s);	// [s3 + s2 + s1 + s0, ..]

	_mm_store_ss(&res, s);	// res -> s0

	for (int i = k * 4; i < n; ++i)
		res += x[i] * y[i];

	return res;
}

float sdot_avx(float *x, float *y, int n)
{
	__declspec(align(32)) float res = 0;

	__m256 *xx = (__m256 *)x;
	__m256 *yy = (__m256 *)y;

	// init vecs w/ zeroes
	__m256 s = _mm256_setzero_ps();
	__m256 tmp1 = _mm256_setzero_ps();

	// float -> 32bit
	int k = n / 8;

	// mul & sum
	for (int i = 0; i < k; ++i) {
		tmp1 = _mm256_mul_ps(xx[i], yy[i]);
		s = _mm256_add_ps(tmp1, s);
	}

	s = _mm256_hadd_ps(s, s);				// [s3 + s2, ..]
	s = _mm256_hadd_ps(s, s);				// [s3 + s2 + s1 + s0, ..]

	tmp1 = _mm256_permute2f128_ps(s, s, 1);	// permute low & high bits

	s = _mm256_add_ps(s, tmp1);				// final sum

	__declspec(align(32)) float tmp2[8];
	_mm256_store_ps(tmp2, s);

	res = tmp2[0];

	for (int i = k * 8; i < n; ++i)
		res += x[i] * y[i];

	return res;
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
	float *x = xmalloc(sizeof(*x) * n);
	float *y = xmalloc(sizeof(*y) * n);
	for (int i = 0; i < n; ++i) {
		x[i] = 2.0;
		y[i] = 3.0;
	}

	float res = sdot(x, y, n);

	double t = wtime();
	for (int i = 0; i < REP; ++i)
		res = sdot(x, y, n);
	t = wtime() - t;

	float valid_result = 2.0f * 3.0f * n;
	printf("Result (scalar): %.6f err = %f\n", res, fabsf(valid_result - res));
	printf("Elapsed time (scalar): %.6f sec.\n", t);
	free(x);
	free(y);
	return t;
}

double run_vectorized()
{
	float *x = _mm_malloc(sizeof(*x) * n, 32);
	float *y = _mm_malloc(sizeof(*y) * n, 32);
	for (int i = 0; i < n; ++i) {
		x[i] = 2.0f;
		y[i] = 3.0f;
	}

	float res = sdot_avx(x, y, n);

	double t = wtime();
	for (int i = 0; i < REP; ++i)
		res = sdot_avx(x, y, n);
	t = wtime() - t;

	float valid_result = 2.0f * 3.0f * n;
	printf("Result (vectorized): %.6f err = %f\n", res, fabsf(valid_result - res));
	printf("Elapsed time (vectorized): %.6f sec.\n", t);
	_mm_free(x);
	_mm_free(y);
	return t;
}

int main(int argc, char **argv)
{
	printf("=========================================\n");
	printf("SDOT: n = %d\n", n);
	double tscalar = run_scalar();
	double tvec = run_vectorized();

	printf("Speedup: %.2f\n", tscalar / tvec);
	printf("=========================================\n");

	return 0;
}
