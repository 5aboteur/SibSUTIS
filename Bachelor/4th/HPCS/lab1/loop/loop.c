/*
 * loop.c:
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hpctimer.h"

enum { n = 16 * 1024 * 1024 };

int main()
{
    int *v, i, sum, s1, s2, s3, s4, s5, s6, s7, s8,
        s9, s10, s11, s12, s13, s14, s15, s16;
    double t;
    
    if ( (v = malloc(sizeof(*v) * n)) == NULL) {
        fprintf(stderr, "No enough memory\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < n; i++)
        v[i] = 1;
   
    t = hpctimer_wtime();
    /* TODO: Unroll this loop */
    s1 = s2 = s3 = s4 = s5 = s6 = s7 = s8 = s9 =
    s10 = s11 = s12 = s13 = s14 = s15 = s16 = 0;

    for (sum = 0, i = 0; i < n; i += 4) {
        s1 += v[i];
	s2 += v[i + 1];
	s3 += v[i + 2];
	s4 += v[i + 3];
    }
    t = hpctimer_wtime() - t;

    sum = s1 + s2 + s3 + s4;

    printf("Sum = %d\n", sum);
    printf("Elapsed time (sec.): %.6f\n", t);

    free(v);
    return 0;
}
