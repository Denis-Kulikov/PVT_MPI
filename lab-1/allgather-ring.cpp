#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))
#define SIZE 4

int main(int argc, char **argv) {
	int rank, size, len, prev, next;
    int commsize;
    // int count = 1024;
 	char host[MPI_MAX_PROCESSOR_NAME];

 	MPI_Init(&argc, &argv);
 	MPI_Get_processor_name(host, &len);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 	MPI_Comm_size(MPI_COMM_WORLD, &commsize);

    // int inbuf[SIZE], outbuf[SIZE];
    double t = MPI_Wtime();
    MPI_Request reqs[commsize];
    MPI_Status stats[commsize];

    // prev = (rank == 0) ? commsize - 1 : rank - 1;
    // next = (rank == (commsize - 1)) ? 0 : rank + 1;

    // создаём массив массивов для приёма
    char *buf = (char*) malloc(sizeof(*buf) * SIZE * commsize);
    if (buf == nullptr)
        return -1;
    // создаём массив для отправки 
    char *sbuf = buf + SIZE * rank;
    for (int i = 0; i < SIZE; i++)
        sbuf[i] = 'A' + i;
    sbuf[3] = '\0';
    printf("Test: [%d] %s\n", rank, buf + SIZE * rank);
    // оптравил/принял сообщение

    // MPI_Isend(sbuf, SIZE, MPI_UNSIGNED, next, 0, MPI_COMM_WORLD, &reqs[i * 2]);
    // MPI_Irecv(rbuf + SIZE * i, SIZE, MPI_UNSIGNED, i, 0, MPI_COMM_WORLD, &reqs[i * 2 + 1]);

    // for (int i = 0; i < (commsize - 1); i++) {
    //     if (rank != i)
    //         MPI_Sendrecv(buf + SIZE * i, SIZE, MPI_UNSIGNED, next, 0, buf + SIZE * i, SIZE, MPI_UNSIGNED, prev, 0, MPI_COMM_WORLD, reqs[i]);
        // if (i < rank) {
        //     MPI_Isend(sbuf, SIZE, MPI_UNSIGNED, next, 0, MPI_COMM_WORLD, &reqs[i * 2]);
        //     MPI_Irecv(rbuf + SIZE * i, SIZE, MPI_UNSIGNED, i, 0, MPI_COMM_WORLD, &reqs[i * 2 + 1]);
        // } else if (i > rank) {
        //     MPI_Isend(sbuf, count, MPI_UNSIGNED, i, 0, MPI_COMM_WORLD, &reqs[(i - 1) * 2]);
        //     MPI_Irecv(rbuf + SIZE * i, SIZE, MPI_UNSIGNED, i, 0, MPI_COMM_WORLD, &reqs[(i - 1) * 2 + 1]);
        // }
    // }


            // MPI_Isend(buf, SIZE, MPI_UNSIGNED, next, 0, MPI_COMM_WORLD, &reqs[cur]);
            // MPI_Irecv(buf + SIZE * cur, SIZE, MPI_UNSIGNED, prev, 0, MPI_COMM_WORLD, &reqs[cur]);
    // int cur = rank;
    // do {
        // prev = (cur == 0) ? commsize - 1   : cur - 1;
        // next = (cur == (commsize - 1)) ? 0 : cur + 1;

        // MPI_Isend(buf + SIZE * cur, size, MPI_UNSIGNED, prev, 0, MPI_COMM_WORLD, &reqs[cur]);
        // MPI_Send(buf + SIZE * cur, size, MPI_UNSIGNED, prev, 0, MPI_COMM_WORLD);
        // printf("Start=%d\n", rank);
        // MPI_Recv(buf + SIZE * next, SIZE, MPI_UNSIGNED, next, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // MPI_Sendrecv(buf + SIZE * cur, SIZE, MPI_UNSIGNED, prev, 0, buf + SIZE * next, SIZE, MPI_UNSIGNED, next, 0, MPI_COMM_WORLD, &stats[cur]);
        // printf("[%d] prev=%d next=%d\n", cur, prev, next);
        // cur = (cur + 1) % commsize;
        // if (rank == 3)
            // printf("cur=%d\n", cur);
    // } while (next != rank);

    int cur = rank;

    prev = (cur == 0) ? commsize - 1   : cur - 1;
    next = (cur == (commsize - 1)) ? 0 : cur + 1;
    MPI_Send(buf + SIZE * cur, NELEMS(sbuf), MPI_UNSIGNED, prev, 0, MPI_COMM_WORLD);
    // MPI_Isend(buf + SIZE * cur, NELEMS(sbuf), MPI_UNSIGNED, prev, 0, MPI_COMM_WORLD, &reqs[cur]);
    MPI_Recv(buf, NELEMS(sbuf), MPI_UNSIGNED, next, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("[%d] %s\n", rank, buf);

    // printf("%d => %d: %d %d\n", cur, (cur + 1) % commsize, (cur == 0) ? commsize - 1   : cur - 1, (cur == (commsize - 1)) ? 0 : cur + 1);
    if (0) {
    // MPI_Wait(&reqs[cur], stats);
    cur = (cur + 1) % commsize;
    prev = (cur == 0) ? commsize - 1   : cur - 1;
    next = (cur == (commsize - 1)) ? 0 : cur + 1;
    MPI_Send(buf + SIZE * cur, NELEMS(sbuf), MPI_CHAR, prev, 0, MPI_COMM_WORLD);
    // MPI_Isend(buf + SIZE * cur, NELEMS(sbuf), MPI_UNSIGNED, prev, 0, MPI_COMM_WORLD, &reqs[cur]);
    MPI_Recv(buf + SIZE * next, NELEMS(sbuf), MPI_CHAR, next, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // MPI_Wait(&reqs[cur], stats);
    }


    t = MPI_Wtime() - t;
    // printf("[%d] Time: %f\n", rank, t);
    // for (int i = 0; i < SIZE; i++)
        // if (inbuf_prev[i] != '*')
    // printf("%d invalid buf\n", rank);

	MPI_Finalize();

    return 0;
}
