#include <stdio.h>
#include <stdlib.h>
#include <iostream>
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
	int count = 8 * 1024 * 1024;
	char *buf = (char*)malloc(sizeof(*buf) * count);
	double t = MPI_Wtime();

    // std::cout << rank << std::endl;

	if (rank == root)
	{
		buf[0] = 'x';
		for (int i = 0; i < size; i++)
		{
			if (i != root)
				MPI_Send(buf, count, MPI_CHAR, i, 0, MPI_COMM_WORLD);
		}
	} else {
		MPI_Recv(buf, count, MPI_CHAR, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if (buf[0] != 'x')
			fprintf(stderr, "%d invalid buf\n", rank);
	}
	t = MPI_Wtime() - t;
	printf("Rank: %d\n", rank);
	printf("Time: %f\n", t);
	MPI_Finalize();

	return 0;
}