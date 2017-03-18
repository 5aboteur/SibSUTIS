#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NIL (printf("\033[0m"))
#define RED (printf("\033[32m"))
#define GRN (printf("\033[33m"))
#define PUR (printf("\033[35m"))

/*
 ************ prog1 ************
 * generate 30000 random values
 * dump 100 of them
 * find Mx & Dx
 * find mx & dx
 * ???
 * PROFIT
 *******************************
 */

/*
 ***********************
 * X | -2  | -1  | 25
 * p | 0.8 | 0.1 | 0.1
 ***********************
 */

int mas[30000];
int i = 0, j = 0, r = 0;
float Mx = 0.0, Dx = 0.0, mx = 0.0, dx = 0.0, sum = 0.0, sum2 = 0.0;
float cnt1 = 0.0, cnt2 = 0.0, cnt3 = 0.0, res1 = 0.0, res2 = 0.0, res3 = 0.0;

int main() {
	printf("============================================================\n");
	printf("First 100 values of sequence: ");
	srand(time(NULL));

	for (i = 0; i < 30000; ++i, ++j) {
		r = rand() % 10;
		if (r == 0) {
			mas[i] = -1;
			++cnt2;
			if (i < 100) RED;
		}
		else if (r == 1) {
			mas[i] = 25;
			++cnt3;
			if (i < 100) GRN;
		}
		else {
			mas[i] = -2;
			++cnt1;
			if (i < 100) PUR;
		}

		if (i < 100) {
			if (j % 10 == 0)
				printf("\n");
			printf(" [%d] ", mas[i]);
		}
		if (i == 100) NIL;

		sum += mas[i];
		sum2 += mas[i] * mas[i];
	}
	printf("\n\n");

	Mx = ((-2*0.8) + (-1*0.1) + (25*0.1));
	printf(" Mx = %.2f\n", Mx);
	Dx = ((4*0.8) + (1*0.1) + (625*0.1) - (Mx*Mx));
	printf(" D[Mx] = %.2f\n", Dx);

	mx = sum / 30000.0;
	printf(" mx = %.6f ~ Mx\n", mx);
	dx = ((sum2 / 30000.0) - (mx * mx));
	printf(" d[mx] = %.6f ~ D[Mx]\n", dx);

	printf("\nRelative frequency:\n");

	res1 = cnt1 / 30000.0;
	printf(" p(-2) = %.6f ~ 0.8\n", res1);
	res2 = cnt2 / 30000.0;
	printf(" p(-1) = %.6f ~ 0.1\n", res2);
	res3 = cnt3 / 30000.0;
	printf(" p(25) = %.6f ~ 0.1\n", res3);

	printf("============================================================\n");

	return 0;
}
