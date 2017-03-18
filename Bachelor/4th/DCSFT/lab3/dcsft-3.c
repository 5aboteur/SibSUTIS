#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define NLAMBDA	3
#define NMU	4
#define NM	3
#define NN	10

int get_mu_l(int l, int m, int mu, int N, int *mu_l) {
	if (l >= (N - m)) {
		*mu_l = (N - l) * mu;
	} else {
		*mu_l = m * mu;
	}
}

double theta(double lambda, int mu, int m, int n, int N) {
	double result = 0.0, temp;
	int j, l, mu_l;

	for (j = n + 1; j <= N; ++j) {
		temp = 1.0;
		for (l = n; l <= j - 1; ++l) {
			get_mu_l(l, m, mu, N, &mu_l);
			temp *= ((double)mu_l / (l * lambda));
		}
		result += ((1.0 / (j * lambda)) * temp);
	}
	return (result + (1.0 / (n * lambda)));
}

double t(double lambda, int mu, int m, int n, int N) {
	double result = 1.0 / mu, temp;
	int j, l, mu_l;

	if (n == 1) {
		return result;
	}

	for (l = 1; l <= n - 1; ++l) {
		get_mu_l(l, m, mu, N, &mu_l);
		result *= ((l * lambda) / (double)mu_l);
	}

	for (j = 1; j <= n - 1; ++j) {
		temp = 1.0;
		for (l = j; l <= n - 1; ++l) {
			get_mu_l(l, m, mu, N, &mu_l);
			temp *= ((l * lambda) / (double)mu_l);
		}
		result += ((1.0 / (j * lambda)) * temp);
	}
	return result;
}

int main() {
	double res1, res2;
	int i, j, k, l, _n, N = 15536;
	double lambda[NLAMBDA] = {0.000001, 0.0000001, 0.00001};
	int mu[NMU] = {1, 100, 100, 1000};
	int m[NM] = {1, 2, 3};
	int n[NN] = {15527, 15528, 15529, 15530, 15531, 15532, 15533, 15534,
				15535, 15536};

	FILE *fp;

	for (i = 0; i < NLAMBDA; ++i) {
		for (j = 0; j < NMU; ++j) {
			for (k = 0; k < NM; ++k) {
				for (l = 0; l < NN; ++l) {
					printf("lambda = %lf \t|\t mu = %d \t|\t m = %d \t|\t n = %d\n",
						lambda[i], mu[j], m[k], n[l]);

					printf("THETA = { ");
					for (_n = n[l]; _n <= N; ++_n) {
						res1 = theta(lambda[i], mu[j], m[k], _n, N);
						printf("%.3lf ", res1);
					}
					printf("}\n");

					printf("T = { ");
					for (_n = n[l]; _n <= N; ++_n) {
						res2 = t(lambda[i], mu[j], m[k], _n, N);
						printf("%.3lf ", res2);
					}
					printf("}\n");

					printf("==========\n");
				}
			}
		}
	}

	return 0;
}
