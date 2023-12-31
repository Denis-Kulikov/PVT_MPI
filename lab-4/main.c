#include <stdio.h>
#include <stdlib.h>

double wtime()
{
        struct timeval t;
        gettimeofday(&t, NULL);
        return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

enum { NPARTICLES = 1000 };
const double G = 6.67e-11;
struct particle {
    float x, y, z;
};

void move_particles(struct particle *p, struct particle *f, struct particle *v, double *m, int n, double dt)
{
    for (int i = 0; i < n; i++) {
        struct particle dv = {
            .x = f[i].x / m[i] * dt,
            .y = f[i].y / m[i] * dt,
            .z = f[i].z / m[i] * dt,
        };
        struct particle dp = {
            .x = (v[i].x + dv.x / 2) * dt,
            .y = (v[i].y + dv.y / 2) * dt,
            .z = (v[i].z + dv.z / 2) * dt,
        };
        v[i].x += dv.x;
        v[i].y += dv.y;
        v[i].z += dv.z;
        p[i].x += dp.x;
        p[i].y += dp.y;
        p[i].z += dp.z;
        f[i].x = f[i].y = f[i].z = 0;
    }
}

void calculate_forces(struct particle *p, struct particle *f, double *m, int n)
{
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            double dist = sqrt(pow(p[i].x - p[j].x, 2) + pow(p[i].y - p[j].y, 2) + pow(p[i].z - p[j].z, 2));
            double mag = (G * m[i] * m[j]) / pow(dist, 2);
            struct particle dir = {
                .x = p[j].x - p[i].x,
                .y = p[j].y - p[i].y,
                .z = p[j].z - p[i].z
            };
            f[i].x += mag * dir.x / dist;
            f[i].y += mag * dir.y / dist;
            f[i].z += mag * dir.z / dist;
            f[j].x -= mag * dir.x / dist;
            f[j].y -= mag * dir.y / dist;
            f[j].z -= mag * dir.z / dist;
        }
    }
}

int main(int argc, char *argv[])
{
    double ttotal = wtime();
    int n = NPARTICLES;
    struct particle *p = malloc(sizeof(*p) * n); 
    struct particle *f = malloc(sizeof(*f) * n); 
    struct particle *v = malloc(sizeof(*v) * n); 
    double *m = malloc(sizeof(*m) * n); 
    for (int i = 0; i < n; i++) {  
        p[i].x = rand() / (float)RAND_MAX - 0.5;
        p[i].y = rand() / (float)RAND_MAX - 0.5;
        p[i].z = rand() / (float)RAND_MAX - 0.5;
        v[i].x = rand() / (float)RAND_MAX - 0.5;
        v[i].y = rand() / (float)RAND_MAX - 0.5;
        v[i].z = rand() / (float)RAND_MAX - 0.5;
        m[i] = rand() / (float)RAND_MAX * 10 + 0.01;
    }
    double dt = 1e-5;
    for (double t = 0; t <= 1; t += dt) {
        calculate_forces(p, f, m, n);  
        move_particles(p, f, v, m, n, dt);  
    }
    ttotal = wtime() - ttotal;
    printf("Elapsed time (sec): %.6f\n", ttotal);
    free(m);
    free(v);
    free(f);
    free(p);
    return 0;
}
