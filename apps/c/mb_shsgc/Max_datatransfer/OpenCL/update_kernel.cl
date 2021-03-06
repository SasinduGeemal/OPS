//
// auto-generated by ops.py
//

#ifdef OCL_FMA
#pragma OPENCL FP_CONTRACT ON
#else
#pragma OPENCL FP_CONTRACT OFF
#endif
#pragma OPENCL EXTENSION cl_khr_fp64:enable

#include "ops_opencl_reduction.h"

#ifndef MIN
#define MIN(a,b) ((a<b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) ((a>b) ? (a) : (b))
#endif
#ifndef SIGN
#define SIGN(a,b) ((b<0.0) ? (a*(-1)) : (a))
#endif
#define OPS_READ 0
#define OPS_WRITE 1
#define OPS_RW 2
#define OPS_INC 3
#define OPS_MIN 4
#define OPS_MAX 5
#define ZERO_double 0.0;
#define INFINITY_double INFINITY;
#define ZERO_float 0.0f;
#define INFINITY_float INFINITY;
#define ZERO_int 0;
#define INFINITY_int INFINITY;
#define ZERO_uint 0;
#define INFINITY_uint INFINITY;
#define ZERO_ll 0;
#define INFINITY_ll INFINITY;
#define ZERO_ull 0;
#define INFINITY_ull INFINITY;
#define ZERO_bool 0;

#undef OPS_ACC0
#undef OPS_ACC1
#undef OPS_ACC2

#undef OPS_ACC_MD3

#define OPS_ACC0(x) (x)
#define OPS_ACC1(x) (x)
#define OPS_ACC2(x) (x)

#define OPS_ACC_MD3(d,x) ((x)*3+(d))

//user function
void update_kernel(__global double * restrict rho_new,__global double * restrict rhou_new,__global double * restrict rhoE_new,
const __global double * restrict s)

 {
  rho_new[OPS_ACC0(0)]  = rho_new[OPS_ACC0(0)]  + s[OPS_ACC_MD3(0,0)];
  rhou_new[OPS_ACC1(0)] = rhou_new[OPS_ACC1(0)] + s[OPS_ACC_MD3(1,0)];
  rhoE_new[OPS_ACC2(0)] = rhoE_new[OPS_ACC2(0)] + s[OPS_ACC_MD3(2,0)];
}



__kernel void ops_update_kernel(
__global double* restrict arg0,
__global double* restrict arg1,
__global double* restrict arg2,
__global const double* restrict arg3,
const int base0,
const int base1,
const int base2,
const int base3,
const int size0 ){


  int idx_x = get_global_id(0);

  if (idx_x < size0) {
    update_kernel(&arg0[base0 + idx_x * 1*1],
                  &arg1[base1 + idx_x * 1*1],
                  &arg2[base2 + idx_x * 1*1],
                  &arg3[base3 + idx_x * 1*3]);
  }

}
