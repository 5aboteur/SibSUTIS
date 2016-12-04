#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

int timeThink = 2;
int timeRestart = 2;

int cnt, turn, winner, start = 1, think = 1;
int quit = 0;
int custom_pause = 0;

int field[9];
int trine[3];
int st[2];

int go = 0;

timer_t timer_think, timer_restart;

timer_t create_timer(int signo)
{
    timer_t timerid;
    struct sigevent se;
    se.sigev_notify = SIGEV_SIGNAL;
    se.sigev_signo = signo;
    timer_create(CLOCK_REALTIME, &se, &timerid);
    return timerid;
}

void set_timer(timer_t timerid, int secs)
{
    struct itimerspec timervals;
    timervals.it_value.tv_sec = secs;
    timervals.it_value.tv_nsec = 0;
    timervals.it_interval.tv_sec = 0;
    timervals.it_interval.tv_nsec = 0;
    timer_settime(timerid, 0, &timervals, NULL);
}

void install_sighandler(int signo, void(*handler)(int))
{
    sigset_t set;
    struct sigaction act;

    act.sa_handler = handler;
    act.sa_flags = SA_RESTART;
    sigaction(signo, &act, 0);

    sigemptyset(&set);
    sigaddset(&set, signo);
    sigprocmask(SIG_UNBLOCK, &set, NULL);
}

enum keys {
    _S, _T, _A, _Q, _UNKNOWN
};

struct termios rk_currentTermState;

int mytermsave()
{
    if (tcgetattr(1, &rk_currentTermState) == -1) return -1;
    return 0;
}

int mytermrestore()
{
    if (tcsetattr(1, TCSANOW, &rk_currentTermState) == -1) return -1;
    return 0;
}

int mytermregime(int regime, int vtime, int vmin, int echo, int sigint)
{
    struct termios term;

    if ((regime > 1) || (regime < 0) || (echo > 1) ||
        (echo < 0) || (sigint > 1) || (sigint < 0))
        return 1;

    if ((vtime < 0) || (vmin < 0)) return -1;
    if (tcgetattr(1, &term) == -1) return -1;

    if (regime == 0) term.c_lflag &= (~ICANON);
    else term.c_lflag |= ICANON;

    if (echo == 0) term.c_lflag &= (~ECHO);
    else term.c_lflag |= ECHO;

    if (sigint == 0) term.c_lflag &= (~ISIG);
    else term.c_lflag |= ISIG;

    term.c_cc[VMIN] = vmin;
    term.c_cc[VTIME] = vtime;

    if (tcsetattr(1, TCSANOW, &term) == -1) return -1;

    return 0;
}

int readkey(enum keys *key)
{
    char ch;
    if (!read(0, &ch, 1)) return -1;
    *key = ch;
    switch (ch) {
        case 's': *key = _S; break;
        case 't': *key = _T; break;
        case 'a': *key = _A; break;
        case 'q': *key = _Q; break;
        default: *key = _UNKNOWN; break;
    }
    return 0;
}

void dump(int line)
{
	int i;

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
	for (i = 0; i < 9; ++i) {
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
    mytermsave();
	int key;
    mytermregime(0, 0, 1, 0, 1);
    while (!quit) {
        readkey((enum keys *) &key);
        switch (key) {
            case _A:
                custom_pause = 1;
                printf("Input a new restart time.\n");
                while (!scanf("%d", &timeRestart));                
                printf("Restart time is now %d secs.\n", timeRestart);
                custom_pause = 0;
                break;
            case _S:
                if (custom_pause) {
                    printf("The game is going to proceed.\n");
                    custom_pause = 0;
                } else {
                    printf("The game is going to pause.\n");
                    custom_pause = 1;
                }
                break;
            case _T:
                custom_pause = 1;
                printf("Input a new think time.\n");
                while (!scanf("%d", &timeThink));
                printf("Think time is now %d secs.\n", timeThink);
                custom_pause = 0;
                break;
            case _Q:
                printf("Attempting to quit the game.\n");
                if (custom_pause)
                    custom_pause = 0;
                quit = 1;
                break;
            default:
                break;
        }
    }
    sleep(timeThink);
    mytermrestore();
}

void game()
{
    printf("The game has begun.\n");
	int idx;

	memset(trine, -1, sizeof(int) * 3);
	memset(field, -1, sizeof(int) * 9);

	turn = 1;
	cnt = 0;
	winner = -1;

	while (1) {
        set_timer(timer_think, timeThink);
		while (think);
        think = 1;
        
        while (custom_pause);
        
        if (quit)
            break;
        
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
//		usleep(70000);
	}
}

void signal_handler(int signo)
{
	if (signo == SIGUSR1) 
        think = 0;
    else if (signo == SIGUSR2)
        start = 0;
}

int main()
{
//	srand((unsigned)time(NULL));
	timer_think = create_timer(SIGUSR1);
    timer_restart = create_timer(SIGUSR2);

    install_sighandler(SIGUSR1, signal_handler);
    install_sighandler(SIGUSR2, signal_handler);

	pthread_t cmdthr;

//	pthread_create(&gamethr, NULL, (void *)game, NULL);
	pthread_create(&cmdthr, NULL, (void *)cmd, NULL);

	while (1) {
        if (st[0] == 9 || st[1] == 9 || quit) {
			printf("\nFin.\n");
			break;
		}
		game();
        set_timer(timer_restart, timeRestart);
        while (start);
        start = 1;
	}

//	pthread_join(gamethr, NULL);
	pthread_join(cmdthr, NULL);
	return 0;
}
