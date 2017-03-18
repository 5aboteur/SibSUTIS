#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

#define MAX_THR 8		// num of threads
#define ASIZE 500050	// alphabet size

typedef unsigned long long int ll;

int NUM_THR = 8;		// cur num of threads
int flg1 = 0;			// is there even one substr?

ll FIRST_DELIM;			// for separated zones

FILE *fin;			// input file ptr
FILE *fout;			// output file ptr
FILE *fwrd;			// for pattern word

////////////////////////////////////////////////////////////////////////////////////////
struct timeval tv1, tv2, dtv;
struct timezone tz;
void time_start() { gettimeofday(&tv1, &tz); }
long time_stop() {
	gettimeofday(&tv2, &tz);
	dtv.tv_sec = tv2.tv_sec - tv1.tv_sec;
	dtv.tv_usec = tv2.tv_usec - tv1.tv_usec;
	if (dtv.tv_usec < 0) { dtv.tv_sec--; dtv.tv_usec += 1000000; }
	return dtv.tv_sec * 1000 + dtv.tv_usec / 1000;
}
////////////////////////////////////////////////////////////////////////////////////////

// mystruct
struct XYN {
//	char pat_str[ASIZE];	// pattern string
//	char txt_str[ASIZE];	// text string
	char *pat_str;
	char *txt_str;

	ll idx;			// index of position
	ll delim;		// delimeter
	int tnum;		// thread number
};

// get delimeter for separated zones
ll getdel(ll m) {
	ll j, res = 0;
	for (j = 0; j < m; j++)
		res = j*2;
	return res;
}

// output the position where substring starts
void output(ll res, int tn) {
//	fprintf(fout, "Position: %lld; thread #%d\n", res + 1, tn);
	flg1 = 1;
}

// quick search algorithm
void *BF(void *s) {
	struct XYN *xyn;
	xyn  = (struct XYN *) s;

	ll d;
	d = xyn->delim;

	ll len;
	len = strlen(xyn->pat_str);

	ll idx;
	idx = xyn->idx;

	ll i, j;

	for (i = 0; i < d; i++) {
		for (j = 0; j < len; j++) {
			if (xyn->txt_str[j + i] == xyn->pat_str[j])
				continue;
			else
				break;
		}
		if (j == len)
			output(i + idx, xyn->tnum);
	}
	pthread_exit(NULL);
	return;
}

int main() {

// VARIABLES BLOCK ////////////////////////////////////////////////////////////////////

	char ch;
	ll cnt = 0;
	ll i = 0, j = 0, m = 0, n = 0;

//	char pat[ASIZE];
//	char txt[ASIZE];
	char *pat;
	char *txt;
	struct XYN xyn[MAX_THR];

	pthread_t threads[MAX_THR];

////////////////////////////////////////////////////////////////////////////////////////

	fin = fopen("input.txt", "r");
	fwrd = fopen("word.txt", "r");
	fout = fopen("output.txt", "w");

	pat = (char*)malloc(ASIZE*sizeof(char));
	txt = (char*)malloc(ASIZE*sizeof(char));

	cnt = 0;
	while (!feof(fwrd)) {
		fscanf(fwrd, "%c", &ch);
		if (ch == '\n') break;
		pat[cnt] = ch;
		cnt++;
	}
	pat[cnt] = '\0';

	cnt = 0;
	while (!feof(fin)) {
		fscanf(fin, "%c", &ch);
		txt[cnt] = ch;
		cnt++;
	}
	txt[cnt] = '\0';

	m = strlen(pat);
	n = strlen(txt);

	if (m > n) {
		printf("\033[33m = ERROR: length of pattern > length of text.");
		return -1;
	}

	FIRST_DELIM = n / NUM_THR;
	for (i = 0; i < MAX_THR; i++) {
		xyn[i].pat_str = (char*)malloc(FIRST_DELIM*sizeof(char));
		xyn[i].txt_str = (char*)malloc(FIRST_DELIM*sizeof(char));
	}
	time_start();
	if (n / m >= 2 && FIRST_DELIM >= m) {
		// main checks
		for (i = 0; i < NUM_THR; i++) {
			xyn[i].tnum = i + 1;
			xyn[i].delim = n / NUM_THR;
			xyn[i].idx = i*xyn[i].delim;

			if (i + 1 == NUM_THR && n % NUM_THR != 0)
				xyn[i].delim = strlen(&txt[xyn[i].idx]);

			for (j = 0; j < m; j++)
				xyn[i].pat_str[j] = pat[j];
			xyn[i].pat_str[j] = '\0';

			for (j = 0; j < xyn[i].delim; j++)
				xyn[i].txt_str[j] = txt[xyn[i].idx + j];
			xyn[i].txt_str[j] = '\0';

			pthread_create(&threads[i], NULL, BF, &xyn[i]);
		}

		for (i = 0; i < NUM_THR; i++)
			pthread_join(threads[i], NULL);

		// another checks in separated zones
		if (m != 1) {
			ll step = 0;
			for (i = 0; i < NUM_THR - 1; i++) {
				xyn[i].tnum = 0;
				xyn[i].delim = getdel(m);
				xyn[i].idx = FIRST_DELIM - (m - 1) + step;

				for (j = 0; j < m; j++)
					xyn[i].pat_str[j] = pat[j];
				xyn[i].pat_str[j] = '\0';

				for (j = 0; j < xyn[i].delim; j++)
					xyn[i].txt_str[j] = txt[xyn[i].idx + j];
				xyn[i].txt_str[j] = '\0';

				pthread_create(&threads[i], NULL, BF, &xyn[i]);
				step += FIRST_DELIM;
			}

			for (i = 0; i < NUM_THR - 1; i++)
				pthread_join(threads[i], NULL);
		}
	}
	else {
		xyn[0].tnum = 1;
		xyn[0].delim = n;
		xyn[0].idx = 0;

		for (j = 0; j < m; j++)
			xyn[0].pat_str[j] = pat[j];
		xyn[0].pat_str[j] = '\0';

		for (j = 0; j < n; j++)
			xyn[0].txt_str[j] = txt[j];
		xyn[0].txt_str[j] = '\0';


		pthread_create(&threads[0], NULL, BF, &xyn[0]);
		pthread_join(threads[0], NULL);
	}

	if (flg1 == 0)
		fprintf(fout, "Substring is not found!\n");
/*
	for (i = 0; txt[i] != '\0'; i++)
		fprintf(fout, "%c", txt[i]);
	fprintf(fout, "\n");

	for (i = 0; pat[i] != '\0'; i++)
		fprintf(fout, "%c", pat[i]);
	fprintf(fout, "\n");
*/
	fprintf(fout, "Elapsed time: %ld ms..\n ", time_stop()); // in ms

	fclose(fin);
	fclose(fout);
	fclose(fwrd);
	free(pat);
	free(txt);
	return 0;
}
