#include <stdio.h>
#include <stdlib.h>
#include <arm_neon.h>
#define N 1024
void data(int a[N], int b[N])
{
    int i ;
    for (i=0 ; i<N ; i++) 
    { 
        a[i] = i ; 
        b[i] = N-i ; 
    }
}
    
    
int dotproduct_cpu(int *a, int *b, int n)
{
    int i ;
    int sum = 0 ;
    for (i=0 ; i<n ; i++) sum += a[i]*b[i] ;
    return sum ;
}


int8_t dotproduct_vecman(int *__restrict a, int *__restrict b, int n)
{
    int32_t sum[4] = {0,0,0,0};
    int i;
    for (i=0 ; i<(n &~3);i+=4) 
    {      
        sum[0] += a[i]*b[i] ;
        sum[1] += a[i+1]*b[i+1] ;
        sum[2] += a[i+2]*b[i+2] ;
        sum[3] += a[i+3]*b[i+3] ;
    }
    return sum[0]+sum[1]+sum[2]+sum[3];
}


int32_t dotproduct_neon(int16_t *__restrict a, int16_t *__restrict b, int n)
{
    int16x4_t a_i,b_i;
    int32x4_t sum_i = {0,0,0,0} ;
    //int32x2_t tmp[2];
    //Verctorres = v_s8(Verctor1,Verctor2);
    int16_t i;
    for (i=0 ; i<(n &~3);i+=4) 
    {      
        a_i[0] = vld1_s16(a[i]);
        a_i[1] = vld1_s16(a[1+i]);
        a_i[2] = vld1_s16(a[2+i]);
        a_i[3] = vld1_s16(a[3+i]);
        
        b_i[0] = vld1_s16(b[i]);
        b_i[1] = vld1_s16(b[1+i]);
        b_i[2] = vld1_s16(b[2+i]);
        b_i[3] = vld1_s16(b[3+i]);
        
        sum_i=vadd_s32(sum_i,vmla1_s16(a_i,b_i));
    }
    //tmp[0]=vget_high_s32(sum_i);
    //tmp[1]=vget_low_s32(sum_i);
    
    return sum_i[0]+sum_i[1]+sum_i[2]+sum_i[3];
    
}


void main()
{
    int result_cpu ;
    int8_t result_vecman ;
    int32_t result_neon ;
    int16_t va1[N], vb2[N] ;
    int v1[N], v2[N] ;
    data(v1, v2) ;
    va1 = vld1_s16(v1);
    vb2 = vld1_s16(v2);
    
    int j;
    //for(j=0;j<50000;j++) result_cpu = dotproduct_cpu(v1, v2, N) ;
    //for(j=0;j<50000;j++) result_vecman = dotproduct_vecman(v1, v2, N) ;
    //for(j=0;j<50000;j++) result_neon = dotproduct_neon(va1, vb2, N) ;
}
