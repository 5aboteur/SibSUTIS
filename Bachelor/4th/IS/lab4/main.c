#include "lib.h"


int main()
{
	long long c[NPLAYERS], d[NPLAYERS], z[NCARDS], u[NCARDS], card[NPLAYERS * 2];
	long long x, y, res = 0, p, q, r, k, l;

	while (1) {
		p = get_prime(_MAX_R, _MIN_R, 1);

//		printf("p: %lli\n", p);

		for (int i = 0; i < NPLAYERS; ++i) {
			c[i] = coprime(p - 1, x, y);
			d[i] = inverse(res, p - 1, x, c[i]);

//			printf("%lli * %lli mod %lli = %lli\n", c[i], d[i], p - 1, c[i] * d[i] % (p - 1));
		}

		for (int i = 0; i < NCARDS; ++i) {
			z[i] = rand() % (p - 1) + 1;

//			printf("%d -> %lli\n", i, z[i]);
		}

		for (int i = 0; i < NCARDS; ++i) u[i] = z[i];

		for (int i = 0; i < NPLAYERS; ++i) {
			for (int j = 0; j < NCARDS; ++j) {
				u[j] = pow_pow(u[j], c[i], p);
			}

//			printf("i: %d, u: ", i);
//			for (int j = 0; j < NCARDS; ++j) {
//				printf("%lli ", u[j]);
//			}
//			printf("\n");

			shuffle_it(u);
		}

		k = 1; l = 1;
		while (k <= NPLAYERS * 2) {
			do {
				r = rand() % NCARDS;
				q = u[r];
			} while (!u[r]);

			u[r] = 0;

			for (int i = 0; i < NPLAYERS; ++i) {
				if (i == NPLAYERS - l) continue;
				q = pow_pow(q, d[i], p);
			}

			q = pow_pow(q, d[NPLAYERS - l], p);
			card[NPLAYERS * 2 - k] = q;
			++k; ++l;

			if (l > NPLAYERS) l = 1;
		}

//		for (int i = 0; i < NCARDS; ++i)
//			printf("\033[32mz[%d]:\033[0m %lli ", i, z[i]);
//		printf("\n");
//
//		for (int i = 0; i < NPLAYERS * 2; ++i)
//			printf("\033[34mcard[%d]:\033[0m %lli ", i, card[i]);
//		printf("\n\n\n\n");

		break;

	}

	print_field(z, card);
	go_five(z, card);

	return 0;
}
