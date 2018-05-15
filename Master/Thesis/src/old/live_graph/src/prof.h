#ifndef _PROF_H_
#define _PROF_H_

#include <assert.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TRY(func)                           \
    do {                                    \
        err = (func);                       \
        if (err != MPI_SUCCESS)             \
            MPI_Abort(MPI_COMM_WORLD, err); \
    } while (0)


#define NAMELEN 100

int err;

#endif
