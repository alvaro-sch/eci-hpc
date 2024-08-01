#include <stdio.h>

#include <omp.h>

int main() {
#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int tc = omp_get_num_threads();

#pragma omp single
        printf("there are %d threads\n", tc);

        printf("hello from %d!\n", tid);
    }
}
