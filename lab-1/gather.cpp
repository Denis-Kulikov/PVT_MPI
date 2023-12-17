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

	int count = 1024;
    char *buf;
    double t = MPI_Wtime();
    MPI_Request reqs[size - 1];
    MPI_Status stats[size - 1];

    if (rank == root) {
        buf = (char*)malloc(sizeof(*buf) * count * size);
		for (int i = 0; i < size * count; i++)
			buf[i] = '*';
	} else {
		buf = (char*)malloc(sizeof(*buf) * count);
		for (int i = 0; i < count; i++)
			buf[i] = '*';
	} 

	if (rank == root) {
		for (int i = 0; i < size; i++)
		{
			if (i != root) 
                MPI_Irecv(buf + count * i, count, MPI_CHAR, i, 0, MPI_COMM_WORLD, &reqs[i - 1]);
		}
	} else {
		MPI_Isend(buf, count, MPI_CHAR, root, 0, MPI_COMM_WORLD, &reqs[0]);
	}

	t = MPI_Wtime() - t;
	printf("[%d] Time: %f\n", rank, t);
	if (rank == root) {
		MPI_Waitall(size - 1, reqs, stats);
		for (int i = 0; i < count * size; i++)
		{
			if (buf[i] != '*')
				printf("%d invalid buf\n", rank);
		}
	}
	
	MPI_Finalize();

	return 0;
}