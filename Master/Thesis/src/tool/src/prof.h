#ifndef __PROFILING_IF_H__
#define __PROFILING_IF_H__

#include <assert.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//#include "gnuplot_i.h"

#define TRY(func)                           \
    do {                                    \
        err = (func);                       \
        if (err != MPI_SUCCESS) {           \
			if (plot_flag)                  \
				free(plot_values);          \
            MPI_Abort(MPI_COMM_WORLD, err); \
		}                                   \
    } while (0)


#define NAMELEN 128
#define DESCLEN 256

int err;

double *plot_values;
int plot_flag;
int recv_cnt;

//void plot(void);

#endif
