//
// auto-generated by ops.py
//
__constant__ int xdim0_advec_mom_kernel_x2;
int xdim0_advec_mom_kernel_x2_h = -1;
__constant__ int ydim0_advec_mom_kernel_x2;
int ydim0_advec_mom_kernel_x2_h = -1;
__constant__ int xdim1_advec_mom_kernel_x2;
int xdim1_advec_mom_kernel_x2_h = -1;
__constant__ int ydim1_advec_mom_kernel_x2;
int ydim1_advec_mom_kernel_x2_h = -1;
__constant__ int xdim2_advec_mom_kernel_x2;
int xdim2_advec_mom_kernel_x2_h = -1;
__constant__ int ydim2_advec_mom_kernel_x2;
int ydim2_advec_mom_kernel_x2_h = -1;
__constant__ int xdim3_advec_mom_kernel_x2;
int xdim3_advec_mom_kernel_x2_h = -1;
__constant__ int ydim3_advec_mom_kernel_x2;
int ydim3_advec_mom_kernel_x2_h = -1;
__constant__ int xdim4_advec_mom_kernel_x2;
int xdim4_advec_mom_kernel_x2_h = -1;
__constant__ int ydim4_advec_mom_kernel_x2;
int ydim4_advec_mom_kernel_x2_h = -1;

#undef OPS_ACC0
#undef OPS_ACC1
#undef OPS_ACC2
#undef OPS_ACC3
#undef OPS_ACC4


#define OPS_ACC0(x,y,z) (x+xdim0_advec_mom_kernel_x2*(y)+xdim0_advec_mom_kernel_x2*ydim0_advec_mom_kernel_x2*(z))
#define OPS_ACC1(x,y,z) (x+xdim1_advec_mom_kernel_x2*(y)+xdim1_advec_mom_kernel_x2*ydim1_advec_mom_kernel_x2*(z))
#define OPS_ACC2(x,y,z) (x+xdim2_advec_mom_kernel_x2*(y)+xdim2_advec_mom_kernel_x2*ydim2_advec_mom_kernel_x2*(z))
#define OPS_ACC3(x,y,z) (x+xdim3_advec_mom_kernel_x2*(y)+xdim3_advec_mom_kernel_x2*ydim3_advec_mom_kernel_x2*(z))
#define OPS_ACC4(x,y,z) (x+xdim4_advec_mom_kernel_x2*(y)+xdim4_advec_mom_kernel_x2*ydim4_advec_mom_kernel_x2*(z))

//user function
__device__

inline void advec_mom_kernel_x2( double *pre_vol, double *post_vol,
                          const double *volume,
                          const double *vol_flux_y,const double *vol_flux_z) {

  post_vol[OPS_ACC1(0,0,0)]  = volume[OPS_ACC2(0,0,0)]  + vol_flux_z[OPS_ACC4(0,0,1)] - vol_flux_z[OPS_ACC4(0,0,0)];
  pre_vol[OPS_ACC0(0,0,0)]   = post_vol[OPS_ACC1(0,0,0)]  + vol_flux_y[OPS_ACC3(0,1,0)] - vol_flux_y[OPS_ACC3(0,0,0)];

}



#undef OPS_ACC0
#undef OPS_ACC1
#undef OPS_ACC2
#undef OPS_ACC3
#undef OPS_ACC4


__global__ void ops_advec_mom_kernel_x2(
double* __restrict arg0,
double* __restrict arg1,
const double* __restrict arg2,
const double* __restrict arg3,
const double* __restrict arg4,
int size0,
int size1,
int size2 ){


  int idx_z = blockDim.z * blockIdx.z + threadIdx.z;
  int idx_y = blockDim.y * blockIdx.y + threadIdx.y;
  int idx_x = blockDim.x * blockIdx.x + threadIdx.x;

  arg0 += idx_x * 1*1 + idx_y * 1*1 * xdim0_advec_mom_kernel_x2 + idx_z * 1*1 * xdim0_advec_mom_kernel_x2 * ydim0_advec_mom_kernel_x2;
  arg1 += idx_x * 1*1 + idx_y * 1*1 * xdim1_advec_mom_kernel_x2 + idx_z * 1*1 * xdim1_advec_mom_kernel_x2 * ydim1_advec_mom_kernel_x2;
  arg2 += idx_x * 1*1 + idx_y * 1*1 * xdim2_advec_mom_kernel_x2 + idx_z * 1*1 * xdim2_advec_mom_kernel_x2 * ydim2_advec_mom_kernel_x2;
  arg3 += idx_x * 1*1 + idx_y * 1*1 * xdim3_advec_mom_kernel_x2 + idx_z * 1*1 * xdim3_advec_mom_kernel_x2 * ydim3_advec_mom_kernel_x2;
  arg4 += idx_x * 1*1 + idx_y * 1*1 * xdim4_advec_mom_kernel_x2 + idx_z * 1*1 * xdim4_advec_mom_kernel_x2 * ydim4_advec_mom_kernel_x2;

  if (idx_x < size0 && idx_y < size1 && idx_z < size2) {
    advec_mom_kernel_x2(arg0, arg1, arg2, arg3,
                   arg4);
  }

}

// host stub function
void ops_par_loop_advec_mom_kernel_x2(char const *name, ops_block block, int dim, int* range,
 ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
 ops_arg arg4) {

  //Timing
  double t1,t2,c1,c2;

  ops_arg args[5] = { arg0, arg1, arg2, arg3, arg4};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_before(args,5,range,21)) return;
  #endif

  if (OPS_diags > 1) {
    ops_timing_realloc(21,"advec_mom_kernel_x2");
    OPS_kernels[21].count++;
    ops_timers_core(&c1,&t1);
  }

  //compute locally allocated range for the sub-block
  int start[3];
  int end[3];
  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  for ( int n=0; n<3; n++ ){
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
  for ( int n=0; n<3; n++ ){
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif

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

  if (xdim0 != xdim0_advec_mom_kernel_x2_h || ydim0 != ydim0_advec_mom_kernel_x2_h || xdim1 != xdim1_advec_mom_kernel_x2_h || ydim1 != ydim1_advec_mom_kernel_x2_h || xdim2 != xdim2_advec_mom_kernel_x2_h || ydim2 != ydim2_advec_mom_kernel_x2_h || xdim3 != xdim3_advec_mom_kernel_x2_h || ydim3 != ydim3_advec_mom_kernel_x2_h || xdim4 != xdim4_advec_mom_kernel_x2_h || ydim4 != ydim4_advec_mom_kernel_x2_h) {
    cudaMemcpyToSymbol( xdim0_advec_mom_kernel_x2, &xdim0, sizeof(int) );
    xdim0_advec_mom_kernel_x2_h = xdim0;
    cudaMemcpyToSymbol( ydim0_advec_mom_kernel_x2, &ydim0, sizeof(int) );
    ydim0_advec_mom_kernel_x2_h = ydim0;
    cudaMemcpyToSymbol( xdim1_advec_mom_kernel_x2, &xdim1, sizeof(int) );
    xdim1_advec_mom_kernel_x2_h = xdim1;
    cudaMemcpyToSymbol( ydim1_advec_mom_kernel_x2, &ydim1, sizeof(int) );
    ydim1_advec_mom_kernel_x2_h = ydim1;
    cudaMemcpyToSymbol( xdim2_advec_mom_kernel_x2, &xdim2, sizeof(int) );
    xdim2_advec_mom_kernel_x2_h = xdim2;
    cudaMemcpyToSymbol( ydim2_advec_mom_kernel_x2, &ydim2, sizeof(int) );
    ydim2_advec_mom_kernel_x2_h = ydim2;
    cudaMemcpyToSymbol( xdim3_advec_mom_kernel_x2, &xdim3, sizeof(int) );
    xdim3_advec_mom_kernel_x2_h = xdim3;
    cudaMemcpyToSymbol( ydim3_advec_mom_kernel_x2, &ydim3, sizeof(int) );
    ydim3_advec_mom_kernel_x2_h = ydim3;
    cudaMemcpyToSymbol( xdim4_advec_mom_kernel_x2, &xdim4, sizeof(int) );
    xdim4_advec_mom_kernel_x2_h = xdim4;
    cudaMemcpyToSymbol( ydim4_advec_mom_kernel_x2, &ydim4, sizeof(int) );
    ydim4_advec_mom_kernel_x2_h = ydim4;
  }



  dim3 grid( (x_size-1)/OPS_block_size_x+ 1, (y_size-1)/OPS_block_size_y + 1, z_size);
  dim3 tblock(OPS_block_size_x,OPS_block_size_y,1);



  int dat0 = args[0].dat->elem_size;
  int dat1 = args[1].dat->elem_size;
  int dat2 = args[2].dat->elem_size;
  int dat3 = args[3].dat->elem_size;
  int dat4 = args[4].dat->elem_size;

  char *p_a[5];

  //set up initial pointers
  int d_m[OPS_MAX_DIM];
  #ifdef OPS_MPI
  for (int d = 0; d < dim; d++) d_m[d] = args[0].dat->d_m[d] + OPS_sub_dat_list[args[0].dat->index]->d_im[d];
  #else
  for (int d = 0; d < dim; d++) d_m[d] = args[0].dat->d_m[d];
  #endif
  int base0 = dat0 * 1 *
  (start[0] * args[0].stencil->stride[0] - args[0].dat->base[0] - d_m[0]);
  base0 = base0+ dat0 *
    args[0].dat->size[0] *
    (start[1] * args[0].stencil->stride[1] - args[0].dat->base[1] - d_m[1]);
  base0 = base0+ dat0 *
    args[0].dat->size[0] *
    args[0].dat->size[1] *
    (start[2] * args[0].stencil->stride[2] - args[0].dat->base[2] - d_m[2]);
  p_a[0] = (char *)args[0].data_d + base0;

  #ifdef OPS_MPI
  for (int d = 0; d < dim; d++) d_m[d] = args[1].dat->d_m[d] + OPS_sub_dat_list[args[1].dat->index]->d_im[d];
  #else
  for (int d = 0; d < dim; d++) d_m[d] = args[1].dat->d_m[d];
  #endif
  int base1 = dat1 * 1 *
  (start[0] * args[1].stencil->stride[0] - args[1].dat->base[0] - d_m[0]);
  base1 = base1+ dat1 *
    args[1].dat->size[0] *
    (start[1] * args[1].stencil->stride[1] - args[1].dat->base[1] - d_m[1]);
  base1 = base1+ dat1 *
    args[1].dat->size[0] *
    args[1].dat->size[1] *
    (start[2] * args[1].stencil->stride[2] - args[1].dat->base[2] - d_m[2]);
  p_a[1] = (char *)args[1].data_d + base1;

  #ifdef OPS_MPI
  for (int d = 0; d < dim; d++) d_m[d] = args[2].dat->d_m[d] + OPS_sub_dat_list[args[2].dat->index]->d_im[d];
  #else
  for (int d = 0; d < dim; d++) d_m[d] = args[2].dat->d_m[d];
  #endif
  int base2 = dat2 * 1 *
  (start[0] * args[2].stencil->stride[0] - args[2].dat->base[0] - d_m[0]);
  base2 = base2+ dat2 *
    args[2].dat->size[0] *
    (start[1] * args[2].stencil->stride[1] - args[2].dat->base[1] - d_m[1]);
  base2 = base2+ dat2 *
    args[2].dat->size[0] *
    args[2].dat->size[1] *
    (start[2] * args[2].stencil->stride[2] - args[2].dat->base[2] - d_m[2]);
  p_a[2] = (char *)args[2].data_d + base2;

  #ifdef OPS_MPI
  for (int d = 0; d < dim; d++) d_m[d] = args[3].dat->d_m[d] + OPS_sub_dat_list[args[3].dat->index]->d_im[d];
  #else
  for (int d = 0; d < dim; d++) d_m[d] = args[3].dat->d_m[d];
  #endif
  int base3 = dat3 * 1 *
  (start[0] * args[3].stencil->stride[0] - args[3].dat->base[0] - d_m[0]);
  base3 = base3+ dat3 *
    args[3].dat->size[0] *
    (start[1] * args[3].stencil->stride[1] - args[3].dat->base[1] - d_m[1]);
  base3 = base3+ dat3 *
    args[3].dat->size[0] *
    args[3].dat->size[1] *
    (start[2] * args[3].stencil->stride[2] - args[3].dat->base[2] - d_m[2]);
  p_a[3] = (char *)args[3].data_d + base3;

  #ifdef OPS_MPI
  for (int d = 0; d < dim; d++) d_m[d] = args[4].dat->d_m[d] + OPS_sub_dat_list[args[4].dat->index]->d_im[d];
  #else
  for (int d = 0; d < dim; d++) d_m[d] = args[4].dat->d_m[d];
  #endif
  int base4 = dat4 * 1 *
  (start[0] * args[4].stencil->stride[0] - args[4].dat->base[0] - d_m[0]);
  base4 = base4+ dat4 *
    args[4].dat->size[0] *
    (start[1] * args[4].stencil->stride[1] - args[4].dat->base[1] - d_m[1]);
  base4 = base4+ dat4 *
    args[4].dat->size[0] *
    args[4].dat->size[1] *
    (start[2] * args[4].stencil->stride[2] - args[4].dat->base[2] - d_m[2]);
  p_a[4] = (char *)args[4].data_d + base4;


  ops_H_D_exchanges_device(args, 5);
  ops_halo_exchanges(args,5,range);

  if (OPS_diags > 1) {
    ops_timers_core(&c2,&t2);
    OPS_kernels[21].mpi_time += t2-t1;
  }


  //call kernel wrapper function, passing in pointers to data
  ops_advec_mom_kernel_x2<<<grid, tblock >>> (  (double *)p_a[0], (double *)p_a[1],
           (double *)p_a[2], (double *)p_a[3],
           (double *)p_a[4],x_size, y_size, z_size);

  if (OPS_diags>1) {
    cutilSafeCall(cudaDeviceSynchronize());
    ops_timers_core(&c1,&t1);
    OPS_kernels[21].time += t1-t2;
  }

  ops_set_dirtybit_device(args, 5);
  ops_set_halo_dirtybit3(&args[0],range);
  ops_set_halo_dirtybit3(&args[1],range);

  if (OPS_diags > 1) {
    //Update kernel record
    ops_timers_core(&c2,&t2);
    OPS_kernels[21].mpi_time += t2-t1;
    OPS_kernels[21].transfer += ops_compute_transfer(dim, start, end, &arg0);
    OPS_kernels[21].transfer += ops_compute_transfer(dim, start, end, &arg1);
    OPS_kernels[21].transfer += ops_compute_transfer(dim, start, end, &arg2);
    OPS_kernels[21].transfer += ops_compute_transfer(dim, start, end, &arg3);
    OPS_kernels[21].transfer += ops_compute_transfer(dim, start, end, &arg4);
  }
}
