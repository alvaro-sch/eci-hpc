#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#define CHECK_MPI(MPICALL) do {  \
    int status = (MPICALL);      \
    if (status != MPI_SUCCESS) { \
        fprintf(stderr, "MPI call failed: "#MPICALL"\n");   \
        MPI_Finalize();          \
        exit(-1);                \
    }                            \
} while (0)

int read_n(int rank, const char *path) {
    if (rank != 0) {
        return 0;
    }

    FILE *in = fopen(path, "r");
    if (in == NULL) {
        fprintf(stderr, "could not open file at %s\n", path);
        MPI_Finalize();
        exit(-1);
    }

    int n;
    fscanf(in, "%d", &n);
    fclose(in);

    return n;
}

float partial_pi(int from, int to, int n) {
    float h = 1.0 / n;
    float partial_sum = 0.0;
    for (int i = from; i < to; ++i) {
        double x = (i + .5) * h;
        partial_sum += 4.0 / (1.0 + x * x);
    }
    return partial_sum;
}

int main(int argc, char *argv[]) {
    CHECK_MPI(MPI_Init(&argc, &argv));

    int rank;
    CHECK_MPI(MPI_Comm_rank(MPI_COMM_WORLD, &rank));

    int proc_count;
    CHECK_MPI(MPI_Comm_size(MPI_COMM_WORLD, &proc_count));

    int n = read_n(rank, "input/pi.dat");
    CHECK_MPI(MPI_Barrier(MPI_COMM_WORLD));
    CHECK_MPI(MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD));

    int op_count = n / proc_count;
    int from = rank * op_count;
    int to = from + op_count;
    to = to > n ? n : to;

    double partial_sum = partial_pi(from, to, n);
    CHECK_MPI(MPI_Barrier(MPI_COMM_WORLD));

    double pi;
    CHECK_MPI(MPI_Reduce(&partial_sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD));

    if (rank == 0) {
        pi /= (double) n;
        printf("pi = %f\n", pi);
    }

    CHECK_MPI(MPI_Finalize());
}

