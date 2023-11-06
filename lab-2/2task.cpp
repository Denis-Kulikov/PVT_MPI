#include <iostream>
#include <cmath>
#include <mpi.h>

#if 1
const double PI = 3.14159265358979323846;
const int n = 10 * 1000000;
const double time_ = 0.610291868000;
#else
const double PI = 3.14159265358979323846;
const int n = 100 * 1000000;
const double time_ = 6.092084578000;
#endif

double getrand(unsigned int *seed)
{
        return (double)rand_r(seed) / RAND_MAX;
}

float func (float x, float y)
{
    return exp(x + y) * exp(x + y);
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, commsize;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);

    double t = MPI_Wtime();
    unsigned int seed = static_cast<unsigned int>(rank);
    int in = 0;
    double s = 0;
    for (int i = rank; i < n; i += commsize) {
        double x = getrand(&seed);
        double y = getrand(&seed);
        if (y <= fabs(1 - x)) {
            in++;
            s += func(x, y);
        }
    }

    int gin = 0;
    MPI_Reduce(&in, &gin, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    double gsum = 0.0;
    MPI_Reduce(&s, &gsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("%.12f\n", time_ / (MPI_Wtime() - t));

    MPI_Finalize();

    return 0;
}
