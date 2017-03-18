#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int N = 30;

int main()
{
	int avg1 = 0, avg2 = 0;
	double ksi[N], tau[N], sig1[N], w1[N], sig2[N], w2[N];

	FILE *fp = fopen("res3", "w");

	w1[0] = w2[0] = 0.0;
	for (int i = 0; i < N; ++i) {
		ksi[i] = rand() / (double) RAND_MAX;
		tau[i] = sqrt(ksi[i]) * 3.1337 / 14.88;
		sig1[i] = (tau[i] + ksi[i]) / (1.228 + ksi[i] * 0.0666);
		sig2[i] = sig1[i];
//		if (i) {
//			w[i] = w[i - 1] - tau[i] + sig[i - 1];
//			avg += w[i];
//		}
	}

	for (int i = 0; i < N - 1; ++i) {
		for (int j = 0; j < N - i - 1; ++j) {
			if (sig1[j] > sig1[j + 1]) {
				double tmp = sig1[j];
        sig1[j] = sig1[j + 1];
        sig1[j + 1] = tmp;
			}
			if (sig2[j] < sig2[j + 1]) {
				double tmp = sig2[j];
        sig2[j] = sig2[j + 1];
        sig2[j + 1] = tmp;
			}
		}
	}

	for (int i = 1; i < N; ++i) {
		w1[i] = w1[i - 1] - tau[i] + sig1[i - 1];
		avg1 += w1[i];
		w2[i] = w2[i - 1] - tau[i] + sig2[i - 1];
		avg2 += w2[i];
	}

	fprintf(fp, "[KSI]\n\n");
	for (int i = 0; i < N; ++i) fprintf(fp, "%.10lf\n", ksi[i]);
	fprintf(fp, "\n\n");
	fprintf(fp, "[TAU]\n\n");
	for (int i = 0; i < N; ++i) fprintf(fp, "%.10lf\n", tau[i]);
	fprintf(fp, "\n\n");
	fprintf(fp, "[SIG1]\n\n");
	for (int i = 0; i < N; ++i) fprintf(fp, "%.10lf\n", sig1[i]);
	fprintf(fp, "\n\n");
	fprintf(fp, "[W1]\n\n");
	for (int i = 0; i < N; ++i) fprintf(fp, "%.10lf\n", w1[i]);
	fprintf(fp, "\n\n");
	fprintf(fp, "%.10lf\n\n", avg1 / (double) N);
	fprintf(fp, "[SIG2]\n\n");
	for (int i = 0; i < N; ++i) fprintf(fp, "%.10lf\n", sig2[i]);
	fprintf(fp, "\n\n");
	fprintf(fp, "[W2]\n\n");
	for (int i = 0; i < N; ++i) fprintf(fp, "%.10lf\n", w2[i]);
	fprintf(fp, "\n\n");
	fprintf(fp, "%.10lf\n\n", avg2 / (double) N);

	return 0;
}
