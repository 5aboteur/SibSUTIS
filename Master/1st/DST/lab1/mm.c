#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int M, N, P;
int **matrixA;
int **matrixB;

int generate()
{
    srand(time(NULL));

    int i, j;

    for (i = 0; i < M; ++i) {
        for (j = 0; j < N; ++j)
            matrixA[i][j] = (rand() % 17) + 1;
    }

    for (i = 0; i < N; ++i) {
        for (j = 0; j < P; ++j)
            matrixB[i][j] = (rand() % 22) + 1;
    }

    return 0;
}

int print()
{
    int i, j;

    printf("\033[1;33m-> A\033[0m\n");
    for (i = 0; i < M; ++i) {
        printf("\033[1;33m|\033[0m");
        for (j = 0; j < N; ++j)
            printf("%5d", matrixA[i][j]);
        printf("\033[1;33m|\033[0m\n");
    }
    printf("\033[1;33m\n\n");

    printf("x\n\n");

    printf("-> B\033[0m\n");
    for (i = 0; i < N; ++i) {
        printf("\033[1;33m|\033[0m");
        for (j = 0; j < P; ++j)
            printf("%5d", matrixB[i][j]);
        printf("\033[1;33m|\033[0m\n");
    }
    printf("\033[1;33m\n\n");

    printf("----------------------\n\n");

    return 0;
}

int calc()
{
    int i = 0, j = 0, k = 0;

    printf("-> C\033[0m\n");
    for (i = 0; i < M; ++i) {
        printf("\033[1;33m|\033[0m");
        for (j = 0; j < P; ++j) {
            int c = 0;
            for (k = 0; k < N; ++k)
                c += matrixA[i][k] * matrixB[k][j];
            printf("%5d ", c);
        }
        printf("\033[1;33m|\033[0m\n");
    }
    printf("\033[0m\n");

    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 4) {
        printf("\033[1;31mInput M & N & P | GTFO\033[0m\n");
        return -1;
    }

    M = atoi(argv[1]);
    N = atoi(argv[2]);
    P = atoi(argv[3]);

    printf("\033[1;35mGenerating a \033[0m%d\033[1;35mx\033[0m%d \033[1;35m"
        "matrix [A] and a \033[0m%d\033[1;35mx\033[0m%d \033[1;35m[B]..\033[0m\n", M, N, N, P);

    int i;

    matrixA = (int **)malloc(sizeof(int *) * M);
    matrixB = (int **)malloc(sizeof(int *) * N);

    for (i = 0; i < M; ++i)
        matrixA[i] = (int *)malloc(sizeof(int) * N);
    for (i = 0; i < N; ++i)
        matrixB[i] = (int *)malloc(sizeof(int) * P);

    generate();
    print();
    calc();

    for (i = 0; i < M; ++i) {
        free(matrixA[i]);
        free(matrixB[i]);
    }

    free(matrixA);
    free(matrixB);

    return 0;
}
