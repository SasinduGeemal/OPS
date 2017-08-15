//
// auto-generated by ops.py
//
__constant__ int xdim0_accelerate_kernel;
int xdim0_accelerate_kernel_h = -1;
__constant__ int ydim0_accelerate_kernel;
int ydim0_accelerate_kernel_h = -1;
__constant__ int xdim1_accelerate_kernel;
int xdim1_accelerate_kernel_h = -1;
__constant__ int ydim1_accelerate_kernel;
int ydim1_accelerate_kernel_h = -1;
__constant__ int xdim2_accelerate_kernel;
int xdim2_accelerate_kernel_h = -1;
__constant__ int ydim2_accelerate_kernel;
int ydim2_accelerate_kernel_h = -1;
__constant__ int xdim3_accelerate_kernel;
int xdim3_accelerate_kernel_h = -1;
__constant__ int ydim3_accelerate_kernel;
int ydim3_accelerate_kernel_h = -1;
__constant__ int xdim4_accelerate_kernel;
int xdim4_accelerate_kernel_h = -1;
__constant__ int ydim4_accelerate_kernel;
int ydim4_accelerate_kernel_h = -1;
__constant__ int xdim5_accelerate_kernel;
int xdim5_accelerate_kernel_h = -1;
__constant__ int ydim5_accelerate_kernel;
int ydim5_accelerate_kernel_h = -1;
__constant__ int xdim6_accelerate_kernel;
int xdim6_accelerate_kernel_h = -1;
__constant__ int ydim6_accelerate_kernel;
int ydim6_accelerate_kernel_h = -1;
__constant__ int xdim7_accelerate_kernel;
int xdim7_accelerate_kernel_h = -1;
__constant__ int ydim7_accelerate_kernel;
int ydim7_accelerate_kernel_h = -1;
__constant__ int xdim8_accelerate_kernel;
int xdim8_accelerate_kernel_h = -1;
__constant__ int ydim8_accelerate_kernel;
int ydim8_accelerate_kernel_h = -1;
__constant__ int xdim9_accelerate_kernel;
int xdim9_accelerate_kernel_h = -1;
__constant__ int ydim9_accelerate_kernel;
int ydim9_accelerate_kernel_h = -1;
__constant__ int xdim10_accelerate_kernel;
int xdim10_accelerate_kernel_h = -1;
__constant__ int ydim10_accelerate_kernel;
int ydim10_accelerate_kernel_h = -1;
__constant__ int xdim11_accelerate_kernel;
int xdim11_accelerate_kernel_h = -1;
__constant__ int ydim11_accelerate_kernel;
int ydim11_accelerate_kernel_h = -1;
__constant__ int xdim12_accelerate_kernel;
int xdim12_accelerate_kernel_h = -1;
__constant__ int ydim12_accelerate_kernel;
int ydim12_accelerate_kernel_h = -1;
__constant__ int xdim13_accelerate_kernel;
int xdim13_accelerate_kernel_h = -1;
__constant__ int ydim13_accelerate_kernel;
int ydim13_accelerate_kernel_h = -1;

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
__device__

void accelerate_kernel_gpu( const double *density0, const double *volume,
                double *stepbymass, const double *xvel0, double *xvel1,
                const double *xarea, const double *pressure,
                const double *yvel0, double *yvel1,
                const double *yarea, const double *viscosity,
                const double *zvel0, double *zvel1,
                const double *zarea) {

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


__global__ void ops_accelerate_kernel(
const double* __restrict arg0,
const double* __restrict arg1,
double* __restrict arg2,
const double* __restrict arg3,
double* __restrict arg4,
const double* __restrict arg5,
const double* __restrict arg6,
const double* __restrict arg7,
double* __restrict arg8,
const double* __restrict arg9,
const double* __restrict arg10,
const double* __restrict arg11,
double* __restrict arg12,
const double* __restrict arg13,
int size0,
int size1,
int size2 ){


  int idx_z = blockDim.z * blockIdx.z + threadIdx.z;
  int idx_y = blockDim.y * blockIdx.y + threadIdx.y;
  int idx_x = blockDim.x * blockIdx.x + threadIdx.x;

  arg0 += idx_x * 1*1 + idx_y * 1*1 * xdim0_accelerate_kernel + idx_z * 1*1 * xdim0_accelerate_kernel * ydim0_accelerate_kernel;
  arg1 += idx_x * 1*1 + idx_y * 1*1 * xdim1_accelerate_kernel + idx_z * 1*1 * xdim1_accelerate_kernel * ydim1_accelerate_kernel;
  arg2 += idx_x * 1*1 + idx_y * 1*1 * xdim2_accelerate_kernel + idx_z * 1*1 * xdim2_accelerate_kernel * ydim2_accelerate_kernel;
  arg3 += idx_x * 1*1 + idx_y * 1*1 * xdim3_accelerate_kernel + idx_z * 1*1 * xdim3_accelerate_kernel * ydim3_accelerate_kernel;
  arg4 += idx_x * 1*1 + idx_y * 1*1 * xdim4_accelerate_kernel + idx_z * 1*1 * xdim4_accelerate_kernel * ydim4_accelerate_kernel;
  arg5 += idx_x * 1*1 + idx_y * 1*1 * xdim5_accelerate_kernel + idx_z * 1*1 * xdim5_accelerate_kernel * ydim5_accelerate_kernel;
  arg6 += idx_x * 1*1 + idx_y * 1*1 * xdim6_accelerate_kernel + idx_z * 1*1 * xdim6_accelerate_kernel * ydim6_accelerate_kernel;
  arg7 += idx_x * 1*1 + idx_y * 1*1 * xdim7_accelerate_kernel + idx_z * 1*1 * xdim7_accelerate_kernel * ydim7_accelerate_kernel;
  arg8 += idx_x * 1*1 + idx_y * 1*1 * xdim8_accelerate_kernel + idx_z * 1*1 * xdim8_accelerate_kernel * ydim8_accelerate_kernel;
  arg9 += idx_x * 1*1 + idx_y * 1*1 * xdim9_accelerate_kernel + idx_z * 1*1 * xdim9_accelerate_kernel * ydim9_accelerate_kernel;
  arg10 += idx_x * 1*1 + idx_y * 1*1 * xdim10_accelerate_kernel + idx_z * 1*1 * xdim10_accelerate_kernel * ydim10_accelerate_kernel;
  arg11 += idx_x * 1*1 + idx_y * 1*1 * xdim11_accelerate_kernel + idx_z * 1*1 * xdim11_accelerate_kernel * ydim11_accelerate_kernel;
  arg12 += idx_x * 1*1 + idx_y * 1*1 * xdim12_accelerate_kernel + idx_z * 1*1 * xdim12_accelerate_kernel * ydim12_accelerate_kernel;
  arg13 += idx_x * 1*1 + idx_y * 1*1 * xdim13_accelerate_kernel + idx_z * 1*1 * xdim13_accelerate_kernel * ydim13_accelerate_kernel;

  if (idx_x < size0 && idx_y < size1 && idx_z < size2) {
    accelerate_kernel_gpu(arg0, arg1, arg2, arg3,
                   arg4, arg5, arg6, arg7, arg8,
                   arg9, arg10, arg11, arg12, arg13);
  }

}

// host stub function
// host stub function
void ops_par_loop_accelerate_kernel_execute(ops_kernel_descriptor *desc) {
  #ifdef OPS_MPI
  ops_block block = desc->block;
  #endif
  int dim = desc->dim;
  int *range = desc->range;
  ops_arg arg0 = desc->args[0];
  ops_arg arg1 = desc->args[1];
  ops_arg arg2 = desc->args[2];
  ops_arg arg3 = desc->args[3];
  ops_arg arg4 = desc->args[4];
  ops_arg arg5 = desc->args[5];
  ops_arg arg6 = desc->args[6];
  ops_arg arg7 = desc->args[7];
  ops_arg arg8 = desc->args[8];
  ops_arg arg9 = desc->args[9];
  ops_arg arg10 = desc->args[10];
  ops_arg arg11 = desc->args[11];
  ops_arg arg12 = desc->args[12];
  ops_arg arg13 = desc->args[13];

  //Timing
  double t1,t2,c1,c2;

  ops_arg args[14] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_before(args,14,range,6)) return;
  #endif

  if (OPS_diags > 1) {
    ops_timing_realloc(6,"accelerate_kernel");
    OPS_kernels[6].count++;
    ops_timers_core(&c1,&t1);
  }

  //compute locally allocated range for the sub-block
  int start[3];
  int end[3];

  for ( int n=0; n<3; n++ ){
    start[n] = range[2*n];end[n] = range[2*n+1];
  }


  int x_size = MAX(0,end[0]-start[0]);
  int y_size = MAX(0,end[1]-start[1]);
  int z_size = MAX(0,end[2]-start[2]);

  int xdim0 = args[0].dat->size[0];
  int ydim0 = args[0].dat->size[1];
  int xdim1 = args[1].dat->size[0];
  int ydim1 = args[1].dat->size[1];
  int xdim2 = args[2].dat->size[0];
  int ydim2 = args[2].dat->size[1];
  int xdim3 = args[3].dat->size[0];
  int ydim3 = args[3].dat->size[1];
  int xdim4 = args[4].dat->size[0];
  int ydim4 = args[4].dat->size[1];
  int xdim5 = args[5].dat->size[0];
  int ydim5 = args[5].dat->size[1];
  int xdim6 = args[6].dat->size[0];
  int ydim6 = args[6].dat->size[1];
  int xdim7 = args[7].dat->size[0];
  int ydim7 = args[7].dat->size[1];
  int xdim8 = args[8].dat->size[0];
  int ydim8 = args[8].dat->size[1];
  int xdim9 = args[9].dat->size[0];
  int ydim9 = args[9].dat->size[1];
  int xdim10 = args[10].dat->size[0];
  int ydim10 = args[10].dat->size[1];
  int xdim11 = args[11].dat->size[0];
  int ydim11 = args[11].dat->size[1];
  int xdim12 = args[12].dat->size[0];
  int ydim12 = args[12].dat->size[1];
  int xdim13 = args[13].dat->size[0];
  int ydim13 = args[13].dat->size[1];

  if (xdim0 != xdim0_accelerate_kernel_h || ydim0 != ydim0_accelerate_kernel_h || xdim1 != xdim1_accelerate_kernel_h || ydim1 != ydim1_accelerate_kernel_h || xdim2 != xdim2_accelerate_kernel_h || ydim2 != ydim2_accelerate_kernel_h || xdim3 != xdim3_accelerate_kernel_h || ydim3 != ydim3_accelerate_kernel_h || xdim4 != xdim4_accelerate_kernel_h || ydim4 != ydim4_accelerate_kernel_h || xdim5 != xdim5_accelerate_kernel_h || ydim5 != ydim5_accelerate_kernel_h || xdim6 != xdim6_accelerate_kernel_h || ydim6 != ydim6_accelerate_kernel_h || xdim7 != xdim7_accelerate_kernel_h || ydim7 != ydim7_accelerate_kernel_h || xdim8 != xdim8_accelerate_kernel_h || ydim8 != ydim8_accelerate_kernel_h || xdim9 != xdim9_accelerate_kernel_h || ydim9 != ydim9_accelerate_kernel_h || xdim10 != xdim10_accelerate_kernel_h || ydim10 != ydim10_accelerate_kernel_h || xdim11 != xdim11_accelerate_kernel_h || ydim11 != ydim11_accelerate_kernel_h || xdim12 != xdim12_accelerate_kernel_h || ydim12 != ydim12_accelerate_kernel_h || xdim13 != xdim13_accelerate_kernel_h || ydim13 != ydim13_accelerate_kernel_h) {
    cudaMemcpyToSymbolAsync( xdim0_accelerate_kernel, &xdim0, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    xdim0_accelerate_kernel_h = xdim0;
    cudaMemcpyToSymbolAsync( ydim0_accelerate_kernel, &ydim0, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    ydim0_accelerate_kernel_h = ydim0;
    cudaMemcpyToSymbolAsync( xdim1_accelerate_kernel, &xdim1, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    xdim1_accelerate_kernel_h = xdim1;
    cudaMemcpyToSymbolAsync( ydim1_accelerate_kernel, &ydim1, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    ydim1_accelerate_kernel_h = ydim1;
    cudaMemcpyToSymbolAsync( xdim2_accelerate_kernel, &xdim2, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    xdim2_accelerate_kernel_h = xdim2;
    cudaMemcpyToSymbolAsync( ydim2_accelerate_kernel, &ydim2, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    ydim2_accelerate_kernel_h = ydim2;
    cudaMemcpyToSymbolAsync( xdim3_accelerate_kernel, &xdim3, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    xdim3_accelerate_kernel_h = xdim3;
    cudaMemcpyToSymbolAsync( ydim3_accelerate_kernel, &ydim3, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    ydim3_accelerate_kernel_h = ydim3;
    cudaMemcpyToSymbolAsync( xdim4_accelerate_kernel, &xdim4, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    xdim4_accelerate_kernel_h = xdim4;
    cudaMemcpyToSymbolAsync( ydim4_accelerate_kernel, &ydim4, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    ydim4_accelerate_kernel_h = ydim4;
    cudaMemcpyToSymbolAsync( xdim5_accelerate_kernel, &xdim5, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    xdim5_accelerate_kernel_h = xdim5;
    cudaMemcpyToSymbolAsync( ydim5_accelerate_kernel, &ydim5, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    ydim5_accelerate_kernel_h = ydim5;
    cudaMemcpyToSymbolAsync( xdim6_accelerate_kernel, &xdim6, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    xdim6_accelerate_kernel_h = xdim6;
    cudaMemcpyToSymbolAsync( ydim6_accelerate_kernel, &ydim6, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    ydim6_accelerate_kernel_h = ydim6;
    cudaMemcpyToSymbolAsync( xdim7_accelerate_kernel, &xdim7, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    xdim7_accelerate_kernel_h = xdim7;
    cudaMemcpyToSymbolAsync( ydim7_accelerate_kernel, &ydim7, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    ydim7_accelerate_kernel_h = ydim7;
    cudaMemcpyToSymbolAsync( xdim8_accelerate_kernel, &xdim8, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    xdim8_accelerate_kernel_h = xdim8;
    cudaMemcpyToSymbolAsync( ydim8_accelerate_kernel, &ydim8, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    ydim8_accelerate_kernel_h = ydim8;
    cudaMemcpyToSymbolAsync( xdim9_accelerate_kernel, &xdim9, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    xdim9_accelerate_kernel_h = xdim9;
    cudaMemcpyToSymbolAsync( ydim9_accelerate_kernel, &ydim9, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    ydim9_accelerate_kernel_h = ydim9;
    cudaMemcpyToSymbolAsync( xdim10_accelerate_kernel, &xdim10, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    xdim10_accelerate_kernel_h = xdim10;
    cudaMemcpyToSymbolAsync( ydim10_accelerate_kernel, &ydim10, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    ydim10_accelerate_kernel_h = ydim10;
    cudaMemcpyToSymbolAsync( xdim11_accelerate_kernel, &xdim11, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    xdim11_accelerate_kernel_h = xdim11;
    cudaMemcpyToSymbolAsync( ydim11_accelerate_kernel, &ydim11, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    ydim11_accelerate_kernel_h = ydim11;
    cudaMemcpyToSymbolAsync( xdim12_accelerate_kernel, &xdim12, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    xdim12_accelerate_kernel_h = xdim12;
    cudaMemcpyToSymbolAsync( ydim12_accelerate_kernel, &ydim12, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    ydim12_accelerate_kernel_h = ydim12;
    cudaMemcpyToSymbolAsync( xdim13_accelerate_kernel, &xdim13, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    xdim13_accelerate_kernel_h = xdim13;
    cudaMemcpyToSymbolAsync( ydim13_accelerate_kernel, &ydim13, sizeof(int),0,cudaMemcpyHostToDevice,stream );
    ydim13_accelerate_kernel_h = ydim13;
  }



  dim3 grid( (x_size-1)/OPS_block_size_x+ 1, (y_size-1)/OPS_block_size_y + 1, z_size);
  dim3 tblock(OPS_block_size_x,OPS_block_size_y,1);




  char *p_a[14];
  int dat0 = args[0].dat->elem_size;
  int dat1 = args[1].dat->elem_size;
  int dat2 = args[2].dat->elem_size;
  int dat3 = args[3].dat->elem_size;
  int dat4 = args[4].dat->elem_size;
  int dat5 = args[5].dat->elem_size;
  int dat6 = args[6].dat->elem_size;
  int dat7 = args[7].dat->elem_size;
  int dat8 = args[8].dat->elem_size;
  int dat9 = args[9].dat->elem_size;
  int dat10 = args[10].dat->elem_size;
  int dat11 = args[11].dat->elem_size;
  int dat12 = args[12].dat->elem_size;
  int dat13 = args[13].dat->elem_size;

  //set up initial pointers
  int base0 = args[0].dat->base_offset;
  base0 += dat0 * (start[0] * args[0].stencil->stride[0]);
  base0 += dat0 *
    args[0].dat->size[0] *
    (start[1] * args[0].stencil->stride[1]);
  base0 += dat0 *
    args[0].dat->size[0] *
    args[0].dat->size[1] *
    (start[2] * args[0].stencil->stride[2]);
  p_a[0] = (char *)args[0].dat->data_d + base0;

  int base1 = args[1].dat->base_offset;
  base1 += dat1 * (start[0] * args[1].stencil->stride[0]);
  base1 += dat1 *
    args[1].dat->size[0] *
    (start[1] * args[1].stencil->stride[1]);
  base1 += dat1 *
    args[1].dat->size[0] *
    args[1].dat->size[1] *
    (start[2] * args[1].stencil->stride[2]);
  p_a[1] = (char *)args[1].dat->data_d + base1;

  int base2 = args[2].dat->base_offset;
  base2 += dat2 * (start[0] * args[2].stencil->stride[0]);
  base2 += dat2 *
    args[2].dat->size[0] *
    (start[1] * args[2].stencil->stride[1]);
  base2 += dat2 *
    args[2].dat->size[0] *
    args[2].dat->size[1] *
    (start[2] * args[2].stencil->stride[2]);
  p_a[2] = (char *)args[2].dat->data_d + base2;

  int base3 = args[3].dat->base_offset;
  base3 += dat3 * (start[0] * args[3].stencil->stride[0]);
  base3 += dat3 *
    args[3].dat->size[0] *
    (start[1] * args[3].stencil->stride[1]);
  base3 += dat3 *
    args[3].dat->size[0] *
    args[3].dat->size[1] *
    (start[2] * args[3].stencil->stride[2]);
  p_a[3] = (char *)args[3].dat->data_d + base3;

  int base4 = args[4].dat->base_offset;
  base4 += dat4 * (start[0] * args[4].stencil->stride[0]);
  base4 += dat4 *
    args[4].dat->size[0] *
    (start[1] * args[4].stencil->stride[1]);
  base4 += dat4 *
    args[4].dat->size[0] *
    args[4].dat->size[1] *
    (start[2] * args[4].stencil->stride[2]);
  p_a[4] = (char *)args[4].dat->data_d + base4;

  int base5 = args[5].dat->base_offset;
  base5 += dat5 * (start[0] * args[5].stencil->stride[0]);
  base5 += dat5 *
    args[5].dat->size[0] *
    (start[1] * args[5].stencil->stride[1]);
  base5 += dat5 *
    args[5].dat->size[0] *
    args[5].dat->size[1] *
    (start[2] * args[5].stencil->stride[2]);
  p_a[5] = (char *)args[5].dat->data_d + base5;

  int base6 = args[6].dat->base_offset;
  base6 += dat6 * (start[0] * args[6].stencil->stride[0]);
  base6 += dat6 *
    args[6].dat->size[0] *
    (start[1] * args[6].stencil->stride[1]);
  base6 += dat6 *
    args[6].dat->size[0] *
    args[6].dat->size[1] *
    (start[2] * args[6].stencil->stride[2]);
  p_a[6] = (char *)args[6].dat->data_d + base6;

  int base7 = args[7].dat->base_offset;
  base7 += dat7 * (start[0] * args[7].stencil->stride[0]);
  base7 += dat7 *
    args[7].dat->size[0] *
    (start[1] * args[7].stencil->stride[1]);
  base7 += dat7 *
    args[7].dat->size[0] *
    args[7].dat->size[1] *
    (start[2] * args[7].stencil->stride[2]);
  p_a[7] = (char *)args[7].dat->data_d + base7;

  int base8 = args[8].dat->base_offset;
  base8 += dat8 * (start[0] * args[8].stencil->stride[0]);
  base8 += dat8 *
    args[8].dat->size[0] *
    (start[1] * args[8].stencil->stride[1]);
  base8 += dat8 *
    args[8].dat->size[0] *
    args[8].dat->size[1] *
    (start[2] * args[8].stencil->stride[2]);
  p_a[8] = (char *)args[8].dat->data_d + base8;

  int base9 = args[9].dat->base_offset;
  base9 += dat9 * (start[0] * args[9].stencil->stride[0]);
  base9 += dat9 *
    args[9].dat->size[0] *
    (start[1] * args[9].stencil->stride[1]);
  base9 += dat9 *
    args[9].dat->size[0] *
    args[9].dat->size[1] *
    (start[2] * args[9].stencil->stride[2]);
  p_a[9] = (char *)args[9].dat->data_d + base9;

  int base10 = args[10].dat->base_offset;
  base10 += dat10 * (start[0] * args[10].stencil->stride[0]);
  base10 += dat10 *
    args[10].dat->size[0] *
    (start[1] * args[10].stencil->stride[1]);
  base10 += dat10 *
    args[10].dat->size[0] *
    args[10].dat->size[1] *
    (start[2] * args[10].stencil->stride[2]);
  p_a[10] = (char *)args[10].dat->data_d + base10;

  int base11 = args[11].dat->base_offset;
  base11 += dat11 * (start[0] * args[11].stencil->stride[0]);
  base11 += dat11 *
    args[11].dat->size[0] *
    (start[1] * args[11].stencil->stride[1]);
  base11 += dat11 *
    args[11].dat->size[0] *
    args[11].dat->size[1] *
    (start[2] * args[11].stencil->stride[2]);
  p_a[11] = (char *)args[11].dat->data_d + base11;

  int base12 = args[12].dat->base_offset;
  base12 += dat12 * (start[0] * args[12].stencil->stride[0]);
  base12 += dat12 *
    args[12].dat->size[0] *
    (start[1] * args[12].stencil->stride[1]);
  base12 += dat12 *
    args[12].dat->size[0] *
    args[12].dat->size[1] *
    (start[2] * args[12].stencil->stride[2]);
  p_a[12] = (char *)args[12].dat->data_d + base12;

  int base13 = args[13].dat->base_offset;
  base13 += dat13 * (start[0] * args[13].stencil->stride[0]);
  base13 += dat13 *
    args[13].dat->size[0] *
    (start[1] * args[13].stencil->stride[1]);
  base13 += dat13 *
    args[13].dat->size[0] *
    args[13].dat->size[1] *
    (start[2] * args[13].stencil->stride[2]);
  p_a[13] = (char *)args[13].dat->data_d + base13;


  ops_H_D_exchanges_device(args, 14);
  ops_halo_exchanges(args,14,range);

  if (OPS_diags > 1) {
    ops_timers_core(&c2,&t2);
    OPS_kernels[6].mpi_time += t2-t1;
  }


  //call kernel wrapper function, passing in pointers to data
  ops_accelerate_kernel<<<grid, tblock, 0, stream >>> (  (double *)p_a[0], (double *)p_a[1],
           (double *)p_a[2], (double *)p_a[3],
           (double *)p_a[4], (double *)p_a[5],
           (double *)p_a[6], (double *)p_a[7],
           (double *)p_a[8], (double *)p_a[9],
           (double *)p_a[10], (double *)p_a[11],
           (double *)p_a[12], (double *)p_a[13],x_size, y_size, z_size);

  if (OPS_diags>1) {
    cutilSafeCall(cudaStreamSynchronize(stream));
    ops_timers_core(&c1,&t1);
    OPS_kernels[6].time += t1-t2;
  }

  ops_set_dirtybit_device(args, 14);
  ops_set_halo_dirtybit3(&args[2],range);
  ops_set_halo_dirtybit3(&args[4],range);
  ops_set_halo_dirtybit3(&args[8],range);
  ops_set_halo_dirtybit3(&args[12],range);

  if (OPS_diags > 1) {
    //Update kernel record
    ops_timers_core(&c2,&t2);
    OPS_kernels[6].mpi_time += t2-t1;
    OPS_kernels[6].transfer += ops_compute_transfer(dim, start, end, &arg0);
    OPS_kernels[6].transfer += ops_compute_transfer(dim, start, end, &arg1);
    OPS_kernels[6].transfer += ops_compute_transfer(dim, start, end, &arg2);
    OPS_kernels[6].transfer += ops_compute_transfer(dim, start, end, &arg3);
    OPS_kernels[6].transfer += ops_compute_transfer(dim, start, end, &arg4);
    OPS_kernels[6].transfer += ops_compute_transfer(dim, start, end, &arg5);
    OPS_kernels[6].transfer += ops_compute_transfer(dim, start, end, &arg6);
    OPS_kernels[6].transfer += ops_compute_transfer(dim, start, end, &arg7);
    OPS_kernels[6].transfer += ops_compute_transfer(dim, start, end, &arg8);
    OPS_kernels[6].transfer += ops_compute_transfer(dim, start, end, &arg9);
    OPS_kernels[6].transfer += ops_compute_transfer(dim, start, end, &arg10);
    OPS_kernels[6].transfer += ops_compute_transfer(dim, start, end, &arg11);
    OPS_kernels[6].transfer += ops_compute_transfer(dim, start, end, &arg12);
    OPS_kernels[6].transfer += ops_compute_transfer(dim, start, end, &arg13);
  }
}

void ops_par_loop_accelerate_kernel(char const *name, ops_block block, int dim, int* range,
 ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3, ops_arg arg4, ops_arg arg5, ops_arg arg6, ops_arg arg7, ops_arg arg8, ops_arg arg9, ops_arg arg10, ops_arg arg11, ops_arg arg12, ops_arg arg13) {
  ops_kernel_descriptor *desc = (ops_kernel_descriptor *)malloc(sizeof(ops_kernel_descriptor));
  desc->name = name;
  desc->block = block;
  desc->dim = dim;
  desc->index = 6;
  desc->hash = 5381;
  desc->hash = ((desc->hash << 5) + desc->hash) + 6;
  for ( int i=0; i<6; i++ ){
    desc->range[i] = range[i];
    desc->orig_range[i] = range[i];
    desc->hash = ((desc->hash << 5) + desc->hash) + range[i];
  }
  desc->nargs = 14;
  desc->args = (ops_arg*)malloc(14*sizeof(ops_arg));
  desc->args[0] = arg0;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg0.dat->index;
  desc->args[1] = arg1;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg1.dat->index;
  desc->args[2] = arg2;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg2.dat->index;
  desc->args[3] = arg3;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg3.dat->index;
  desc->args[4] = arg4;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg4.dat->index;
  desc->args[5] = arg5;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg5.dat->index;
  desc->args[6] = arg6;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg6.dat->index;
  desc->args[7] = arg7;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg7.dat->index;
  desc->args[8] = arg8;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg8.dat->index;
  desc->args[9] = arg9;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg9.dat->index;
  desc->args[10] = arg10;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg10.dat->index;
  desc->args[11] = arg11;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg11.dat->index;
  desc->args[12] = arg12;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg12.dat->index;
  desc->args[13] = arg13;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg13.dat->index;
  desc->function = ops_par_loop_accelerate_kernel_execute;
  if (OPS_diags > 1) {
    ops_timing_realloc(6,"accelerate_kernel");
  }
  ops_enqueue_kernel(desc);
}