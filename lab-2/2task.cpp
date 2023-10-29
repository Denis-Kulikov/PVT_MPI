#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

const float eps = 1E-6; 
const int n0 = 10000;

const float a = 0.1f;
const float b = 1.0f;

float func (float x)
{
    return log(1 + x) / x;
}

int main(int argc, char **argv)
{
    int commsize, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = n0, k;
    double sq[2], delta = 1;

    for (k = 0; delta > eps; n *= 2, k ^= 1) {
        int points_per_proc = n / commsize;
        double h = (b - a) / n;
        double s = 0.0;

        for (int i = rank; i <= n; i += commsize)
            s += func(a + h * (i + 0.5));
        MPI_Allreduce(&s, &sq[k], 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        sq[k] *= h;
        if (n > n0)
            delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
    }

    if (rank == 0) 
        printf("Result Pi: %.12f; Runge rule: EPS %e, n %d\n", sq[k] * sq[k], eps, n / 2);

    MPI_Finalize();
    
    return 0;
}
