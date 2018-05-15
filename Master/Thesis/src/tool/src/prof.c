#include "dump.h"
#include "plot.h"
#include "prof.h"

extern double *umq_plot_values;
extern double *pmq_plot_values;

extern int umq_recv_cnt;
extern int pmq_recv_cnt;

static MPI_T_pvar_session session;
static MPI_T_pvar_handle umq_sz_handle;
static MPI_T_pvar_handle pmq_sz_handle;

static int rank;
static int size;

static int iters;

int MPI_Init(int *argc, char ***argv)
{
	int count, desc_len, err, i, name_len, num, thread_sup;
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

	int umq_pvar_index = -1;
	int pmq_pvar_index = -1;

	int pvars_to_handle = 2;

	// Iterate thru all pvars until don`t get the desired one
	for (i = 0; i < num; i++) {
		name_len = NAMELEN;
		desc_len = DESCLEN;

		TRY (PMPI_T_pvar_get_info(i, name, &name_len, &verbosity, &var_class, &datatype,
			&enumtype, desc, &desc_len, &bind, &readonly, &continuous, &atomic));

		if (strcmp(name, "posted_recvq_length") == 0) {
			pmq_pvar_index = i;
			pvars_to_handle--;
		}

		if (strcmp(name, "unexpected_recvq_length") == 0) {
			umq_pvar_index = i;
			pvars_to_handle--;
		}

		if (!pvars_to_handle) break;
	}

	assert(umq_pvar_index >= 0);
	assert(pmq_pvar_index >= 0);
	assert(var_class == MPI_T_PVAR_CLASS_LEVEL);
	assert(datatype == MPI_UNSIGNED);

	// Create MPI tools interface session
	TRY (PMPI_T_pvar_session_create(&session));

	comm = MPI_COMM_WORLD;

	// Get a handle and bind it to MPI_COMM_WORLD

	TRY (PMPI_T_pvar_handle_alloc(session, umq_pvar_index, &comm, &umq_sz_handle, &count));
	assert(count == 1);

	TRY (PMPI_T_pvar_handle_alloc(session, pmq_pvar_index, &comm, &pmq_sz_handle, &count));
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

	umq_plot_values = calloc(iters, sizeof(double));
	umq_plot_flag = 1;

	pmq_plot_values = calloc(iters, sizeof(double));
	pmq_plot_flag = 1;

	// Start (don't work?)
//	TRY (PMPI_T_pvar_start(session, umq_sz_handle));

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
	int umq_sz, pmq_sz;

	TRY (PMPI_T_pvar_read(session, umq_sz_handle, &umq_sz));
	TRY (PMPI_T_pvar_read(session, pmq_sz_handle, &pmq_sz));

//	printf("%d -- %d\n", recv_cnt + 1, umq_sz);
	if (umq_recv_cnt < iters && pmq_recv_cnt < iters) {
		umq_plot_values[umq_recv_cnt] = (double)umq_sz;
		umq_recv_cnt++;

		pmq_plot_values[pmq_recv_cnt] = (double)pmq_sz;
		pmq_recv_cnt++;
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
	int umq_sz, pmq_sz;

	TRY (PMPI_T_pvar_read(session, umq_sz_handle, &umq_sz));
	TRY (PMPI_T_pvar_read(session, pmq_sz_handle, &pmq_sz));

//	printf("%d -- %d\n", recv_cnt + 1, umq_sz);
	if (umq_recv_cnt < iters && pmq_recv_cnt < iters) {
		umq_plot_values[umq_recv_cnt] = (double)umq_sz;
		umq_recv_cnt++;

		pmq_plot_values[pmq_recv_cnt] = (double)pmq_sz;
		pmq_recv_cnt++;
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
	TRY (PMPI_T_pvar_handle_free(session, &pmq_sz_handle));
	TRY (PMPI_T_pvar_session_free(&session));
	TRY (PMPI_T_finalize());

	if (rank == 0) {
		plot();
		dump();
	}

	free(umq_plot_values);
	free(pmq_plot_values);

	return PMPI_Finalize();
}
