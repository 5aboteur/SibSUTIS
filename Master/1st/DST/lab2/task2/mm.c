#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int M, N, P;
int **matrixA;
int **matrixB;
int **matrixC;

int mmalloc()
{
    int i;

    matrixA = (int **)malloc(sizeof(int *) * M);
    matrixB = (int **)malloc(sizeof(int *) * N);
    matrixC = (int **)malloc(sizeof(int *) * M);

    for (i = 0; i < M; ++i)
        matrixA[i] = (int *)malloc(sizeof(int) * N);
    for (i = 0; i < N; ++i)
        matrixB[i] = (int *)malloc(sizeof(int) * P);
    for (i = 0; i < M; ++i)
        matrixC[i] = (int *)malloc(sizeof(int) * P);

	return 0;
}

int mfree()
{
	int i;

    for (i = 0; i < M; ++i)
        free(matrixA[i]);
    for (i = 0; i < N; ++i)
        free(matrixB[i]);
    for (i = 0; i < M; ++i)
        free(matrixC[i]);

    free(matrixA);
    free(matrixB);
	free(matrixC);

	return 0;
}

int generate()
{
    srand(time(NULL));

    int i, j;

	mmalloc();

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

    printf("-> B\033[0m\n");
    for (i = 0; i < N; ++i) {
        printf("\033[1;33m|\033[0m");
        for (j = 0; j < P; ++j)
            printf("%5d", matrixB[i][j]);
        printf("\033[1;33m|\033[0m\n");
    }
    printf("\033[1;33m\n");

    return 0;
}

int compute()
{
    int i, j, k;

	for (i = 0; i < M; ++i)
		memset(matrixC[i], 0, sizeof(int) * P);

    for (i = 0; i < M; ++i) {
        for (j = 0; j < P; ++j) {
            for (k = 0; k < N; ++k)
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
        }
    }

    printf("\033[1;33m-> C\033[0m\n");
	for (i = 0; i < M; ++i) {
        printf("\033[1;33m|\033[0m");
		for (j = 0; j < P; ++j)
            printf("%5d ", matrixC[i][j]);
        printf("\033[1;33m|\033[0m\n");
	}
    printf("\033[0m\n");

    return 0;
}

int store(const char *filename, const char *mode)
{
	int i, j;

	FILE *fp = fopen(filename, mode);

	fprintf(fp, "%d ", M);
	fprintf(fp, "%d\n", N);

	for (i = 0; i < M; ++i) {
		for (j = 0; j < N; ++j)
			fprintf(fp, "%d ", matrixA[i][j]);
		fprintf(fp, "\n");
	}

	fprintf(fp, "%d ", N);
	fprintf(fp, "%d\n", P);

	for (i = 0; i < N; ++i) {
		for (j = 0; j < P; ++j)
			fprintf(fp, "%d ", matrixB[i][j]);
		fprintf(fp, "\n");
	}

	fclose(fp);
	return 0;
}

int load(const char *filename, const char *mode)
{
	int i, j;

	FILE *fp = fopen(filename, mode);

	fscanf(fp, "%d", &M);
	fscanf(fp, "%d", &N);

	for (i = 0; i < M; ++i) {
		for (j = 0; j < N; ++j)
			fscanf(fp, "%d", &matrixA[i][j]);
	}

	fscanf(fp, "%d", &N);
	fscanf(fp, "%d", &P);

	for (i = 0; i < N; ++i) {
		for (j = 0; j < P; ++j)
			fscanf(fp, "%d", &matrixB[i][j]);
	}

	fclose(fp);
	return 0;
}

int rowcol()
{
    int i, j, k;

	for (i = 0; i < M; ++i)
		memset(matrixC[i], 0, sizeof(int) * P);

    for (i = 0; i < M; ++i) {
        for (j = 0; j < P; ++j) {
            for (k = 0; k < N; ++k)
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
        }
    }

    printf("\033[1;33m-> rowcol\033[0m\n");
	for (i = 0; i < M; ++i) {
        printf("\033[1;33m|\033[0m");
		for (j = 0; j < P; ++j)
            printf("%5d ", matrixC[i][j]);
        printf("\033[1;33m|\033[0m\n");
	}
    printf("\033[0m\n");

    return 0;
}

int rowrow()
{
    int i, j, k;

	for (i = 0; i < M; ++i)
		memset(matrixC[i], 0, sizeof(int) * P);

    for (i = 0; i < M; ++i) {
        for (k = 0; k < N; ++k) {
        	for (j = 0; j < P; ++j)
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
        }
    }

    printf("\033[1;33m-> rowrow\033[0m\n");
	for (i = 0; i < M; ++i) {
        printf("\033[1;33m|\033[0m");
		for (j = 0; j < P; ++j)
            printf("%5d ", matrixC[i][j]);
        printf("\033[1;33m|\033[0m\n");
	}
    printf("\033[0m\n");

    return 0;
}

int colcol()
{
    int i, j, k;

	for (i = 0; i < M; ++i)
		memset(matrixC[i], 0, sizeof(int) * P);

    for (j = 0; j < P; ++j) {
    	for (i = 0; i < M; ++i) {
            for (k = 0; k < N; ++k)
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
        }
    }

    printf("\033[1;33m-> colcol\033[0m\n");
	for (i = 0; i < M; ++i) {
        printf("\033[1;33m|\033[0m");
		for (j = 0; j < P; ++j)
            printf("%5d ", matrixC[i][j]);
        printf("\033[1;33m|\033[0m\n");
	}
    printf("\033[0m\n");

    return 0;
}

int colrow()
{
    int i, j, k;

	for (i = 0; i < M; ++i)
		memset(matrixC[i], 0, sizeof(int) * P);

    for (j = 0; j < P; ++j) {
        for (k = 0; k < N; ++k) {
		    for (i = 0; i < M; ++i)
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
        }
    }

    printf("\033[1;33m-> colrow\033[0m\n");
	for (i = 0; i < M; ++i) {
        printf("\033[1;33m|\033[0m");
		for (j = 0; j < P; ++j)
            printf("%5d ", matrixC[i][j]);
        printf("\033[1;33m|\033[0m\n");
	}
    printf("\033[0m\n");

    return 0;
}

int zfunc()
{
	int i;

	for (i = 16; i <= 128; i += 16) {
		M = N = P = i;
		printf(" M=%d, N=%d, P=%d\n", M, N, P);
		generate();
		store("superfile", "a");
		mfree();
		usleep(50000);
	}

	return 0;
}

void help()
{
	printf("\033[1;35mG - Generate.\nP - Print.\n"
		"C - Compute.\nS - Store.\nL - Load.\n"
		"H - Help.\nQ - Quit.\n\n"
		"Z - \033[32mz\033[33mf\033[34mu\033[35mn"
		"\033[36mc\033[31m()\033[1;35m.\n\033[0m");
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

	char fname[BUFSIZ];
	int lives = 9, isAlloc = 0, isExist = 0;

	while (1) {
		char choice;
		printf("\033[1;31m *** CHOOSE YOUR DESTINY ***\n  >\033[0m");
		scanf(" %c", &choice);

		switch (choice) {
		case 'g':
		    printf("\033[1;35mGenerating a \033[0m%d\033[1;35mx\033[0m%d \033[1;35m"
    		    "matrix [A] and a \033[0m%d\033[1;35mx\033[0m%d \033[1;35m[B]..\033[0m\n\n", M, N, N, P);
		    generate();
			isAlloc = 1;
			break;
		case 'p':
			if (isAlloc)
			    print();
			else
				printf("\033[1;35mOops! There is nothing to print..\033[0m\n\n");
			break;
		case 'c':
			if (isAlloc) {
				printf("\033[1;35mCompute a product of matrices A & B.\033[0m\n\n");
			    compute();
			}
			else
				printf("\033[1;35mYou`re trying to compute product of nonexistent matrices.."
					" That`s bad.\033[0m\n\n");
			break;
		case 's':
			if (isAlloc) {
				printf("\033[1;35mInput filename : \033[0m");
				scanf("%s", fname);
				printf("\033[1;35mStore matrices A & B to a file `\033[0m%s\033[1;35m`.\033[0m\n\n", fname);
				store(fname, "w");
				isExist = 1;
			}
			else 
				printf("\033[1;35mYou`re trying to store nonexistent matrices to a file.."
					" That`s bad.\033[0m\n\n");
			break;
		case 'l':
			if (isExist) {
				printf("\033[1;35mLoad matrices A & B from a file `\033[0m%s\033[1;35m`.\033[0m\n\n", fname);
				load(fname, "r");
			}
			else
				printf("\033[1;35mYou`re trying to load matrices from a nonexistent file.."
					" That`s bad.\033[0m\n\n");
			break;
		case 'x':
			rowcol();
			rowrow();
			colcol();
			colrow();
			break;
		case 'z':;
			int _M = M;
			int _N = N;
			int _P = P;

			printf("\033[1;35mGenerating random matrices A & B from size \033[0m16\033[1;35mx\033[0m16\033[1;35m"
				" to \033[0m4096\033[1;35mx\033[0m4096\033[1;35m\nwith span=\033[0m16\033[1;35m and store them"
				" into the `\033[0msuperfile\033[1;35m`.\033[0m\n");
			zfunc();

			isAlloc = 0;
			isExist = 0;
			M = _M; N = _N; P = _P;

			break;
		case 'h':
			help();
			break;
		case 'q':
			printf("\033[1;35mOK. BYE!\033[0m\n");
			if (isAlloc)
				mfree();
			exit(0);
		default:
			if (lives == 1) {
				printf("\033[33m%d life remaining.\033[0m\n\n", lives);
			}
			else if (lives == 0) {
				printf("\033[31mYOU`RE DEAD ..\033[0m\n");
				if (isAlloc)
					mfree();
				exit(0);
			}
			else
				printf("\033[32m%d lives remaining.\033[0m\n\n", lives);
			--lives;
			break;
		}
	}

    return 0;
}
