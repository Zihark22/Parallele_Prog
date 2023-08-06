#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

#define N 500000
#define CHUNKSIZE 25000

void gen_data(float *in1, float *in2, int length)
{
    int i ;
    for (i=0 ; i < length ; i++) {
    in1[i] = i * 1.0 ;
    in2[i] = i * 2.0 ;}
}

void add_cpu(float *in1, float *in2, float *out, int length) {
    int i ;
    for (i=0 ; i < length ; i++) {
    out[i] = in1[i] + in2[i] ;}
}

void add_omp(float *in1, float *in2, float *out, int length, int chunk) {
    #pragma omp parallel shared(in1,in2,out,length,chunk) 
    {
        //int tid = omp_get_thread_num();
        int i;
        #pragma omp for schedule(dynamic,chunk)
        for (i=0 ; i < length ; i++) {
           out[i] = in1[i] + in2[i] ;
           //printf("threead = %d, Out:%.0f \n",tid,out[i]);
        }
        
    }
}

int main (int argc, char *argv[])
{
    omp_set_num_threads(4);
    int i,chunk = CHUNKSIZE;
    float a[N], b[N], c[N] ;
    double starttime, stoptime, executiontime ;
    gen_data(a,b,N) ;
    /*
    starttime = omp_get_wtime() ;
    add_omp(a,b,c,N,chunk) ;
    stoptime = omp_get_wtime() ;
    executiontime = stoptime-starttime ;
    printf("tps exe omp = %lf\n",executiontime);
    */
    starttime = omp_get_wtime() ;
    add_cpu(a,b,c,N) ;
    stoptime = omp_get_wtime() ;
    executiontime = stoptime-starttime ;
    printf("tps exe cpu = %lf\n",executiontime);
    
    return 0 ;
}
