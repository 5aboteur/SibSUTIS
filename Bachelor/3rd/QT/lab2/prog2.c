#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NIL (printf("\033[0m"))
#define RED (printf("\033[32m"))
#define GRN (printf("\033[33m"))
#define PUR (printf("\033[35m"))

/*
 ************ prog2 ************
 * generate 10000 random values
 * dump 100 of them
 * find Mx & Dx
 * find mx & dx
 * ???
 * PROFIT
 *******************************
 */

/*
 ***********************
 * f(x):
 * --------------------
 * 0.1x, (4 < x < 6)
 * 0, else
 ***********************
 */

float mas[10000];
int i = 0, j = 0;
float Mx = 0.0, Dx = 0.0, mx = 0.0, dx = 0.0, sum = 0.0, sum2 = 0.0;
float r = 0.0, z = 0.0;

float integral(int a, int b, int d) {
	return (0.1 * pow(b, d) / d) - (0.1 * pow(a, d) / d);
}

int main() {
	printf("==========================================================================================\n");
	printf("First 100 values of sequence: ");
	srand(time(NULL));

	for (i = 0; i < 10000; ++i, ++j) {
		r = rand() % 1000 * 0.001;
		z = sqrt((r + 0.8) / 0.05);
		mas[i] = z;

		if (i < 100) {
			if (j % 10 == 0)
				printf("\n");
			printf(" [%.3f] ", mas[i]);
		}
		if (i == 100) NIL;

		sum += mas[i];
		sum2 += mas[i] * mas[i];
	}
	printf("\n\n");

	Mx = integral(4, 6, 3);
	printf(" Mx = %.2f\n", Mx);
	Dx = integral(4, 6, 4) - Mx*Mx;
	printf(" D[Mx] = %.2f\n", Dx);

	mx = sum / 10000.0;
	printf(" mx = %.6f ~ Mx\n", mx);
	dx = sum2 / 10000.0 - mx * mx;
	printf(" d[mx] = %.6f ~ D[Mx]\n", dx);

	printf("==========================================================================================\n");

	return 0;
}
