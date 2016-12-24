#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int rank, ntasks;

    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;

    MPI_Get_processor_name(processor_name, &name_len);

    char filename[64] = "";

    snprintf(filename, sizeof(filename), "%d.pid", (int)getpid());

    FILE *fp = fopen(filename, "w");

    fprintf(fp, "Hi, Jack! From processor %s, rank %d out of"
    " %d processors.\n", processor_name, rank, ntasks);

    fprintf(fp, "PID: %d\n", getpid());
    fprintf(fp, "PPID: %d\n", getppid());
    fprintf(fp, "GID: %d\n", getgid());
    fprintf(fp, "EGID: %d\n", getegid());
    fprintf(fp, "UID: %d\n", getuid());
    fprintf(fp, "EUID: %d\n\n", geteuid());

    fclose(fp);

    MPI_Finalize();
    return 0;
}
