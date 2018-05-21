#include <mpi.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "../../tool/src/tool.h"

#define EAGER_MSG_SIZE 10
#define RNDV_MSG_SIZE 1000000

static int ITERS;

int test_eager(int rank, int size);
int test_rndv(int rank, int size);

int main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	int rank, commsize;

	if (argc < 3) {
		fprintf(stderr, "Usage: ./prog <niters> <proto>\n");
		exit(1);
	}

	ITERS = atoi(argv[1]);

	// Init MPI
	TRY (MPI_Init(&argc, &argv));

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &commsize);

	if (commsize < 2) {
		fprintf(stderr, "Need at least two processes. Quit..\n");
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		exit(1);
	}

	struct timeval start, end;

	gettimeofday(&start, NULL);

	// Select test
	if (!strcmp(argv[2], "--eager") || !strcmp(argv[2], "-e")) {
		TRY (test_eager(rank, commsize));
	} else if (!strcmp(argv[2], "--rendezvous") || !strcmp(argv[2], "-r")) {
		TRY (test_rndv(rank, commsize));
	} else {
		fprintf(stderr, "Specify test you want to run.\n");
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		exit(1);
	}

	gettimeofday(&end, NULL);

	if (rank == 0) {
		const double time_taken = (((double)end.tv_sec) + (double)end.tv_usec * 1.0e-6) -
			(((double)start.tv_sec) + (double)start.tv_usec * 1.0e-6);
		const double msgs_recv = ((double)(ITERS * (commsize - 1)));
		const double data_recv = (((double)ITERS) * ((double)EAGER_MSG_SIZE) *
			((double)(commsize - 1))) / (1024.0 * 1024.0);

		printf("# Statistics:\n");
		printf("# %20s %20s %20s\n", "Time Taken", "Msgs/s", "MB/s");
		printf("  %20.6f %20f %20.6f\n", time_taken, msgs_recv / time_taken,
			data_recv / time_taken);
	}

	TRY (MPI_Finalize());

	return 0;
}

int test_eager(int rank, int size)
{
	TRY (MPI_Barrier(MPI_COMM_WORLD));

	printf("Eager test, %d in\n", rank);

	int i, j, p, err;

	// Sender side
	if (rank != 0) {
		for (i = 0; i < ITERS; i++) {
			int tag = rank;
			size_t send_buf[EAGER_MSG_SIZE];

//			MPI_Request req;

			for (j = 0; j < EAGER_MSG_SIZE; j++)
				send_buf[j] = rand() % 666;

//			printf("Message size: %li\n", EAGER_MSG_SIZE * sizeof(int));

			TRY (MPI_Send(send_buf, EAGER_MSG_SIZE, MPI_INT, 0, tag, MPI_COMM_WORLD));
//			TRY (MPI_Isend(send_buf, EAGER_MSG_SIZE, MPI_INT, 0, tag, MPI_COMM_WORLD, &req));
		}
	}
	// Receiver side
	else {
		for (p = 1; p < size; p++) {
			for (i = 0; i < ITERS; i++) {
				MPI_Status status;
				int tag = p;
				size_t recv_buf[EAGER_MSG_SIZE];

//				MPI_Request req;

				for (j = 0; j < EAGER_MSG_SIZE; j++)
					recv_buf[j] = -1;

				TRY (MPI_Recv(recv_buf, EAGER_MSG_SIZE, MPI_INT, p, tag, MPI_COMM_WORLD, &status));
//				TRY (MPI_Irecv(recv_buf, EAGER_MSG_SIZE, MPI_INT, p, tag, MPI_COMM_WORLD, &req));
//				TRY (MPI_Wait(&req, MPI_STATUS_IGNORE));
			}
		}
	}
/*
	if (rank != 0)
	{
		printf("\n<%d> -- Message delivered!\n\n", rank);
	}
	else
	{
		printf("\n<%d> -- Message received!\n\n", rank);
	}
*/

	return MPI_SUCCESS;
}

int test_rndv(int rank, int size)
{
	TRY (MPI_Barrier(MPI_COMM_WORLD));

	printf("Rendezvous test, %d in\n", rank);

	int i, j, p, err;

	// Sender side
	if (rank != 0) {
		for (i = 0; i < ITERS; i++) {
			int tag = rank;
			size_t send_buf[RNDV_MSG_SIZE];

//			MPI_Request req;

			for (j = 0; j < RNDV_MSG_SIZE; j++)
			{
				send_buf[j] = rand() % 777;
			}

//			printf("Message size: %li\n", EAGER_MSG_SIZE * sizeof(int));

			TRY (MPI_Send(send_buf, RNDV_MSG_SIZE, MPI_INT, 0, tag, MPI_COMM_WORLD));
//			TRY (MPI_Isend(send_buf, EAGER_MSG_SIZE, MPI_INT, 0, tag, MPI_COMM_WORLD, &req));
		}
	}
	// Receiver side
	else {
		for (p = 1; p < size; p++) {
			for (i = 0; i < ITERS; i++) {
				MPI_Status status;
				int tag = p;
				size_t recv_buf[RNDV_MSG_SIZE];

//				MPI_Request req;

				for (j = 0; j < RNDV_MSG_SIZE; j++)
				{
					recv_buf[j] = -1;
				}

				TRY (MPI_Recv(recv_buf, RNDV_MSG_SIZE, MPI_INT, p, tag, MPI_COMM_WORLD, &status));
//				TRY (MPI_Irecv(recv_buf, EAGER_MSG_SIZE, MPI_INT, p, tag, MPI_COMM_WORLD, &req));
//				TRY (MPI_Wait(&req, MPI_STATUS_IGNORE));
			}
		}
	}
/*
	if (rank != 0)
	{
		printf("\n<%d> -- Message delivered!\n\n", rank);
	}
	else
	{
		printf("\n<%d> -- Message received!\n\n", rank);
	}
*/

	return MPI_SUCCESS;
}
