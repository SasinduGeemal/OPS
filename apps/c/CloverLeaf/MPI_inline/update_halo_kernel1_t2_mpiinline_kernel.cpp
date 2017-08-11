//
// auto-generated by ops.py
//
#include "./MPI_inline/clover_leaf_common.h"

extern int xdim0_update_halo_kernel1_t2;
int xdim0_update_halo_kernel1_t2_h = -1;
extern int xdim1_update_halo_kernel1_t2;
int xdim1_update_halo_kernel1_t2_h = -1;
extern int xdim2_update_halo_kernel1_t2;
int xdim2_update_halo_kernel1_t2_h = -1;
extern int xdim3_update_halo_kernel1_t2;
int xdim3_update_halo_kernel1_t2_h = -1;
extern int xdim4_update_halo_kernel1_t2;
int xdim4_update_halo_kernel1_t2_h = -1;
extern int xdim5_update_halo_kernel1_t2;
int xdim5_update_halo_kernel1_t2_h = -1;
extern int xdim6_update_halo_kernel1_t2;
int xdim6_update_halo_kernel1_t2_h = -1;

#ifdef __cplusplus
extern "C" {
#endif
void update_halo_kernel1_t2_c_wrapper(
  double *p_a0,
  double *p_a1,
  double *p_a2,
  double *p_a3,
  double *p_a4,
  double *p_a5,
  double *p_a6,
  int *p_a7,
  int x_size, int y_size);

#ifdef __cplusplus
}
#endif

// host stub function
void ops_par_loop_update_halo_kernel1_t2(char const *name, ops_block block, int dim, int* range,
 ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3, ops_arg arg4, ops_arg arg5, ops_arg arg6, ops_arg arg7) {

  ops_arg args[8] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_before(args,8,range,45)) return;
  #endif

  ops_timing_realloc(45,"update_halo_kernel1_t2");
  OPS_kernels[45].count++;

  //compute localy allocated range for the sub-block
  int start[2];
  int end[2];
  #ifdef OPS_MPI
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


  xdim0 = args[0].dat->size[0];
  xdim1 = args[1].dat->size[0];
  xdim2 = args[2].dat->size[0];
  xdim3 = args[3].dat->size[0];
  xdim4 = args[4].dat->size[0];
  xdim5 = args[5].dat->size[0];
  xdim6 = args[6].dat->size[0];

  //Timing
  double t1,t2,c1,c2;
  ops_timers_core(&c2,&t2);

  if (xdim0 != xdim0_update_halo_kernel1_t2_h || xdim1 != xdim1_update_halo_kernel1_t2_h || xdim2 != xdim2_update_halo_kernel1_t2_h || xdim3 != xdim3_update_halo_kernel1_t2_h || xdim4 != xdim4_update_halo_kernel1_t2_h || xdim5 != xdim5_update_halo_kernel1_t2_h || xdim6 != xdim6_update_halo_kernel1_t2_h) {
    xdim0_update_halo_kernel1_t2 = xdim0;
    xdim0_update_halo_kernel1_t2_h = xdim0;
    xdim1_update_halo_kernel1_t2 = xdim1;
    xdim1_update_halo_kernel1_t2_h = xdim1;
    xdim2_update_halo_kernel1_t2 = xdim2;
    xdim2_update_halo_kernel1_t2_h = xdim2;
    xdim3_update_halo_kernel1_t2 = xdim3;
    xdim3_update_halo_kernel1_t2_h = xdim3;
    xdim4_update_halo_kernel1_t2 = xdim4;
    xdim4_update_halo_kernel1_t2_h = xdim4;
    xdim5_update_halo_kernel1_t2 = xdim5;
    xdim5_update_halo_kernel1_t2_h = xdim5;
    xdim6_update_halo_kernel1_t2 = xdim6;
    xdim6_update_halo_kernel1_t2_h = xdim6;
  }

  int dat0 = args[0].dat->elem_size;
  int dat1 = args[1].dat->elem_size;
  int dat2 = args[2].dat->elem_size;
  int dat3 = args[3].dat->elem_size;
  int dat4 = args[4].dat->elem_size;
  int dat5 = args[5].dat->elem_size;
  int dat6 = args[6].dat->elem_size;


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
  double *p_a0 = (double *)((char *)args[0].data + base0);

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
  double *p_a1 = (double *)((char *)args[1].data + base1);

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
  double *p_a2 = (double *)((char *)args[2].data + base2);

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
  double *p_a3 = (double *)((char *)args[3].data + base3);

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
  double *p_a4 = (double *)((char *)args[4].data + base4);

  #ifdef OPS_MPI
  for (int d = 0; d < dim; d++) d_m[d] = args[5].dat->d_m[d] + OPS_sub_dat_list[args[5].dat->index]->d_im[d];
  #else
  for (int d = 0; d < dim; d++) d_m[d] = args[5].dat->d_m[d];
  #endif
  int base5 = dat5 * 1 * 
    (start[0] * args[5].stencil->stride[0] - args[5].dat->base[0] - d_m[0]);
  base5 = base5+ dat5 *
    args[5].dat->size[0] *
    (start[1] * args[5].stencil->stride[1] - args[5].dat->base[1] - d_m[1]);
  double *p_a5 = (double *)((char *)args[5].data + base5);

  #ifdef OPS_MPI
  for (int d = 0; d < dim; d++) d_m[d] = args[6].dat->d_m[d] + OPS_sub_dat_list[args[6].dat->index]->d_im[d];
  #else
  for (int d = 0; d < dim; d++) d_m[d] = args[6].dat->d_m[d];
  #endif
  int base6 = dat6 * 1 * 
    (start[0] * args[6].stencil->stride[0] - args[6].dat->base[0] - d_m[0]);
  base6 = base6+ dat6 *
    args[6].dat->size[0] *
    (start[1] * args[6].stencil->stride[1] - args[6].dat->base[1] - d_m[1]);
  double *p_a6 = (double *)((char *)args[6].data + base6);

  int *p_a7 = (int *)args[7].data;

  ops_H_D_exchanges_host(args, 8);
  ops_halo_exchanges(args,8,range);

  ops_timers_core(&c1,&t1);
  OPS_kernels[45].mpi_time += t1-t2;

  update_halo_kernel1_t2_c_wrapper(
    p_a0,
    p_a1,
    p_a2,
    p_a3,
    p_a4,
    p_a5,
    p_a6,
    p_a7,
    x_size, y_size);

  ops_timers_core(&c2,&t2);
  OPS_kernels[45].time += t2-t1;
  ops_set_dirtybit_host(args, 8);
  ops_set_halo_dirtybit3(&args[0],range);
  ops_set_halo_dirtybit3(&args[1],range);
  ops_set_halo_dirtybit3(&args[2],range);
  ops_set_halo_dirtybit3(&args[3],range);
  ops_set_halo_dirtybit3(&args[4],range);
  ops_set_halo_dirtybit3(&args[5],range);
  ops_set_halo_dirtybit3(&args[6],range);

  //Update kernel record
  OPS_kernels[45].transfer += ops_compute_transfer(dim, start, end, &arg0);
  OPS_kernels[45].transfer += ops_compute_transfer(dim, start, end, &arg1);
  OPS_kernels[45].transfer += ops_compute_transfer(dim, start, end, &arg2);
  OPS_kernels[45].transfer += ops_compute_transfer(dim, start, end, &arg3);
  OPS_kernels[45].transfer += ops_compute_transfer(dim, start, end, &arg4);
  OPS_kernels[45].transfer += ops_compute_transfer(dim, start, end, &arg5);
  OPS_kernels[45].transfer += ops_compute_transfer(dim, start, end, &arg6);
}
