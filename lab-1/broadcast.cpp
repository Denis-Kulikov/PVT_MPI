#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (int argc, char **argv)
{
	int rank, size, len;
	int root = 0;
 	char procname[MPI_MAX_PROCESSOR_NAME];
 	MPI_Init(&argc, &argv);
 	MPI_Get_processor_name(procname, &len);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int count = 1024 * 1024;
	char *buf = (char*)malloc(sizeof(*buf) * count);
	double t = MPI_Wtime();

	if (rank == root)
	{
		for (int i = 0; i < count; i++)
			buf[i] = '*';
		for (int i = 0; i < size; i++)
		{
			if (i != root)
				MPI_Send(buf, count, MPI_CHAR, i, 0, MPI_COMM_WORLD);
		}
	} else {
		MPI_Recv(buf, count, MPI_CHAR, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int i = 0; i < count; i++)
			if (buf[i] != '*')
				printf("%d invalid buf\n", rank);
	}
	t = MPI_Wtime() - t;
	printf("[%d] Time: %f\n", rank, t);
	MPI_Finalize();

	return 0;
}