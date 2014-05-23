//
// auto-generated by ops.py on 2014-05-23 10:05
//

int xdim0_advec_cell_kernel4_xdir;
int xdim1_advec_cell_kernel4_xdir;
int xdim2_advec_cell_kernel4_xdir;
int xdim3_advec_cell_kernel4_xdir;
int xdim4_advec_cell_kernel4_xdir;
int xdim5_advec_cell_kernel4_xdir;
int xdim6_advec_cell_kernel4_xdir;
int xdim7_advec_cell_kernel4_xdir;
int xdim8_advec_cell_kernel4_xdir;
int xdim9_advec_cell_kernel4_xdir;
int xdim10_advec_cell_kernel4_xdir;

#define OPS_ACC0(x,y) (x+xdim0_advec_cell_kernel4_xdir*(y))
#define OPS_ACC1(x,y) (x+xdim1_advec_cell_kernel4_xdir*(y))
#define OPS_ACC2(x,y) (x+xdim2_advec_cell_kernel4_xdir*(y))
#define OPS_ACC3(x,y) (x+xdim3_advec_cell_kernel4_xdir*(y))
#define OPS_ACC4(x,y) (x+xdim4_advec_cell_kernel4_xdir*(y))
#define OPS_ACC5(x,y) (x+xdim5_advec_cell_kernel4_xdir*(y))
#define OPS_ACC6(x,y) (x+xdim6_advec_cell_kernel4_xdir*(y))
#define OPS_ACC7(x,y) (x+xdim7_advec_cell_kernel4_xdir*(y))
#define OPS_ACC8(x,y) (x+xdim8_advec_cell_kernel4_xdir*(y))
#define OPS_ACC9(x,y) (x+xdim9_advec_cell_kernel4_xdir*(y))
#define OPS_ACC10(x,y) (x+xdim10_advec_cell_kernel4_xdir*(y))

//user function

inline void advec_cell_kernel4_xdir( double *density1, double *energy1,
                         const double *mass_flux_x, const double *vol_flux_x,
                         const double *pre_vol, const double *post_vol,
                         double *pre_mass, double *post_mass,
                         double *advec_vol, double *post_ener,
                         const double *ener_flux) {

  pre_mass[OPS_ACC6(0,0)] = density1[OPS_ACC0(0,0)] * pre_vol[OPS_ACC4(0,0)];
  post_mass[OPS_ACC7(0,0)] = pre_mass[OPS_ACC6(0,0)] + mass_flux_x[OPS_ACC2(0,0)] - mass_flux_x[OPS_ACC2(1,0)];
  post_ener[OPS_ACC9(0,0)] = ( energy1[OPS_ACC1(0,0)] * pre_mass[OPS_ACC6(0,0)] + ener_flux[OPS_ACC10(0,0)] - ener_flux[OPS_ACC10(1,0)])/post_mass[OPS_ACC7(0,0)];
  advec_vol[OPS_ACC8(0,0)] = pre_vol[OPS_ACC4(0,0)] + vol_flux_x[OPS_ACC3(0,0)] - vol_flux_x[OPS_ACC3(1,0)];
  density1[OPS_ACC0(0,0)] = post_mass[OPS_ACC7(0,0)]/advec_vol[OPS_ACC8(0,0)];
  energy1[OPS_ACC1(0,0)] = post_ener[OPS_ACC9(0,0)];

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



// host stub function
void ops_par_loop_advec_cell_kernel4_xdir(char const *name, ops_block Block, int dim, int* range,
 ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3, ops_arg arg4, ops_arg arg5, ops_arg arg6, ops_arg arg7, ops_arg arg8, ops_arg arg9, ops_arg arg10) {

  ops_arg args[11] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10};

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
  ops_timing_realloc(10,"advec_cell_kernel4_xdir");
  ops_timers_core(&c2,&t2);

  if (OPS_kernels[10].count == 0) {
    xdim0_advec_cell_kernel4_xdir = args[0].dat->block_size[0]*args[0].dat->dim;
    xdim1_advec_cell_kernel4_xdir = args[1].dat->block_size[0]*args[1].dat->dim;
    xdim2_advec_cell_kernel4_xdir = args[2].dat->block_size[0]*args[2].dat->dim;
    xdim3_advec_cell_kernel4_xdir = args[3].dat->block_size[0]*args[3].dat->dim;
    xdim4_advec_cell_kernel4_xdir = args[4].dat->block_size[0]*args[4].dat->dim;
    xdim5_advec_cell_kernel4_xdir = args[5].dat->block_size[0]*args[5].dat->dim;
    xdim6_advec_cell_kernel4_xdir = args[6].dat->block_size[0]*args[6].dat->dim;
    xdim7_advec_cell_kernel4_xdir = args[7].dat->block_size[0]*args[7].dat->dim;
    xdim8_advec_cell_kernel4_xdir = args[8].dat->block_size[0]*args[8].dat->dim;
    xdim9_advec_cell_kernel4_xdir = args[9].dat->block_size[0]*args[9].dat->dim;
    xdim10_advec_cell_kernel4_xdir = args[10].dat->block_size[0]*args[10].dat->dim;
  }

  int dat0 = args[0].dat->size;
  int dat1 = args[1].dat->size;
  int dat2 = args[2].dat->size;
  int dat3 = args[3].dat->size;
  int dat4 = args[4].dat->size;
  int dat5 = args[5].dat->size;
  int dat6 = args[6].dat->size;
  int dat7 = args[7].dat->size;
  int dat8 = args[8].dat->size;
  int dat9 = args[9].dat->size;
  int dat10 = args[10].dat->size;


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

  //set up initial pointers
  int base2 = dat2 * 1 * 
  (start_add[0] * args[2].stencil->stride[0] - args[2].dat->offset[0]);
  base2 = base2  + dat2 * args[2].dat->block_size[0] * 
  (start_add[1] * args[2].stencil->stride[1] - args[2].dat->offset[1]);
  #ifdef OPS_GPU
  double *p_a2 = (double *)((char *)args[2].data_d + base2);
  #else
  double *p_a2 = (double *)((char *)args[2].data + base2);
  #endif

  //set up initial pointers
  int base3 = dat3 * 1 * 
  (start_add[0] * args[3].stencil->stride[0] - args[3].dat->offset[0]);
  base3 = base3  + dat3 * args[3].dat->block_size[0] * 
  (start_add[1] * args[3].stencil->stride[1] - args[3].dat->offset[1]);
  #ifdef OPS_GPU
  double *p_a3 = (double *)((char *)args[3].data_d + base3);
  #else
  double *p_a3 = (double *)((char *)args[3].data + base3);
  #endif

  //set up initial pointers
  int base4 = dat4 * 1 * 
  (start_add[0] * args[4].stencil->stride[0] - args[4].dat->offset[0]);
  base4 = base4  + dat4 * args[4].dat->block_size[0] * 
  (start_add[1] * args[4].stencil->stride[1] - args[4].dat->offset[1]);
  #ifdef OPS_GPU
  double *p_a4 = (double *)((char *)args[4].data_d + base4);
  #else
  double *p_a4 = (double *)((char *)args[4].data + base4);
  #endif

  //set up initial pointers
  int base5 = dat5 * 1 * 
  (start_add[0] * args[5].stencil->stride[0] - args[5].dat->offset[0]);
  base5 = base5  + dat5 * args[5].dat->block_size[0] * 
  (start_add[1] * args[5].stencil->stride[1] - args[5].dat->offset[1]);
  #ifdef OPS_GPU
  double *p_a5 = (double *)((char *)args[5].data_d + base5);
  #else
  double *p_a5 = (double *)((char *)args[5].data + base5);
  #endif

  //set up initial pointers
  int base6 = dat6 * 1 * 
  (start_add[0] * args[6].stencil->stride[0] - args[6].dat->offset[0]);
  base6 = base6  + dat6 * args[6].dat->block_size[0] * 
  (start_add[1] * args[6].stencil->stride[1] - args[6].dat->offset[1]);
  #ifdef OPS_GPU
  double *p_a6 = (double *)((char *)args[6].data_d + base6);
  #else
  double *p_a6 = (double *)((char *)args[6].data + base6);
  #endif

  //set up initial pointers
  int base7 = dat7 * 1 * 
  (start_add[0] * args[7].stencil->stride[0] - args[7].dat->offset[0]);
  base7 = base7  + dat7 * args[7].dat->block_size[0] * 
  (start_add[1] * args[7].stencil->stride[1] - args[7].dat->offset[1]);
  #ifdef OPS_GPU
  double *p_a7 = (double *)((char *)args[7].data_d + base7);
  #else
  double *p_a7 = (double *)((char *)args[7].data + base7);
  #endif

  //set up initial pointers
  int base8 = dat8 * 1 * 
  (start_add[0] * args[8].stencil->stride[0] - args[8].dat->offset[0]);
  base8 = base8  + dat8 * args[8].dat->block_size[0] * 
  (start_add[1] * args[8].stencil->stride[1] - args[8].dat->offset[1]);
  #ifdef OPS_GPU
  double *p_a8 = (double *)((char *)args[8].data_d + base8);
  #else
  double *p_a8 = (double *)((char *)args[8].data + base8);
  #endif

  //set up initial pointers
  int base9 = dat9 * 1 * 
  (start_add[0] * args[9].stencil->stride[0] - args[9].dat->offset[0]);
  base9 = base9  + dat9 * args[9].dat->block_size[0] * 
  (start_add[1] * args[9].stencil->stride[1] - args[9].dat->offset[1]);
  #ifdef OPS_GPU
  double *p_a9 = (double *)((char *)args[9].data_d + base9);
  #else
  double *p_a9 = (double *)((char *)args[9].data + base9);
  #endif

  //set up initial pointers
  int base10 = dat10 * 1 * 
  (start_add[0] * args[10].stencil->stride[0] - args[10].dat->offset[0]);
  base10 = base10  + dat10 * args[10].dat->block_size[0] * 
  (start_add[1] * args[10].stencil->stride[1] - args[10].dat->offset[1]);
  #ifdef OPS_GPU
  double *p_a10 = (double *)((char *)args[10].data_d + base10);
  #else
  double *p_a10 = (double *)((char *)args[10].data + base10);
  #endif


  #ifdef OPS_GPU
  ops_H_D_exchanges_cuda(args, 11);
  #else
  ops_H_D_exchanges(args, 11);
  #endif
  ops_halo_exchanges(args,11,range);

  ops_timers_core(&c1,&t1);
  OPS_kernels[10].mpi_time += t1-t2;

  #ifdef OPS_GPU
  #pragma acc parallel deviceptr(p_a0,p_a1,p_a2,p_a3,p_a4,p_a5,p_a6,p_a7,p_a8,p_a9,p_a10)
  #pragma acc loop
  #endif
  for ( int n_y=0; n_y<y_size; n_y++ ){
    #ifdef OPS_GPU
    #pragma acc loop
    #endif
    for ( int n_x=0; n_x<x_size; n_x++ ){
      advec_cell_kernel4_xdir(  p_a0+ n_x*1+n_y*xdim0_advec_cell_kernel4_xdir*1, p_a1+ n_x*1+n_y*xdim1_advec_cell_kernel4_xdir*1,
           p_a2+ n_x*1+n_y*xdim2_advec_cell_kernel4_xdir*1, p_a3+ n_x*1+n_y*xdim3_advec_cell_kernel4_xdir*1,
           p_a4+ n_x*1+n_y*xdim4_advec_cell_kernel4_xdir*1, p_a5+ n_x*1+n_y*xdim5_advec_cell_kernel4_xdir*1,
           p_a6+ n_x*1+n_y*xdim6_advec_cell_kernel4_xdir*1, p_a7+ n_x*1+n_y*xdim7_advec_cell_kernel4_xdir*1,
           p_a8+ n_x*1+n_y*xdim8_advec_cell_kernel4_xdir*1, p_a9+ n_x*1+n_y*xdim9_advec_cell_kernel4_xdir*1,
           p_a10+ n_x*1+n_y*xdim10_advec_cell_kernel4_xdir*1 );

    }
  }


  ops_timers_core(&c2,&t2);
  OPS_kernels[10].time += t2-t1;
  #ifdef OPS_GPU
  ops_set_dirtybit_cuda(args, 11);
  #else
  ops_set_dirtybit_host(args, 11);
  #endif
  ops_set_halo_dirtybit3(&args[0],range);
  ops_set_halo_dirtybit3(&args[1],range);
  ops_set_halo_dirtybit3(&args[6],range);
  ops_set_halo_dirtybit3(&args[7],range);
  ops_set_halo_dirtybit3(&args[8],range);
  ops_set_halo_dirtybit3(&args[9],range);

  //Update kernel record
  OPS_kernels[10].count++;
  OPS_kernels[10].transfer += ops_compute_transfer(dim, range, &arg0);
  OPS_kernels[10].transfer += ops_compute_transfer(dim, range, &arg1);
  OPS_kernels[10].transfer += ops_compute_transfer(dim, range, &arg2);
  OPS_kernels[10].transfer += ops_compute_transfer(dim, range, &arg3);
  OPS_kernels[10].transfer += ops_compute_transfer(dim, range, &arg4);
  OPS_kernels[10].transfer += ops_compute_transfer(dim, range, &arg5);
  OPS_kernels[10].transfer += ops_compute_transfer(dim, range, &arg6);
  OPS_kernels[10].transfer += ops_compute_transfer(dim, range, &arg7);
  OPS_kernels[10].transfer += ops_compute_transfer(dim, range, &arg8);
  OPS_kernels[10].transfer += ops_compute_transfer(dim, range, &arg9);
  OPS_kernels[10].transfer += ops_compute_transfer(dim, range, &arg10);
}
