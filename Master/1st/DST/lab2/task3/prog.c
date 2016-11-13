#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1024

void foo(float *v1, float *v2)
{
	for (int i = 0; i < SIZE; ++i) {
		v1[i] = (float)rand() / (float)RAND_MAX;
		v2[i] = (float)rand() / (float)RAND_MAX;
	}
}

void bar(float *v1, float *v2, float *v3)
{
	for (int i = 0; i < SIZE; ++i)
		v3[i] = sqrtf(v1[i] * v2[i]);
}
/*
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
*/
int main()
{
	srand((unsigned)time(NULL));

	float vec1[SIZE], vec2[SIZE], vec3[SIZE];

	foo(vec1, vec2);
	bar(vec1, vec2, vec3);

//	print(vec1, vec2, vec3);

	return 0;
}
