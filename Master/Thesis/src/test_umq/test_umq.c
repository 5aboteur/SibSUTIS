#include <mpi.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "../tool/src/tool.h"

#define EAGER_MSG_SIZE 10
//#define RNDV_MSG_SIZE 1000000
#define ITERS 1000

double plot_values[10000];

pthread_t plot_thr;

int test_eager(int rank, int size);
//int test_rndv(int rank; int size);

int main(int argc, char *argv[])
{
	int rank, size;

	TRY (MPI_Init(&argc, &argv));

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (size < 2)
	{
		printf("Need at least two processes. Quit..\n");
		MPI_Abort(MPI_COMM_WORLD, err);
		exit(1);
	}

	// Create plot routine
	if (rank == 0)
	{
		int *total_msg_cnt = (int *)malloc(sizeof(int));
		*total_msg_cnt = ITERS * (size - 1);

		if (pthread_create(&plot_thr, NULL, plot, (void *)total_msg_cnt))
		{
			fprintf(stderr, "Cannot create plot thread.\n");
		}
	}

	TRY (test_eager(rank, size));

	// Wait 'til plot is done
	if (rank == 0)
	{
		if (pthread_join(plot_thr, NULL))
		{
			fprintf(stderr, "Cannot join plot thread.\n");
		}
	}

	TRY (MPI_Barrier(MPI_COMM_WORLD));
	TRY (MPI_Finalize());

	return 0;
}

int test_eager(int rank, int size)
{
	TRY (MPI_Barrier(MPI_COMM_WORLD));
	printf("%d in\n", rank);
	int i, j, p, err;

	// Sender side
	if (rank != 0)
	{
		for (i = 0; i < ITERS; i++)
		{
			int tag = rank;
			size_t send_buf[EAGER_MSG_SIZE];

			MPI_Request req;

			for (j = 0; j < EAGER_MSG_SIZE; j++)
			{
				send_buf[j] = 666;
			}

//			printf("Message size: %li\n", EAGER_MSG_SIZE * sizeof(int));

			TRY (MPI_Isend(send_buf, EAGER_MSG_SIZE, MPI_INT, 0, tag, MPI_COMM_WORLD, &req));

//			printf("<%d> -- %d sent\n", rank, i);
		}
	}
	// Receiver side
	else
	{
		for (p = 1; p < size; p++)
		{
			for (i = 0; i < ITERS; i++)
			{
				int tag = p;
				size_t recv_buf[EAGER_MSG_SIZE];

				MPI_Request req;

				for (j = 0; j < EAGER_MSG_SIZE; j++)
				{
					recv_buf[j] = -1;
				}

				TRY (MPI_Irecv(recv_buf, EAGER_MSG_SIZE, MPI_INT, p, tag, MPI_COMM_WORLD, &req));
				TRY (MPI_Wait(&req, MPI_STATUS_IGNORE));

//				printf("<%d> -- %d received from %d\n", rank, i, p);
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
	TRY (MPI_Barrier(MPI_COMM_WORLD));

	return MPI_SUCCESS;
}
