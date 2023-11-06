#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <mpi.h>

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))
#define SIZE 1024

int main (int argc, char **argv)
{
	int rank, size, len, prev, next;
  int commsize;
	int root = 0;
 	char host[MPI_MAX_PROCESSOR_NAME];

 	MPI_Init(&argc, &argv);
 	MPI_Get_processor_name(host, &len);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 	MPI_Comm_size(MPI_COMM_WORLD, &commsize);

  char inbuf_prev[SIZE], inbuf_next[SIZE], outbuf_prev[SIZE], outbuf_next[SIZE];
  double t = MPI_Wtime();
  MPI_Request reqs[2];
  MPI_Status stats[2];

  prev = (rank + commsize - 1) % commsize;
  next = (rank + 1) % commsize;

  for (int i = 0; i < SIZE; i++)
      outbuf_next[i] = '*';

  // MPI_Isend(outbuf_next, NELEMS(outbuf_next), MPI_CHAR, next, 0, MPI_COMM_WORLD, &reqs[0]);
  // MPI_Irecv(inbuf_prev, NELEMS(inbuf_prev), MPI_CHAR, prev, 0, MPI_COMM_WORLD, &reqs[1]);
  
  // MPI_Waitall(2, reqs, stats);

  MPI_Send(outbuf_next, NELEMS(outbuf_next), MPI_CHAR, next, 0, MPI_COMM_WORLD);
  MPI_Recv(inbuf_prev, NELEMS(inbuf_prev), MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  
  t = MPI_Wtime() - t;
  printf("[%d] Time: %f\n", rank, t);
  for (int i = 0; i < SIZE; i++)
      if (inbuf_prev[i] != '*')
  printf("%d invalid buf\n", rank);

	MPI_Finalize();

	return 0;
}
