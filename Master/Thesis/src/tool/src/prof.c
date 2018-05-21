#include "dump.h"
#include "plot.h"
#include "prof.h"

static MPI_T_pvar_session session;

static MPI_T_pvar_handle pmq_sz_handle;
static MPI_T_pvar_handle pmq_ma_handle;
static MPI_T_pvar_handle pmq_st_handle;

static MPI_T_pvar_handle umq_sz_handle;
static MPI_T_pvar_handle umq_ma_handle;
static MPI_T_pvar_handle umq_st_handle;

static MPI_T_pvar_handle umq_bs_handle;

static int rank;
static int size;

//static int iters;

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

	int pmq_sz_pvar_index = -1;
	int umq_sz_pvar_index = -1;

	int pmq_ma_pvar_index = -1;
	int umq_ma_pvar_index = -1;

	int pmq_st_pvar_index = -1;
	int umq_st_pvar_index = -1;

	int umq_bs_pvar_index = -1;

	int pvars_to_handle = 7;

	// Iterate thru all pvars until don`t get the desired one
	for (i = 0; i < num; i++) {
		name_len = NAMELEN;
		desc_len = DESCLEN;

		TRY (PMPI_T_pvar_get_info(i, name, &name_len, &verbosity, &var_class, &datatype,
			&enumtype, desc, &desc_len, &bind, &readonly, &continuous, &atomic));

		if (strcmp(name, "posted_recvq_length") == 0) {
			pmq_sz_pvar_index = i;
			pvars_to_handle--;
			assert(pmq_sz_pvar_index >= 0);
			assert(var_class == MPI_T_PVAR_CLASS_LEVEL);
			assert(datatype == MPI_UNSIGNED);
		}

		if (strcmp(name, "unexpected_recvq_length") == 0) {
			umq_sz_pvar_index = i;
			pvars_to_handle--;
			assert(umq_sz_pvar_index >= 0);
			assert(var_class == MPI_T_PVAR_CLASS_LEVEL);
			assert(datatype == MPI_UNSIGNED);
		}

		if (strcmp(name, "posted_recvq_match_attempts") == 0) {
			pmq_ma_pvar_index = i;
			pvars_to_handle--;
			assert(pmq_ma_pvar_index >= 0);
			assert(var_class == MPI_T_PVAR_CLASS_COUNTER);
			assert(datatype == MPI_UNSIGNED_LONG_LONG);
		}

		if (strcmp(name, "unexpected_recvq_match_attempts") == 0) {
			umq_ma_pvar_index = i;
			pvars_to_handle--;
			assert(umq_ma_pvar_index >= 0);
			assert(var_class == MPI_T_PVAR_CLASS_COUNTER);
			assert(datatype == MPI_UNSIGNED_LONG_LONG);
		}

		if ((strcmp(name, "time_failed_matching_postedq") == 0) &&
			(var_class == MPI_T_PVAR_CLASS_TIMER)) {
			pmq_st_pvar_index = i;
			pvars_to_handle--;
			assert(pmq_st_pvar_index >= 0);
			assert(var_class == MPI_T_PVAR_CLASS_TIMER);
			assert(datatype == MPI_DOUBLE);
		}

		if ((strcmp(name, "time_matching_unexpectedq") == 0) &&
			(var_class == MPI_T_PVAR_CLASS_TIMER)) {
			umq_st_pvar_index = i;
			pvars_to_handle--;
			assert(umq_st_pvar_index >= 0);
			assert(var_class == MPI_T_PVAR_CLASS_TIMER);
			assert(datatype == MPI_DOUBLE);
		}

		if (strcmp(name, "unexpected_recvq_buffer_size") == 0) {
			umq_bs_pvar_index = i;
			pvars_to_handle--;
			assert(umq_bs_pvar_index >= 0);
			assert(var_class == MPI_T_PVAR_CLASS_LEVEL);
			assert(datatype == MPI_UNSIGNED_LONG_LONG);
		}

		if (!pvars_to_handle) break;
	}

	// Create MPI tools interface session
	TRY (PMPI_T_pvar_session_create(&session));

	comm = MPI_COMM_WORLD;

	// Get a handle and bind it to MPI_COMM_WORLD

	TRY (PMPI_T_pvar_handle_alloc(session, pmq_sz_pvar_index, &comm, &pmq_sz_handle, &count));
	assert(count == 1);

	TRY (PMPI_T_pvar_handle_alloc(session, umq_sz_pvar_index, &comm, &umq_sz_handle, &count));
	assert(count == 1);

	TRY (PMPI_T_pvar_handle_alloc(session, pmq_ma_pvar_index, &comm, &pmq_ma_handle, &count));
	assert(count == 1);

	TRY (PMPI_T_pvar_handle_alloc(session, umq_ma_pvar_index, &comm, &umq_ma_handle, &count));
	assert(count == 1);

	TRY (PMPI_T_pvar_handle_alloc(session, pmq_st_pvar_index, &comm, &pmq_st_handle, &count));
	assert(count == 1);

	TRY (PMPI_T_pvar_handle_alloc(session, umq_st_pvar_index, &comm, &umq_st_handle, &count));
	assert(count == 1);

	TRY (PMPI_T_pvar_handle_alloc(session, umq_bs_pvar_index, &comm, &umq_bs_handle, &count));
	assert(count == 1);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	printf("Rank %d/%d init\n", rank, size - 1);

	if ((*argc) < 2) {
		fprintf(stderr, "Number of iterations is not specified. Exit.\n");
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		exit(1);
	}

	iters = atoi((*argv)[1]);

	pmq_sz_plot_values = calloc(iters, sizeof(double));
	pmq_sz_plot_flag = 1;

	umq_sz_plot_values = calloc(iters, sizeof(double));
	umq_sz_plot_flag = 1;

	pmq_ma_plot_values = calloc(iters, sizeof(double));
	pmq_ma_plot_flag = 1;

	umq_ma_plot_values = calloc(iters, sizeof(double));
	umq_ma_plot_flag = 1;

	pmq_st_plot_values = calloc(iters, sizeof(double));
	pmq_st_plot_flag = 1;

	umq_st_plot_values = calloc(iters, sizeof(double));
	umq_st_plot_flag = 1;

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
	unsigned int pmq_sz, umq_sz;
	unsigned long long pmq_ma, umq_ma, umq_bs;
	double pmq_st, umq_st;

	TRY (PMPI_T_pvar_read(session, pmq_sz_handle, &pmq_sz));
	TRY (PMPI_T_pvar_read(session, umq_sz_handle, &umq_sz));

	TRY (PMPI_T_pvar_read(session, pmq_ma_handle, &pmq_ma));
	TRY (PMPI_T_pvar_read(session, umq_ma_handle, &umq_ma));

	TRY (PMPI_T_pvar_read(session, pmq_st_handle, &pmq_st));
	TRY (PMPI_T_pvar_read(session, umq_st_handle, &umq_st));

	TRY (PMPI_T_pvar_read(session, umq_bs_handle, &umq_bs));

//	printf("%d: %u - %u\n", source, pmq_sz, umq_sz);

	if (recv_cnt < iters) {
		pmq_sz_plot_values[recv_cnt] = (double)pmq_sz;
		umq_sz_plot_values[recv_cnt] = (double)umq_sz;

		pmq_ma_plot_values[recv_cnt] = (double)pmq_ma;
		umq_ma_plot_values[recv_cnt] = (double)umq_ma;

		pmq_st_plot_values[recv_cnt] = pmq_st;
		umq_st_plot_values[recv_cnt] = umq_st;

		recv_cnt++;

		if (umq_bs_min > umq_bs) umq_bs_min = umq_bs;
		if (umq_bs_max < umq_bs) umq_bs_max = umq_bs;

		umq_bs_tot += umq_bs;
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
	usleep(10000);
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
	unsigned int pmq_sz, umq_sz;
	unsigned long long pmq_ma, umq_ma, umq_bs;
	double pmq_st, umq_st;

	TRY (PMPI_T_pvar_read(session, pmq_sz_handle, &pmq_sz));
	TRY (PMPI_T_pvar_read(session, umq_sz_handle, &umq_sz));

	TRY (PMPI_T_pvar_read(session, pmq_ma_handle, &pmq_ma));
	TRY (PMPI_T_pvar_read(session, umq_ma_handle, &umq_ma));

	TRY (PMPI_T_pvar_read(session, pmq_st_handle, &pmq_st));
	TRY (PMPI_T_pvar_read(session, umq_st_handle, &umq_st));

	TRY (PMPI_T_pvar_read(session, umq_bs_handle, &umq_bs));

//	printf("%d: %u - %u\n", source, pmq_sz, umq_sz);

	if (recv_cnt < iters) {
		pmq_sz_plot_values[recv_cnt] = (double)pmq_sz;
		umq_sz_plot_values[recv_cnt] = (double)umq_sz;

		pmq_ma_plot_values[recv_cnt] = (double)pmq_ma;
		umq_ma_plot_values[recv_cnt] = (double)umq_ma;

		pmq_st_plot_values[recv_cnt] = pmq_st;
		umq_st_plot_values[recv_cnt] = umq_st;

		recv_cnt++;

		if (umq_bs_min > umq_bs) umq_bs_min = umq_bs;
		if (umq_bs_max < umq_bs) umq_bs_max = umq_bs;

		umq_bs_tot += umq_bs;
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
	TRY (PMPI_T_pvar_handle_free(session, &pmq_sz_handle));
	TRY (PMPI_T_pvar_handle_free(session, &umq_sz_handle));

	TRY (PMPI_T_pvar_handle_free(session, &pmq_ma_handle));
	TRY (PMPI_T_pvar_handle_free(session, &umq_ma_handle));

	TRY (PMPI_T_pvar_handle_free(session, &pmq_st_handle));
	TRY (PMPI_T_pvar_handle_free(session, &umq_st_handle));

	TRY (PMPI_T_pvar_handle_free(session, &umq_bs_handle));

	TRY (PMPI_T_pvar_session_free(&session));
	TRY (PMPI_T_finalize());

	if (rank == 3) {
		plot();
		dump();
	}

	free(pmq_sz_plot_values);
	free(umq_sz_plot_values);

	free(pmq_ma_plot_values);
	free(umq_ma_plot_values);

	free(pmq_st_plot_values);
	free(umq_st_plot_values);

	return PMPI_Finalize();
}
