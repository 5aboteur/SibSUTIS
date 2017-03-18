#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INF 10

long int fact(int n) {
	long int result = 1;

	for (int i = 2; i <= n; ++i) {
		result *= i;
	}

	return result;
}


// Pi
double Pfunc(int N, int n, int m, int i, int t, double u, double lambda)
{
	if (m == 1) {
		double sum = 0.0;

		for (int l = 0; l <= N; ++l) {
			sum += (pow(u / lambda, l) * (1.0 / (double)fact(l)));
		}

		return ((pow(u / lambda, i) * (1.0 / (double) fact(i)) * (1.0 / sum)));
	}
	else {
		return ((double)(fact(N) / ((double)fact(N - i) * (double)fact(i))) *
			((pow(u, i) * pow(lambda, N - i)) / pow(lambda + u, N)));
	}
}

// Qi(t)
double QfuncR(int N, int n, int m, int i, int t, double u, double lambda)
{
/*
	int delta1 = N - i - m;
	if (delta1 >= 0) delta1 = 1;
	else delta1 = 0;

	int delta2 = m - N + i;
	if (delta2 >= 0) delta2 = 1;
	else delta2 = 0;
*/
//	for (int l = 0; l <= INF; ++l) {
//		ul = (pow(u * t, l) / fact(l)) * ((delta1 * pow(m, l) * exp(-m * u * t)) +
//			(delta2 * pow(N - i, l) * exp(-(N - i) * u * t)));



//	}

//	return Qt;
}

// Qi(t)
double QfuncU(int N, int n, int m, int i, int t, double u, double lambda)
{
	double Qt = 0.0, pir = 0.0, ul;



	return Qt;
}

// R*(t)
double Rfunc(int N, int n, int m, int t, double u, double lambda)
{
	double R = 0.0;
	double Qt = 0.0, pir = 0.0, ul;

	for (int i = n; i <= N; ++i) {
//		R += (Pfunc(N, n, m, i, t, u, lambda) * QfuncR(N, n, m, i, t, u, lambda));
		pir = 0.0;
		for (int r = 0; r <= i - n; ++r) {
			if (!t && !r)
				pir += (1.0 / (double)fact(r)) * exp(-i * lambda * t);
			else
				pir += (pow(i * lambda * t, r) / (double)fact(r)) * exp(-i * lambda * t);
		}

		R += Pfunc(N, n, m, i, t, u, lambda) * pir;
	}
	return R;
}

// U*(t)
double Ufunc(int N, int n, int m, int t, double u, double lambda)
{
	double U = 0.0;
	double Qt = 0.0, pir = 0.0, ul;


	for (int i = 0; i <= n - 1; ++i) {
		double delta1;
		if (N - i - m >= 0) delta1 = 1.0;
		else delta1 = 0.0;

		double delta2;
		if (m - N + i >= 0) delta2 = 1.0;
		else delta2 = 0.0;

		ul = 0.0;

		for (int l = 0; l <= n - i - 1; ++l) {
			if (!t && !l && !(N - i))
				ul += (1.0 / (double)fact(l)) * ((delta1 * pow(m, l) * exp(-i * u * t)) +
					(delta2 * 1.0 * exp(-(N - i) * u * t)));
			else if (!t && !l && (N - i))
				ul += (1.0 / (double)fact(l)) * ((delta1 * pow(m, l) * exp(-i * u * t)) +
					(delta2 * pow(N - i, l) * exp(-(N - i) * u * t)));
			else if (t && !l && !(N - i))
				ul += (pow(u * t, l) / (double)fact(l)) * ((delta1 * pow(m, l) * exp(-i * u * t)) +
					(delta2 * 1.0 * exp(-(N - i) * u * t)));
			else
				ul += (pow(u * t, l) / (double)fact(l)) * ((delta1 * pow(m, l) * exp(-i * u * t)) +
					(delta2 * pow(N - i, l) * exp(-(N - i) * u * t)));
		}
//		printf("u = %lf\n", ul);
//		Qt = Pfunc(N, n, m, i, t, u, lambda);
//		printf("P = %lf\n", Qt);
		U += Pfunc(N, n, m, i, t, u, lambda) * ul;
	}
	return 1.0 - U;
}

// S(t)
double Sfunc(int N, int n, int m, int t, double u, double lambda)
{
	if (m == 1) {
		double sum = 0.0;

		for (int j = 0; j <= n - 1; ++j)
			sum += Pfunc(N, n, m, j, t, u, lambda);

		return 1 - sum;
	}
	else {
		return 1 - (pow(lambda, N - n + 1) * pow(lambda + u, -(N - n + 1)));
	}
}

int main() {

	int N, n, m, t, j;
	double lambda, u, res;
	char file[6] = "0";

	FILE *fp;
	// exp 1
/*	N = 10; m = 1; u = 0.71; lambda = 0.024;
	for (j = 1, n = 8; n <= 10; ++n, ++j) {
		fp = fopen(file, "w");
		for (t = 0; t <= 24; t += 2) {
			res = Rfunc(N, n, m, t, u, lambda);
			printf("R*(%d) = %lf\n", t, res);
			fprintf(fp, "%d %lf\n", t, res);
		}
		fclose(fp);
		file[j] = n + '0';
	}

	N = 16; m = 1; u = 0.71; lambda = 0.024;
	for (j = 1, n = 10; n <= 16; n += 3, ++j) {
		fp = fopen(file, "w");
		for (t = 0; t <= 24; t += 2) {
			res = Ufunc(N, n, m, t, u, lambda);
			printf("U*(%d) = %lf\n", t, res);
			fprintf(fp, "%d %lf\n", t, res);
		}
		fclose(fp);
		file[j] = j + '0';
	}
*/
	N = 16; u = 0.71; lambda = 0.024; m = 16;
	for (n = 11; n <= 16; ++n) {
		res = Sfunc(N, n, m, t, u, lambda);
		printf("S(%d) = %lf\n", n, res);
	}


/*
	// exp 2
	N = 10; n = 9; m = 1; u = 0.71; lambda = 0.024;
	for (t = 0; t <= 24; t += 2) {
		res = Rfunc(N, n, m, t, u, lambda);
		printf("R*(%d) = %lf\n", t, res);
	}

	// exp 3
	N = 10; n = 10; m = 1; u = 0.71; lambda = 0.024;
	for (t = 0; t <= 24; t += 2) {
		res = Rfunc(N, n, m, t, u, lambda);
		printf("R*(%d) = %lf\n", t, res);
	}

	// exp 4
	N = 16; n = 10; m = 1; u = 0.71; lambda = 0.024;
	for (t = 0; t <= 24; t += 2) {
		res = Ufunc(N, n, m, t, u, lambda);
		printf("U*(%d) = %lf\n", t, res);
	}

	// exp 5
	N = 16; n = 13; m = 1; u = 0.71; lambda = 0.024;
	for (t = 0; t <= 24; t += 2) {
		res = Ufunc(N, n, m, t, u, lambda);
		printf("U*(%d) = %lf\n", t, res);
	}

	// exp 6
	N = 16; n = 16; m = 1; u = 0.71; lambda = 0.024;
	for (t = 0; t <= 24; t += 2) {
		res = Ufunc(N, n, m, t, u, lambda);
		printf("U*(%d) = %lf\n", t, res);
	}
*/
	return 0;
}
