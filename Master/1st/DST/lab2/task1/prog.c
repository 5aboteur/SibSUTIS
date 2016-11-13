#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

int flg = 1;

void printA() { printf("\033[1;34mA\033[0m"); }
void printB() { printf("\033[1;31mB\033[0m"); }

void sighandler()
{
	if (flg) flg = 0;
	else flg = 1;
}

int main()
{
	signal(SIGALRM, sighandler);

	struct itimerval nval, oval;

	nval.it_interval.tv_sec = 3;
	nval.it_interval.tv_usec = 0;
	nval.it_value.tv_sec = 3;
	nval.it_value.tv_usec = 0;

	setitimer(ITIMER_REAL, &nval, &oval);

	while (1) {
		if (flg) printA();
		else printB();
		usleep(1337);
	}

	return 0;
}
