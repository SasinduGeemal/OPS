//
// auto-generated by ops.py
//

#define OPS_GPU

extern int xdim0_initialise_chunk_kernel_yy;
int xdim0_initialise_chunk_kernel_yy_h = -1;
extern int ydim0_initialise_chunk_kernel_yy;
int ydim0_initialise_chunk_kernel_yy_h = -1;

#ifdef __cplusplus
extern "C" {
#endif
void initialise_chunk_kernel_yy_c_wrapper(
  int *p_a0,
  int *p_a1,
  int arg_idx0, int arg_idx1, int arg_idx2,
  int x_size, int y_size, int z_size);

#ifdef __cplusplus
}
#endif

// host stub function
void ops_par_loop_initialise_chunk_kernel_yy(char const *name, ops_block block, int dim, int* range,
 ops_arg arg0, ops_arg arg1) {

  //Timing
  double t1,t2,c1,c2;
  ops_arg args[2] = { arg0, arg1};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_before(args, 2, range, 1))
    return;
  #endif

  if (OPS_diags > 1) {
    ops_timing_realloc(1, "initialise_chunk_kernel_yy");
    OPS_kernels[1].count++;
    ops_timers_core(&c1,&t1);
  }

  //compute localy allocated range for the sub-block

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

  int arg_idx[3];
  #ifdef OPS_MPI
  arg_idx[0] = sb->decomp_disp[0]+start[0];
  arg_idx[1] = sb->decomp_disp[1]+start[1];
  arg_idx[2] = sb->decomp_disp[2]+start[2];
  #else
  arg_idx[0] = start[0];
  arg_idx[1] = start[1];
  arg_idx[2] = start[2];
  #endif

  xdim0 = args[0].dat->size[0];
  ydim0 = args[0].dat->size[1];
  if (xdim0 != xdim0_initialise_chunk_kernel_yy_h || ydim0 != ydim0_initialise_chunk_kernel_yy_h) {
    xdim0_initialise_chunk_kernel_yy = xdim0;
    xdim0_initialise_chunk_kernel_yy_h = xdim0;
    ydim0_initialise_chunk_kernel_yy = ydim0;
    ydim0_initialise_chunk_kernel_yy_h = ydim0;
  }



  //set up initial pointers
  int base0 = args[0].dat->base_offset + (OPS_soa ? args[0].dat->type_size : args[0].dat->elem_size) * start[0] * args[0].stencil->stride[0];
  base0 = base0 + (OPS_soa ? args[0].dat->type_size : args[0].dat->elem_size) *
    args[0].dat->size[0] *
    start[1] * args[0].stencil->stride[1];
  base0 = base0 + (OPS_soa ? args[0].dat->type_size : args[0].dat->elem_size) *
    args[0].dat->size[0] *
    args[0].dat->size[1] *
    start[2] * args[0].stencil->stride[2];
  #ifdef OPS_GPU
  int *p_a0 = (int *)((char *)args[0].data_d + base0);
  #else
  int *p_a0 = (int *)((char *)args[0].data + base0);
  #endif

  int *p_a1 = NULL;


  #ifdef OPS_GPU
  ops_H_D_exchanges_device(args, 2);
  #else
  ops_H_D_exchanges_host(args, 2);
  #endif
  ops_halo_exchanges(args,2,range);

  #ifdef OPS_GPU
  ops_H_D_exchanges_device(args, 2);
  #else
  ops_H_D_exchanges_host(args, 2);
  #endif
  if (OPS_diags > 1) {
    ops_timers_core(&c2,&t2);
    OPS_kernels[1].mpi_time += t2 - t1;
  }

  initialise_chunk_kernel_yy_c_wrapper(
    p_a0,
    p_a1,
    arg_idx[0], arg_idx[1], arg_idx[2],
    x_size, y_size, z_size);

  if (OPS_diags > 1) {
    ops_timers_core(&c1,&t1);
    OPS_kernels[1].time += t1 - t2;
  }
  #ifdef OPS_GPU
  ops_set_dirtybit_device(args, 2);
  #else
  ops_set_dirtybit_host(args, 2);
  #endif
  ops_set_halo_dirtybit3(&args[0],range);

  if (OPS_diags > 1) {
    //Update kernel record
    ops_timers_core(&c2,&t2);
    OPS_kernels[1].mpi_time += t2 - t1;
    OPS_kernels[1].transfer += ops_compute_transfer(dim, start, end, &arg0);
  }
}
