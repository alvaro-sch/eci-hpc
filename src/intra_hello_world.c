#include <stdio.h>

#include <omp.h>

int main() {
#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int tc = omp_get_num_threads();

        printf("hello from %d of %d\n", tid, tc);
    }
}
