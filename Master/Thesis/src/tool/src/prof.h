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
			if (pmq_sz_plot_flag)           \
				free(pmq_sz_plot_values);   \
			if (umq_sz_plot_flag)           \
				free(umq_sz_plot_values);   \
			if (pmq_ma_plot_flag)           \
				free(pmq_ma_plot_values);   \
			if (umq_ma_plot_flag)           \
				free(umq_ma_plot_values);   \
			if (pmq_st_plot_flag)           \
				free(pmq_st_plot_values);   \
			if (umq_st_plot_flag)           \
				free(umq_st_plot_values);   \
            MPI_Abort(MPI_COMM_WORLD, err); \
		}                                   \
    } while (0)


#define NAMELEN 128
#define DESCLEN 256

int err;
int iters;
int recv_cnt;

/* Posted queue size */

double *pmq_sz_plot_values;
int pmq_sz_plot_flag;

/* Unexpected queue size */

double *umq_sz_plot_values;
int umq_sz_plot_flag;

/* Posted queue matching attempts */

double *pmq_ma_plot_values;
int pmq_ma_plot_flag;

/* Unexpected queue matching attempts */

double *umq_ma_plot_values;
int umq_ma_plot_flag;

/* Posted queue searching time spent */

double *pmq_st_plot_values;
int pmq_st_plot_flag;

/* Unexpected queue searching time spent */

double *umq_st_plot_values;
int umq_st_plot_flag;

/* Unexpected queue allocated buffer size */

unsigned long long umq_bs_tot;
unsigned long long umq_bs_max;
unsigned long long umq_bs_min;

#endif
