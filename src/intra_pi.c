
#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

int read_n(const char *path) {
    FILE *in = fopen(path, "r");
    if (in == NULL) {
        fprintf(stderr, "could not open file at %s\n", path);
        exit(-1);
    }

    int n;
    fscanf(in, "%d", &n);
    fclose(in);

    return n;
}

double partial_pi(int from, int to, int n) {
    double h = 1.0 / n;
    double partial_sum = 0.0;
    for (int i = from; i < to; ++i) {
        double x = (i + .5) * h;
        partial_sum += 4.0 / (1.0 + x * x);
    }
    return partial_sum;
}

int main(int argc, char *argv[]) {
    (void) argc; (void) argv;
    int n = read_n("input/pi.dat");

    double pi = 0;
    double h = 1.0 / n;
#pragma omp parallel for default(none) shared(n, h) reduction(+:pi)
    for (int i = 0; i < n; ++i) {
        double x = (i + .5) * h;
        pi += 4.0 / (1.0 + x * x);
    }

    pi /= (double)n;

    printf("pi=%f\n", pi);
}
