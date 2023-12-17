#include <iostream>
#include <cmath>
#include <mpi.h>

const float eps = 1E-6;
const int n0 = 1000000;

const float a = 0.1f;
const float b = 1.0f;

const double time_ = 0.083639;

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
    double t_reduc = 0;
    double t_reduc_sum = 0;

    for (k = 0; delta > eps; n *= 2, k ^= 1) {
        int points_per_proc = n / commsize;
        double h = (b - a) / n;
        double s = 0.0;

        for (int i = rank; i <= n; i += commsize)
            s += func(a + h * (i + 0.5));
        t_reduc = MPI_Wtime();
        MPI_Allreduce(&s, &sq[k], 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        t_reduc_sum += MPI_Wtime() - t_reduc;
        sq[k] *= h;
        if (n > n0)
            delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
    }

    if (rank == 0)
        printf("%.12f\t%.12f\n",  MPI_Wtime() - t, t_reduc_sum);

    MPI_Finalize();

    return 0;
}
