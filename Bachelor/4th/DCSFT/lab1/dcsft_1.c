#include <stdio.h>
#include <stdlib.h>

// M - expected value
// T - recovery time

double exp_val_func(int N, int n, int m, double lambda, double u)
{
	double res = 0;

	for (int j = n + 1; j <= N; ++j)
	{
		double mul = 1;

		for (int l = n; l <= j - 1; ++l)
		{
			double ul;

			if (((N - m) <= l) && (l <= N))
				ul = (N - l) * u;
			else
				ul = m * u;

			mul *= ul / (l * lambda);
		}

		res += 1 / (j * lambda) * mul;
	}

	return res + 1 / (n * lambda);
}

double rec_time_func(int N, int n, int m, double lambda, double u)
{
	double mul1 = 1 / u;

	for (int l = 1; l <= n - 1; ++l)
	{
		double ul;

		if (((N - m) <= l) && (l <= N))
			ul = (N - l) * u;
		else
			ul = m * u;

		mul1 *= (l * lambda) / ul;
	}

	double sum = 0;

	for (int j = 1; j <= n - 1; ++j)
	{
		double mul2 = 1 / (j * lambda);

		for (int l = j; l <= n - 1; ++l)
		{
			double ul;

			if (((N - m) <= l) && (l <= N))
				ul = (N - l) * u;
			else
				ul = m * u;

			mul2 *= (l * lambda) / ul;
		}

		sum += mul2;
	}

	return mul1 + sum;
}

int main()
{
	int N, n, m;
	double lambda, u, M, T;
	char file[6] = "0";
	FILE *fp;

	N = 65536; lambda = 1e-5;  m = 1;

	// experiment 1 ('u' variations)
	for (int i = 1, u = 1; u <= 1000; u *= 10, ++i)
	{
		for (n = 65527; n <= N; ++n)
		{
			T = rec_time_func(N, n, m, lambda, u);
			printf("M = %.9f (u = %d)\n", M, u);
		}
	}
/*
	N = 65536; lambda = 1e-5;  m = 1;

	// experiment 1 ('u' variations)
	for (int i = 1, u = 1; u <= 1000; u *= 10, ++i)
	{
		for (n = 65527; n <= N; ++n)
		{
			M = exp_val_func(N, n, m, lambda, u);
			printf("M = %.9f (u = %d)\n", M, u);
		}
	}

	N = 65536; u = 1;  m = 1;
	int i = 1;
	// experiment 2 ('lambda' variations)
	for (double lambda = 1e-5; lambda >= 1e-9; lambda *= 0.1, ++i)
	{
		for (n = 65527; n <= N; ++n)
		{
			M = exp_val_func(N, n, m, lambda, u);
			printf("M = %.9f (lambda = %.9f)\n", M, lambda);
		}
	}

	N = 65536; lambda = 1e-5; u = 1;

	// experiment 3 ('m' variations)
	for (int i = 1, m = 1; m <= 4; ++m, ++i)
	{
		for (n = 65527; n <= N; ++n)
		{
			M = exp_val_func(N, n, m, lambda, u);
			printf("M = %.9f (m = %d)\n", M, m);
		}
	}

	N = 1000; lambda = 1e-3; m = 1;

	// experiment 4 ('u' variations)
	for (int i = 1, u = 1; u <= 6; u += 2, ++i)
	{
		if (u == 3) u = 2;
		for (n = 900; n <= N; n += 10)
		{
			T = rec_time_func(N, n, m, lambda, u);
			printf("T = %.9f (u = %d)\n", T, u);
		}
	}

	N = 8192; u = 1; m = 1;
	int i = 1;
	// experiment 5 ('lambda' variations)
	for (double lambda = 1e-5; lambda >= 1e-9; lambda *= 0.1, ++i)
	{
		for (n = 8092; n <= N; n += 10)
		{
			T = rec_time_func(N, n, m, lambda, u);
			printf("T = %.9f (lambda = %.9f)\n", T, lambda);
		}
	}

	N = 8192; u = 1; lambda = 1e-5;

	// experiment 6 ('m' variations)
	for (int i = 1, m = 1; m <= 4; ++m, ++i)
	{
		fp = fopen(file, "w");
		for (n = 8092; n <= N; n += 10)
		{
			T = rec_time_func(N, n, m, lambda, u);
			printf("T = %.9f (m = %d)\n", T, m);
			fprintf(fp, "%d %lf\n", n, T);
		}
		fclose(fp);
		file[i] = i + '0';
	}
*/
	return 0;
}
