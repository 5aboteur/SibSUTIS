#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int timeThink;
int timeRestart;

int cnt;
int turn;
int winner;

int field[9];
int trine[3];
int st[2];

void dump(int line)
{
	if (line) {
		if (turn)
			winner = 1;
		else
			winner = 0;

		switch (line) {
		case 1:
			trine[0] = 0;
			trine[1] = 1;
			trine[2] = 2;
			break;
		case 2:
			trine[0] = 3;
			trine[1] = 4;
			trine[2] = 5;
			break;
		case 3:
			trine[0] = 6;
			trine[1] = 7;
			trine[2] = 8;
			break;
		case 4:
			trine[0] = 0;
			trine[1] = 3;
			trine[2] = 6;
			break;
		case 5:
			trine[0] = 1;
			trine[1] = 4;
			trine[2] = 7;
			break;
		case 6:
			trine[0] = 2;
			trine[1] = 5;
			trine[2] = 8;
			break;
		case 7:
			trine[0] = 0;
			trine[1] = 4;
			trine[2] = 8;
			break;
		case 8:
			trine[0] = 2;
			trine[1] = 4;
			trine[2] = 6;
			break;
		default:
			printf("Incorrect line.\n");
			break;
		}
	}

	printf("\n\033[1;34m*\033[1;35m*\033[1;36m*\033[1;35m*\033[1;34m*\033[0m");
	printf(" turn: %d\n", cnt + 1);
	for (int i = 0; i < 9; ++i) {
		if (i == trine[0] || i == trine[1] || i == trine[2])
			printf("\033[1;31m");
		if (field[i] != -1) {
			if (field[i] == 1)
				printf("x");
			else
				printf("o");
			if (i % 3 == 2)
				printf("\033[0m\n");
			else
				printf("\033[0m|");
		} else if ((i % 3 == 2))
			printf("\033[0m-\n");
		else
			printf("\033[0m-|");
	}
}

void stats()
{
	if (st[0] > st[1]) {
		printf("\033[1;33m");
		printf("+-------+\n");
		printf("| \033[1;32mX\033[1;33m | o |\n");
		printf("+-------+\n");
		printf("|%2d |%2d |\n", st[0], st[1]);
		printf("+-------+\n");
		printf("\033[0m");
	} else if (st[0] < st[1]) {
		printf("\033[1;33m");
		printf("+-------+\n");
		printf("| x | \033[1;32mO\033[1;33m |\n");
		printf("+-------+\n");
		printf("|%2d |%2d |\n", st[0], st[1]);
		printf("+-------+\n");
		printf("\033[0m");
	} else {
		printf("\033[1;33m");
		printf("+-------+\n");
		printf("| x | o |\n");
		printf("+-------+\n");
		printf("|%2d |%2d |\n", st[0], st[1]);
		printf("+-------+\n");
		printf("\033[0m");
	}
}

void check(int c, int idx)
{
	switch (idx) {
	case 0:
		if (field[1] == c && field[2] == c)
			dump(1);
		else if (field[3] == c && field[6] == c)
			dump(4);
		else if (field[4] == c && field[8] == c)
			dump(7);
		else
			dump(0);
		break;
	case 1:
		if (field[0] == c && field[2] == c)
			dump(1);
		else if (field[4] == c && field[7] == c)
			dump(5);
		else
			dump(0);
		break;
	case 2:
		if (field[1] == c && field[0] == c)
			dump(1);
		else if (field[5] == c && field[8] == c)
			dump(6);
		else if (field[4] == c && field[6] == c)
			dump(8);
		else
			dump(0);
		break;
	case 3:
		if (field[0] == c && field[6] == c)
			dump(4);
		else if (field[4] == c && field[5] == c)
			dump(2);
		else
			dump(0);
		break;
	case 4:
		if (field[3] == c && field[5] == c)
			dump(2);
		else if (field[1] == c && field[7] == c)
			dump(5);
		else if (field[0] == c && field[8] == c)
			dump(7);
		else if (field[2] == c && field[6] == c)
			dump(8);
		else
			dump(0);
		break;
	case 5:
		if (field[3] == c && field[4] == c)
			dump(2);
		else if (field[2] == c && field[8] == c)
			dump(6);
		else
			dump(0);
		break;
	case 6:
		if (field[0] == c && field[3] == c)
			dump(4);
		else if (field[7] == c && field[8] == c)
			dump(3);
		else if (field[4] == c && field[2] == c)
			dump(8);
		else
			dump(0);
		break;
	case 7:
		if (field[6] == c && field[8] == c)
			dump(3);
		else if (field[1] == c && field[4] == c)
			dump(5);
		else
			dump(0);
		break;
	case 8:
		if (field[6] == c && field[7] == c)
			dump(3);
		else if (field[2] == c && field[5] == c)
			dump(6);
		else if (field[0] == c && field[4] == c)
			dump(7);
		else
			dump(0);
		break;
	}
}

void cmd()
{
	
}

void game()
{
	int idx;

	memset(trine, -1, sizeof(int) * 3);
	memset(field, -1, sizeof(int) * 9);

	turn = 1;
	cnt = 0;
	winner = -1;
/*
	for (int i = 0; i < 9; ++i)
		printf("%d ", field[i]);
	printf("\n");
	for (int i = 0; i < 3; ++i)
		printf("%d ", trine[i]);
*/
	while (1) {
//		printf("\nturn=%d, cnt=%d, winner=%d\n\n", turn, cnt, winner);
		do {
			idx = rand() % 9;
		} while (field[idx] != -1);

		if (turn) {
			// x
			field[idx] = 1;
			check(1, idx);
			turn--; cnt++;

			if (winner == 1) {
				printf("\033[1;33mResult: `x` wins the game.\033[0m\n");
				st[0]++;
				stats();
				break;
			}
		} else {
			// o
			field[idx] = 0;
			check(0, idx);
			turn++;	cnt++;

			if (winner == 0) {
				printf("\033[1;33mResult: `o` wins the game.\033[0m\n");
				st[1]++;
				stats();
				break;
			}
		}

		if (cnt == 9) {
			printf("\033[1;32mResult: draw.\033[0m\n");
			break;
		}

		usleep(70000);
	}
}

int main()
{
	srand((unsigned)time(NULL));
//	pthread_create(&gamethr, , NULL, game);
//	pthread_create(&cmdthr, , NULL, cmd);

//	pthread_join(gamethr, 0);
//	pthread_join(cmdthr, 0);
	while (1) {
		game();
		if (st[0] == 9 || st[1] == 9) {
			printf("\nFin.\n");
			break;
		}
	}

	return 0;
}
