#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TRY(func)							\
	do {									\
		err = (func);						\
		if (err != MPI_SUCCESS)				\
			MPI_Abort(MPI_COMM_WORLD, err);	\
	} while (0)

#define EAGER_MSG_SIZE 10
#define RNDV_MSG_SIZE 1000000
#define ITERS 1000

int err;

int test_eager(int rank, int size);
//int test_rndv(int rank; int size);

int main(int argc, char *argv[])
{
	int rank, size;

	MPI_Init(&argc, &argv);
	if (err != MPI_SUCCESS) return err;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (size < 2)
	{
		printf("Need at least two processes. Quitting...\n");
		MPI_Abort(MPI_COMM_WORLD, err);
		exit(1);
	}

//	printf("Rank: %d, size: %d\n", rank, size);

	err = test_eager(rank, size);
	if (err != MPI_SUCCESS) return err;

	err = MPI_Finalize();
	if (err != MPI_SUCCESS) return err;

	return 0;
}

int test_eager(int rank, int size)
{
	int i, j, p, err;

	// Sender side
	if (rank != 0)
	{
		for (i = 0; i < ITERS; i++)
		{
			int tag = rank;//(i + 1) * rank * ITERS;
			size_t send_buf[EAGER_MSG_SIZE];

			MPI_Request req;

			for (j = 0; j < EAGER_MSG_SIZE; j++)
			{
				send_buf[j] = 666;
			}

//			printf("Message size: %li\n", EAGER_MSG_SIZE * sizeof(int));

			err = MPI_Isend(send_buf, EAGER_MSG_SIZE, MPI_INT, 0, tag, MPI_COMM_WORLD, &req);
			if (err != MPI_SUCCESS) return err;

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

				err = MPI_Irecv(recv_buf, EAGER_MSG_SIZE, MPI_INT, p, tag, MPI_COMM_WORLD, &req);
				if (err != MPI_SUCCESS) return err;

				MPI_Wait(&req, MPI_STATUS_IGNORE);

//				printf("<%d> -- %d received from %d\n", rank, i, p);
			}
		}
	}

	if (rank != 0)
	{
		printf("\n<%d> -- Message delivered!\n\n", rank);
	}
	else
	{
		printf("\n<%d> -- Message received!\n\n", rank);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	return MPI_SUCCESS;
}
