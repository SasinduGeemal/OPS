//
// auto-generated by ops.py on 2013-11-27 11:54
//

#include "ops_lib_openmp.h"
#ifdef _OPENMP
#include <omp.h>
#endif
//user function
#include "field_summary_kernel.h"

// host stub function
void ops_par_loop_field_summary_kernel(char const *name, int dim, int* range,
 ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
 ops_arg arg4, ops_arg arg5, ops_arg arg6, ops_arg arg7,
 ops_arg arg8, ops_arg arg9, ops_arg arg10) {

  int  offs[11][2];
  ops_arg args[11] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10};

  offs[0][0] = 1;  //unit step in x dimension
  offs[0][1] = ops_offs_set(range[0],range[2]+1, args[0]) - ops_offs_set(range[1],range[2], args[0]);
  if (args[0].stencil->stride[0] == 0) {
    offs[0][0] = 0;
    offs[0][1] = args[0].dat->block_size[0];
  }
  //stride in x as y stride is 0
  else if (args[0].stencil->stride[1] == 0) {
    offs[0][0] = 1;
    offs[0][1] = -( range[1] - range[0] );
  }

  offs[1][0] = 1;  //unit step in x dimension
  offs[1][1] = ops_offs_set(range[0],range[2]+1, args[1]) - ops_offs_set(range[1],range[2], args[1]);
  if (args[1].stencil->stride[0] == 0) {
    offs[1][0] = 0;
    offs[1][1] = args[1].dat->block_size[0];
  }
  //stride in x as y stride is 0
  else if (args[1].stencil->stride[1] == 0) {
    offs[1][0] = 1;
    offs[1][1] = -( range[1] - range[0] );
  }

  offs[2][0] = 1;  //unit step in x dimension
  offs[2][1] = ops_offs_set(range[0],range[2]+1, args[2]) - ops_offs_set(range[1],range[2], args[2]);
  if (args[2].stencil->stride[0] == 0) {
    offs[2][0] = 0;
    offs[2][1] = args[2].dat->block_size[0];
  }
  //stride in x as y stride is 0
  else if (args[2].stencil->stride[1] == 0) {
    offs[2][0] = 1;
    offs[2][1] = -( range[1] - range[0] );
  }

  offs[3][0] = 1;  //unit step in x dimension
  offs[3][1] = ops_offs_set(range[0],range[2]+1, args[3]) - ops_offs_set(range[1],range[2], args[3]);
  if (args[3].stencil->stride[0] == 0) {
    offs[3][0] = 0;
    offs[3][1] = args[3].dat->block_size[0];
  }
  //stride in x as y stride is 0
  else if (args[3].stencil->stride[1] == 0) {
    offs[3][0] = 1;
    offs[3][1] = -( range[1] - range[0] );
  }

  offs[4][0] = 1;  //unit step in x dimension
  offs[4][1] = ops_offs_set(range[0],range[2]+1, args[4]) - ops_offs_set(range[1],range[2], args[4]);
  if (args[4].stencil->stride[0] == 0) {
    offs[4][0] = 0;
    offs[4][1] = args[4].dat->block_size[0];
  }
  //stride in x as y stride is 0
  else if (args[4].stencil->stride[1] == 0) {
    offs[4][0] = 1;
    offs[4][1] = -( range[1] - range[0] );
  }

  offs[5][0] = 1;  //unit step in x dimension
  offs[5][1] = ops_offs_set(range[0],range[2]+1, args[5]) - ops_offs_set(range[1],range[2], args[5]);
  if (args[5].stencil->stride[0] == 0) {
    offs[5][0] = 0;
    offs[5][1] = args[5].dat->block_size[0];
  }
  //stride in x as y stride is 0
  else if (args[5].stencil->stride[1] == 0) {
    offs[5][0] = 1;
    offs[5][1] = -( range[1] - range[0] );
  }

  int off0_1 = offs[0][0];
  int off0_2 = offs[0][1];
  int dat0 = args[0].dat->size;
  int off1_1 = offs[1][0];
  int off1_2 = offs[1][1];
  int dat1 = args[1].dat->size;
  int off2_1 = offs[2][0];
  int off2_2 = offs[2][1];
  int dat2 = args[2].dat->size;
  int off3_1 = offs[3][0];
  int off3_2 = offs[3][1];
  int dat3 = args[3].dat->size;
  int off4_1 = offs[4][0];
  int off4_2 = offs[4][1];
  int dat4 = args[4].dat->size;
  int off5_1 = offs[5][0];
  int off5_2 = offs[5][1];
  int dat5 = args[5].dat->size;

  double*arg6h = (double *)arg6.data;
  double*arg7h = (double *)arg7.data;
  double*arg8h = (double *)arg8.data;
  double*arg9h = (double *)arg9.data;
  double*arg10h = (double *)arg10.data;

  #ifdef _OPENMP
  int nthreads = omp_get_max_threads( );
  #else
  int nthreads = 1;
  #endif

  //allocate and initialise arrays for global reduction
  //assumes a max of 64 threads with a cacche line size of 64 bytes
  double arg_gbl6[1 * 64 * 64];
  double arg_gbl7[1 * 64 * 64];
  double arg_gbl8[1 * 64 * 64];
  double arg_gbl9[1 * 64 * 64];
  double arg_gbl10[1 * 64 * 64];
  for ( int thr=0; thr<nthreads; thr++ ){
    for ( int d=0; d<1; d++ ){
      arg_gbl6[d+64*thr] = ZERO_double;
    }
    for ( int d=0; d<1; d++ ){
      arg_gbl7[d+64*thr] = ZERO_double;
    }
    for ( int d=0; d<1; d++ ){
      arg_gbl8[d+64*thr] = ZERO_double;
    }
    for ( int d=0; d<1; d++ ){
      arg_gbl9[d+64*thr] = ZERO_double;
    }
    for ( int d=0; d<1; d++ ){
      arg_gbl10[d+64*thr] = ZERO_double;
    }
  }

  int y_size = range[3]-range[2];

  xdim0 = args[0].dat->block_size[0];
  xdim1 = args[1].dat->block_size[0];
  xdim2 = args[2].dat->block_size[0];
  xdim3 = args[3].dat->block_size[0];
  xdim4 = args[4].dat->block_size[0];
  xdim5 = args[5].dat->block_size[0];


  //Timing
  double t1,t2,c1,c2;
  ops_timing_realloc(4,"field_summary_kernel");
  ops_timers_core(&c1,&t1);

  ops_halo_exchanges(args, 11);

  #pragma omp parallel for
  for ( int thr=0; thr<nthreads; thr++ ){

    char *p_a[11];

    int start = range[2] + ((y_size-1)/nthreads+1)*thr;
    int finish = range[2] +  MIN(((y_size-1)/nthreads+1)*(thr+1),y_size);

    //set up initial pointers
    p_a[0] = &args[0].data[
    + args[0].dat->size * args[0].dat->block_size[0] * ( start * 1 - args[0].dat->offset[1] )
    + args[0].dat->size * ( range[0] * 1 - args[0].dat->offset[0] ) ];

    p_a[1] = &args[1].data[
    + args[1].dat->size * args[1].dat->block_size[0] * ( start * 1 - args[1].dat->offset[1] )
    + args[1].dat->size * ( range[0] * 1 - args[1].dat->offset[0] ) ];

    p_a[2] = &args[2].data[
    + args[2].dat->size * args[2].dat->block_size[0] * ( start * 1 - args[2].dat->offset[1] )
    + args[2].dat->size * ( range[0] * 1 - args[2].dat->offset[0] ) ];

    p_a[3] = &args[3].data[
    + args[3].dat->size * args[3].dat->block_size[0] * ( start * 1 - args[3].dat->offset[1] )
    + args[3].dat->size * ( range[0] * 1 - args[3].dat->offset[0] ) ];

    p_a[4] = &args[4].data[
    + args[4].dat->size * args[4].dat->block_size[0] * ( start * 1 - args[4].dat->offset[1] )
    + args[4].dat->size * ( range[0] * 1 - args[4].dat->offset[0] ) ];

    p_a[5] = &args[5].data[
    + args[5].dat->size * args[5].dat->block_size[0] * ( start * 1 - args[5].dat->offset[1] )
    + args[5].dat->size * ( range[0] * 1 - args[5].dat->offset[0] ) ];

    p_a[6] = (char *)args[6].data;

    p_a[7] = (char *)args[7].data;

    p_a[8] = (char *)args[8].data;

    p_a[9] = (char *)args[9].data;

    p_a[10] = (char *)args[10].data;


    for ( int n_y=start; n_y<finish; n_y++ ){
      for ( int n_x=range[0]; n_x<range[0]+(range[1]-range[0])/4; n_x++ ){
        //call kernel function, passing in pointers to data - vectorised
        for ( int i=0; i<4; i++ ){
          field_summary_kernel(  (double *)p_a[0]+ i*1, (double *)p_a[1]+ i*1,
           (double *)p_a[2]+ i*1, (double *)p_a[3]+ i*1,
           (double *)p_a[4]+ i*1, (double *)p_a[5]+ i*1,
           &arg_gbl6[64*thr], &arg_gbl7[64*thr],
           &arg_gbl8[64*thr], &arg_gbl9[64*thr],
           &arg_gbl10[64*thr] );

        }

        //shift pointers to data x direction
        p_a[0]= p_a[0] + (dat0 * off0_1)*4;
        p_a[1]= p_a[1] + (dat1 * off1_1)*4;
        p_a[2]= p_a[2] + (dat2 * off2_1)*4;
        p_a[3]= p_a[3] + (dat3 * off3_1)*4;
        p_a[4]= p_a[4] + (dat4 * off4_1)*4;
        p_a[5]= p_a[5] + (dat5 * off5_1)*4;
      }

      for ( int n_x=range[0]+((range[1]-range[0])/4)*4; n_x<range[1]; n_x++ ){
        //call kernel function, passing in pointers to data - remainder
        field_summary_kernel(  (double *)p_a[0], (double *)p_a[1],
           (double *)p_a[2], (double *)p_a[3],
           (double *)p_a[4], (double *)p_a[5],
           &arg_gbl6[64*thr], &arg_gbl7[64*thr],
           &arg_gbl8[64*thr], &arg_gbl9[64*thr],
           &arg_gbl10[64*thr] );


        //shift pointers to data x direction
        p_a[0]= p_a[0] + (dat0 * off0_1);
        p_a[1]= p_a[1] + (dat1 * off1_1);
        p_a[2]= p_a[2] + (dat2 * off2_1);
        p_a[3]= p_a[3] + (dat3 * off3_1);
        p_a[4]= p_a[4] + (dat4 * off4_1);
        p_a[5]= p_a[5] + (dat5 * off5_1);
      }

      //shift pointers to data y direction
      p_a[0]= p_a[0] + (dat0 * off0_2);
      p_a[1]= p_a[1] + (dat1 * off1_2);
      p_a[2]= p_a[2] + (dat2 * off2_2);
      p_a[3]= p_a[3] + (dat3 * off3_2);
      p_a[4]= p_a[4] + (dat4 * off4_2);
      p_a[5]= p_a[5] + (dat5 * off5_2);
    }
  }


  // combine reduction data
  for ( int thr=0; thr<nthreads; thr++ ){
    for ( int d=0; d<1; d++ ){
      arg6h[0] += arg_gbl6[64*thr];
    }
    for ( int d=0; d<1; d++ ){
      arg7h[0] += arg_gbl7[64*thr];
    }
    for ( int d=0; d<1; d++ ){
      arg8h[0] += arg_gbl8[64*thr];
    }
    for ( int d=0; d<1; d++ ){
      arg9h[0] += arg_gbl9[64*thr];
    }
    for ( int d=0; d<1; d++ ){
      arg10h[0] += arg_gbl10[64*thr];
    }
  }
  ops_set_dirtybit(args, 11);


  //Update kernel record
  ops_timers_core(&c2,&t2);
  OPS_kernels[4].count++;
  OPS_kernels[4].time += t2-t1;
  OPS_kernels[4].transfer += ops_compute_transfer(dim, range, &arg0);
  OPS_kernels[4].transfer += ops_compute_transfer(dim, range, &arg1);
  OPS_kernels[4].transfer += ops_compute_transfer(dim, range, &arg2);
  OPS_kernels[4].transfer += ops_compute_transfer(dim, range, &arg3);
  OPS_kernels[4].transfer += ops_compute_transfer(dim, range, &arg4);
  OPS_kernels[4].transfer += ops_compute_transfer(dim, range, &arg5);
}
