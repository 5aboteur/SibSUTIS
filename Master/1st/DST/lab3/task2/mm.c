#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

/* Defining the basic variables */

int **A, **B, **C;
int M, N, K;

/* Getting the time */

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

/* Filling a matrix */

void imm_fill(int **array, int x, int y)
{
    for (int i = 0; i < x; ++i)
        for (int j = 0; j < y; ++j)
            array[i][j] = 4;
}

/* Displaying a matrix */

void imm_display(int **array, int x, int y)
{
    for (int i = 0; i < x; ++i) {
        printf("| ");
        for (int j = 0; j < y; ++j) {
            printf("%6d ", array[i][j]);
        }
        printf(" |\n");
    }
}

double imm_run_serial(int **a, int **b, int **c, int m, int n, int k)
{
    /* Allocating the memory */

    a = (int **)malloc(sizeof(int *) * m);
    for (int i = 0; i < m; ++i)
        a[i] = (int *)malloc(sizeof(int) * n);

    b = (int **)malloc(sizeof(int *) * n);
    for (int i = 0; i < n; ++i)
        b[i] = (int *)malloc(sizeof(int) * k);

    c = (int **)malloc(sizeof(int *) * m);
    for (int i = 0; i < m; ++i) {
        c[i] = (int *)malloc(sizeof(int) * k);
        memset(c[i], 0, sizeof(int) * k);
    }

    /* Filling the matrices */

    srand((unsigned)time(NULL));
    imm_fill(a, m, n);
    imm_fill(b, n, k);

    /* Computing the result */

    double t = wtime();
    for (int i = 0; i < m; ++i) {
        for (int q = 0; q < n; ++q) {
            for (int j = 0; j < k; ++j) {
                c[i][j] += a[i][q] * b[q][j];
            }
        }
    }
    t = wtime() - t;

    /* Displaying the result */
    /*
    printf("Matrix A:\n");
    imm_display(a, m, n);
    printf("\nMatrix B:\n");
    imm_display(b, n, k);
    printf("\n\033[0;36mMatrix C (A x B):\033[0m\n");
    imm_display(c, m, k);
    */
    printf("\n\033[0;33mElapsed time: %.6lf sec.\033[0m\n", t);
    /* Deallocating the memory */

    for (int i = 0; i < m; ++i)
        free(a[i]);
    for (int i = 0; i < n; ++i)
        free(b[i]);
    for (int i = 0; i < m; ++i)
        free(c[i]);
    free(a);
    free(b);
    free(c);

    return t;
}

double imm_run_parallel(int **a, int **b, int **c, int m, int n, int k)
{
    /* Allocating the memory */

    a = (int **)malloc(sizeof(int *) * m);
    for (int i = 0; i < m; ++i)
        a[i] = (int *)malloc(sizeof(int) * n);

    b = (int **)malloc(sizeof(int *) * n);
    for (int i = 0; i < n; ++i)
        b[i] = (int *)malloc(sizeof(int) * k);

    c = (int **)malloc(sizeof(int *) * m);
    for (int i = 0; i < m; ++i) {
        c[i] = (int *)malloc(sizeof(int) * k);
        memset(c[i], 0, sizeof(int) * k);
    }

    /* Filling the matrices */

    srand((unsigned)time(NULL));
    imm_fill(a, m, n);
    imm_fill(b, n, k);

    /* Computing the result */

    int i, j, q;
    double t = wtime();
    #pragma omp parallel shared(a, b, c) private(q, j)
    {
        #pragma omp for
        for (i = 0; i < m; ++i) {
            for (q = 0; q < n; ++q) {
                for (j = 0; j < k; ++j) {
                    c[i][j] += a[i][q] * b[q][j];
                }
            }
        }
    }
    t = wtime() - t;

    /* Displaying the result */
    /*
    printf("Matrix A:\n");
    imm_display(a, m, n);
    printf("\nMatrix B:\n");
    imm_display(b, n, k);
    printf("\n\033[0;36mMatrix C (A x B):\033[0m\n");
    imm_display(c, m, k);
    */
    printf("\n\033[0;33mElapsed time: %.6lf sec.\033[0m\n", t);
    /* Deallocating the memory */

    for (int i = 0; i < m; ++i)
        free(a[i]);
    for (int i = 0; i < n; ++i)
        free(b[i]);
    for (int i = 0; i < m; ++i)
        free(c[i]);
    free(a);
    free(b);
    free(c);

    return t;
}

int main(int argc, char **argv)
{
    if (argc != 4) {
        printf("\033[0;31mYou must supply exactly 3 arguments: M, N, and K.\n");
        return -1;
    }

    /* Reading the arguments */

    M = atoi(argv[1]);
    N = atoi(argv[2]);
    K = atoi(argv[3]);

    /* Running the routines */

    double t_ser = imm_run_serial(A, B, C, M, N, K);
    double t_par = imm_run_parallel(A, B, C, M, N, K);

    printf("Speedup is %.2lf\n", t_ser / t_par);
    return 0;
}
