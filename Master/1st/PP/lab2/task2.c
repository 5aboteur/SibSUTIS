#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
#define EPS 1E-6

enum {
	n = 1000007
};

void compute_sqrt(float *in, float *out, int n)
{
	for (int i = 0; i < n; i++)
		out[i] = in[i] > 0 ? sqrtf(in[i]) : 0.0f;
}

void compute_sqrt_sse(float *in, float *out, int n)
{
	__m128 *in_vec  = (__m128 *)in;
	__m128 *out_vec = (__m128 *)out;

	__m128 zero = _mm_setzero_ps();

	// float -> 32bit
	int k = n / 4;

	for (int i = 0; i < k; ++i) {
		__m128 v = _mm_load_ps((float *)&in_vec[i]);
		__m128 sqrt_vec = _mm_sqrt_ps(v);
		__m128 mask = _mm_cmpgt_ps(v, zero);
		__m128 gtzero_vec = _mm_and_ps(mask, sqrt_vec);
		__m128 lezero_vec = _mm_andnot_ps(mask, zero);

		out_vec[i] = _mm_or_ps(gtzero_vec, lezero_vec);
	}

	// compute tail (if it exist)
	for (int i = k * 4; i < n; ++i)
		out[i] = in[i] > 0 ? sqrtf(in[i]) : 0.0f;
}

void compute_sqrt_avx(float *in, float *out, int n)
{
	__m256 *in_vec = (__m256 *)in;
	__m256 *out_vec = (__m256 *)out;

	__m256 zero = _mm256_setzero_ps();

	// float -> 32bit
	int k = n / 8;

	for (int i = 0; i < k; ++i) {
		__m256 v = _mm256_load_ps((float *)&in_vec[i]);
		__m256 sqrt_vec = _mm256_sqrt_ps(v);
		__m256 mask = _mm256_cmp_ps(v, zero, _CMP_GT_OQ);

		out_vec[i] = _mm256_blendv_ps(zero, sqrt_vec, mask);
	}

	// compute tail (if it exist)
	for (int i = k * 8; i < n; ++i)
		out[i] = in[i] > 0 ? sqrtf(in[i]) : 0.0f;
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
	float *in = xmalloc(sizeof(*in) * n);
	float *out = xmalloc(sizeof(*out) * n);

	srand(0);

	for (int i = 0; i < n; i++) {
		in[i] = rand() > RAND_MAX / 2 ? 0 : rand() / (float)RAND_MAX * 1000.0f;
	}

	//double t = wtime();
	//compute_sqrt(in, out, n);
	//t = wtime() - t;
	double start = clock();
	compute_sqrt(in, out, n);
	double t = (clock() - start) / 1000.0f;

#if 0
	for (int i = 0; i < n; i++)
		printf("%.4f ", out[i]);
	printf("\n");
#endif

	printf("Elapsed time (scalar): %.6f sec.\n", t);

	free(in);
	free(out);

	return t;
}

double run_vectorized()
{
	float *in = _mm_malloc(sizeof(*in) * n, 32);
	float *out = _mm_malloc(sizeof(*out) * n, 32);

	srand(0);

	for (int i = 0; i < n; i++) {
		in[i] = rand() > RAND_MAX / 2 ? 0 : rand() / (float)RAND_MAX * 1000.0f;
	}

	//double t = wtime();
	//compute_sqrt_sse(in, out, n);
	//t = wtime() - t;
	double start = clock();
	compute_sqrt_avx(in, out, n);
	double t = (clock() - start) / 1000.0f;

#if 0
	for (int i = 0; i < n; i++)
		printf("%.4f ", out[i]);
	printf("\n");
#endif

	for (int i = 0; i < n; i++) {
		float r = in[i] > 0 ? sqrtf(in[i]) : 0.0f;
		if (fabs(out[i] - r) > EPS) {
			fprintf(stderr, "Verification: FAILED at out[%d] = %f != %f\n", i, out[i], r);
			break;
		}
	}

	printf("Elapsed time (vectorized): %.6f sec.\n", t);

	_mm_free(in);
	_mm_free(out);

	return t;
}

int main(int argc, char **argv)
{
	printf("Tabulate sqrt: n = %d\n", n);
	double tscalar = run_scalar();
	double tvec = run_vectorized();

	printf("Speedup: %.2f\n", tscalar / tvec);

	return 0;
}
