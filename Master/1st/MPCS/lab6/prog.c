#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PROB(arg, t) (float)(1.0 - powf(M_E, -(arg * t)))

int main(int argc, char *argv[])
{
	if (argc != 4) {
		fprintf(stderr, "Input arguments, dude.\n");
		exit(1);
	}

	srand48((unsigned)time(NULL));

	int i, treq = 0, tdone = 0;

	int    N = atoi(argv[1]);
	float lm = atof(argv[2]);
	float mu = atof(argv[3]);

	static uint32_t qcnt = 0;

	uint32_t lm_t;
	uint32_t mu_t;

	for (i = 0; i < N; ++i, ++lm_t, ++mu_t) {
		float lm_p = drand48();
		float mu_p = drand48();

		if (lm_p < PROB(lm, lm_t)) {
			++treq;
			++qcnt;
			lm_t = 0;
		}

		if (mu_p < PROB(mu, mu_t)) {
			if (qcnt) {
				--qcnt;
				++tdone;
				mu_t = 0;
			}
		}
	}

	printf("#%d : tasks [%d] : busy [%d]\n",
		i, qcnt, (qcnt > 0 ? 1 : 0));

	printf("Tasks (req/done): %d / %d\n",
		treq, tdone);

	return 0;
}
