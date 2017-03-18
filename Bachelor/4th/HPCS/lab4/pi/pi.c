/*
 * pi.c:
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "hpctimer.h"

int main(int argc, char **argv)
{
    double PI25DT = 3.141592653589793238462643;
    int i, nsteps;
    double pi, x, step, sum;
    double t;

    nsteps = (argc > 1) ? atoi(argv[1]) : 1000000;
    step = 1.0 / (double)nsteps;

    t = hpctimer_getwtime();

    sum = 0.0;
    for (i = 1; i <= nsteps; i++) {
        x = (i - 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }
    pi = step * sum;

    t = hpctimer_getwtime() - t;

    printf("PI is approximately %.16f, Error is %.16f\n",
           pi, fabs(pi - PI25DT));
    printf("(nsteps = %d, step = %f)\n", nsteps, step);
    printf("Elapsed time = %.6f sec.\n", t);

    return EXIT_SUCCESS;
}

