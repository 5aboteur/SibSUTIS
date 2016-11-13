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

    printf("\033[1;33m-> C\033[0m\n");
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

int store()
{
	int i, j;

	FILE *fp = fopen("superfile", "w");

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

int load()
{
	int i, j;

	FILE *fp = fopen("superfile", "r");

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

void help()
{
	printf("\033[1;35mG - Generate.\nP - Print.\n"
		"C - Compute.\nS - Store.\nL - Load.\n"
		"H - Help.\nQ - Quit.\n\033[0m");
}

int mfree()
{
	int i;

    for (i = 0; i < M; ++i) {
        free(matrixA[i]);
        free(matrixB[i]);
    }

    free(matrixA);
    free(matrixB);

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

    int i;

    matrixA = (int **)malloc(sizeof(int *) * M);
    matrixB = (int **)malloc(sizeof(int *) * N);

    for (i = 0; i < M; ++i)
        matrixA[i] = (int *)malloc(sizeof(int) * N);
    for (i = 0; i < N; ++i)
        matrixB[i] = (int *)malloc(sizeof(int) * P);

	int lives = 9, flag = 0;

	while (1) {
		char choice;
		printf("\033[1;31m *** CHOOSE YOUR DESTINY ***\n  >\033[0m");
		scanf(" %c", &choice);

		switch (choice) {
		case 'g':
		    printf("\033[1;35mGenerating a \033[0m%d\033[1;35mx\033[0m%d \033[1;35m"
    		    "matrix [A] and a \033[0m%d\033[1;35mx\033[0m%d \033[1;35m[B]..\033[0m\n\n", M, N, N, P);
		    generate();
			break;
		case 'p':
		    print();
			break;
		case 'c':
			printf("\033[1;35mCompute a product of matrices A & B.\033[0m\n\n");
		    compute();
			break;
		case 's':
			printf("\033[1;35mStore matrices A & B to a file.\033[0m\n\n");
			store();
			flag = 1;
			break;
		case 'l':
			if (flag) {
				printf("\033[1;35mLoad matrices A & B from a file.\033[0m\n\n");
				load();
			}
			else
				printf("\033[1;35mYou`re trying to load matrices from a nonexistent file"
					" That`s bad.\033[0m\n\n");
			break;
		case 'h':
			help();
			break;
		case 'q':
			printf("\033[1;35mOK. BYE!\033[0m\n");
			mfree();
			exit(0);
		default:
			if (lives == 1) {
				printf("\033[33m%d life remaining.\033[0m\n\n", lives);
			}
			else if (lives == 0) {
				printf("\033[31mYOU`RE DEAD ..\033[0m\n");
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
