/*
 * primes.c:
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <omp.h>

#include "hpctimer.h"

enum { NTHREADS_MAX = 32 };

int is_prime_number(int num)
{
    int limit, factor = 3;

    limit = (long)(sqrtf((float)num) + 0.5f);
    while ((factor <= limit) && (num % factor))
        factor ++;
    return (factor > limit);
}

int main(int argc, char **argv)
{
    int start, end, nprimes;
    int i, tid, nthreads;
    double threadtime[NTHREADS_MAX];
    double t;
        
    if (argc < 3) {
        fprintf(stderr, "usage: primes <start_number> <end_number>\n");
        exit(EXIT_FAILURE);
    }
    start = atoi(argv[1]);
    end = atoi(argv[2]);

    t = hpctimer_getwtime();
    if((start % 2) == 0 )
        start = start + 1;

    nprimes = 0;
    if (start <= 2)
        nprimes++;
        
#pragma omp parallel private(tid)
{
#pragma omp master
{
    nthreads = omp_get_num_threads();
}   
    tid = omp_get_thread_num();
    threadtime[tid] = omp_get_wtime();

#pragma omp parallel for schedule(static, 1) reduction(+:nprimes) nowait
    for (i = start; i <= end; i += 2) {
        if (is_prime_number(i))
            nprimes++;
    }
    threadtime[tid] = omp_get_wtime() - threadtime[tid];
}
    t = hpctimer_getwtime() - t;
    printf("Number of primes found: %d\n", nprimes);
    
    for (i = 0; i < nthreads; i++) {
        printf("Thread %d time: %.6f\n", i, threadtime[i]); 
    }

    printf("Elapsed time: %.6f sec.\n", t);
    return EXIT_SUCCESS;
}

