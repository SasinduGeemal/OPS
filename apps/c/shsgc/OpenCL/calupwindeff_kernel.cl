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


#undef OPS_ACC_MD0
#undef OPS_ACC_MD1
#undef OPS_ACC_MD2
#undef OPS_ACC_MD3
#undef OPS_ACC_MD4
#undef OPS_ACC_MD5
#undef OPS_ACC_MD6


#define OPS_ACC_MD0(d,x) ((x)*3+(d))
#define OPS_ACC_MD1(d,x) ((x)*3+(d))
#define OPS_ACC_MD2(d,x) ((x)*3+(d))
#define OPS_ACC_MD3(d,x) ((x)*3+(d))
#define OPS_ACC_MD4(d,x) ((x)*3+(d))
#define OPS_ACC_MD5(d,x) ((x)*9+(d))
#define OPS_ACC_MD6(d,x) ((x)*3+(d))

//user function
void calupwindeff_kernel(const __global double* restrict  cmp,const __global double * restrict gt,const __global double* restrict  cf,
const __global double* restrict  al,const __global double* restrict  ep2,const __global double* restrict  r,__global double* restrict  eff)

 {

		double e1 = (cmp[OPS_ACC_MD0(0,0)] * (gt[OPS_ACC_MD1(0,0)] + gt[OPS_ACC_MD1(0,1)]) - cf[OPS_ACC_MD2(0,0)] * al[OPS_ACC_MD3(0,0)]) * ep2[OPS_ACC_MD4(0,0)];
		double e2 = (cmp[OPS_ACC_MD0(1,0)] * (gt[OPS_ACC_MD1(1,0)] + gt[OPS_ACC_MD1(1,1)]) - cf[OPS_ACC_MD2(1,0)] * al[OPS_ACC_MD3(1,0)]) * ep2[OPS_ACC_MD4(1,0)];
		double e3 = (cmp[OPS_ACC_MD0(2,0)] * (gt[OPS_ACC_MD1(2,0)] + gt[OPS_ACC_MD1(2,1)]) - cf[OPS_ACC_MD2(2,0)] * al[OPS_ACC_MD3(2,0)]) * ep2[OPS_ACC_MD4(2,0)];

		eff[OPS_ACC_MD6(0,0)]=e1 * r[OPS_ACC_MD5(0,0)] + e2 * r[OPS_ACC_MD5(1,0)] + e3 * r[OPS_ACC_MD5(2,0)];
		eff[OPS_ACC_MD6(1,0)]=e1 * r[OPS_ACC_MD5(3,0)] + e2 * r[OPS_ACC_MD5(4,0)] + e3 * r[OPS_ACC_MD5(5,0)];
		eff[OPS_ACC_MD6(2,0)]=e1 * r[OPS_ACC_MD5(6,0)] + e2 * r[OPS_ACC_MD5(7,0)] + e3 * r[OPS_ACC_MD5(8,0)];
}



__kernel void ops_calupwindeff_kernel(
__global const double* restrict arg0,
__global const double* restrict arg1,
__global const double* restrict arg2,
__global const double* restrict arg3,
__global const double* restrict arg4,
__global const double* restrict arg5,
__global double* restrict arg6,
const int base0,
const int base1,
const int base2,
const int base3,
const int base4,
const int base5,
const int base6,
const int size0 ){


  int idx_x = get_global_id(0);

  if (idx_x < size0) {
    calupwindeff_kernel(&arg0[base0 + idx_x * 1*3],
                        &arg1[base1 + idx_x * 1*3],
                        &arg2[base2 + idx_x * 1*3],
                        &arg3[base3 + idx_x * 1*3],
                        &arg4[base4 + idx_x * 1*3],
                        &arg5[base5 + idx_x * 1*9],
                        &arg6[base6 + idx_x * 1*3]);
  }

}
