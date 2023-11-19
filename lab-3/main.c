#include <inttypes.h>
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
// #include "malloc.h"

double timeLin;

const int n = 1000;
const int m = 1000;

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

void dgemv(double *a, double *b, double *c, int m, int n)
{
    int commsize, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows_per_proc = m / commsize;
    int lb = rank * rows_per_proc;
    int ub = (rank == commsize - 1) ? (m - 1) : (lb + rows_per_proc - 1);
    for (int i = lb; i <= ub; i++) {
    c[i] = 0.0;
    for (int j = 0; j < n; j++)
    c[i] += a[i * n + j] * b[j];
    }
    if (rank == 0) {
    int *displs = malloc(sizeof(*displs) * commsize);
    int *rcounts = malloc(sizeof(*rcounts) * commsize);
    for (int i = 0; i < commsize; i++) {
    rcounts[i] = (i == commsize - 1) ? m - i * rows_per_proc : rows_per_proc;
    displs[i] = (i > 0) ? displs[i - 1] + rcounts[i - 1]: 0;
    }
    MPI_Gatherv(MPI_IN_PLACE, ub - lb + 1, MPI_DOUBLE, c, rcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } else
    MPI_Gatherv(&c[lb], ub - lb + 1, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

int main(int argc, char **argv)
{
    int commsize, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double t = wtime();
    double *a, *b, *c;
    a = malloc(sizeof(*a) * m * n);
    b = malloc(sizeof(*b) * n);
    c = malloc(sizeof(*c) * m);
    for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++)
    a[i * n + j] = i + 1;
    }
    for (int j = 0; j < n; j++)
    b[j] = j + 1;
    dgemv(a, b, c, m, n);
    t = wtime() - t;
    // Продолжение main()
    if (rank == 0) {
    // Проверка
    for (int i = 0; i < m; i++) {
    double r = (i + 1) * (n / 2.0 + pow(n, 2) / 2.0);
    if (fabs(c[i] - r) > 1E-6) {
    fprintf(stderr, "Validation failed: elem %d = %f (real value %f)\n", i, c[i], r);
    break;
    }
    }
    printf("DGEMV: matrix-vector product (c[m] = a[m, n] * b[n]; m = %d, n = %d)\n", m, n);
    printf("Memory used: %" PRIu64 " MiB\n",
    (uint64_t)(((double)m * n + m + n) * sizeof(double)) >> 20);
    double gflop = 2.0 * m * n * 1E-9;
    printf("Elapsed time (%d procs): %.6f sec.\n", commsize, t);
    printf("Performance: %.2f GFLOPS\n", gflop / t);
    }
    free(a); free(b); free(c);
    MPI_Finalize();
    return 0;
}