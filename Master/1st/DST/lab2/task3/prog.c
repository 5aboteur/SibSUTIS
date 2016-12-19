#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <xmmintrin.h>

#define SIZE 8

void foo(float *v1, float *v2)
{
	for (int i = 0; i < SIZE; ++i) {
//		v1[i] = (float)rand() / (float)RAND_MAX;
//		v2[i] = (float)rand() / (float)RAND_MAX;
		v1[i] = 2;
		v2[i] = 4;

	}
}

void bar(float *v1, float *v2, float *v3)
{
	__m128 *vv1 = (__m128 *)v1, *vv2 = (__m128 *)v2,
			*vv3 = (__m128 *)v3;

	for (int i = 0; i < SIZE / 4; ++i) {
		__m128 t1 = _mm_mul_ps(vv1[i], vv2[i]); 
		vv3[i] = _mm_sqrt_ps(t1);
	}
//		v3[i] = sqrtf(v1[i] * v2[i]);
}

void print(float *v1, float *v2, float *v3)
{
	printf("\033[1;32mVec1:\n*****\n\033[0m");
	for (int i = 0; i < SIZE; ++i) {
		if (!(i % 10) && i)
			printf("\n");
		printf("%5.2f ", v1[i]);
	}
	printf("\n");

	printf("\033[1;32mVec2:\n*****\n\033[0m");
	for (int i = 0; i < SIZE; ++i) {
		if (!(i % 10) && i)
			printf("\n");
		printf("%5.2f ", v2[i]);
	}
	printf("\n");

	printf("\033[1;32mVec3:\n*****\n\033[0m");
	for (int i = 0; i < SIZE; ++i) {
		if (!(i % 10) && i)
			printf("\n");
		printf("%5.2f ", v3[i]);
	}
	printf("\n");
}

int main()
{
	srand((unsigned)time(NULL));

	float vec1[SIZE], vec2[SIZE], vec3[SIZE];

	foo(vec1, vec2);
	bar(vec1, vec2, vec3);

	print(vec1, vec2, vec3);

	return 0;
}
