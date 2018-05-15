#include "prof.h"

double plot_values[10000];
int recv_cnt;
int delay_ms = 5000;

static MPI_T_pvar_session session;
static MPI_T_pvar_handle umq_sz_handle;

int MPI_Init(int * argc, char *** argv)
{
	int count, desc_len, err, i, name_len, num, pvar_index, thread_sup;
	int atomic, bind, continuous, readonly, var_class, verbosity;
	char desc[NAMELEN], name[NAMELEN];

	MPI_Comm comm;
	MPI_Datatype datatype;
	MPI_T_enum enumtype;

	// Init MPI
	TRY (PMPI_Init(argc, argv));

	// Init MPI tools interface
	TRY (PMPI_T_init_thread(MPI_THREAD_FUNNELED, &thread_sup));

	if (thread_sup != MPI_THREAD_FUNNELED)
	{
		fprintf(stderr, "This MPI implementation doesn`t support multiple threads.\n");
		fprintf(stderr, "Ret lvl: %d, need: %d\n", thread_sup, MPI_THREAD_MULTIPLE);
		return -1;
	}

	// Get total number of pvars
	TRY (PMPI_T_pvar_get_num(&num));

	pvar_index = -1;
	i = 0;

	// Iterate thru all pvars until don`t get the desired one
	for (i = 0; i < num; i++)
	{
		name_len = NAMELEN;

		TRY (PMPI_T_pvar_get_info(i, name, &name_len, &verbosity, &var_class, &datatype,
			&enumtype, desc, &desc_len, &bind, &readonly, &continuous, &atomic));

		if (strcmp(name, "unexpected_recvq_length") == 0)
		{
			pvar_index = i;
//			printf("idx: %d\n", pvar_index);
			break;
		}

		i++;
	}

	assert(pvar_index >= 0);
//	assert(var_class == MPI_T_PVAR_CLASS_LEVEL);
//	assert(datatype == MPI_INT);
//	assert(bind == MPI_T_BIND_MPI_COMM);

	if (*argc == 3 && 2 == atoi(*argv[3]))
	{
		int commsize;
		MPI_Comm_size(MPI_COMM_WORLD, &commsize);

		int dims[2] = {0, 0}, periodic[2] = {0, 0};
		MPI_Dims_create(commsize, 2, dims);

		MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periodic, 0, &comm);
	}
	else
	{
		comm = MPI_COMM_WORLD;
	}

	// Create MPI tools interface session
	TRY (PMPI_T_pvar_session_create(&session));

	// Get a handle and bind it to MPI_COMM_WORLD
	TRY (PMPI_T_pvar_handle_alloc(session, pvar_index, &comm, &umq_sz_handle, &count));

	assert(count == 1);

	// Start variable
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
	int umq_sz;

	TRY (PMPI_T_pvar_read(session, umq_sz_handle, &umq_sz));

	printf("%d -- %d\n", recv_cnt + 1, umq_sz);
	plot_values[recv_cnt] = (double)umq_sz;
	recv_cnt++;
	usleep(delay_ms);

	return PMPI_Recv(buffer, count, datatype, source, tag, comm, status);
}

int MPI_Send(const void * buffer,
			 int count,
			 MPI_Datatype datatype,
			 int source,
			 int tag,
			 MPI_Comm comm)
{
	usleep(delay_ms);
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

	printf("%d -- %d\n", recv_cnt + 1, umq_sz);
	plot_values[recv_cnt] = (double)umq_sz;
	recv_cnt++;	// hmm.. not threadsafe?! but final value is OK
	usleep(delay_ms);

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
	usleep(delay_ms);
	return PMPI_Isend(buffer, count, datatype, source, tag, comm, request);
}

int MPI_Finalize(void)
{
	TRY (PMPI_T_pvar_handle_free(session, &umq_sz_handle));
	TRY (PMPI_T_pvar_session_free(&session));
	TRY (PMPI_T_finalize());

	return PMPI_Finalize();
}
