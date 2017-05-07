#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EPS 200

#define MAX(a, b) ((a > b) ? (a) : (b))
#define PROB(arg, t) (float)(1.0 - powf(M_E, -(arg * t)))

int main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Input arguments, dude.\n");
		exit(1);
	}

	srand48((unsigned)time(NULL));

	FILE *fp;

	uint32_t i, cnt = 0, tfail = 0, tdone = 0, downtime = 0;

	float E = 0.0f, C1 = 1.0f, C2 = 0.5f, C3 = 0.15f, C4 = 0.85f;

	uint32_t N = atoi(argv[1]);
	float	lm = atof(argv[2]);
	float	mu = 0.01f;
	float	_mu = mu;

	static uint32_t busy = 0;

	uint32_t lm_t;
	uint32_t mu_t;

	if (!(fp = fopen("result.log", "w"))) {
		fprintf(stderr, "Cannot create the file.\n");
		exit(1);
	}

	while (cnt++ < EPS) {
		for (i = 0, lm_t = 0, mu_t = 0; i < N; ++i, ++lm_t, ++mu_t) {
			float lm_p = drand48();
			float pr_p = drand48();

			if ((lm_p < PROB(lm, lm_t)) && !busy) {
				busy = 1;
				lm_t = 0;
			} else if (busy)
				++tfail;

			if ((pr_p < PROB(mu, mu_t)) && busy) {
				++tdone;
				busy = 0;
				mu_t = 0;
			}

			if (busy) ++downtime;
		}

		E = fabs((C1 - C2) * tdone - C3 * tfail - C4 * downtime);
		mu = MAX(_mu, mu);

		fprintf(fp, "%.2f %.2f\n", mu, E);

		_mu += 0.01f;
	}

	printf("Tasks (done / fail): %d / %d\n", tdone, tfail);
	printf("mu = %f\n", mu);

	fclose(fp);

	return 0;
}
