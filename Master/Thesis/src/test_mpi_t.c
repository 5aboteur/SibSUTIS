#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define MSGSIZE 10
#define NAMELEN 100

int get_cvar_value(int index, MPI_Comm comm, int *val);
int print_all_cvars(void);

//int get_pvar_value(int index, MPI_Comm comm, int *val);
int print_all_pvars(void);
void _print_title(char *str, int n);

int main(int argc, char *argv[])
{
    int i, err, size, rank, dest, src, tag = 1337;
    int cvar_val;
    int threadsupport;
    int msg[MSGSIZE];

    MPI_Status status;

    // Init MPI
    err = MPI_Init_thread(0, 0, MPI_THREAD_SINGLE, &threadsupport);
    if (err != MPI_SUCCESS) return err;

    // Init MPI tools interface
    err = MPI_T_init_thread(MPI_THREAD_SINGLE, &threadsupport);
    if (err != MPI_SUCCESS) return err;

    // Get size
    err = MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (err != MPI_SUCCESS) return err;

    // Get rank
    err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (err != MPI_SUCCESS) return err;

//    printf("Comm size: %d\n Comm rank: %d\n", size, rank);

    // Print MPI vars
    if (rank == 0) {
        printf("\n");
        err = print_all_pvars();
        if (err != MPI_SUCCESS) return err;

        err = print_all_cvars();
        if (err != MPI_SUCCESS) return err;

//        printf("\u2502\n");

        // 42 ~> MPIR_CVAR_CH3_EAGER_MAX_MSG_SIZE
        err = get_cvar_value(42, MPI_COMM_WORLD, &cvar_val);
        if (err != MPI_SUCCESS) return err;

        printf("\u2514\u2500> \033[1;33mCH3\033[0m Eager limit: %d\n", cvar_val);

        // 60 ~> MPIR_CVAR_NEMESIS_LMT_DMA_THRESHOLD
        err = get_cvar_value(60, MPI_COMM_WORLD, &cvar_val);
        if (err != MPI_SUCCESS) return err;

        printf("\u2514\u2500> \033[1;33mNEM\033[0m LMT threshold: %d\n\n", cvar_val);
    }
/*
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
*/
    // MPI tools interface job is done
    err = MPI_T_finalize();
    if (err != MPI_SUCCESS) return err;

    // MPI job is done
    err = MPI_Finalize();
    if (err != MPI_SUCCESS) return err;

//    printf("\nJob is done!\n");

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
    int i, err, namelen, num, bind, verbose, scope;
    char name[NAMELEN];

    MPI_Datatype datatype;

    err = MPI_T_cvar_get_num(&num);
    if (err != MPI_SUCCESS) return err;

    _print_title("MPI CONTROL VARIABLES", num);

    printf("\u2502\t %-50s \u2502\n", " . . . ");

    for (i = 0; i < num; ++i) {
        namelen = NAMELEN;
        err = MPI_T_cvar_get_info(i, name, &namelen, &verbose,
            &datatype, NULL, NULL, NULL, &bind, &scope);

        if (err != MPI_SUCCESS || err == MPI_T_ERR_INVALID_INDEX)
            return err;

        if (strstr(name, "EAGER") != NULL ||
            strstr(name, "SHM") != NULL ||
            strstr(name, "LMT") != NULL)
            printf("\u2502 %3d \u2502 %-50s  \u2502\n", i, name);
    }

    printf("\u2502\t %-50s \u2502\n", " . . . ");

    return err;
}

int print_all_pvars(void)
{
    int i, err, namelen, num, bind, verbose,
        varclass, readonly, continuous, atomic;
    char name[NAMELEN];

    MPI_Datatype datatype;

    err = MPI_T_pvar_get_num(&num);
    if (err != MPI_SUCCESS) return err;

    _print_title("MPI PERFORMANCE VARIABLES", num);

    for (i = 0; i < num; ++i) {
        namelen = NAMELEN;
        err = MPI_T_pvar_get_info(i, name, &namelen, &verbose, NULL,
            &datatype, NULL, NULL, NULL, &bind, &readonly, &continuous,
            &atomic);

        if (err != MPI_SUCCESS || err == MPI_T_ERR_INVALID_INDEX)
            return err;

        printf("\u2502 %3d \u2502 %-50s  \u2502\n", i, name);
    }

    return err;
}

void _print_title(char *str, int n)
{
    printf("\u250c");
    printf("\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500");
    printf("\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500");
    printf("\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500");
    printf("\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500");
    printf("\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500");
    printf("\u2510\n");
    printf("\u2502  #  \u2502\t\t%s {%d}\t\t    \u2502\n", str, n);
    printf("\u251c");
    printf("\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500");
    printf("\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500");
    printf("\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500");
    printf("\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500");
    printf("\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500");
    printf("\u2524\n");
}
