#define _POSIX_C_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <sys/time.h> 
#include <omp.h>


double timeLin;

#if 1
const int n = 10 * 1000000;
#else
const int n = 100 * 1000000;
#endif

double wtime()
{
        struct timeval t;
        gettimeofday(&t, NULL);
        return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

double getrand(unsigned int *seed)
{
	return (double)rand_r(seed) / RAND_MAX;
}

double func(double x, double y)
{
	return exp(x + y) * exp(x + y);
}

double lin()
{
	unsigned int seed = 1; 
	uint64_t in = 0;
	double s = 0;
	double t = wtime();
	
    for (int i = i; i < n; i++) {
        double x = getrand(&seed);
        double y = getrand(&seed);
        if (y <= fabs(1 - x)) {
            in++;
            s += func(x, y);
        }
    }
	double res = 3 * s / (double)n;
	
	t = wtime() - t;

	return t;
}

int main()
{
    timeLin = lin();
    printf("%f\n", timeLin);

	return 0;
}
