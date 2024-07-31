#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#define CHECK_MPI(MPICALL)                                                     \
    do {                                                                       \
        int status = (MPICALL);                                                \
        if (status != MPI_SUCCESS) {                                           \
            fprintf(stderr, "MPI call failed: " #MPICALL "\n");                \
            MPI_Finalize();                                                    \
            exit(-1);                                                          \
        }                                                                      \
    } while (0)

int main(int argc, char *argv[]) {
    CHECK_MPI(MPI_Init(&argc, &argv));

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int proc_count;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

    if (rank == 0) {
        printf("there are %d procs\n", proc_count);
        fflush(stdout);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank != 0) {
        printf("hello from %d!\n", rank);
    }

    CHECK_MPI(MPI_Finalize());
}
