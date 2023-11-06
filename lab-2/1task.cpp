#include <iostream>
#include <cmath>
#include <mpi.h>

const float eps = 1E-9;
const int n0 = 100;

const float a = 0.1f;
const float b = 1.0f;

const double time_ = 0.042609305000;

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
    double t = MPI_Wtime();

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
        printf("%.12f\n", time_ / (MPI_Wtime() - t));

    MPI_Finalize();

    return 0;
}