#include <stdio.h>
#include <stdlib.h>
#include <arm_neon.h>

#define Nc 8     // nombre d' coeffs
#define Ne 8192 // nombre d'echantillons
#define N 10000 // nombre d'executions

int16_t gen_data()
{
    int min = -500;
    int max = 500;
    return (int16_t) (rand()%(max-min) + min);
}

void fir_cpu(int16_t *in, const int16_t *coef, int32_t *out, const int16_t ncoef, const int16_t nech)
{
    int16_t i,j;
    int32_t result = 0;
    for(i=0;i<nech;i++) out[i] = 0;
    for(i=0;i<nech;i++) out[i] = in[i];
    for(i=ncoef;i<nech;i++)
    {
        result=0;
        for(j=0;j<ncoef;j++)
        {
            result += coef[j] * in[i] ;
        }
        out[i]=result;
    }
}

void fir_vec_in(int16_t *in, const int16_t *coef, int32_t *out, const int16_t ncoef, const int16_t nech)
{
    int16_t i,j;
    int32_t sum[4];
    for(i=0;i<nech;i++) out[i] = 0;
    for(i=0;i<nech;i++) out[i] = in[i];
    for(i=ncoef;i<nech;i++)
    {
        sum[0]=0;
        sum[1]=sum[0];
        sum[2]=sum[0];
        sum[3]=sum[0];
        for(j=0;j<ncoef;j+=4)
        {
            sum[0] += coef[j] * in[i];
            sum[1] += coef[j+1] * in[i];
            sum[2] += coef[j+2] * in[i];
            sum[3] += coef[j+3] * in[i];
        }
        out[i]=sum[0]+sum[1]+sum[2]+sum[3];
    }
}

void fir_vec_out(int16_t *in, const int16_t *coef, int32_t *out, const int16_t ncoef, const int16_t nech)
{
    int16_t i,j;
    for(i=0;i<nech;i++) out[i] = 0;
    for(i=0;i<nech;i++) out[i] = in[i];
    for(i=ncoef;i<nech;i+=4)
    {
        out[0] = coef[j] * in[i];
        out[1] = coef[j+1] * in[i+1];
        out[2] = coef[j+2] * in[i+2];
        out[3] = coef[j+3] * in[i+3];
    }
}

void fir_vec_inout(int16_t *in, const int16_t *coef, int32_t *out, const int16_t ncoef, const int16_t nech)
{
    int16_t i,j,k;
    int32_t sum[4];
    int32_t acumul[4];
    for(i=0;i<nech;i++) out[i] = 0;
    for(i=0;i<nech;i++) out[i] = in[i];
    acumul[0]=0;
    acumul[1]=acumul[0];
    acumul[2]=acumul[0];
    acumul[3]=acumul[0];
    for(i=ncoef;i<nech;i+=4)
    {
        
        for(k=0;k<4;k++)
        {
            sum[0]=0;
            sum[1]=sum[0];
            sum[2]=sum[0];
            sum[3]=sum[0];
            for(j=0;j<ncoef;j+=4)
            {
                sum[0] += coef[j] * in[i];
                sum[1] += coef[j+1] * in[i+1];
                sum[2] += coef[j+2] * in[i+2];
                sum[3] += coef[j+3] * in[i+3];
            }
            acumul[k]=sum[0]+sum[1]+sum[2]+sum[3];
        }
        out[i]=acumul[0];
        out[i+1]=acumul[1];
        out[i+2]=acumul[2];
        out[i+3]=acumul[3];
    }
}

void fir_intrinsics(int16_t *in, const int16_t *coef, int32_t *out, const int16_t ncoef, const int16_t nech)
{
    int16_t i,j,k;
    int32x4_t result;
    int16x4_t coefi,ini;
    for(i=0;i<nech;i++) out[i] = 0;
    for(i=0;i<nech;i++) out[i] = in[i];
    acumul[0]=0;
    acumul[1]=acumul[0];
    acumul[2]=acumul[0];
    acumul[3]=acumul[0];
    for(i=ncoef;i<nech;i++)
    {
        vdupq_n_s32(0);
        for(j=0;j<ncoef;j+=4)
        {
            ini[i]=vld1_s16(in[i]);
            ini[i+1]=vld1_s16(in[i+1]);
            ini[i+2]=vld1_s16(in[i+2]);
            ini[i+3]=vld1_s16(in[i+3]);
            
            coefi[i]=vld1_s16(coefi[i]);
            coefi[i+1]=vld1_s16(coefi[i+1]);
            coefi[i+2]=vld1_s16(coefi[i+2]);
            coefi[i+3]=vld1_s16(coefi[i+3]);
            
            sum[0] += coef[j] * in[i];
            sum[1] += coef[j+1] * in[i+1];
            sum[2] += coef[j+2] * in[i+2];
            sum[3] += coef[j+3] * in[i+3];
        }
        acumul[k]=sum[0]+sum[1]+sum[2]+sum[3];
        
        out[i]=acumul[0];
        out[i+1]=acumul[1];
        out[i+2]=acumul[2];
        out[i+3]=acumul[3];
    }
}

int main()
{
    int16_t i;
    const int16_t cof[] = {1,-2,4,-2,-2,4,-2,1};
    int16_t xin[Ne];
    int32_t yout[Ne];
    
    for(i=0;i<Ne;i++)
    {
        xin[i] = gen_data(); // créer entrée
    }
    
    for(i=0;i<N;i++) // filtrage 10000 fois
    {
        //fir_cpu(xin,cof,yout,Nc,Ne);
        //fir_vec_in(xin,cof,yout,Nc,Ne);  
        //fir_vec_out(xin,cof,yout,Nc,Ne);   
        fir_vec_inout(xin,cof,yout,Nc,Ne);  
    }
    
    return 0;
}





















