#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h> 
#include <inttypes.h>
#include <omp.h>
#include <math.h>

const double eps = 1E-6;
const int n0 = 1000000;

const float a = 0.1f;
const float b = 1.0f;

double func(double x)
{
	return log(1 + x) / x;
}

double lin()
{
	double t = omp_get_wtime();

	double sq[2];
	int n = n0, k;
	double delta = 1;

	for (k = 0; delta > eps; n *= 2, k ^= 1) {
        double h = (b - a) / n;
        double s = 0.0;
        for (int i = 0; i <= n; i++)
            s += func(a + h * (i + 0.5));
        sq[k] *= h;
        if (n > n0)
            delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
	}

	t = omp_get_wtime() - t;
	return t;
}

int main(int argc, char **argv)
{
    double timeLin = lin();
    printf("%f\n", timeLin);
}
