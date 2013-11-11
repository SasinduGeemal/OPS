//
// auto-generated by ops.py on 2013-11-08 13:55
//


#include "data.h"
#include "definitions.h"

//user function
#include "accelerate_kernel.h"



__global__ void ops_accelerate_kernel_stepbymass(
const double* __restrict arg0,
const double* __restrict arg1,
double* __restrict arg2,
int size0,
int size1 ){

  int idx_y = blockDim.y * blockIdx.y + threadIdx.y;
  int idx_x = blockDim.x * blockIdx.x + threadIdx.x;

  arg0 += idx_x * 1 + idx_y * 1 * xdim0_accel;
  arg1 += idx_x * 1 + idx_y * 1 * xdim1_accel;
  arg2 += idx_x * 1 + idx_y * 1 * xdim2_accel;
  if (idx_x < size0 && idx_y < size1) {
    accelerate_kernel_stepbymass(arg0 ,arg1 ,arg2 );
  }
}

// host stub function
void ops_par_loop_accelerate_kernel_stepbymass(char const *name, int dim, int* range,
 ops_arg arg0, ops_arg arg1, ops_arg arg2) {

  ops_arg args[3] = { arg0, arg1, arg2};

  int x_size = range[1]-range[0];
  int y_size = range[3]-range[2];

  int xdim0 = args[0].dat->block_size[0];
  int xdim1 = args[1].dat->block_size[0];
  int xdim2 = args[2].dat->block_size[0];

  cudaMemcpyToSymbol( xdim0_accel, &xdim0, sizeof(int) );
  cudaMemcpyToSymbol( xdim1_accel, &xdim1, sizeof(int) );
  cudaMemcpyToSymbol( xdim2_accel, &xdim2, sizeof(int) );
  cudaMemcpyToSymbol( dt_device,  &dt, sizeof(double) );

  char *p_a[3];


  //set up initial pointers
  p_a[0] = &args[0].data_d[
  + args[0].dat->size * args[0].dat->block_size[0] * ( range[2] * 1 - args[0].dat->offset[1] )
  + args[0].dat->size * ( range[0] * 1 - args[0].dat->offset[0] ) ];

  p_a[1] = &args[1].data_d[
  + args[1].dat->size * args[1].dat->block_size[0] * ( range[2] * 1 - args[1].dat->offset[1] )
  + args[1].dat->size * ( range[0] * 1 - args[1].dat->offset[0] ) ];

  p_a[2] = &args[2].data_d[
  + args[2].dat->size * args[2].dat->block_size[0] * ( range[2] * 1 - args[2].dat->offset[1] )
  + args[2].dat->size * ( range[0] * 1 - args[2].dat->offset[0] ) ];


  ops_halo_exchanges_cuda(args, 3);

  int block_size = 16;
  dim3 grid( (x_size-1)/block_size+ 1, (y_size-1)/block_size + 1, 1);
  dim3 block(16,16,1);

  //call kernel wrapper function, passing in pointers to data
  ops_accelerate_kernel_stepbymass<<<grid, block >>> (  (double *)p_a[0],  (double *)p_a[1],
           (double *)p_a[2], x_size, y_size);

  cudaDeviceSynchronize();
  ops_set_dirtybit_cuda(args, 3);
  ops_halo_exchanges(args, 3);
}
