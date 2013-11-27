//
// auto-generated by ops.py on 2013-11-27 12:14
//

__constant__ int xdim0_update_halo_kernel3_minus_2_a;
__constant__ int xdim1_update_halo_kernel3_minus_2_a;

#define OPS_ACC0(x,y) (x+xdim0_update_halo_kernel3_minus_2_a*(y))
#define OPS_ACC1(x,y) (x+xdim1_update_halo_kernel3_minus_2_a*(y))

//user function
__device__

inline void update_halo_kernel3_minus_2_a(double *vol_flux_x, double *mass_flux_x, const int* fields) {
  if(fields[FIELD_VOL_FLUX_X] == 1)  vol_flux_x[OPS_ACC0(0,0)]  = -(vol_flux_x[OPS_ACC0(2,0)]);
  if(fields[FIELD_MASS_FLUX_X] == 1) mass_flux_x[OPS_ACC1(0,0)] = -(mass_flux_x[OPS_ACC1(2,0)]);
}



#undef OPS_ACC0
#undef OPS_ACC1


__global__ void ops_update_halo_kernel3_minus_2_a(
double* __restrict arg0,
double* __restrict arg1,
const int* __restrict arg2,
int size0,
int size1 ){


  int idx_y = blockDim.y * blockIdx.y + threadIdx.y;
  int idx_x = blockDim.x * blockIdx.x + threadIdx.x;

  arg0 += idx_x * 1 + idx_y * 1 * xdim0_update_halo_kernel3_minus_2_a;
  arg1 += idx_x * 1 + idx_y * 1 * xdim1_update_halo_kernel3_minus_2_a;

  if (idx_x < size0 && idx_y < size1) {
    update_halo_kernel3_minus_2_a(arg0, arg1, arg2);
  }

}

// host stub function
void ops_par_loop_update_halo_kernel3_minus_2_a(char const *name, int dim, int* range,
 ops_arg arg0, ops_arg arg1, ops_arg arg2) {

  ops_arg args[3] = { arg0, arg1, arg2};


  int x_size = range[1]-range[0];
  int y_size = range[3]-range[2];

  int xdim0 = args[0].dat->block_size[0];
  int xdim1 = args[1].dat->block_size[0];


  //Timing
  double t1,t2,c1,c2;
  ops_timing_realloc(64,"update_halo_kernel3_minus_2_a");
  ops_timers_core(&c1,&t1);

  if (OPS_kernels[64].count == 0) {
    cudaMemcpyToSymbol( xdim0_update_halo_kernel3_minus_2_a, &xdim0, sizeof(int) );
    cudaMemcpyToSymbol( xdim1_update_halo_kernel3_minus_2_a, &xdim1, sizeof(int) );
  }


  int *arg2h = (int *)arg2.data;

  dim3 grid( (x_size-1)/OPS_block_size_x+ 1, (y_size-1)/OPS_block_size_y + 1, 1);
  dim3 block(OPS_block_size_x,OPS_block_size_y,1);

  int consts_bytes = 0;

  consts_bytes += ROUND_UP(NUM_FIELDS*sizeof(int));

  reallocConstArrays(consts_bytes);

  consts_bytes = 0;
  arg2.data = OPS_consts_h + consts_bytes;
  arg2.data_d = OPS_consts_d + consts_bytes;
  for (int d=0; d<NUM_FIELDS; d++) ((int *)arg2.data)[d] = arg2h[d];
  consts_bytes += ROUND_UP(NUM_FIELDS*sizeof(int));
  mvConstArraysToDevice(consts_bytes);

  char *p_a[3];


  //set up initial pointers
  p_a[0] = &args[0].data_d[
  + args[0].dat->size * args[0].dat->block_size[0] * ( range[2] * 1 - args[0].dat->offset[1] )
  + args[0].dat->size * ( range[0] * 1 - args[0].dat->offset[0] ) ];

  p_a[1] = &args[1].data_d[
  + args[1].dat->size * args[1].dat->block_size[0] * ( range[2] * 1 - args[1].dat->offset[1] )
  + args[1].dat->size * ( range[0] * 1 - args[1].dat->offset[0] ) ];


  ops_halo_exchanges_cuda(args, 3);


  //call kernel wrapper function, passing in pointers to data
  ops_update_halo_kernel3_minus_2_a<<<grid, block >>> (  (double *)p_a[0], (double *)p_a[1],
           (int *)arg2.data_d,x_size, y_size);

  if (OPS_diags>1) cudaDeviceSynchronize();
  ops_set_dirtybit_cuda(args, 3);

  //Update kernel record
  ops_timers_core(&c2,&t2);
  OPS_kernels[64].count++;
  OPS_kernels[64].time += t2-t1;
  OPS_kernels[64].transfer += ops_compute_transfer(dim, range, &arg0);
  OPS_kernels[64].transfer += ops_compute_transfer(dim, range, &arg1);
}
