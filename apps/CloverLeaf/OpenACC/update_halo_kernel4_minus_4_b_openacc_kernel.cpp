//
// auto-generated by ops.py on 2014-05-23 10:05
//

int xdim0_update_halo_kernel4_minus_4_b;
int xdim1_update_halo_kernel4_minus_4_b;

#define OPS_ACC0(x,y) (x+xdim0_update_halo_kernel4_minus_4_b*(y))
#define OPS_ACC1(x,y) (x+xdim1_update_halo_kernel4_minus_4_b*(y))

//user function

inline void update_halo_kernel4_minus_4_b(double *vol_flux_y, double *mass_flux_y, const int* fields) {
  if(fields[FIELD_VOL_FLUX_Y] == 1) vol_flux_y[OPS_ACC0(0,0)] = -(vol_flux_y[OPS_ACC0(0,-4)]);
  if(fields[FIELD_MASS_FLUX_Y] == 1) mass_flux_y[OPS_ACC1(0,0)] = -(mass_flux_y[OPS_ACC1(0,-4)]);
}



#undef OPS_ACC0
#undef OPS_ACC1



// host stub function
void ops_par_loop_update_halo_kernel4_minus_4_b(char const *name, ops_block Block, int dim, int* range,
 ops_arg arg0, ops_arg arg1, ops_arg arg2) {

  ops_arg args[3] = { arg0, arg1, arg2};

  sub_block_list sb = OPS_sub_block_list[Block->index];
  //compute localy allocated range for the sub-block
  int start_add[2];
  int end_add[2];
  for ( int n=0; n<2; n++ ){
    start_add[n] = sb->istart[n];end_add[n] = sb->iend[n]+1;
    if (start_add[n] >= range[2*n]) {
      start_add[n] = 0;
    }
    else {
      start_add[n] = range[2*n] - start_add[n];
    }
    if (end_add[n] >= range[2*n+1]) {
      end_add[n] = range[2*n+1] - sb->istart[n];
    }
    else {
      end_add[n] = sb->sizes[n];
    }
  }


  int x_size = MAX(0,end_add[0]-start_add[0]);
  int y_size = MAX(0,end_add[1]-start_add[1]);


  //Timing
  double t1,t2,c1,c2;
  ops_timing_realloc(75,"update_halo_kernel4_minus_4_b");
  ops_timers_core(&c2,&t2);

  if (OPS_kernels[75].count == 0) {
    xdim0_update_halo_kernel4_minus_4_b = args[0].dat->block_size[0]*args[0].dat->dim;
    xdim1_update_halo_kernel4_minus_4_b = args[1].dat->block_size[0]*args[1].dat->dim;
  }

  int dat0 = args[0].dat->size;
  int dat1 = args[1].dat->size;

  int *arg2h = (int *)args[2].data;
  //Upload large globals
  int consts_bytes = 0;
  consts_bytes += ROUND_UP(NUM_FIELDS*sizeof(int));
  reallocConstArrays(consts_bytes);
  consts_bytes = 0;
  args[2].data = OPS_consts_h + consts_bytes;
  args[2].data_d = OPS_consts_d + consts_bytes;
  for (int d=0; d<NUM_FIELDS; d++) ((int *)args[2].data)[d] = arg2h[d];
  consts_bytes += ROUND_UP(NUM_FIELDS*sizeof(int));
  mvConstArraysToDevice(consts_bytes);

  //set up initial pointers
  int base0 = dat0 * 1 * 
  (start_add[0] * args[0].stencil->stride[0] - args[0].dat->offset[0]);
  base0 = base0  + dat0 * args[0].dat->block_size[0] * 
  (start_add[1] * args[0].stencil->stride[1] - args[0].dat->offset[1]);
  #ifdef OPS_GPU
  double *p_a0 = (double *)((char *)args[0].data_d + base0);
  #else
  double *p_a0 = (double *)((char *)args[0].data + base0);
  #endif

  //set up initial pointers
  int base1 = dat1 * 1 * 
  (start_add[0] * args[1].stencil->stride[0] - args[1].dat->offset[0]);
  base1 = base1  + dat1 * args[1].dat->block_size[0] * 
  (start_add[1] * args[1].stencil->stride[1] - args[1].dat->offset[1]);
  #ifdef OPS_GPU
  double *p_a1 = (double *)((char *)args[1].data_d + base1);
  #else
  double *p_a1 = (double *)((char *)args[1].data + base1);
  #endif

  #ifdef OPS_GPU
  int *p_a2 = (int *)args[2].data_d;
  #else
  int *p_a2 = arg2h;
  #endif


  #ifdef OPS_GPU
  ops_H_D_exchanges_cuda(args, 3);
  #else
  ops_H_D_exchanges(args, 3);
  #endif
  ops_halo_exchanges(args,3,range);

  ops_timers_core(&c1,&t1);
  OPS_kernels[75].mpi_time += t1-t2;

  #ifdef OPS_GPU
  #pragma acc parallel deviceptr(p_a0,p_a1,p_a2)
  #pragma acc loop
  #endif
  for ( int n_y=0; n_y<y_size; n_y++ ){
    #ifdef OPS_GPU
    #pragma acc loop
    #endif
    for ( int n_x=0; n_x<x_size; n_x++ ){
      update_halo_kernel4_minus_4_b(  p_a0+ n_x*1+n_y*xdim0_update_halo_kernel4_minus_4_b*1, p_a1+ n_x*1+n_y*xdim1_update_halo_kernel4_minus_4_b*1,
           p_a2 );

    }
  }


  ops_timers_core(&c2,&t2);
  OPS_kernels[75].time += t2-t1;
  ops_timers_core(&c1,&t1);
  OPS_kernels[75].mpi_time += t1-t2;
  #ifdef OPS_GPU
  ops_set_dirtybit_cuda(args, 3);
  #else
  ops_set_dirtybit_host(args, 3);
  #endif
  ops_set_halo_dirtybit3(&args[0],range);
  ops_set_halo_dirtybit3(&args[1],range);

  //Update kernel record
  OPS_kernels[75].count++;
  OPS_kernels[75].transfer += ops_compute_transfer(dim, range, &arg0);
  OPS_kernels[75].transfer += ops_compute_transfer(dim, range, &arg1);
}
