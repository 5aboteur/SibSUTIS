#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

#define MAX_THR 8		// num of threads
#define ASIZE 300000		// alphabet size

typedef unsigned long long int ll;

int NUM_THR = 2;		// cur num of threads
int flg1 = 0;			// is there even one substr?
int flg2 = 0;			// need more options?

int opt_one = 0;		// for case 1
int opt_two = 0;		// for case 2

ll FIRST_DELIM;			// for separated zones
ll dump_arr[ASIZE];		// just for dump stuff
ll dump_cnt = 0;		// that's too

FILE *fin;			// input file ptr
FILE *fout;			// output file ptr

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
	char pat_str[ASIZE];	// pattern string
	char txt_str[ASIZE];	// text string

	ll idx;			// index of position
	ll delim;		// delimeter
	int tnum;		// thread number
};

ll getdel(ll m) {
	ll j, res = 0;
	for (j = 0; j < m; j++)
		res = j*2;
	return res;
}

void dump_options(struct XYN *xyn, int i) {
	printf("\n\033[33m PATTERN:\033[0m %s\n", xyn[i].pat_str);
	printf("\033[33m TEXT:\033[0m %s\n", xyn[i].txt_str);
	printf("\033[33m INDEX:\033[0m %lld\n", xyn[i].idx);
	printf("\033[33m DELIM:\033[0m %lld\n\n", xyn[i].delim);
}

void dump_visualization(char *txt, ll n, ll m) {
	ll i, j, k, temp;

	for (i = 0; i < dump_cnt; i++) {
		for (j = i + 1; j < dump_cnt; j++) {
			if (dump_arr[i] > dump_arr[j]) {
				temp = dump_arr[i];
				dump_arr[i] = dump_arr[j];
				dump_arr[j] = temp;
			}
		}
	}

//	for (i = 0; i < dump_cnt; i++)
//		printf("%d ", dump_arr[i]);
	printf("\n\033[33m VISUALIZATION: \033[0m");
	for (i = 0, k = 0; i < n; i++) {
		if (i == dump_arr[k]) {
			printf("\033[31m%c\033[0m", txt[i]);
			k++;
		}
		else
			printf("%c", txt[i]);
	}
}

void dump_menu() {
	printf("\n\033[32m==================================================\n");
	printf("\033[33m #1\033[32m: FTTT (from term to term)\n");
	printf("\033[33m #2\033[32m: FFTF (from file to file)\n");
	printf("\033[33m #3\033[32m: QUIT\n");
	printf("-------\n");
	printf("\033[33m #4\033[32m: Change num of threads: [%d]\n", NUM_THR);
	printf("\033[33m #5\033[32m: Clear screen\n");
	printf("\033[33m #6\033[32m: Show details: [%d]\n", flg2);
	printf("==================================================\n");
	printf("->");
	printf("\033[0m");
}

// output the position where substring starts
void output(ll res, int tn) {
	if (opt_one == 1 && opt_two == 0) {
		printf("\033[33m = Position: %lld; thread #%d\n\033[0m", res + 1, tn);
		flg1 = 1;
		if (flg2) {
			dump_arr[dump_cnt] = res;
			dump_cnt++;
		}
	}
	else if (opt_two == 1 && opt_one == 0) {
		fprintf(fout, "Position: %lld; thread #%d\n", res + 1, tn);
		flg1 = 1;
	}
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

	ll i = 0, j = 0, m = 0, n = 0;
	int choice;

	char pat[ASIZE];
	char txt[ASIZE];

	struct XYN xyn[MAX_THR];

	pthread_t threads[MAX_THR];

////////////////////////////////////////////////////////////////////////////////////////

	while (1) {
		dump_cnt = 0;
		for (i = 0; i < ASIZE; i++)
			dump_arr[i] = 0;

		flg1 = 0;
		opt_one = 0;
		opt_two = 0;

		dump_menu();
		scanf("%d", &choice);

		switch (choice) {
		case 4:
			printf("\033[33m = Input new value: \033[0m");
			scanf("%d", &NUM_THR);
			break;
		case 5:
			printf("\033[2J\033[0;0H");
			break;
		case 6:
			flg2 = (!flg2);
			break;
		case 1:
			opt_one = 1;
			printf("\033[33m = Input string-pattern: \033[0m");
			scanf("%s", pat);
			printf("\033[33m = Input string-text: \033[0m");
			scanf("%s", txt);

			m = strlen(pat);
			n = strlen(txt);

			if (m > n) {
				printf("\033[33m = ERROR: length of pattern > length of text.");
				return -1;
			}

			FIRST_DELIM = n / NUM_THR;

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

					if (flg2)
						dump_options(xyn, i);
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

						if (flg2)
							dump_options(xyn, i);
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

				if (flg2)
					dump_options(xyn, 0);
			}

			if (flg1 == 0)
				printf("\033[33m = Substring is not found!\n\033[0m");
			if (flg2 == 1)
				dump_visualization(txt, n, m);
			break;
		case 2:
			opt_two = 1;
			fin = fopen("input.txt", "r");
			fout = fopen("output.txt", "w");

			printf("\033[33m = Input string-pattern: \033[0m");
			scanf("%s", pat);

			char ch;
			ll cnt = 0;

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
			printf(" \033[33mElapsed time: %ld ms..\033[0m\n", time_stop());
//			fprintf(fout, "Elapsed time: %ld ms..\n ", time_stop()); // in ms

			fclose(fin);
			fclose(fout);
			break;
		case 3:
			// just quit
			return 0;
		}
	}
	return 0;
}
