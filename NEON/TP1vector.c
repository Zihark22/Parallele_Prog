#include <stdio.h>
#include <stdlib.h>
#include <arm_neon.h>

#define COUNT 256

void add_cpu(int8_t v1[COUNT], int8_t v2[COUNT], int8_t vres[COUNT])
{
    int i ;
    for (i=0 ; i<COUNT ; i++) vres[i] = v1[i]+v2[i] ;
}

void add_vectorisation_c(int8_t *__restrict v1, int8_t *__restrict v2, int8_t vres[COUNT])
{
    int i ;
    for (i=0 ; i<(COUNT &~3); i++) vres[i] = v1[i]+v2[i] ;
}

void add_vectorisation_m(int8_t v1[COUNT], int8_t v2[COUNT], int8_t vres[COUNT])
{
    vres[0] = v1[0]+v2[0];
    vres[1] = v1[1]+v2[1];
    vres[2] = v1[2]+v2[2];
    vres[3] = v1[3]+v2[3];
    vres[4] = v1[4]+v2[4];
    vres[5] = v1[5]+v2[5];
    vres[6] = v1[6]+v2[6];
    vres[7] = v1[7]+v2[7];
}

void add_neon(int8_t v1[COUNT], int8_t v2[COUNT], int8_t vres[COUNT])
{
    int8x8_t Verctor1, Verctor2, Verctorres;
    Verctor1 = vld1_s8(v1);
    Verctor2 = vld1_s8(v2);
    Verctorres = vadd_s8(Verctor1,Verctor2);
    
}

void main()
{
    int8_t v1_vec[COUNT] = {1,2,3,4,5,6,7,8} ;
    int8_t v2_vec[COUNT] = {8,7,6,5,4,3,2,1} ;
    int8_t vres_vec[COUNT] ;
    
    int j;
    for(j=0;j<1000000;j++) add_vectorisation_c(v1_vec, v2_vec, vres_vec) ;
    //for(j=0;j<1000000;j++) add_cpu(v1_vec, v2_vec, vres_vec) ;
    //for(j=0;j<1000000;j++) add_vectorisation_m(v1_vec, v2_vec, vres_vec) ;
    //for(j=0;j<1000000;j++) add_neon(v1_vec, v2_vec, vres_vec) ;
}
