#include "lib.h"

char _CLS[3] = { 'R', 'B', 'Y' };

void generate_rand_graph()
{
	long long n, m;

	FILE *frg;
	frg = fopen("rgfile.in", "w");

	n = (rand() % 30) + 20;
	m = (n - 2) * 2 + 1;

	fprintf(frg, "%lli %lli\n", n, m);

	for (long long i = 0; i < n - 2; ++i) {
		fprintf(frg, "%lli %lli\n", i, i + 1);
		fprintf(frg, "%lli %lli\n", i, i + 2);
	}

	for (long long i = 0; i < n; ++i)
		fprintf(frg, "%c", _CLS[i % 3]);

	fclose(frg);
	return;
}

struct vertex {
	long long idx;

	long long p;
	long long q;
	long long c;
	long long d;
	long long N;
	long long r;
	long long z;
	long long phi;

	int col;
};

struct edge {
	struct vertex *a;
	struct vertex *b;
};

struct vertex *V;
struct edge *E;

int main()
{
	srand((unsigned)time(NULL));

	FILE *fin, *fop;
	fop = fopen("frr.pi", "w");

	int A = 100;
	char ch;

	long long n, m, a, b, e, x, y, v1, v2, cnt = 0, phi, res = 0, z1, z2, p, q, lie = 0;
	long long zz1, zz2, rc;

	int cls[3] = { 0, 1, 2 };

//	generate_rand_graph();

	fin = fopen("file.in", "r");
	fscanf(fin, "%lli%lli", &n, &m);

	cnt = A * m;

	printf("cnt: %lli, n: %lli, m: %lli\n*****\n", cnt, n, m);

	V = (struct vertex *) malloc (sizeof(struct vertex) * n);
	E = (struct edge *) malloc (sizeof(struct edge) * m);

	for (long long i = 0; i < m; ++i) {
		fscanf(fin, "%lli %lli", &a, &b);
		E[i].a = (& V[a]);
		E[i].b = (& V[b]);

		printf("\033[1;34mEDGE [%lli]: %lli --- %lli\033[0m\n", i, a, b);
	}

	fscanf(fin, "%c", &ch);

	for (long long i = 0; i < n; ++i) {
		V[i].idx = i;
		fscanf(fin, "%c", &ch);

		switch (ch) {
		case 'R':
			V[i].col = 0; break;
		case 'B':
			V[i].col = 1; break;
		case 'Y':
			V[i].col = 2; break;
		default:
			printf("Wrong!\n");
			return -1;
		}

		printf("\033[1;33mVERTEX (%lli): %c\033[0m\n", i, _CLS[V[i].col]);
	}

	fclose(fin);

	printf("*****\n\n");

	// a * |E|
	while (cnt--) {
		printf("COLORS (before shuffle): '%c', '%c', '%c'\n", _CLS[cls[0]], _CLS[cls[1]], _CLS[cls[2]]);
		shuffle_it(cls);
		printf("COLORS (after shuffle): '%c', '%c', '%c'\n", _CLS[cls[0]], _CLS[cls[1]], _CLS[cls[2]]);

		for (long long i = 0; i < n; ++i) {
			switch (V[i].col) {
			case 0:
				V[i].col = cls[0]; break;
			case 1:
				V[i].col = cls[1]; break;
			case 2:
				V[i].col = cls[2]; break;
			default:
				return -1;
			}
		}

		printf("*****\n");

		for (long long i = 0; i < n; ++i)
			printf("\033[1;33mVERTEX (%lli): %c\033[0m (shuffled)\n", i, _CLS[V[i].col]);

		printf("*****\n");

		// Alice`s routine
		for (long long i = 0; i < n; ++i) {
			V[i].p = get_prime(_MAX_R, _MIN_R, 0);
			V[i].q = get_prime(_MAX_R, _MIN_R, 0);

			V[i].N = V[i].p * V[i].q;
			V[i].phi = (V[i].p - 1) * (V[i].q - 1);

//			printf("p: %lli, q: %lli, N: %lli, phi: %lli\n", p, q, V[i].N, V[i].phi);

			V[i].c = coprime(V[i].phi, x, y);
			V[i].d = inverse(res, V[i].phi, x, V[i].c);

			printf("V[%lli] -> (c * d mod phi): %lli * %lli mod %lli = %lli\n", i, V[i].c, V[i].d, V[i].phi, V[i].c * V[i].d % V[i].phi);

			V[i].r = (((rand() % (100)) + 3) & ~3) | V[i].col;
/*
			printf("V[%lli] -> r: %lli (before)\n", i, V[i].r);

			switch (V[i].col) {
			case 0:
				V[i].r >>= 2;
				V[i].r <<= 2;	// ..xx00
				break;
			case 1:
				V[i].r >>= 2;
				V[i].r <<= 2;
				V[i].r |= 1;	// ..xx01
				break;
			case 2:
				V[i].r >>= 2;
				V[i].r <<= 2;
				V[i].r |= 2;	// ..xx10
				break;
			default:
				return -1;
			}

			printf("V[%lli] -> r: %lli (after)\n", i, V[i].r);
*/
			V[i].z = pow_pow(V[i].r, V[i].d, V[i].N);
			printf("V[%lli] -> z: %lli\n", i, V[i].z);
			printf("**********************************\n");
		}

		// Bob`s routine
		for (long long i = 0; i < m; ++i)
			printf("\033[1;34mEDGE [%lli]: %lli ---- %lli\033[0m\n", i, E[i].a->idx, E[i].b->idx);

		e = rand() % m; // index of E

		printf("BOB`S CHOSEN EDGE: E[%lli], vertices: (%lli) ---- (%lli)\n", e, E[e].a->idx, E[e].b->idx);
//		v1 = E[e].a->idx;
//		v2 = E[e].b->idx;

		printf("*****\n");

		printf("V[%lli] -> z: %lli, c: %lli, N: %lli\n", E[e].a->idx, E[e].a->z, E[e].a->c, E[e].a->N);
		printf("V[%lli] -> z: %lli, c: %lli, N: %lli\n", E[e].b->idx, E[e].b->z, E[e].b->c, E[e].b->N);

		printf("*****\n");

		z1 = pow_pow(E[e].a->z, E[e].a->c, E[e].a->N);
		zz1 = z1;
		z2 = pow_pow(E[e].b->z, E[e].b->c, E[e].b->N);
		zz2 = z2;

		if ((z1 & 3) == 3 || (z2 & 3) == 3) {
			fprintf(fop, "a:::::\n");
			fprintf(fop, "z1 -> %lli (must be equal with r)...... col: %d, p -> %lli, q -> %lli\n", zz1, E[e].a->col, E[e].a->p, E[e].a->q);

			rc = pow_pow(E[e].a->r, E[e].a->c, E[e].a->N);
			fprintf(fop, "===r ^ c: %lli===\n", rc);
			rc = pow_pow(rc, E[e].a->d, E[e].a->N);
			fprintf(fop, "===rc ^ d: %lli===\n", rc);

			fprintf(fop, ">>>phi: %lli\n\n", E[e].a->phi);

			fprintf(fop, "z: %lli, r: %lli, c: %lli, d: %lli, c * d mod phi: %lli, N: %lli\n",
					E[e].a->z, E[e].a->r, E[e].a->c, E[e].a->d, E[e].a->c * E[e].a->d % E[e].a->phi, E[e].a->N);
			fprintf(fop, "b:::::\n");
			fprintf(fop, "z2 -> %lli (must be equal with r).....col: %d, p -> %lli, q -> %lli\n", zz2, E[e].b->col, E[e].b->p, E[e].b->q);
			fprintf(fop, "z: %lli, r: %lli, c: %lli, d: %lli, c * d mod phi: %lli, N: %lli\n\n",
					E[e].b->z, E[e].b->r, E[e].b->c, E[e].b->d, E[e].b->c * E[e].b->d % E[e].b->phi, E[e].b->N);
		};
//		fprintf(fop, "z1: %lli, z2: %lli\n", z1, z2);
//		printf("z1: %lli, z2: %lli \033[1;31mBUT\033[0m ", z1, z2);
//		printf("V[%lli] -> r: %lli, V[%lli] -> r: %lli\n", E[e].a->idx, E[e].a->r, E[e].b->idx, E[e].b->r);

//		if (z1 != V[v1].r || z2 != V[v2].r) { printf("Something`s going wrong!\n"); return -1; }
//		if ( (((z1 >> 0) & 1) == ((z2 >> 0) & 1)) && (((z1 >> 1) & 1) == ((z2 >> 1) & 1)) ){ printf("Liar :P\n"); return 0; }
		if (z1 == z2) { printf("You're liar!\n"); ++lie; }
	}

	switch (lie) {
	case 0:
		printf("\n*****\nCNT: %lli, LIE:\033[1;32m %lli\033[0m\n*****\n", A * m, lie); break;
	case 1:
		printf("\n*****\nCNT: %lli, LIE:\033[1;33m %lli\033[0m\n*****\n", A * m, lie); break;
	default:
		printf("\n*****\nCNT: %lli, LIE:\033[1;31m %lli\033[0m\n*****\n", A * m, lie); break;
	}

	return 0;
}
