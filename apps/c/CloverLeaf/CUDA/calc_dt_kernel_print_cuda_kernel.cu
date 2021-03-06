//
// auto-generated by ops.py
//
__constant__ int xdim0_calc_dt_kernel_print;
int xdim0_calc_dt_kernel_print_h = -1;
__constant__ int xdim1_calc_dt_kernel_print;
int xdim1_calc_dt_kernel_print_h = -1;
__constant__ int xdim2_calc_dt_kernel_print;
int xdim2_calc_dt_kernel_print_h = -1;
__constant__ int xdim3_calc_dt_kernel_print;
int xdim3_calc_dt_kernel_print_h = -1;
__constant__ int xdim4_calc_dt_kernel_print;
int xdim4_calc_dt_kernel_print_h = -1;
__constant__ int xdim5_calc_dt_kernel_print;
int xdim5_calc_dt_kernel_print_h = -1;

#undef OPS_ACC0
#undef OPS_ACC1
#undef OPS_ACC2
#undef OPS_ACC3
#undef OPS_ACC4
#undef OPS_ACC5


#define OPS_ACC0(x,y) (x+xdim0_calc_dt_kernel_print*(y))
#define OPS_ACC1(x,y) (x+xdim1_calc_dt_kernel_print*(y))
#define OPS_ACC2(x,y) (x+xdim2_calc_dt_kernel_print*(y))
#define OPS_ACC3(x,y) (x+xdim3_calc_dt_kernel_print*(y))
#define OPS_ACC4(x,y) (x+xdim4_calc_dt_kernel_print*(y))
#define OPS_ACC5(x,y) (x+xdim5_calc_dt_kernel_print*(y))

//user function
__device__

void calc_dt_kernel_print_gpu(const double *xvel0, const double *yvel0,
                        const double *density0, const double *energy0,
                        const double *pressure, const double *soundspeed, double *output) {
  output[0] = xvel0[OPS_ACC0(1,0)];
  output[1] = yvel0[OPS_ACC1(1,0)];
  output[2] = xvel0[OPS_ACC0(-1,0)];
  output[3] = yvel0[OPS_ACC1(-1,0)];
  output[4] = xvel0[OPS_ACC0(0,1)];
  output[5] = yvel0[OPS_ACC1(0,1)];
  output[6] = xvel0[OPS_ACC0(0,-1)];
  output[7] = yvel0[OPS_ACC1(0,-1)];
  output[8] = density0[OPS_ACC2(0,0)];
  output[9] = energy0[OPS_ACC3(0,0)];
  output[10]= pressure[OPS_ACC4(0,0)];
  output[11]= soundspeed[OPS_ACC5(0,0)];

}



#undef OPS_ACC0
#undef OPS_ACC1
#undef OPS_ACC2
#undef OPS_ACC3
#undef OPS_ACC4
#undef OPS_ACC5


__global__ void ops_calc_dt_kernel_print(
const double* __restrict arg0,
const double* __restrict arg1,
const double* __restrict arg2,
const double* __restrict arg3,
const double* __restrict arg4,
const double* __restrict arg5,
double* __restrict arg6,
int size0,
int size1 ){

  double arg6_l[12];
  for (int d=0; d<12; d++) arg6_l[d] = ZERO_double;

  int idx_y = blockDim.y * blockIdx.y + threadIdx.y;
  int idx_x = blockDim.x * blockIdx.x + threadIdx.x;

  arg0 += idx_x * 1*1 + idx_y * 1*1 * xdim0_calc_dt_kernel_print;
  arg1 += idx_x * 1*1 + idx_y * 1*1 * xdim1_calc_dt_kernel_print;
  arg2 += idx_x * 1*1 + idx_y * 1*1 * xdim2_calc_dt_kernel_print;
  arg3 += idx_x * 1*1 + idx_y * 1*1 * xdim3_calc_dt_kernel_print;
  arg4 += idx_x * 1*1 + idx_y * 1*1 * xdim4_calc_dt_kernel_print;
  arg5 += idx_x * 1*1 + idx_y * 1*1 * xdim5_calc_dt_kernel_print;

  if (idx_x < size0 && idx_y < size1) {
    calc_dt_kernel_print_gpu(arg0, arg1, arg2, arg3,
                   arg4, arg5, arg6_l);
  }
  for (int d=0; d<12; d++)
    ops_reduction_cuda<OPS_INC>(&arg6[d+(blockIdx.x + blockIdx.y*gridDim.x)*12],arg6_l[d]);

}

// host stub function
#ifndef OPS_LAZY
void ops_par_loop_calc_dt_kernel_print(char const *name, ops_block block, int dim, int* range,
 ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
 ops_arg arg4, ops_arg arg5, ops_arg arg6) {
#else
void ops_par_loop_calc_dt_kernel_print_execute(ops_kernel_descriptor *desc) {
  int dim = desc->dim;
  int *range = desc->range;
  ops_arg arg0 = desc->args[0];
  ops_arg arg1 = desc->args[1];
  ops_arg arg2 = desc->args[2];
  ops_arg arg3 = desc->args[3];
  ops_arg arg4 = desc->args[4];
  ops_arg arg5 = desc->args[5];
  ops_arg arg6 = desc->args[6];
  #endif

  //Timing
  double t1,t2,c1,c2;

  ops_arg args[7] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6};


  #if CHECKPOINTING && !OPS_LAZY
  if (!ops_checkpointing_before(args,7,range,54)) return;
  #endif

  if (OPS_diags > 1) {
    ops_timing_realloc(54,"calc_dt_kernel_print");
    OPS_kernels[54].count++;
    ops_timers_core(&c1,&t1);
  }

  //compute locally allocated range for the sub-block
  int start[2];
  int end[2];
  #if OPS_MPI && !OPS_LAZY
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  for ( int n=0; n<2; n++ ){
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) {
      start[n] = 0;
    }
    else {
      start[n] = range[2*n] - start[n];
    }
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) {
      end[n] = range[2*n+1] - sb->decomp_disp[n];
    }
    else {
      end[n] = sb->decomp_size[n];
    }
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else
  for ( int n=0; n<2; n++ ){
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif

  int x_size = MAX(0,end[0]-start[0]);
  int y_size = MAX(0,end[1]-start[1]);

  int xdim0 = args[0].dat->size[0];
  int xdim1 = args[1].dat->size[0];
  int xdim2 = args[2].dat->size[0];
  int xdim3 = args[3].dat->size[0];
  int xdim4 = args[4].dat->size[0];
  int xdim5 = args[5].dat->size[0];

  if (xdim0 != xdim0_calc_dt_kernel_print_h || xdim1 != xdim1_calc_dt_kernel_print_h || xdim2 != xdim2_calc_dt_kernel_print_h || xdim3 != xdim3_calc_dt_kernel_print_h || xdim4 != xdim4_calc_dt_kernel_print_h || xdim5 != xdim5_calc_dt_kernel_print_h) {
    cudaMemcpyToSymbol( xdim0_calc_dt_kernel_print, &xdim0, sizeof(int) );
    xdim0_calc_dt_kernel_print_h = xdim0;
    cudaMemcpyToSymbol( xdim1_calc_dt_kernel_print, &xdim1, sizeof(int) );
    xdim1_calc_dt_kernel_print_h = xdim1;
    cudaMemcpyToSymbol( xdim2_calc_dt_kernel_print, &xdim2, sizeof(int) );
    xdim2_calc_dt_kernel_print_h = xdim2;
    cudaMemcpyToSymbol( xdim3_calc_dt_kernel_print, &xdim3, sizeof(int) );
    xdim3_calc_dt_kernel_print_h = xdim3;
    cudaMemcpyToSymbol( xdim4_calc_dt_kernel_print, &xdim4, sizeof(int) );
    xdim4_calc_dt_kernel_print_h = xdim4;
    cudaMemcpyToSymbol( xdim5_calc_dt_kernel_print, &xdim5, sizeof(int) );
    xdim5_calc_dt_kernel_print_h = xdim5;
  }


  #ifdef OPS_LAZY
  ops_block block = desc->block;
  #endif
  #ifdef OPS_MPI
  double *arg6h = (double *)(((ops_reduction)args[6].data)->data + ((ops_reduction)args[6].data)->size * block->index);
  #else
  double *arg6h = (double *)(((ops_reduction)args[6].data)->data);
  #endif

  dim3 grid( (x_size-1)/OPS_block_size_x+ 1, (y_size-1)/OPS_block_size_y + 1, 1);
  dim3 tblock(OPS_block_size_x,OPS_block_size_y,OPS_block_size_z);

  int nblocks = ((x_size-1)/OPS_block_size_x+ 1)*((y_size-1)/OPS_block_size_y + 1);
  int maxblocks = nblocks;
  int reduct_bytes = 0;
  int reduct_size = 0;

  reduct_bytes += ROUND_UP(maxblocks*12*sizeof(double));
  reduct_size = MAX(reduct_size,sizeof(double)*12);

  reallocReductArrays(reduct_bytes);
  reduct_bytes = 0;

  arg6.data = OPS_reduct_h + reduct_bytes;
  arg6.data_d = OPS_reduct_d + reduct_bytes;
  for (int b=0; b<maxblocks; b++)
  for (int d=0; d<12; d++) ((double *)arg6.data)[d+b*12] = ZERO_double;
  reduct_bytes += ROUND_UP(maxblocks*12*sizeof(double));


  mvReductArraysToDevice(reduct_bytes);
  int dat0 = (OPS_soa ? args[0].dat->type_size : args[0].dat->elem_size);
  int dat1 = (OPS_soa ? args[1].dat->type_size : args[1].dat->elem_size);
  int dat2 = (OPS_soa ? args[2].dat->type_size : args[2].dat->elem_size);
  int dat3 = (OPS_soa ? args[3].dat->type_size : args[3].dat->elem_size);
  int dat4 = (OPS_soa ? args[4].dat->type_size : args[4].dat->elem_size);
  int dat5 = (OPS_soa ? args[5].dat->type_size : args[5].dat->elem_size);

  char *p_a[7];

  //set up initial pointers
  int base0 = args[0].dat->base_offset + 
           dat0 * 1 * (start[0] * args[0].stencil->stride[0]);
  base0 = base0+ dat0 *
    args[0].dat->size[0] *
    (start[1] * args[0].stencil->stride[1]);
  p_a[0] = (char *)args[0].data_d + base0;

  int base1 = args[1].dat->base_offset + 
           dat1 * 1 * (start[0] * args[1].stencil->stride[0]);
  base1 = base1+ dat1 *
    args[1].dat->size[0] *
    (start[1] * args[1].stencil->stride[1]);
  p_a[1] = (char *)args[1].data_d + base1;

  int base2 = args[2].dat->base_offset + 
           dat2 * 1 * (start[0] * args[2].stencil->stride[0]);
  base2 = base2+ dat2 *
    args[2].dat->size[0] *
    (start[1] * args[2].stencil->stride[1]);
  p_a[2] = (char *)args[2].data_d + base2;

  int base3 = args[3].dat->base_offset + 
           dat3 * 1 * (start[0] * args[3].stencil->stride[0]);
  base3 = base3+ dat3 *
    args[3].dat->size[0] *
    (start[1] * args[3].stencil->stride[1]);
  p_a[3] = (char *)args[3].data_d + base3;

  int base4 = args[4].dat->base_offset + 
           dat4 * 1 * (start[0] * args[4].stencil->stride[0]);
  base4 = base4+ dat4 *
    args[4].dat->size[0] *
    (start[1] * args[4].stencil->stride[1]);
  p_a[4] = (char *)args[4].data_d + base4;

  int base5 = args[5].dat->base_offset + 
           dat5 * 1 * (start[0] * args[5].stencil->stride[0]);
  base5 = base5+ dat5 *
    args[5].dat->size[0] *
    (start[1] * args[5].stencil->stride[1]);
  p_a[5] = (char *)args[5].data_d + base5;


  #ifndef OPS_LAZY
  ops_H_D_exchanges_device(args, 7);
  ops_halo_exchanges(args,7,range);
  #endif

  if (OPS_diags > 1) {
    ops_timers_core(&c2,&t2);
    OPS_kernels[54].mpi_time += t2-t1;
  }

  int nshared = 0;
  int nthread = OPS_block_size_x*OPS_block_size_y*OPS_block_size_z;

  nshared = MAX(nshared,sizeof(double)*12);

  nshared = MAX(nshared*nthread,reduct_size*nthread);

  //call kernel wrapper function, passing in pointers to data
  if (x_size > 0 && y_size > 0)
    ops_calc_dt_kernel_print<<<grid, tblock, nshared >>> (  (double *)p_a[0], (double *)p_a[1],
           (double *)p_a[2], (double *)p_a[3],
           (double *)p_a[4], (double *)p_a[5],
           (double *)arg6.data_d,x_size, y_size);

  cutilSafeCall(cudaGetLastError());

  mvReductArraysToHost(reduct_bytes);
  for ( int b=0; b<maxblocks; b++ ){
    for ( int d=0; d<12; d++ ){
      arg6h[d] = arg6h[d] + ((double *)arg6.data)[d+b*12];
    }
  }
  arg6.data = (char *)arg6h;

  if (OPS_diags>1) {
    cutilSafeCall(cudaDeviceSynchronize());
    ops_timers_core(&c1,&t1);
    OPS_kernels[54].time += t1-t2;
  }

  #ifndef OPS_LAZY
  ops_set_dirtybit_device(args, 7);
  #endif

  if (OPS_diags > 1) {
    //Update kernel record
    ops_timers_core(&c2,&t2);
    OPS_kernels[54].mpi_time += t2-t1;
    OPS_kernels[54].transfer += ops_compute_transfer(dim, start, end, &arg0);
    OPS_kernels[54].transfer += ops_compute_transfer(dim, start, end, &arg1);
    OPS_kernels[54].transfer += ops_compute_transfer(dim, start, end, &arg2);
    OPS_kernels[54].transfer += ops_compute_transfer(dim, start, end, &arg3);
    OPS_kernels[54].transfer += ops_compute_transfer(dim, start, end, &arg4);
    OPS_kernels[54].transfer += ops_compute_transfer(dim, start, end, &arg5);
  }
}

#ifdef OPS_LAZY
void ops_par_loop_calc_dt_kernel_print(char const *name, ops_block block, int dim, int* range,
 ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3, ops_arg arg4, ops_arg arg5, ops_arg arg6) {
  ops_kernel_descriptor *desc = (ops_kernel_descriptor *)malloc(sizeof(ops_kernel_descriptor));
  desc->name = name;
  desc->block = block;
  desc->dim = dim;
  desc->device = 1;
  desc->index = 54;
  desc->hash = 5381;
  desc->hash = ((desc->hash << 5) + desc->hash) + 54;
  for ( int i=0; i<4; i++ ){
    desc->range[i] = range[i];
    desc->orig_range[i] = range[i];
    desc->hash = ((desc->hash << 5) + desc->hash) + range[i];
  }
  desc->nargs = 7;
  desc->args = (ops_arg*)malloc(7*sizeof(ops_arg));
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
  desc->function = ops_par_loop_calc_dt_kernel_print_execute;
  if (OPS_diags > 1) {
    ops_timing_realloc(54,"calc_dt_kernel_print");
  }
  ops_enqueue_kernel(desc);
}
#endif
