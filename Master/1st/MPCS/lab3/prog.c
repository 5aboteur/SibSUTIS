#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int *pipe;	// conveyor

/* Sync pipe routine*/
int sync_routine(int nd, int nt, int ns)
{
	int i, j, /*k,*/ details_left = nd;
	int *stages_buf = (int *)malloc(ns * sizeof(int));
	int total_tics = 0;

	while (1) {
		for (i = 0; i < ns; ++i) {
			if (!i && !stages_buf[0] && details_left) {
				pipe[0] = 1;
				--details_left;
			}

			if (!stages_buf[i] && pipe[i] == 1) {
				stages_buf[i] = nt;
				pipe[i] = 0;
			}

			if (stages_buf[i] <= nt && stages_buf[i] > 0) {
				--stages_buf[i];

				if (!stages_buf[i]) {
					if (!pipe[i + 1])
						pipe[i + 1] = 1;
					else
						stages_buf[i] = 1;
				}
			}

			if (i == ns - 1)
				pipe[ns] = 0;
/*
			printf("[%d]: ", i);
			for (k = 0; k < ns; ++k)
				printf("%d ", pipe[k]);
			printf("\n");
*/
		}

		++total_tics;

		for (i = 0, j = 0; i < ns; ++i) {
			if (!stages_buf[i])
				++j;
		}

		if (!details_left && j == ns)
			break;
	}

	return total_tics;
}

/* Async pipe routine */
int async_routine(int nd, int nt, int ns)
{
	int i, j, /*k,*/ details_left = nd;
	int *stages_buf = (int *)malloc(ns * sizeof(int));
	int total_tics = 0;

	while (1) {
		for (i = 0; i < ns; ++i) {
			if (!i && !stages_buf[0] && details_left) {
				pipe[0] = 1;
				--details_left;
			}

			if (!stages_buf[i] && pipe[i] == 1) {
				stages_buf[i] = 5 + rand() % 6;
				pipe[i] = 0;
			}

			if (stages_buf[i] <= nt && stages_buf[i] > 0) {
				--stages_buf[i];

				if (!stages_buf[i]) {
					if (!pipe[i + 1])
						pipe[i + 1] = 1;
					else
						stages_buf[i] = 1;
				}
			}

			if (i == ns - 1)
				pipe[ns] = 0;
/*
			printf("[%d]: ", i);
			for (k = 0; k < ns; ++k)
				printf("%d ", pipe[k]);
			printf("\n");
*/
		}

		++total_tics;

		for (i = 0, j = 0; i < ns; ++i) {
			if (!stages_buf[i])
				++j;
		}

		if (!details_left && j == ns)
			break;
	}

	return total_tics;
}

int main(int argc, char *argv[])
{
	if (argc != 4) {
		printf("Input args, dude: ndetails, ntacts, nstages.\n");
		return -1;
	}

	srand((unsigned)time(NULL));

	int ndetails, ntacts, nstages;
//	clock_t start, t_sync, t_async;
	int t_sync, t_async;

	// Init stuff
	ndetails = atoi(argv[1]);
	ntacts = atoi(argv[2]);
	nstages = atoi(argv[3]);

	// --- SYNC ---------

	pipe = (int *)malloc(nstages * sizeof(int));

//	start = clock();
	t_sync = sync_routine(ndetails, ntacts, nstages);
//	t_sync = clock() - start;

	printf("T_SYNC: %d tics\n", t_sync);

	// --- ASYNC ---------

//	memset(&pipe, 0, nstages * sizeof(int));

//	start = clock();
	t_async = async_routine(ndetails, ntacts, nstages);
//	t_async = clock() - start;

	printf("T_ASYNC: %d tics\n", t_async);

	printf("T_SYNC / T_ASYNC = %.5lf\n", (double) t_sync / t_async);

//	free(pipe);
	return 0;
}
