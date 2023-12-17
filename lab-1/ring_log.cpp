#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <mpi.h>

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))
#define SIZE 1024

#define STR_LEN (9 * 2)

int main (int argc, char **argv)
{
	int rank, size, len, prev, next;
    int commsize = 4;
	int root = 0;
    int node = 2;
 	char host[MPI_MAX_PROCESSOR_NAME];
    float time[commsize];
    FILE* f;

    if (node == 1) {
        f = fopen("output.txt", "w");
        for (int i = 0; i < STR_LEN * commsize; i++)
            fprintf(f, "*");
        fclose(f);
    }

 	MPI_Init(&argc, &argv);
 	MPI_Get_processor_name(host, &len);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 	MPI_Comm_size(MPI_COMM_WORLD, &size);

    char inbuf_prev[SIZE], inbuf_next[SIZE], outbuf_prev[SIZE], outbuf_next[SIZE];
    char data[50];
    double t = MPI_Wtime();
    MPI_File file;
    MPI_Offset offset;
    MPI_Request reqs[2];
    // MPI_Status stats[2];

    prev = (rank + commsize - 1) % commsize;
    next = (rank + 1) % commsize;

    // MPI_Isend(outbuf_prev, NELEMS(outbuf_prev), MPI_CHAR, prev, 0, MPI_COMM_WORLD, &reqs[0]);
    MPI_Isend(outbuf_next, NELEMS(outbuf_next), MPI_CHAR, next, 0, MPI_COMM_WORLD, &reqs[1]);
    MPI_Irecv(inbuf_prev, NELEMS(inbuf_prev), MPI_CHAR, prev, 0, MPI_COMM_WORLD, &reqs[2]);
    // MPI_Irecv(inbuf_next, NELEMS(inbuf_next), MPI_CHAR, next, 0, MPI_COMM_WORLD, &reqs[3]);

    MPI_Waitall(2, reqs, stats);
	t = MPI_Wtime() - t;
    printf("[%d] Time: %f\n", rank, t);



    offset = rank * STR_LEN + 9 * (node - 1);
    MPI_File_open(MPI_COMM_WORLD, "output.txt", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);
    if (node == 2)
        sprintf(data, "%d%.5f\n", node, t);
    else
        sprintf(data, "%d%.5f\t", node, t);
    MPI_File_write_at(file, offset, data, 9, MPI_CHAR, MPI_STATUS_IGNORE);
    MPI_File_close(&file);

    // printf("[%d] Msg from %d (prev)\n", rank, stats[2].MPI_SOURCE, inbuf_prev);
    // printf("[%d] Msg from %d (next)\n", rank, stats[3].MPI_SOURCE, inbuf_next);

	MPI_Finalize();

	return 0;
}