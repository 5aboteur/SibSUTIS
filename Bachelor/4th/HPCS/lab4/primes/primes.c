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
    int i;
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
        
#pragma omp parallel for schedule(static, 1) reduction(+:nprimes)
    for (i = start; i <= end; i += 2) {
        if (is_prime_number(i))
            nprimes++;
    }

    t = hpctimer_getwtime() - t;
    printf("Number of primes found: %d\n", nprimes);

    printf("Elapsed time: %.6f sec.\n", t);
    return EXIT_SUCCESS;
}

