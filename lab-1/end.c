#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_INT 2147483647

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))
#define GET_PREV(rank, size) (((int)(rank) - 1 + (int)(MAX_INT)) % (int)(size))
#define GET_NEXT(rank, size) (((int)(rank) + 1) % (int)(size))

// #define GET_PREV(x, max) ((int)(x) == 0 ? (int)(max) : (int)(x) - 1)
// #define GET_NEXT(x, max) ((int)(x) == (int)(max) ? 0 : (int)(x) + 1)
#define SIZE 128

unsigned int *get_buf(int commsize, int rank)
{
    unsigned *buf = (unsigned int*) malloc(sizeof(*buf) * SIZE * commsize);
    if (buf == nullptr)
        return nullptr;

    for (int i = 0; i < SIZE; i++)
        *(buf + SIZE * rank + i)= 0xFFFFFFFF;

    return buf;
}

int main(int argc, char **argv) {
	int rank, size, len, commsize;
 	char host[MPI_MAX_PROCESSOR_NAME];

 	MPI_Init(&argc, &argv);
 	MPI_Get_processor_name(host, &len);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 	MPI_Comm_size(MPI_COMM_WORLD, &commsize);

    MPI_Status stats[commsize];

    unsigned int *buf = get_buf(commsize, rank);
    if (buf == nullptr)
        return -1;

    int cur = rank;
    int prev = GET_PREV(rank, (commsize - 1));
    int next = GET_NEXT(rank, (commsize - 1));
    // do {
        // MPI_Sendrecv(buf + SIZE * cur, SIZE, MPI_UNSIGNED, prev, 0, buf + SIZE * GET_NEXT(cur, (commsize - 1)), SIZE, MPI_UNSIGNED, next, 0, MPI_COMM_WORLD, &stats[cur]);

        if (rank == 0) {
            printf("%d %d\n", prev, next);
        }
        cur = GET_NEXT(cur, (commsize - 1));
    // } while (GET_NEXT(cur, (commsize - 1)) != rank);

    unsigned int *recv_buf = (unsigned int*) malloc(sizeof(*recv_buf) * SIZE * commsize);
    MPI_Reduce(buf, recv_buf, SIZE * commsize, MPI_UNSIGNED, MPI_LAND, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        for (int i = 0; i < commsize * SIZE; i++) {
            if (*(buf + i) != 0xFFFFFFFF) {
                printf("Error!\n");
                free(buf);
                free(recv_buf);
                return -1;
            }
        }
    }

    free(buf);
    free(recv_buf);

	MPI_Finalize();

    return 0;
}
