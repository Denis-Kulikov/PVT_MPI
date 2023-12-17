#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))
#define GET_PREV(x, max) ((int)(x) == 0 ? (int)(max) : (int)(x) - 1)
#define GET_NEXT(x, max) ((int)(x) == (int)(max) ? 0 : (int)(x) + 1)
#define SIZE 3

int main(int argc, char **argv) {
	int rank, size, len;
    int commsize;
 	char host[MPI_MAX_PROCESSOR_NAME];

 	MPI_Init(&argc, &argv);
 	MPI_Get_processor_name(host, &len);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 	MPI_Comm_size(MPI_COMM_WORLD, &commsize);

    MPI_Request reqs[commsize];
    MPI_Status stats[commsize];
    // создаём массив массивов для приёма
    int *buf = (int*) malloc(sizeof(*buf) * SIZE * commsize);
    if (buf == nullptr)
        return -1;
    // создаём массив для отправки 
    int *sbuf = buf + SIZE * rank;
    for (int i = 0; i <= SIZE; i++)
        sbuf[i] = 1 + i + SIZE * rank;
    // sbuf[3] = '\0';

    int cur = rank;
    int prev = GET_PREV(rank, (commsize - 1));
    int next = GET_NEXT(rank, (commsize - 1));
    do {
        // MPI_Send(buf + SIZE * cur, NELEMS(sbuf), MPI_CHAR, prev, 0, MPI_COMM_WORLD);
        // MPI_Recv(buf + SIZE * GET_NEXT(cur, (commsize - 1)), NELEMS(sbuf), MPI_CHAR, next, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // printf("[%d] %d>>\"%s\" %d<<\"%s\"\n", rank, cur, buf + SIZE * cur, GET_NEXT(cur, (commsize - 1)), buf + SIZE * GET_NEXT(cur, (commsize - 1)));
        MPI_Sendrecv(buf + SIZE * cur, SIZE, MPI_INT, prev, 0, buf + SIZE * GET_NEXT(cur, (commsize - 1)), SIZE, MPI_INT, next, 0, MPI_COMM_WORLD, &stats[cur]);
        cur = GET_NEXT(cur, (commsize - 1));
    } while (GET_NEXT(cur, (commsize - 1)) != rank);

    for(int i = 0; i < 100000; i++);

    if (rank == 0) {
        for (int i = 0; i < SIZE * commsize; i++)
            printf("%d ", *(buf + i));
    }
	MPI_Finalize();

    return 0;
}
