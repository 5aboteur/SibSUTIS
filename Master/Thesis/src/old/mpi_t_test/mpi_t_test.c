#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define EAGER_MSG_SIZE 10
#define RNDV_MSG_SIZE 1000000

#define NAMELEN 100

int test_eager(MPI_T_pvar_session session, MPI_T_pvar_handle pvar_handler, int rank);
int test_rndv(MPI_T_pvar_session session, MPI_T_pvar_handle pvar_handler, int rank);

int get_cvar_value(int index, MPI_Comm comm, int *val);
int print_all_cvars(void);
int print_all_pvars(void);

void _print_title(char *str, int n);

int main(int argc, char *argv[])
{
    int err, size, rank, count, threadsupport;

    MPI_T_pvar_session session;
    MPI_T_pvar_handle umq_sz_handler;

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

    // Print MPI vars
    if (rank == 0) {
        printf("\n");
        err = print_all_pvars();
        if (err != MPI_SUCCESS) return err;

        err = print_all_cvars();
        if (err != MPI_SUCCESS) return err;

		int cvar_val;

        // 42 ~> MPIR_CVAR_CH3_EAGER_MAX_MSG_SIZE
        err = get_cvar_value(42, MPI_COMM_WORLD, &cvar_val);
        if (err != MPI_SUCCESS) return err;

        printf("\u2514\u2500> \033[1;33mCH3\033[0m Eager limit: %d\n", cvar_val);

        // 60 ~> MPIR_CVAR_NEMESIS_LMT_DMA_THRESHOLD
        err = get_cvar_value(60, MPI_COMM_WORLD, &cvar_val);
        if (err != MPI_SUCCESS) return err;

        printf("\u2514\u2500> \033[1;33mNEM\033[0m LMT threshold: %d\n\n", cvar_val);
    }

    // 2 ~> unexpected_recvq_buffer_size
    MPI_T_pvar_session_create(&session);
    MPI_T_pvar_handle_alloc(session, 2, NULL, &umq_sz_handler, &count);

    // Test send thru Eager proto
    if (rank == 0)
        printf("\033[1;32m > EAGER TEST < \033[0m\n");
    err = test_eager(session, umq_sz_handler, rank);
    if (err != MPI_SUCCESS) return err;

    // Test send thru Rendezvous proto
    if (rank == 0)
        printf("\033[1;31m > RNDV TEST < \033[0m\n");
    err = test_rndv(session, umq_sz_handler, rank);
    if (err != MPI_SUCCESS) return err;

    // Free session
    MPI_T_pvar_handle_free(session, &umq_sz_handler);
    MPI_T_pvar_session_free(&session);

    // MPI tools interface job is done
    err = MPI_T_finalize();
    if (err != MPI_SUCCESS) return err;

    // MPI job is done
    err = MPI_Finalize();
    if (err != MPI_SUCCESS) return err;

    printf("\n<%d> -- Job is done!\n", rank);

    return 0;
}

int test_eager(MPI_T_pvar_session session, MPI_T_pvar_handle pvar_handler, int rank)
{
    int err, pvar_val;

    // Sender side
    if (rank == 0) {
        int i;
        size_t send_buf[EAGER_MSG_SIZE];

        for (i = 0; i < EAGER_MSG_SIZE; ++i)
            send_buf[i] = 666;

        printf("Message size: %li (x 5)\n", EAGER_MSG_SIZE * sizeof(int));

        for (i = 0; i < 5; ++i) {
            err = MPI_Send(send_buf, EAGER_MSG_SIZE, MPI_INT, 1, i, MPI_COMM_WORLD);
            if (err != MPI_SUCCESS) return err;
            printf("<%d> -- Chunk %d sent\n", rank, i);
        }
    }

    // Receiver side
    if (rank == 1) {
        int i;
        size_t recv_buf[EAGER_MSG_SIZE];

        MPI_Status status;

        for (i = 0; i < EAGER_MSG_SIZE; ++i)
            recv_buf[i] = -1;

        for (i = 4; i >= 0; --i) {
            err = MPI_Recv(recv_buf, EAGER_MSG_SIZE, MPI_INT, 0, i, MPI_COMM_WORLD, &status);
            if (err != MPI_SUCCESS) return err;

            MPI_T_pvar_read(session, pvar_handler, &pvar_val);
            printf("<%d> -- Chunk %d received\n", rank, i);
            printf("UMQ size: %d\n", pvar_val);

//            printf("Gotcha! Here it is: ");

//            for (i = 0; i < EAGER_MSG_SIZE; ++i)
//                printf("%d ", recv_buf[i]);

//            printf("\n");
        }
    }

    if (rank == 0)
        printf("\n<%d> -- Message delivered!\n\n", rank);
    else
        printf("\n<%d> -- Message received!\n\n", rank);

    MPI_Barrier(MPI_COMM_WORLD);

    return err;
}

int test_rndv(MPI_T_pvar_session session, MPI_T_pvar_handle pvar_handler, int rank)
{
    int err, pvar_val;

    // Sender side
    if (rank == 0) {
        int i;
        size_t send_buf[RNDV_MSG_SIZE];

        for (i = 0; i < RNDV_MSG_SIZE; ++i)
            send_buf[i] = 999;

        printf("Message size: %li (x 5)\n", RNDV_MSG_SIZE * sizeof(int));

        for (i = 0; i < 5; ++i) {
            err = MPI_Send(send_buf, RNDV_MSG_SIZE, MPI_INT, 1, i, MPI_COMM_WORLD);
            if (err != MPI_SUCCESS) return err;
            printf("<%d> -- Chunk %d sent\n", rank, i);
        }
    }

    // Receiver side
    if (rank == 1) {
        int i;
        size_t recv_buf[RNDV_MSG_SIZE];

        MPI_Status status;

        for (i = 0; i < RNDV_MSG_SIZE; ++i)
            recv_buf[i] = -1;

        for (i = 0; i < 5; ++i) {
            err = MPI_Recv(recv_buf, RNDV_MSG_SIZE, MPI_INT, 0, i, MPI_COMM_WORLD, &status);
            if (err != MPI_SUCCESS) return err;

            MPI_T_pvar_read(session, pvar_handler, &pvar_val);
            printf("<%d> -- Chunk %d received\n", rank, i);
            printf("UMQ size: %d\n", pvar_val);

//            printf("Gotcha! Here it is: ");

//            for (i = 0; i < EAGER_MSG_SIZE; ++i)
//                printf("%d ", recv_buf[i]);

//            printf("\n");
        }
    }

    if (rank == 0)
        printf("\n<%d> -- Message delivered!\n\n", rank);
    else
        printf("\n<%d> -- Message received!\n\n", rank);

    MPI_Barrier(MPI_COMM_WORLD);

    return err;
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
    int i, err, namelen, num, bind, verbose, readonly, continuous, atomic;
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
