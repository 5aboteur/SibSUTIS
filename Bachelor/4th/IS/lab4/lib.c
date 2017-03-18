#include "lib.h"


/**************************
*		     	  *
* >> LAB 1 STARTS HERE << *
*		   	  *
**************************/


/***********************
* Euclidean algorithms *
***********************/


unsigned long long gcd(unsigned long long a, unsigned long long b)
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


unsigned long long pow_pow(unsigned long long a, unsigned long long x, unsigned long long m)
{
	unsigned long long r = 1;

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


int is_prime(unsigned long long v)
{
	for (unsigned long long i = 2; i * i < v; ++i)
	{
		if (!(v % i)) return 0;
	}

	return 1;
}


unsigned long long get_prime(unsigned long long max, unsigned long long min, int reset)
{
	unsigned long long p;
	if (reset)
	srand((unsigned)time(NULL));

	do {
		p = rand() % max + min;

		if (!(p % 2)) ++p;

	} while (!is_prime(p));

	return p;
}


/***************************
* Diffie-Hellman algorithm *
***************************/


unsigned long long dh_public(unsigned long long g, unsigned long long priv_key, unsigned long long p)
{
	return pow_pow(g, priv_key, p);
}


unsigned long long dh_shared(unsigned long long pub_key, unsigned long long priv_key, unsigned long long p)
{
	return pow_pow(pub_key, priv_key, p);
}


/*********************************
* Baby-step Giant-step algorithm *
*********************************/


struct bucket {
	unsigned long long val;
	unsigned long long idx;
	int flg;
};

struct bucket *huge_babe;


int compare(const void *a, const void *b)
{
	struct bucket *lval = (struct bucket *) a;
	struct bucket *rval = (struct bucket *) b;

	return (lval->val - rval->val);
}


unsigned long long bstep_gstep(unsigned long long a, unsigned long long p, unsigned long long y)
{
	unsigned long long i, j;
	unsigned long long m = (unsigned long long)sqrt(p + .0);
	unsigned long long k = m + 1;

	while (m * k < p) ++k;

	huge_babe = (struct bucket *) malloc(sizeof(struct bucket) * (m + k));

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


/**************************
*     			  *
* >> LAB 2 STARTS HERE << *
*			  *
**************************/


/*******************
* Shamir algorithm *
*******************/


long long coprime(long long p, long long x, long long y)
{
	long long cx;

	do {
		cx = (rand() % (p - 2) + 1) | 1;
	} while (ext_gcd(p, cx, &x, &y) != 1);

	return cx;
}


long long inverse(long long res, long long p, long long x, long long cx)
{
	long long dx;

	res = ext_gcd(p, cx, &x, &dx);
	if (dx < 0) dx += p;

	return dx;
}


/********************
* ElGamal algorithm *
********************/


void generate_keys(unsigned long long *cx, unsigned long long *dx, unsigned long long p, unsigned long long g)
{
	*cx = rand() % (p - 2) + 2;
	*dx = pow_pow(g, *cx, p);

	return;
}


void elgamal_encrypt(FILE *fp, FILE *fe, unsigned long long p, unsigned long long g, unsigned long long d)
{
	char ch;
	unsigned long long k, r, e;

	while ((ch = fgetc(fp)) != EOF)
	{
		k = rand() % (p - 2) + 1;

		r = pow_pow(g, k, p);
		e = ((int)ch * pow_pow(d, k, p)) % p;

		fprintf(fe, "%lli %lli\n", r, e);
	}

	return;
}


void elgamal_decrypt(FILE *fe, FILE *fd, unsigned long long p, unsigned long long c)
{
	char ch;
	unsigned long long r, e;

	fseek(fe, 0L, SEEK_SET);

	while (!feof(fe))
	{
		fscanf(fe, "%lli", &r);
		fscanf(fe, "%lli", &e);

		ch = (char)((e * pow_pow(r, p - 1 - c, p)) % p);

		fputc(ch, fd);
	}

	return;
}


/*******************
* Vernam algorithm *
*******************/


void otp_encrypt(FILE *fp, FILE *fk, FILE *fe)
{
	char ch, key;
	unsigned long long _size;

	fseek(fp, 0L, SEEK_END);
	_size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	srand((unsigned)time(NULL));

	for (unsigned long long i = 0; i < _size; ++i)
	{
		ch = fgetc(fp);
		key = rand() % CHAR_MAX;
		fputc(key, fk);
		ch ^= key;
		fputc(ch, fe);
	}

	return;
}


void otp_decrypt(FILE *fe, FILE *fk, FILE *fd)
{
	char ch, key;
	unsigned long long _size;

	fseek(fe, 0L, SEEK_END);
	_size = ftell(fe);
	fseek(fe, 0L, SEEK_SET);
	fseek(fk, 0L, SEEK_SET);

	for (unsigned long long i = 0; i < _size; ++i)
	{
		ch = fgetc(fe);
		key = fgetc(fk);
		ch ^= key;
		fputc(ch, fd);
	}

	return;
}


/****************
* RSA algorithm *
****************/

void rsa_encrypt(FILE *fp, FILE *fe, unsigned long long d, unsigned long long n)
{
	char ch;
	unsigned long long e;

	while ((ch = fgetc(fp)) != EOF)
	{
		e = pow_pow((int)ch, d, n);
		fprintf(fe, "%lli\n", e);
	}

	return;
}


void rsa_decrypt(FILE *fe, FILE *fd, unsigned long long c, unsigned long long n)
{
	char ch;
	unsigned long long e;

	fseek(fe, 0L, SEEK_SET);

	while (!feof(fe))
	{
		fscanf(fe, "%lli", &e);

		ch = (char)pow_pow(e, c, n);

		fputc(ch, fd);
	}
	return;
}


/**************************
*     			  *
* >> LAB 3 STARTS HERE << *
*			  *
**************************/


unsigned long long hashy(char *fname)
{
	char ch;
	unsigned long long res = 0, flen = 0, p = 1;

	FILE *fp;
	fp = fopen(fname, "r");

	if (!fp) {
		puts("File doesn't exist.\n");
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	flen = ftell(fp);
	rewind(fp);

	for (unsigned long long i = 0; i < flen; ++i, p *= MAGIC) {
		ch = fgetc(fp);
		res += ((int) ch * p) % ULONG_MAX;
	}

	fclose(fp);

	return res;
}


/**************************
*     			  *
* >> LAB 4 STARTS HERE << *
*			  *
**************************/


void shuffle_it(long long arr[])
{
	long long r, s;
	long long s_arr[NCARDS];

	for (int i = 0; i < NCARDS; ++i) {
		do {
			r = rand() % NCARDS;
			s = arr[r];
		} while (!arr[r]);

		arr[r] = 0;
		s_arr[i] = s;
	}

	for (int i = 0; i < NCARDS; ++i) arr[i] = s_arr[i];

	return;
}

char _DECKY_[13][2] = {
			"02", "03", "04", "05",
			"06", "07", "08", "09",
			"10", "JK", "QN", "KG", "AE"
		      };

char _NICKS_[15][20] = {
			"1337h4k3r", "OlegStalker1994", "breivik_king", "TOXA3000", "Adolfik88",
			"V.V.Putin", "NaGibaToR228", "sportsssman", "FUCK_USA!!!!!!1", "sexy_kisa",
			"Yasha_Lava", "KRbIM_HAW[RUS]", "Sergei_42rus", "__=(^.^)=__", "Mr.Dickson"
		       };

void print_field(long long deck[], long long card[])
{
	int c1, c2, nick;

	printf("\033[30;42m* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ");
	printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
	printf("\033[30;42m* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ");
	printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
	printf("\033[30;42m* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ");
	printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n* * ");

	for (int i = 0; i < NPLAYERS; ++i) {
		for (int j = 0; j < NCARDS; ++j) {
			if (card[i] == deck[j]) {
				c1 = j; deck[j] = 0; break;
			}
		}

		for (int j = 0; j < NCARDS; ++j) {
			if (card[i + NPLAYERS] == deck[j]) {
				c2 = j; deck[j] = 0; break;
			}
		}

		// diamonds
		if (c1 < 13) {
			do {
				nick = rand() % 15;
			} while (_NICKS_[nick][0] == NULL);

			printf("|%20s|", _NICKS_[nick]);
			_NICKS_[nick][0] = NULL;
			printf("\033[1;31;47m %c%c \033[0;30;42m", _DECKY_[c1][0], _DECKY_[c1][1]);
			printf(",");

			// diamonds
			if (c2 < 13) {
				printf("\033[1;31;47m %c%c \033[0;30;42m", _DECKY_[c2][0], _DECKY_[c2][1]);
				printf("|");
			}
			// hearts
			else if (c2 < 26) {
				printf("\033[31;47m %c%c \033[30;42m", _DECKY_[c2 - 13][0], _DECKY_[c2 - 13][1]);
				printf("|");
			}
			// clubs
			else if (c2 < 39) {
				printf("\033[1;30;47m %c%c \033[0;30;42m", _DECKY_[c2 - 26][0], _DECKY_[c2 - 26][1]);
				printf("|");
			}
			// spades
			else {
				printf("\033[30;47m %c%c \033[30;42m", _DECKY_[c2 - 39][0], _DECKY_[c2 - 39][1]);
				printf("|");
			}
		}
		// hearts
		else if (c1 < 26) {
			do {
				nick = rand() % 15;
			} while (_NICKS_[nick][0] == NULL);

			printf("|%20s|", _NICKS_[nick]);
			_NICKS_[nick][0] = NULL;

			printf("\033[31;47m %c%c \033[30;42m", _DECKY_[c1 - 13][0], _DECKY_[c1 - 13][1]);
			printf(",");

			// diamonds
			if (c2 < 13) {
				printf("\033[1;31;47m %c%c \033[0;30;42m", _DECKY_[c2][0], _DECKY_[c2][1]);
				printf("|");
			}
			// hearts
			else if (c2 < 26) {
				printf("\033[31;47m %c%c \033[30;42m", _DECKY_[c2 - 13][0], _DECKY_[c2 - 13][1]);
				printf("|");
			}
			// clubs
			else if (c2 < 39) {
				printf("\033[1;30;47m %c%c \033[0;30;42m", _DECKY_[c2 - 26][0], _DECKY_[c2 - 26][1]);
				printf("|");
			}
			// spades
			else {
				printf("\033[30;47m %c%c \033[30;42m", _DECKY_[c2 - 39][0], _DECKY_[c2 - 39][1]);
				printf("|");
			}
		}
		// clubs
		else if (c1 < 39) {
			do {
				nick = rand() % 15;
			} while (_NICKS_[nick][0] == NULL);

			printf("|%20s|", _NICKS_[nick]);
			_NICKS_[nick][0] = NULL;

			printf("\033[1;30;47m %c%c \033[0;30;42m", _DECKY_[c1 - 26][0], _DECKY_[c1 - 26][1]);
			printf(",");

			// diamonds
			if (c2 < 13) {
				printf("\033[1;31;47m %c%c \033[0;30;42m", _DECKY_[c2][0], _DECKY_[c2][1]);
				printf("|");
			}
			// hearts
			else if (c2 < 26) {
				printf("\033[31;47m %c%c \033[30;42m", _DECKY_[c2 - 13][0], _DECKY_[c2 - 13][1]);
				printf("|");
			}
			// clubs
			else if (c2 < 39) {
				printf("\033[1;30;47m %c%c \033[0;30;42m", _DECKY_[c2 - 26][0], _DECKY_[c2 - 26][1]);
				printf("|");
			}
			// spades
			else {
				printf("\033[30;47m %c%c \033[30;42m", _DECKY_[c2 - 39][0], _DECKY_[c2 - 39][1]);
				printf("|");
			}
		}
		// spades
		else {
			do {
				nick = rand() % 15;
			} while (_NICKS_[nick][0] == NULL);

			printf("|%20s|", _NICKS_[nick]);
			_NICKS_[nick][0] = NULL;

			printf("\033[30;47m %c%c \033[30;42m", _DECKY_[c1 - 39][0], _DECKY_[c1 - 39][1]);
			printf(",");

			// diamonds
			if (c2 < 13) {
				printf("\033[1;31;47m %c%c \033[0;30;42m", _DECKY_[c2][0], _DECKY_[c2][1]);
				printf("|");
			}
			// hearts
			else if (c2 < 26) {
				printf("\033[31;47m %c%c \033[30;42m", _DECKY_[c2 - 13][0], _DECKY_[c2 - 13][1]);
				printf("|");
			}
			// clubs
			else if (c2 < 39) {
				printf("\033[1;30;47m %c%c \033[0;30;42m", _DECKY_[c2 - 26][0], _DECKY_[c2 - 26][1]);
				printf("|");
			}
			// spades
			else {
				printf("\033[30;47m %c%c \033[30;42m", _DECKY_[c2 - 39][0], _DECKY_[c2 - 39][1]);
				printf("|");
			}
		}

//		printf("\t\t\t");

		if (!((i + 1) % 3)) printf("\033[0;32;42m%20d\033[30;42m| * *\n* *\033[0;32;42m%118d\033[30;42m * *\n* * ", 1, 1);
	}
	printf("\033[30;42m* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ");
	printf("\033[30;42m* * * * * * * * * * * * * * * * * * * * * * * * *\n");
//	printf("\033[0m\n\n\n\n");
	sleep(2);
}


void go_five(long long deck[], long long card[])
{
	int r;

	printf("\033[30;42m* * * * * * * * * * * * ");

	for (int i = 0; i < 5; ++i) {
		do {
			r = rand() % 51;
		} while (deck[r] == 0);

		deck[r] = 0;

		// diamonds
		if (r < 13) {
			printf("\033[1;31;47m %c%c \033[0;30;42m", _DECKY_[r][0], _DECKY_[r][1]);
		}
		// hearts
		else if (r < 26) {
			printf("\033[31;47m %c%c \033[30;42m", _DECKY_[r - 13][0], _DECKY_[r - 13][1]);
		}
		// clubs
		else if (r < 39) {
			printf("\033[1;30;47m %c%c \033[0;30;42m", _DECKY_[r - 26][0], _DECKY_[r - 26][1]);
		}
		// spades
		else {
			printf("\033[30;47m %c%c \033[30;42m", _DECKY_[r - 39][0], _DECKY_[r - 39][1]);
		}

		if (i != 4) printf(",");
	}

	printf("\033[30;42m  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
	printf("\033[30;42m* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ");
	printf("\033[30;42m* * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
	sleep(2);

	printf("\033[30;42m* * * * * * * * * * * * * * * ");
	printf("DRUZHBA WON :)");
	printf("\033[30;42m  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
	printf("\033[30;42m* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ");
	printf("\033[30;42m* * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
	printf("\033[30;42m* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ");
	printf("\033[30;42m* * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
	printf("\033[30;42m* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ");
	printf("\033[30;42m* * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
	sleep(2);

	return;
}
