#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
	int rank, size, len, prev, next;
    int commsize;
    int count = 1024;
 	char host[MPI_MAX_PROCESSOR_NAME];

 	MPI_Init(&argc, &argv);
 	MPI_Get_processor_name(host, &len);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 	MPI_Comm_size(MPI_COMM_WORLD, &commsize);

    int inbuf[SIZE], outbuf[SIZE];
    double t = MPI_Wtime();
    MPI_Request reqs[2];
    MPI_Status stats[2];

    prev = (rank + commsize - 1) % commsize;
    next = (rank + 1) % commsize;

    for (int i = 0; i < SIZE; i++)
        outbuf[i] = 1;

    for (int i = 0; i < size; i++) {
        if (i != rank) {
            // MPI_Isend(outbuf_next, NELEMS(outbuf_next), MPI_CHAR, next, 0, MPI_COMM_WORLD);
            // MPI_Irecv(inbuf_prev, NELEMS(inbuf_prev), MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Sendrecv(outbuf, count, MPI_INT, next, 0, inbuf, count, MPI_INT, rank, 0, MPI_COMM_WORLD, &reqs[0]);
        }
    }

    t = MPI_Wtime() - t;
    printf("[%d] Time: %f\n", rank, t);
    for (int i = 0; i < SIZE; i++)
        if (inbuf_prev[i] != '*')
    printf("%d invalid buf\n", rank);

	MPI_Finalize();

    return 0;
}



