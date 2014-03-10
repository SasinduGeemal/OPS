//
// auto-generated by ops.py on 2014-03-10 13:05
//

#ifdef _OPENMP
#include <omp.h>
#endif
//user function
#include "calc_dt_kernel.h"

// host stub function
void ops_par_loop_calc_dt_kernel_min(char const *name, ops_block block, int dim, int* range,
 ops_arg arg0, ops_arg arg1) {

  int  offs[2][2];
  ops_arg args[2] = { arg0, arg1};


  sub_block_list sb = OPS_sub_block_list[block->index];
  //compute localy allocated range for the sub-block
  int ndim = sb->ndim;
  int start_add[ndim*2];
  int end_add[ndim*2];

  int s[ndim];
  int e[ndim];

  for ( int n=0; n<ndim; n++ ){
    s[n] = sb->istart[n];e[n] = sb->iend[n]+1;
    if (s[n] >= range[2*n]) {
      s[n] = 0;
    }
    else {
      s[n] = range[2*n] - s[n];
    }
    if (e[n] >= range[2*n+1]) {
      e[n] = range[2*n+1] - sb->istart[n];
    }
    else {
      e[n] = sb->sizes[n];
    }
  }

  for ( int i=0; i<2; i++ ){
    for ( int n=0; n<ndim; n++ ){
      start_add[i*ndim+n] = s[n];
      end_add[i*ndim+n]   = e[n];
    }
  }

  #ifdef OPS_DEBUG
  ops_register_args(args, "calc_dt_kernel_min");
  #endif

  offs[0][0] = args[0].stencil->stride[0]*1;  //unit step in x dimension
  offs[0][1] = off2D(1, &start_add[0*2],
  &end_add[0*2],args[0].dat->block_size, args[0].stencil->stride);



  int off0_1 = offs[0][0];
  int off0_2 = offs[0][1];
  int dat0 = args[0].dat->size;

  double*arg1h = (double *)arg1.data;

  #ifdef _OPENMP
  int nthreads = omp_get_max_threads( );
  #else
  int nthreads = 1;
  #endif
  //allocate and initialise arrays for global reduction
  //assumes a max of 64 threads with a cacche line size of 64 bytes
  double arg_gbl1[1 * 64 * 64];
  for ( int thr=0; thr<nthreads; thr++ ){
    for ( int d=0; d<1; d++ ){
      arg_gbl1[d+64*thr] = INFINITY_double;
    }
  }
  xdim0 = args[0].dat->block_size[0];

  //Halo Exchanges
  ops_exchange_halo(&args[0],2);

  ops_H_D_exchanges(args, 2);


  //Timing
  double t1,t2,c1,c2;
  ops_timing_realloc(29,"calc_dt_kernel_min");
  ops_timers_core(&c1,&t1);

  int start_thread_add[ndim*nthreads*64];

  #pragma omp parallel for
  for ( int thr=0; thr<nthreads; thr++ ){

    int y_size = e[1]-s[1];
    char *p_a[2];

    int start = s[1] + ((y_size-1)/nthreads+1)*thr;
    int finish = s[1] + MIN(((y_size-1)/nthreads+1)*(thr+1),y_size);

    //get addresss per thread
    start_thread_add[64*ndim*thr+0] = s[0];
    start_thread_add[64*ndim*thr+1] = s[1] + ((y_size-1)/nthreads+1)*thr;

    //set up initial pointers 
    int base0 = dat0 * 1 * 
    (start_thread_add[64*ndim*thr+0] * args[0].stencil->stride[0] - args[0].dat->offset[0]);
    base0 = base0  + dat0 * args[0].dat->block_size[0] * 
    (start_thread_add[64*ndim*thr+1] * args[0].stencil->stride[1] - args[0].dat->offset[1]);
    p_a[0] = (char *)args[0].data + base0;

    p_a[1] = (char *)args[1].data;


    for ( int n_y=start; n_y<finish; n_y++ ){
      for ( int n_x=s[0]; n_x<s[0]+(e[0]-s[0])/SIMD_VEC; n_x++ ){
        //call kernel function, passing in pointers to data -vectorised
        for ( int i=0; i<SIMD_VEC; i++ ){
          calc_dt_kernel_min(  (double *)p_a[0]+ i*1, &arg_gbl1[64*thr] );

        }

        //shift pointers to data x direction
        p_a[0]= p_a[0] + (dat0 * off0_1)*SIMD_VEC;
      }

      for ( int n_x=s[0]+((e[0]-s[0])/SIMD_VEC)*SIMD_VEC; n_x<e[0]; n_x++ ){
        //call kernel function, passing in pointers to data - remainder
        calc_dt_kernel_min(  (double *)p_a[0], &arg_gbl1[64*thr] );


        //shift pointers to data x direction
        p_a[0]= p_a[0] + (dat0 * off0_1);
      }

      //shift pointers to data y direction
      p_a[0]= p_a[0] + (dat0 * off0_2);
    }
  }

  // combine reduction data
  for ( int thr=0; thr<nthreads; thr++ ){
    for ( int d=0; d<1; d++ ){
      arg1h[0] = MIN(arg1h[0], arg_gbl1[64*thr]);
    }
  }
  ops_mpi_reduce(&arg1,(double *)arg1h);
  ops_set_dirtybit_host(args, 2);


  //Update kernel record
  ops_timers_core(&c2,&t2);
  OPS_kernels[29].count++;
  OPS_kernels[29].time += t2-t1;
  OPS_kernels[29].transfer += ops_compute_transfer(dim, range, &arg0);
}
