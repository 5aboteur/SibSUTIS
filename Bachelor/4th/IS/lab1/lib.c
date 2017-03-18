#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define _MAX_R 1333
#define _MIN_R 666


 /**************************
 * 		     	   *
 * >> LAB 1 STARTS HERE << *
 *	             	   *
 **************************/


 /***********************
 * Euclidean algorithms *
 ***********************/


long long gcd(long long a, long long b)
{
	return b ? gcd(b, a % b) : a;
}


long long ext_gcd(long long a, long long b, long long *x, long long *y)
{
	if (!a)
	{
		*x = 0;
		*y = 1;

		return b;
	}

	long long q = 0;

	long long u[3] = { a, 1, 0 };
	long long v[3] = { b, 0, 1 };
	long long t[3] = { 0 };

	while (v[0])
	{
		q = u[0] / v[0];

		t[0] = u[0] % v[0];
		t[1] = u[1] - q * v[1];
		t[2] = u[2] - q * v[2];

		for (int i = 0; i < 3; ++i)
		{
			u[i] = v[i];
			v[i] = t[i];
		}
	}

	*x = u[1];
	*y = u[2];

	return u[0];
}


 /***********************************
 * Modular exponentiation algorithm *
 ***********************************/


long long pow_pow(long long a, long long x, long long m)
{
	long long r = 1;

	while (x)
	{
		if (x & 1) r = (r * a) % m;

		a = (a * a) % m;
		x >>= 1;
	}

	return r;
}


 /***************************
 * Prime number computation *
 ***************************/


int is_prime(long long v)
{
	for (int i = 2; i * i < v; ++i)
	{
		if (!(v % i)) return 0;
	}

	return 1;
}


long long get_prime(long long max, long long min)
{
	long long p;

	srand(time(NULL));

	do {
		p = rand() % max + min;

		if (!(p % 2)) ++p;

	} while (!is_prime(p));

	return p;
}


 /***************************
 * Diffie-Hellman algorithm *
 ***************************/


long long dh_public(long long g, long long priv_key, long long p)
{
	return pow_pow(g, priv_key, p);
}


long long dh_shared(long long pub_key, long long priv_key, long long p)
{
	return pow_pow(pub_key, priv_key, p);
}


 /*********************************
 * Baby-step Giant-step algorithm *
 *********************************/


struct bucket {
	long long val;
	long long idx;
	int flg;
};

struct bucket *huge_babe;

int compare(const void *a, const void *b)
{
	struct bucket *lval = (struct bucket *) a;
	struct bucket *rval = (struct bucket *) b;

	return (lval->val - rval->val);
}

long long bstep_gstep(long long a, long long p, long long y)
{
	long long i, j;
	long long m = (long long) sqrt(p + .0);
	long long k = m + 1;

	while (m * k < p) ++k;

	huge_babe = (struct bucket *) malloc (sizeof(struct bucket) * (m + k));

	for (i = 0; i < m; ++i)
	{
		huge_babe[i].val = (y * pow_pow(a, i, p)) % p;
		huge_babe[i].idx = i;
		huge_babe[i].flg = 0;
	}
	for (j = 1; i < m + k; ++i, ++j)
	{
		huge_babe[i].val = pow_pow(a, j * m, p);
		huge_babe[i].idx = j;
		huge_babe[i].flg = 1;
	}

	qsort(huge_babe, m + k, sizeof(struct bucket), compare);

	for (i = 0; i < m + k; ++i)
	{
		if ((huge_babe[i].val == huge_babe[i + 1].val) &&
		    (huge_babe[i].flg != huge_babe[i + 1].flg))
		{
			if (huge_babe[i].flg)
			{
				return (huge_babe[i].idx * m - huge_babe[i + 1].idx);
			}
			else
			{
				return (huge_babe[i + 1].idx * m - huge_babe[i].idx);
			}
		}
	}

	return -1;
}
