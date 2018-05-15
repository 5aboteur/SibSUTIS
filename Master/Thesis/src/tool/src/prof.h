#ifndef __PROFILING_IF_H__
#define __PROFILING_IF_H__

#include <assert.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TRY(func)                           \
    do {                                    \
        err = (func);                       \
        if (err != MPI_SUCCESS) {           \
			if (umq_plot_flag)              \
				free(umq_plot_values);      \
			if (pmq_plot_flag)              \
				free(pmq_plot_values);      \
            MPI_Abort(MPI_COMM_WORLD, err); \
		}                                   \
    } while (0)


#define NAMELEN 128
#define DESCLEN 256

int err;

double *umq_plot_values;
int umq_plot_flag;
int umq_recv_cnt;

double *pmq_plot_values;
int pmq_plot_flag;
int pmq_recv_cnt;

#endif
