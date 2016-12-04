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

int N, K, T;
int *rooms, *customers;

int work = 1;

pthread_t *thr;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

timer_t work_timer;

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


void signal_handler(int signo)
{
	if (signo == SIGUSR1)
        work = 0;
}

void *customers_routine(void *idx_ptr)
{
    int cust_idx = *((int *) idx_ptr);
    printf("Privet, ja cust %d!\n", cust_idx);
    
    int attempts = N;
    
    while (1 < 2) {
        pthread_mutex_lock(&mut);
        int i = rand() % N - 1;
        if (rooms[i]) {
            printf("I`ll (%d) take that hernja from %d!\n", cust_idx,
                i);
            
            if (customers[cust_idx] < rooms[i]) {
                customers[cust_idx] = 0;
                rooms[i] -= customers[cust_idx];
            }
            else {
                customers[cust_idx] -= rooms[i];
                rooms[i] = 0;
            }
            
            pthread_mutex_unlock(&mut);
            attempts = N;
            sleep(5);
        }
        else {
            pthread_mutex_unlock(&mut);
            --attempts;
        }
        

        
        if (!customers[cust_idx] || !attempts) {
            printf("YA OFF.\n");
            break;
        }
    }
    
    return NULL;
}

void *bulldozer_routine()
{
    printf("Privet, ja dozer!\n");
    
    while (work) {
        for (int i = 0; i < N; ++i) {
            if (!rooms[i]) {
                printf("I`ll fill that hujna w/ 40 shit %d!\n", i);
                pthread_mutex_lock(&mut);
                rooms[i] = 40;
                pthread_mutex_unlock(&mut);
                
               
                printf("Rooms state:\n");
                for (int i = 0; i < N; ++i) {
                    if (!(i % 10) && i)
                        printf("\n");
                    if (!rooms[i])
                        printf("\033[1;31m%4d \033[0m", rooms[i]);
                    else if (rooms[i] == 40)
                        printf("\033[1;32m%4d \033[0m", rooms[i]);
                    else
                        printf("\033[1;33m%4d \033[0m", rooms[i]);
                }
                printf("\n");
                sleep(5);
                break;
            }
        }

    }
    printf("\033[1;31mPORA DOMOJ!!!\033[0m\n");
    
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        printf("Invalid arguments supplied.\n");
        exit(1);
    }
    srand(time(NULL));
    
    N = atoi(argv[1]);
    K = atoi(argv[2]);
    T = atoi(argv[3]);
    
    printf("Hello there. Your input parameters:\n");
    printf("Quantity of rooms in the store: %d\n", N);
    printf("Quantity of customers: %d\n", K);
    printf("Uptime: %d secs.\n", T);
    
    if ((rooms = (int *)malloc(sizeof(int) * N)) == NULL) {
        printf("Error while allocating memory.\n");
        exit(2);
    }
    if ((customers = (int *)malloc(sizeof(int) * K)) == NULL) {
        printf("Error while allocating memory.\n");
        exit(2);
    }
    if ((thr = (pthread_t *)malloc(sizeof(pthread_t) * K + 1))
        == NULL) {
        printf("Error while allocating memory.\n");
        exit(2);
    }
    
    work_timer = create_timer(SIGUSR1);
    install_sighandler(SIGUSR1, signal_handler);
    set_timer(work_timer, T);
    
    printf("Initial rooms state:\n");
    for (int i = 0; i < N; ++i) {
        rooms[i] = rand() % 40 + 1;
        if (!(i % 10) && i)
            printf("\n");
        printf("%4d ", rooms[i]);
    }
    printf("\n");
    
    printf("Customers shit:\n");
    for (int i = 0; i < K; ++i) {
        customers[i] = rand() % 1000 + 1;
        if (!(i % 10) && i)
            printf("\n");
        printf("%4d ", customers[i]);
    }
    printf("\n");
    
    for (int i = 0; i < K + 1; ++i) {
        if (i == K) {
            if (pthread_create(&thr[i], NULL, bulldozer_routine, NULL))
                printf("pthread_create pidor.\n");
            break;
        }
        
        int *idx = malloc(sizeof(*idx));
        *idx = i;
        if (pthread_create(&thr[i], NULL, customers_routine, idx))
            printf("pthread_create pidor.\n");
    }
    
    for (int i = 0; i < K + 1; ++i)
        pthread_join(thr[i], NULL);
    
    return 0;
}
