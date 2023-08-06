#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define N 40

void gen_data_cpu(float *in1, float *in2, int length)
{
    int i;
    for (i=0 ; i < length ; i++)
        in1[i] = in2[i] = i * 1.0 ;
}

void gen_data_omp(float *in1, float *in2, int length)
{
    #pragma omp parallel
    {
    int i;
    #pragma omp for
        for (i=0 ; i < length ; i++) {
            in1[i] = in2[i] = i * 1.0 ;}
    }
}

float prod_cpu(float *in1, float *in2, int length) {
    int i ;
    float out=0;
    for (i=0 ; i < length ; i++) {
        out = out + (in1[i] * in2[i]) ;}
    return out ;
}

float prod_omp(float *in1, float *in2, int length) {
    float out = 0;
    #pragma omp parallel shared(in1,in2,out)
    {
    int i;
    float out_omp=0;    
    #pragma omp for reduction(+ :out)
    for (i=0 ; i < length ; i++) {
        out_omp = out_omp + (in1[i] * in2[i]) ;}
    printf("Nombre de threads = %d\n",omp_get_num_threads());
    out = out + out_omp;
    }
    return out;
}

int main (int argc, char *argv[])
{
    int i ;
    float a[N], b[N], result_cpu=0.0 ;
    gen_data_cpu(a,b,N) ;
    result_cpu = prod_omp(a,b,N) ;
    printf("%f\n",result_cpu);
}
