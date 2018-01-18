#include <mpi.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../tool/src/tool.h"

#define EAGER_MSG_SIZE 10
#define RNDV_MSG_SIZE 1000000
#define ITERS 1000

pthread_t plot_thr;

int test_eager(int rank, int size);
int test_rndv(int rank, int size);

int main(int argc, char *argv[])
{
	int rank, commsize;

	TRY (MPI_Init(&argc, &argv));

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &commsize);

	if (commsize < 2)
	{
		fprintf(stderr, "Need at least two processes. Quit..\n");
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		exit(1);
	}

	// Create plot routine
	if (rank == 0)
	{
		int *total_msg_cnt = (int *)malloc(sizeof(int));
		*total_msg_cnt = ITERS * (commsize - 1);

		if (pthread_create(&plot_thr, NULL, plot, (void *)total_msg_cnt))
		{
			fprintf(stderr, "Cannot create plot thread.\n");
		}
	}

	// Select test
	if (!strcmp(argv[1], "--eager") || !strcmp(argv[1], "-e"))
	{
		TRY (test_eager(rank, commsize));
	}
	else if (!strcmp(argv[1], "--rendezvous") || !strcmp(argv[1], "-r"))
	{
		TRY (test_rndv(rank, commsize));
	}
	else
	{
		fprintf(stderr, "Specify test you want to run.\n");
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		exit(1);
	}

	// Wait 'til plot is done
	if (rank == 0)
	{
		if (pthread_join(plot_thr, NULL))
		{
			fprintf(stderr, "Cannot join plot thread.\n");
		}
	}

	TRY (MPI_Finalize());

	return 0;
}

int test_eager(int rank, int size)
{
	TRY (MPI_Barrier(MPI_COMM_WORLD));

	printf("Eager test, %d in\n", rank);

	int i, j, p, err;

	srand((unsigned)time(NULL));

	// Sender side
	if (rank != 0)
	{
		for (i = 0; i < ITERS; i++)
		{
			int tag = rank;
			size_t send_buf[EAGER_MSG_SIZE];

//			MPI_Request req;

			for (j = 0; j < EAGER_MSG_SIZE; j++)
			{
				send_buf[j] = rand() % 666;
			}

//			printf("Message size: %li\n", EAGER_MSG_SIZE * sizeof(int));

			TRY (MPI_Send(send_buf, EAGER_MSG_SIZE, MPI_INT, 0, tag, MPI_COMM_WORLD));
//			TRY (MPI_Isend(send_buf, EAGER_MSG_SIZE, MPI_INT, 0, tag, MPI_COMM_WORLD, &req));
		}
	}
	// Receiver side
	else
	{
		for (p = 1; p < size; p++)
		{
			for (i = 0; i < ITERS; i++)
			{
				MPI_Status status;
				int tag = p;
				size_t recv_buf[EAGER_MSG_SIZE];

//				MPI_Request req;

				for (j = 0; j < EAGER_MSG_SIZE; j++)
				{
					recv_buf[j] = -1;
				}

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

	srand((unsigned)time(NULL));

	// Sender side
	if (rank != 0)
	{
		for (i = 0; i < ITERS; i++)
		{
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
	else
	{
		for (p = 1; p < size; p++)
		{
			for (i = 0; i < ITERS; i++)
			{
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
