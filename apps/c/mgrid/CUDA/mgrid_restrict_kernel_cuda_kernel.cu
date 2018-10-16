//
// auto-generated by ops.py
//
__constant__ int xdim0_mgrid_restrict_kernel;
int xdim0_mgrid_restrict_kernel_h = -1;
__constant__ int xdim1_mgrid_restrict_kernel;
int xdim1_mgrid_restrict_kernel_h = -1;

#undef OPS_ACC0
#undef OPS_ACC1


#define OPS_ACC0(x,y) (x+xdim0_mgrid_restrict_kernel*(y))
#define OPS_ACC1(x,y) (x+xdim1_mgrid_restrict_kernel*(y))

//user function
__device__

void mgrid_restrict_kernel_gpu(const double *fine, double *coarse, int *idx) {

  coarse[OPS_ACC1(0,0)] = fine[OPS_ACC0(0,0)];
}



#undef OPS_ACC0
#undef OPS_ACC1


__global__ void ops_mgrid_restrict_kernel(
const double* __restrict arg0,
int stride_00, int stride_01,
double* __restrict arg1,
int arg_idx0, int arg_idx1,
int size0,
int size1 ){


  int idx_y = blockDim.y * blockIdx.y + threadIdx.y;
  int idx_x = blockDim.x * blockIdx.x + threadIdx.x;

  int arg_idx[2];
  arg_idx[0] = arg_idx0+idx_x;
  arg_idx[1] = arg_idx1+idx_y;
  arg0 += idx_x*stride_00 * 1*1 + idx_y*stride_01 * 1*1 * xdim0_mgrid_restrict_kernel;
  arg1 += idx_x * 1*1 + idx_y * 1*1 * xdim1_mgrid_restrict_kernel;

  if (idx_x < size0 && idx_y < size1) {
    mgrid_restrict_kernel_gpu(arg0, arg1, arg_idx);
  }

}

// host stub function
#ifndef OPS_LAZY
void ops_par_loop_mgrid_restrict_kernel(char const *name, ops_block block, int dim, int* range,
 ops_arg arg0, ops_arg arg1, ops_arg arg2) {
#else
void ops_par_loop_mgrid_restrict_kernel_execute(ops_kernel_descriptor *desc) {
  int dim = desc->dim;
  int *range = desc->range;
  #ifdef OPS_MPI
  ops_block block = desc->block;
  #endif
  ops_arg arg0 = desc->args[0];
  ops_arg arg1 = desc->args[1];
  ops_arg arg2 = desc->args[2];
  #endif

  //Timing
  double t1,t2,c1,c2;

  ops_arg args[3] = { arg0, arg1, arg2};


  #if CHECKPOINTING && !OPS_LAZY
  if (!ops_checkpointing_before(args,3,range,5)) return;
  #endif

  if (OPS_diags > 1) {
    ops_timing_realloc(5,"mgrid_restrict_kernel");
    OPS_kernels[5].count++;
    ops_timers_core(&c1,&t1);
  }

  //compute locally allocated range for the sub-block
  int start[2];
  int end[2];

  int arg_idx[2];
  #ifdef OPS_MPI
  #ifdef OPS_LAZY
  sub_block_list sb = OPS_sub_block_list[block->index];
  for ( int n=0; n<2; n++ ){
    start[n] = range[2*n];end[n] = range[2*n+1];
    arg_idx[n] = sb->decomp_disp[n]+start[n];
  }
  #else
  if (compute_ranges(args, 3,block, range, start, end, arg_idx) < 0) return;
  #endif
  #else
  for ( int n=0; n<2; n++ ){
    start[n] = range[2*n];end[n] = range[2*n+1];
    arg_idx[n] = start[n];
  }
  #endif
  int global_idx[2];
  #ifdef OPS_MPI
  global_idx[0] = arg_idx[0];
  global_idx[1] = arg_idx[1];
  #else
  global_idx[0] = start[0];
  global_idx[1] = start[1];
  #endif

  int xdim0 = args[0].dat->size[0];
  int xdim1 = args[1].dat->size[0];

  if (xdim0 != xdim0_mgrid_restrict_kernel_h || xdim1 != xdim1_mgrid_restrict_kernel_h) {
    cudaMemcpyToSymbol( xdim0_mgrid_restrict_kernel, &xdim0, sizeof(int) );
    xdim0_mgrid_restrict_kernel_h = xdim0;
    cudaMemcpyToSymbol( xdim1_mgrid_restrict_kernel, &xdim1, sizeof(int) );
    xdim1_mgrid_restrict_kernel_h = xdim1;
  }



  int x_size = MAX(0,end[0]-start[0]);
  int y_size = MAX(0,end[1]-start[1]);

  dim3 grid( (x_size-1)/OPS_block_size_x+ 1, (y_size-1)/OPS_block_size_y + 1, 1);
  dim3 tblock(OPS_block_size_x,OPS_block_size_y,OPS_block_size_z);



  int dat0 = (OPS_soa ? args[0].dat->type_size : args[0].dat->elem_size);
  int dat1 = (OPS_soa ? args[1].dat->type_size : args[1].dat->elem_size);

  char *p_a[3];
  //This arg has a prolong stencil - so create different ranges
  int start_0[2]; int end_0[2]; int stride_0[2];int d_size_0[2];
  #ifdef OPS_MPI
  for ( int n=0; n<2; n++ ){
    sub_dat *sd0 = OPS_sub_dat_list[args[0].dat->index];
    stride_0[n] = args[0].stencil->mgrid_stride[n];
    d_size_0[n] = args[0].dat->d_m[n] + sd0->decomp_size[n] - args[0].dat->d_p[n];
    start_0[n] = global_idx[n]*stride_0[n] - sd0->decomp_disp[n] + args[0].dat->d_m[n];
    end_0[n] = start_0[n] + d_size_0[n];
  }
  #else
  for ( int n=0; n<2; n++ ){
    stride_0[n] = args[0].stencil->mgrid_stride[n];
    d_size_0[n] = args[0].dat->d_m[n] + args[0].dat->size[n] - args[0].dat->d_p[n];
    start_0[n] = global_idx[n]*stride_0[n];
    end_0[n] = start_0[n] + d_size_0[n];
  }
  #endif

  //set up initial pointers
  int base0 = args[0].dat->base_offset + 
           dat0 * 1 * (start_0[0] * args[0].stencil->stride[0]);
  base0 = base0+ dat0 *
    args[0].dat->size[0] *
    (start_0[1] * args[0].stencil->stride[1]);
  p_a[0] = (char *)args[0].data_d + base0;

  int base1 = args[1].dat->base_offset + 
           dat1 * 1 * (start[0] * args[1].stencil->stride[0]);
  base1 = base1+ dat1 *
    args[1].dat->size[0] *
    (start[1] * args[1].stencil->stride[1]);
  p_a[1] = (char *)args[1].data_d + base1;


  #ifndef OPS_LAZY
  ops_H_D_exchanges_device(args, 3);
  ops_halo_exchanges(args,3,range);
  #endif

  if (OPS_diags > 1) {
    ops_timers_core(&c2,&t2);
    OPS_kernels[5].mpi_time += t2-t1;
  }


  //call kernel wrapper function, passing in pointers to data
  if (x_size > 0 && y_size > 0)
    ops_mgrid_restrict_kernel<<<grid, tblock >>> (  (double *)p_a[0],stride_0[0],stride_0[1], (double *)p_a[1],
         arg_idx[0], arg_idx[1],x_size, y_size);

  cutilSafeCall(cudaGetLastError());

  if (OPS_diags>1) {
    cutilSafeCall(cudaDeviceSynchronize());
    ops_timers_core(&c1,&t1);
    OPS_kernels[5].time += t1-t2;
  }

  #ifndef OPS_LAZY
  ops_set_dirtybit_device(args, 3);
  ops_set_halo_dirtybit3(&args[1],range);
  #endif

  if (OPS_diags > 1) {
    //Update kernel record
    ops_timers_core(&c2,&t2);
    OPS_kernels[5].mpi_time += t2-t1;
    OPS_kernels[5].transfer += ops_compute_transfer(dim, start, end, &arg0);
    OPS_kernels[5].transfer += ops_compute_transfer(dim, start, end, &arg1);
  }
}

#ifdef OPS_LAZY
void ops_par_loop_mgrid_restrict_kernel(char const *name, ops_block block, int dim, int* range,
 ops_arg arg0, ops_arg arg1, ops_arg arg2) {
  ops_kernel_descriptor *desc = (ops_kernel_descriptor *)malloc(sizeof(ops_kernel_descriptor));
  desc->name = name;
  desc->block = block;
  desc->dim = dim;
  desc->device = 1;
  desc->index = 5;
  desc->hash = 5381;
  desc->hash = ((desc->hash << 5) + desc->hash) + 5;
  for ( int i=0; i<4; i++ ){
    desc->range[i] = range[i];
    desc->orig_range[i] = range[i];
    desc->hash = ((desc->hash << 5) + desc->hash) + range[i];
  }
  desc->nargs = 3;
  desc->args = (ops_arg*)malloc(3*sizeof(ops_arg));
  desc->args[0] = arg0;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg0.dat->index;
  desc->args[1] = arg1;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg1.dat->index;
  desc->args[2] = arg2;
  desc->function = ops_par_loop_mgrid_restrict_kernel_execute;
  if (OPS_diags > 1) {
    ops_timing_realloc(5,"mgrid_restrict_kernel");
  }
  ops_enqueue_kernel(desc);
}
#endif
