#include "dump.h"
#include "plot.h"
#include "prof.h"

extern double *plot_values;
extern int recv_cnt;

static MPI_T_pvar_session session;
static MPI_T_pvar_handle umq_sz_handle;

static int rank;
static int size;

static int iters;

int MPI_Init(int *argc, char ***argv)
{
	int count, desc_len, err, i, name_len, num, pvar_index, thread_sup;
	int atomic, bind, continuous, readonly, var_class, verbosity;
	char desc[DESCLEN], name[NAMELEN];

	MPI_Comm comm;
	MPI_Datatype datatype;
	MPI_T_enum enumtype;

	// Init MPI
	TRY (PMPI_Init(argc, argv));

	// Init MPI tools interface
	TRY (PMPI_T_init_thread(MPI_THREAD_SINGLE, &thread_sup));

	if (thread_sup != MPI_THREAD_SINGLE) {
//		fprintf(stderr, "This MPI implementation doesn`t support multiple threads.\n");
		fprintf(stderr, "Ret lvl: %d, need: %d\n", thread_sup, MPI_THREAD_SINGLE);
		return -1;
	}

	// Get total number of pvars
	TRY (PMPI_T_pvar_get_num(&num));

	pvar_index = -1;
	i = 0;

	// Iterate thru all pvars until don`t get the desired one
	for (i = 0; i < num; i++) {
		name_len = NAMELEN;
		desc_len = DESCLEN;

		TRY (PMPI_T_pvar_get_info(i, name, &name_len, &verbosity, &var_class, &datatype,
			&enumtype, desc, &desc_len, &bind, &readonly, &continuous, &atomic));

		if (strcmp(name, "unexpected_recvq_length") == 0) {
			pvar_index = i;
			break;
		}
	}

	assert(pvar_index >= 0);
	assert(var_class == MPI_T_PVAR_CLASS_LEVEL);
	assert(datatype == MPI_UNSIGNED);

	// Create MPI tools interface session
	TRY (PMPI_T_pvar_session_create(&session));

	comm = MPI_COMM_WORLD;

	// Get a handle and bind it to MPI_COMM_WORLD
	TRY (PMPI_T_pvar_handle_alloc(session, pvar_index, &comm, &umq_sz_handle, &count));

	assert(count == 1);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	printf("rank %d/%d init\n", rank, size);

	if ((*argc) < 2) {
		fprintf(stderr, "Number of iterations is not specified. Exit.\n");
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		exit(1);
	}

	iters = atoi((*argv)[1]);

	plot_values = calloc(iters, sizeof(double));
	plot_flag = 1;

	// Start (don't work?)
//	TRY (PMPI_T_pvar_start(session, umq_sz_handle));

	TRY (MPI_Barrier(MPI_COMM_WORLD));

	return MPI_SUCCESS;
}

int MPI_Recv(void * buffer,
			 int count,
			 MPI_Datatype datatype,
			 int source,
			 int tag,
			 MPI_Comm comm,
			 MPI_Status * status)
{
	int umq_sz;

	TRY (PMPI_T_pvar_read(session, umq_sz_handle, &umq_sz));

//	printf("%d -- %d\n", recv_cnt + 1, umq_sz);
	if (recv_cnt < iters) {
		plot_values[recv_cnt] = (double)umq_sz;
		recv_cnt++;
	}

	return PMPI_Recv(buffer, count, datatype, source, tag, comm, status);
}

int MPI_Send(const void * buffer,
			 int count,
			 MPI_Datatype datatype,
			 int source,
			 int tag,
			 MPI_Comm comm)
{
	// Stub
	return PMPI_Send(buffer, count, datatype, source, tag, comm);
}

int MPI_Irecv(void * buffer,
			 int count,
			 MPI_Datatype datatype,
			 int source,
			 int tag,
			 MPI_Comm comm,
			 MPI_Request * request)
{
	int umq_sz;

	TRY (PMPI_T_pvar_read(session, umq_sz_handle, &umq_sz));

//	printf("%d -- %d\n", recv_cnt + 1, umq_sz);
	if (recv_cnt < iters) {
		plot_values[recv_cnt] = (double)umq_sz;
		recv_cnt++;
	}

	return PMPI_Irecv(buffer, count, datatype, source, tag, comm, request);
}

int MPI_Isend(const void * buffer,
			 int count,
			 MPI_Datatype datatype,
			 int source,
			 int tag,
			 MPI_Comm comm,
			 MPI_Request * request)
{
	// Stub
	return PMPI_Isend(buffer, count, datatype, source, tag, comm, request);
}

int MPI_Finalize(void)
{
	TRY (PMPI_T_pvar_handle_free(session, &umq_sz_handle));
	TRY (PMPI_T_pvar_session_free(&session));
	TRY (PMPI_T_finalize());

	if (rank == 0) {
		plot();
		dump();
	}

	free(plot_values);

	return PMPI_Finalize();
}
