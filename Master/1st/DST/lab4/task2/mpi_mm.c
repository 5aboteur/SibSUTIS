#include <stdio.h>
#include <sys/time.h>
#include <mpi.h>

#define N 1000

MPI_Status status;

int a[N][N], b[N][N], c[N][N];

int main(int argc, char **argv)
{
  int ntasks, rank, numworkers, source,
    dest, rows, offset, i, j, k;

  struct timeval start, stop;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

  numworkers = ntasks - 1;

  /***** Master side starts here *****/
  if (rank == 0) {
    for (i = 0; i < N; ++i) {
      for (j = 0; j < N; ++j) {
        a[i][j] = 1;
        b[i][j] = 2;
      }
    }

    gettimeofday(&start, 0);

    /* Send matrix data to the worker tasks */
    rows = N / numworkers;
    offset = 0;

    for (dest = 1; dest <= numworkers; ++dest) {
      MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&a[offset][0], rows * N, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&b, N * N, MPI_INT, dest, 1, MPI_COMM_WORLD);
      offset += rows;
    }

    /* Wait for results from all worker tasks */
    for (i = 1; i <= numworkers; ++i) {
      source = i;
      MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&c[offset][0], rows * N, MPI_INT, source, 2,
        MPI_COMM_WORLD, &status);
    }

    gettimeofday(&stop, 0);
/*
    printf("Here is the result matrix:\n");
    for (i = 0; i < N; ++i) {
      for (j = 0; j < N; ++j)
        printf("%d ", c[i][j]);
      printf("\n");
    }
*/
    fprintf(stdout, "Time = %.6f\n\n", (stop.tv_sec+stop.tv_usec*1e-6) -
        (start.tv_sec+start.tv_usec*1e-6));

  }

  /***** Worker side starts here *****/
  if (rank > 0) {
    source = 0;
    MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&a, rows * N, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&b, N * N, MPI_INT, source, 1, MPI_COMM_WORLD, &status);

    /* Matrix multiplication */
    for (k = 0; k < N; ++k)
      for (i = 0; i < rows; ++i) {
        c[i][k] = 0;
        for (j = 0; j < N; ++j)
          c[i][k] += a[i][j] * b[j][k];
      }

    MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&c, rows * N, MPI_INT, 0, 2, MPI_COMM_WORLD);
  }

  MPI_Finalize();

  return 0;
}
