#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

#define N 7

void main() {
    #pragma omp parallel num_threads(N) //shared(N)
    {
        int tid = omp_get_thread_num();
        printf("Hello world: %d\n",tid);
        if (tid == 0)
        {
            printf("Nombre de threads = %d\n",omp_get_num_threads());
        }
    }
}
