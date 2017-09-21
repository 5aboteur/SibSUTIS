#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MSGSIZE 10
#define NAMELEN 100

int get_cvar_value(int index, MPI_Comm comm, int *val);
int print_all_cvars(void);

int main(int argc, char *argv[])
{
    int i, err, size, rank, dest, src, tag = 1337;
    int eager_msg_size;
    int threadsupport;
    int msg[MSGSIZE];

    MPI_Status status;

    // Init MPI tools interface
    err = MPI_T_init_thread(MPI_THREAD_SINGLE, &threadsupport);
    if (err != MPI_SUCCESS) return err;

    // Init MPI
    err = MPI_Init(&argc, &argv);
    if (err != MPI_SUCCESS) return err;

    // Get size
    err = MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (err != MPI_SUCCESS) return err;

    // Get rank
    err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (err != MPI_SUCCESS) return err;

    printf("Comm size: %d\n Comm rank: %d\n", size, rank);

/*
    err = print_all_cvars();
    if (err != MPI_SUCCESS) return err;
*/
    if (rank == 0) {
        // 42 ~> MPIR_CVAR_CH3_EAGER_MAX_MSG_SIZE
        err = get_cvar_value(42, MPI_COMM_WORLD, &eager_msg_size);
        if (err != MPI_SUCCESS) return err;

        printf("Eager limit: %d\n", eager_msg_size);
    }

    // Sender side
    if (rank == 0) {
        for (i = 0; i < MSGSIZE; ++i)
            msg[i] = i;

        err = MPI_Send(msg, MSGSIZE, MPI_INT, 1, tag, MPI_COMM_WORLD);
        if (err != MPI_SUCCESS) return err;
    }

    // Receiver side
    if (rank == 1) {
        for (i = 0; i < MSGSIZE; ++i)
            msg[i] = -1;

        err = MPI_Recv(msg, MSGSIZE, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        if (err != MPI_SUCCESS) return err;

        printf("Gotcha! Here it is: ");

        for (i = 0; i < MSGSIZE; ++i)
            printf("%d ", msg[i]);

        printf("\n");
    }

    // MPI job is done
    err = MPI_Finalize();
    if (err != MPI_SUCCESS) return err;

    // MPI tools interface job is done
    err = MPI_T_finalize();
    if (err != MPI_SUCCESS) return err;

    return 0;
}

int get_cvar_value(int index, MPI_Comm comm, int *val)
{
    int err, count;
    MPI_T_cvar_handle handle;

    err = MPI_T_cvar_handle_alloc(index, &comm, &handle, &count);
    if (err != MPI_SUCCESS) return err;

    err = MPI_T_cvar_read(handle, val);
    if (err != MPI_SUCCESS) return err;

    err = MPI_T_cvar_handle_free(&handle);

    return err;
}

int print_all_cvars(void)
{
    int i, err, num, bind, verbose, scope;
    int namelen = NAMELEN;
    char name[NAMELEN];

    MPI_Datatype datatype;

    err = MPI_T_cvar_get_num(&num);
    if (err != MPI_SUCCESS) return err;

    for (i = 0; i < num; ++i) {
        err = MPI_T_cvar_get_info(i, name, &namelen, &verbose,
            &datatype, NULL, NULL, NULL, &bind, &scope);

        if (err != MPI_SUCCESS || err == MPI_T_ERR_INVALID_INDEX)
            return err;

        printf("Var %i: %s\n", i, name);
    }

    return err;
}
