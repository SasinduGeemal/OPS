//
// auto-generated by ops.py
//

#ifdef OCL_FMA
#pragma OPENCL FP_CONTRACT ON
#else
#pragma OPENCL FP_CONTRACT OFF
#endif
#pragma OPENCL EXTENSION cl_khr_fp64:enable

#include "user_types.h"
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
#undef OPS_ACC3
#undef OPS_ACC4
#undef OPS_ACC5
#undef OPS_ACC6
#undef OPS_ACC7
#undef OPS_ACC8
#undef OPS_ACC9
#undef OPS_ACC10
#undef OPS_ACC11
#undef OPS_ACC12
#undef OPS_ACC13


#define OPS_ACC0(x,y,z) (x+xdim0_accelerate_kernel*(y)+xdim0_accelerate_kernel*ydim0_accelerate_kernel*(z))
#define OPS_ACC1(x,y,z) (x+xdim1_accelerate_kernel*(y)+xdim1_accelerate_kernel*ydim1_accelerate_kernel*(z))
#define OPS_ACC2(x,y,z) (x+xdim2_accelerate_kernel*(y)+xdim2_accelerate_kernel*ydim2_accelerate_kernel*(z))
#define OPS_ACC3(x,y,z) (x+xdim3_accelerate_kernel*(y)+xdim3_accelerate_kernel*ydim3_accelerate_kernel*(z))
#define OPS_ACC4(x,y,z) (x+xdim4_accelerate_kernel*(y)+xdim4_accelerate_kernel*ydim4_accelerate_kernel*(z))
#define OPS_ACC5(x,y,z) (x+xdim5_accelerate_kernel*(y)+xdim5_accelerate_kernel*ydim5_accelerate_kernel*(z))
#define OPS_ACC6(x,y,z) (x+xdim6_accelerate_kernel*(y)+xdim6_accelerate_kernel*ydim6_accelerate_kernel*(z))
#define OPS_ACC7(x,y,z) (x+xdim7_accelerate_kernel*(y)+xdim7_accelerate_kernel*ydim7_accelerate_kernel*(z))
#define OPS_ACC8(x,y,z) (x+xdim8_accelerate_kernel*(y)+xdim8_accelerate_kernel*ydim8_accelerate_kernel*(z))
#define OPS_ACC9(x,y,z) (x+xdim9_accelerate_kernel*(y)+xdim9_accelerate_kernel*ydim9_accelerate_kernel*(z))
#define OPS_ACC10(x,y,z) (x+xdim10_accelerate_kernel*(y)+xdim10_accelerate_kernel*ydim10_accelerate_kernel*(z))
#define OPS_ACC11(x,y,z) (x+xdim11_accelerate_kernel*(y)+xdim11_accelerate_kernel*ydim11_accelerate_kernel*(z))
#define OPS_ACC12(x,y,z) (x+xdim12_accelerate_kernel*(y)+xdim12_accelerate_kernel*ydim12_accelerate_kernel*(z))
#define OPS_ACC13(x,y,z) (x+xdim13_accelerate_kernel*(y)+xdim13_accelerate_kernel*ydim13_accelerate_kernel*(z))


//user function
void accelerate_kernel( const __global double * restrict density0,const __global double * restrict volume,__global double * restrict stepbymass,
const __global double * restrict xvel0,__global double * restrict xvel1,const __global double * restrict xarea,const __global double * restrict pressure,
const __global double * restrict yvel0,__global double * restrict yvel1,const __global double * restrict yarea,const __global double * restrict viscosity,
const __global double * restrict zvel0,__global double * restrict zvel1,const __global double * restrict zarea,
  const double dt)

 {

  double nodal_mass = 0.0;
  nodal_mass =(density0[OPS_ACC0(-1,-1, 0)] * volume[OPS_ACC1(-1,-1, 0)] +
               density0[OPS_ACC0( 0,-1, 0)] * volume[OPS_ACC1( 0,-1, 0)] +
               density0[OPS_ACC0( 0, 0, 0)] * volume[OPS_ACC1( 0, 0, 0)] +
               density0[OPS_ACC0(-1, 0, 0)] * volume[OPS_ACC1(-1, 0, 0)] +
               density0[OPS_ACC0(-1,-1,-1)] * volume[OPS_ACC1(-1,-1,-1)] +
               density0[OPS_ACC0( 0,-1,-1)] * volume[OPS_ACC1( 0,-1,-1)] +
               density0[OPS_ACC0( 0, 0,-1)] * volume[OPS_ACC1( 0, 0,-1)] +
               density0[OPS_ACC0(-1, 0,-1)] * volume[OPS_ACC1(-1, 0,-1)]) * 0.125;

  stepbymass[OPS_ACC2(0,0,0)] = 0.25*dt / nodal_mass;

  xvel1[OPS_ACC4(0,0,0)] = xvel0[OPS_ACC3(0,0,0)] - stepbymass[OPS_ACC2(0,0,0)] *
            ( xarea[OPS_ACC5(0,0,0)]  * ( pressure[OPS_ACC6(0,0,0)] - pressure[OPS_ACC6(-1,0,0)] ) +
              xarea[OPS_ACC5(0,-1,0)] * ( pressure[OPS_ACC6(0,-1,0)] - pressure[OPS_ACC6(-1,-1,0)] ) +
              xarea[OPS_ACC5(0,0,-1)] * ( pressure[OPS_ACC6(0,0,-1)] - pressure[OPS_ACC6(-1,0,-1)] ) +
              xarea[OPS_ACC5(0,-1,-1)] * ( pressure[OPS_ACC6(0,-1,-1)] - pressure[OPS_ACC6(-1,-1,-1)] ) );

  yvel1[OPS_ACC8(0,0,0)] = yvel0[OPS_ACC7(0,0,0)] - stepbymass[OPS_ACC2(0,0,0)] *
            ( yarea[OPS_ACC9(0,0,0)]  * ( pressure[OPS_ACC6(0,0,0)] - pressure[OPS_ACC6(0,-1,0)] ) +
              yarea[OPS_ACC9(-1,0,0)] * ( pressure[OPS_ACC6(-1,0,0)] - pressure[OPS_ACC6(-1,-1,0)] ) +
              yarea[OPS_ACC9(0,0,-1)] * ( pressure[OPS_ACC6(0,0,-1)] - pressure[OPS_ACC6(0,-1,-1)] ) +
              yarea[OPS_ACC9(-1,0,-1)]* ( pressure[OPS_ACC6(-1,0,-1)] - pressure[OPS_ACC6(-1,-1,-1)] ) );

  zvel1[OPS_ACC12(0,0,0)] = zvel0[OPS_ACC11(0,0,0)] - stepbymass[OPS_ACC2(0,0,0)] *
            ( zarea[OPS_ACC13(0,0,0)]  * ( pressure[OPS_ACC6(0,0,0)] - pressure[OPS_ACC6(0,0,-1)] ) +
              zarea[OPS_ACC13(0,-1,0)] * ( pressure[OPS_ACC6(0,-1,0)] - pressure[OPS_ACC6(0,-1,-1)] ) +
              zarea[OPS_ACC13(-1,0,0)] * ( pressure[OPS_ACC6(-1,0,0)] - pressure[OPS_ACC6(-1,0,-1)] ) +
              zarea[OPS_ACC13(-1,-1,0)]* ( pressure[OPS_ACC6(-1,-1,0)] - pressure[OPS_ACC6(-1,-1,-1)] ) );

  xvel1[OPS_ACC4(0,0,0)] = xvel1[OPS_ACC4(0,0,0)] - stepbymass[OPS_ACC2(0,0,0)] *
            ( xarea[OPS_ACC5(0,0,0)]  * ( viscosity[OPS_ACC10(0,0,0)] - viscosity[OPS_ACC10(-1,0,0)] ) +
              xarea[OPS_ACC5(0,-1,0)] * ( viscosity[OPS_ACC10(0,-1,0)] - viscosity[OPS_ACC10(-1,-1,0)] ) +
              xarea[OPS_ACC5(0,0,-1)] * ( viscosity[OPS_ACC10(0,0,-1)] - viscosity[OPS_ACC10(-1,0,-1)] ) +
              xarea[OPS_ACC5(0,-1,-1)]* ( viscosity[OPS_ACC10(0,-1,-1)] - viscosity[OPS_ACC10(-1,-1,-1)] ) );

  yvel1[OPS_ACC8(0,0,0)] = yvel1[OPS_ACC8(0,0,0)] - stepbymass[OPS_ACC2(0,0,0)] *
            ( yarea[OPS_ACC9(0,0,0)]  * ( viscosity[OPS_ACC10(0,0,0)]  - viscosity[OPS_ACC10(0,-1,0)] ) +
              yarea[OPS_ACC9(-1,0,0)] * ( viscosity[OPS_ACC10(-1,0,0)] - viscosity[OPS_ACC10(-1,-1,0)] ) +
              yarea[OPS_ACC9(0,0,-1)] * ( viscosity[OPS_ACC10(0,0,-1)] - viscosity[OPS_ACC10(0,-1,-1)] ) +
              yarea[OPS_ACC9(-1,0,-1)]* ( viscosity[OPS_ACC10(-1,0,-1)]- viscosity[OPS_ACC10(-1,-1,-1)] ) );

  zvel1[OPS_ACC12(0,0,0)] = zvel1[OPS_ACC12(0,0,0)] - stepbymass[OPS_ACC2(0,0,0)] *
            ( zarea[OPS_ACC13(0,0,0)]  * ( viscosity[OPS_ACC10(0,0,0)]  - viscosity[OPS_ACC10(0,0,-1)] ) +
              zarea[OPS_ACC13(0,-1,0)] * ( viscosity[OPS_ACC10(0,-1,0)] - viscosity[OPS_ACC10(0,-1,-1)] ) +
              zarea[OPS_ACC13(-1,0,0)] * ( viscosity[OPS_ACC10(-1,0,0)] - viscosity[OPS_ACC10(-1,0,-1)] ) +
              zarea[OPS_ACC13(-1,-1,0)]* ( viscosity[OPS_ACC10(-1,-1,0)]- viscosity[OPS_ACC10(-1,-1,-1)] ) );

}



__kernel void ops_accelerate_kernel(
__global const double* restrict arg0,
__global const double* restrict arg1,
__global double* restrict arg2,
__global const double* restrict arg3,
__global double* restrict arg4,
__global const double* restrict arg5,
__global const double* restrict arg6,
__global const double* restrict arg7,
__global double* restrict arg8,
__global const double* restrict arg9,
__global const double* restrict arg10,
__global const double* restrict arg11,
__global double* restrict arg12,
__global const double* restrict arg13,
const double dt,
const int base0,
const int base1,
const int base2,
const int base3,
const int base4,
const int base5,
const int base6,
const int base7,
const int base8,
const int base9,
const int base10,
const int base11,
const int base12,
const int base13,
const int size0,
const int size1,
const int size2 ){


  int idx_y = get_global_id(1);
  int idx_z = get_global_id(2);
  int idx_x = get_global_id(0);

  if (idx_x < size0 && idx_y < size1 && idx_z < size2) {
    accelerate_kernel(&arg0[base0 + idx_x * 1*1 + idx_y * 1*1 * xdim0_accelerate_kernel + idx_z * 1*1 * xdim0_accelerate_kernel * ydim0_accelerate_kernel],
                      &arg1[base1 + idx_x * 1*1 + idx_y * 1*1 * xdim1_accelerate_kernel + idx_z * 1*1 * xdim1_accelerate_kernel * ydim1_accelerate_kernel],
                      &arg2[base2 + idx_x * 1*1 + idx_y * 1*1 * xdim2_accelerate_kernel + idx_z * 1*1 * xdim2_accelerate_kernel * ydim2_accelerate_kernel],
                      &arg3[base3 + idx_x * 1*1 + idx_y * 1*1 * xdim3_accelerate_kernel + idx_z * 1*1 * xdim3_accelerate_kernel * ydim3_accelerate_kernel],
                      &arg4[base4 + idx_x * 1*1 + idx_y * 1*1 * xdim4_accelerate_kernel + idx_z * 1*1 * xdim4_accelerate_kernel * ydim4_accelerate_kernel],
                      &arg5[base5 + idx_x * 1*1 + idx_y * 1*1 * xdim5_accelerate_kernel + idx_z * 1*1 * xdim5_accelerate_kernel * ydim5_accelerate_kernel],
                      &arg6[base6 + idx_x * 1*1 + idx_y * 1*1 * xdim6_accelerate_kernel + idx_z * 1*1 * xdim6_accelerate_kernel * ydim6_accelerate_kernel],
                      &arg7[base7 + idx_x * 1*1 + idx_y * 1*1 * xdim7_accelerate_kernel + idx_z * 1*1 * xdim7_accelerate_kernel * ydim7_accelerate_kernel],
                      &arg8[base8 + idx_x * 1*1 + idx_y * 1*1 * xdim8_accelerate_kernel + idx_z * 1*1 * xdim8_accelerate_kernel * ydim8_accelerate_kernel],
                      &arg9[base9 + idx_x * 1*1 + idx_y * 1*1 * xdim9_accelerate_kernel + idx_z * 1*1 * xdim9_accelerate_kernel * ydim9_accelerate_kernel],
                      &arg10[base10 + idx_x * 1*1 + idx_y * 1*1 * xdim10_accelerate_kernel + idx_z * 1*1 * xdim10_accelerate_kernel * ydim10_accelerate_kernel],
                      &arg11[base11 + idx_x * 1*1 + idx_y * 1*1 * xdim11_accelerate_kernel + idx_z * 1*1 * xdim11_accelerate_kernel * ydim11_accelerate_kernel],
                      &arg12[base12 + idx_x * 1*1 + idx_y * 1*1 * xdim12_accelerate_kernel + idx_z * 1*1 * xdim12_accelerate_kernel * ydim12_accelerate_kernel],
                      &arg13[base13 + idx_x * 1*1 + idx_y * 1*1 * xdim13_accelerate_kernel + idx_z * 1*1 * xdim13_accelerate_kernel * ydim13_accelerate_kernel],
                      dt);
  }

}
