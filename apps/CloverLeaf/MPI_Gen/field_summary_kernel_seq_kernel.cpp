//
// auto-generated by ops.py on 2014-02-19 15:18
//

#include "ops_mpi_core.h"
#include "lib.h"
//user function
#include "field_summary_kernel.h"

// host stub function
void ops_par_loop_field_summary_kernel(char const *name, ops_block block, int dim, int* range,
 ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
 ops_arg arg4, ops_arg arg5, ops_arg arg6, ops_arg arg7,
 ops_arg arg8, ops_arg arg9, ops_arg arg10) {

  char *p_a[11];
  int  offs[11][2];
  ops_arg args[11] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10};


  sub_block_list sb = OPS_sub_block_list[block->index];
  //compute localy allocated range for the sub-block
  int ndim = sb->ndim;
  int start[ndim*11];
  int end[ndim*11];

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

  for ( int i=0; i<11; i++ ){
    for ( int n=0; n<ndim; n++ ){
      start[i*ndim+n] = s[n];
      end[i*ndim+n]   = e[n];
    }
  }

  #ifdef OPS_DEBUG
  ops_register_args(args, "field_summary_kernel");
  #endif

  offs[0][0] = args[0].stencil->stride[0]*1;  //unit step in x dimension
  for ( int n=1; n<ndim; n++ ){
    offs[0][n] = off2(ndim, n, &start[0*ndim],
    &end[0*ndim],args[0].dat->block_size, args[0].stencil->stride);
  }
  //set up initial pointers
  p_a[0] = (char *)args[0].data
  + address2(ndim, args[0].dat->size, &start[0*ndim],
  args[0].dat->block_size, args[0].stencil->stride, args[0].dat->offset);
  ops_exchange_halo(&args[0],2);

  offs[1][0] = args[1].stencil->stride[0]*1;  //unit step in x dimension
  for ( int n=1; n<ndim; n++ ){
    offs[1][n] = off2(ndim, n, &start[1*ndim],
    &end[1*ndim],args[1].dat->block_size, args[1].stencil->stride);
  }
  //set up initial pointers
  p_a[1] = (char *)args[1].data
  + address2(ndim, args[1].dat->size, &start[1*ndim],
  args[1].dat->block_size, args[1].stencil->stride, args[1].dat->offset);
  ops_exchange_halo(&args[1],2);

  offs[2][0] = args[2].stencil->stride[0]*1;  //unit step in x dimension
  for ( int n=1; n<ndim; n++ ){
    offs[2][n] = off2(ndim, n, &start[2*ndim],
    &end[2*ndim],args[2].dat->block_size, args[2].stencil->stride);
  }
  //set up initial pointers
  p_a[2] = (char *)args[2].data
  + address2(ndim, args[2].dat->size, &start[2*ndim],
  args[2].dat->block_size, args[2].stencil->stride, args[2].dat->offset);
  ops_exchange_halo(&args[2],2);

  offs[3][0] = args[3].stencil->stride[0]*1;  //unit step in x dimension
  for ( int n=1; n<ndim; n++ ){
    offs[3][n] = off2(ndim, n, &start[3*ndim],
    &end[3*ndim],args[3].dat->block_size, args[3].stencil->stride);
  }
  //set up initial pointers
  p_a[3] = (char *)args[3].data
  + address2(ndim, args[3].dat->size, &start[3*ndim],
  args[3].dat->block_size, args[3].stencil->stride, args[3].dat->offset);
  ops_exchange_halo(&args[3],2);

  offs[4][0] = args[4].stencil->stride[0]*1;  //unit step in x dimension
  for ( int n=1; n<ndim; n++ ){
    offs[4][n] = off2(ndim, n, &start[4*ndim],
    &end[4*ndim],args[4].dat->block_size, args[4].stencil->stride);
  }
  //set up initial pointers
  p_a[4] = (char *)args[4].data
  + address2(ndim, args[4].dat->size, &start[4*ndim],
  args[4].dat->block_size, args[4].stencil->stride, args[4].dat->offset);
  ops_exchange_halo(&args[4],2);

  offs[5][0] = args[5].stencil->stride[0]*1;  //unit step in x dimension
  for ( int n=1; n<ndim; n++ ){
    offs[5][n] = off2(ndim, n, &start[5*ndim],
    &end[5*ndim],args[5].dat->block_size, args[5].stencil->stride);
  }
  //set up initial pointers
  p_a[5] = (char *)args[5].data
  + address2(ndim, args[5].dat->size, &start[5*ndim],
  args[5].dat->block_size, args[5].stencil->stride, args[5].dat->offset);
  ops_exchange_halo(&args[5],2);

  p_a[6] = (char *)args[6].data;


  p_a[7] = (char *)args[7].data;


  p_a[8] = (char *)args[8].data;


  p_a[9] = (char *)args[9].data;


  p_a[10] = (char *)args[10].data;



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

  xdim0 = args[0].dat->block_size[0];
  xdim1 = args[1].dat->block_size[0];
  xdim2 = args[2].dat->block_size[0];
  xdim3 = args[3].dat->block_size[0];
  xdim4 = args[4].dat->block_size[0];
  xdim5 = args[5].dat->block_size[0];

  for ( int n_y=s[1]; n_y<e[1]; n_y++ ){
    for ( int n_x=s[0]; n_x<s[0]+(e[0]-s[0])/4; n_x++ ){
      //call kernel function, passing in pointers to data -vectorised
      for ( int i=0; i<4; i++ ){
        field_summary_kernel(  (double *)p_a[0]+ i*1, (double *)p_a[1]+ i*1, (double *)p_a[2]+ i*1,
           (double *)p_a[3]+ i*1, (double *)p_a[4]+ i*1, (double *)p_a[5]+ i*1, (double *)p_a[6],
           (double *)p_a[7], (double *)p_a[8], (double *)p_a[9], (double *)p_a[10] );

      }

      //shift pointers to data x direction
      p_a[0]= p_a[0] + (dat0 * off0_1)*4;
      p_a[1]= p_a[1] + (dat1 * off1_1)*4;
      p_a[2]= p_a[2] + (dat2 * off2_1)*4;
      p_a[3]= p_a[3] + (dat3 * off3_1)*4;
      p_a[4]= p_a[4] + (dat4 * off4_1)*4;
      p_a[5]= p_a[5] + (dat5 * off5_1)*4;
    }

    for ( int n_x=s[0]+((e[0]-s[0])/4)*4; n_x<e[0]; n_x++ ){
      //call kernel function, passing in pointers to data - remainder
      field_summary_kernel(  (double *)p_a[0], (double *)p_a[1], (double *)p_a[2],
           (double *)p_a[3], (double *)p_a[4], (double *)p_a[5], (double *)p_a[6],
           (double *)p_a[7], (double *)p_a[8], (double *)p_a[9], (double *)p_a[10] );


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
  ops_mpi_reduce(&arg0,(double *)p_a[0]);
  ops_mpi_reduce(&arg1,(double *)p_a[1]);
  ops_mpi_reduce(&arg2,(double *)p_a[2]);
  ops_mpi_reduce(&arg3,(double *)p_a[3]);
  ops_mpi_reduce(&arg4,(double *)p_a[4]);
  ops_mpi_reduce(&arg5,(double *)p_a[5]);
  ops_mpi_reduce(&arg6,(double *)p_a[6]);
  ops_mpi_reduce(&arg7,(double *)p_a[7]);
  ops_mpi_reduce(&arg8,(double *)p_a[8]);
  ops_mpi_reduce(&arg9,(double *)p_a[9]);
  ops_mpi_reduce(&arg10,(double *)p_a[10]);
  ops_set_halo_dirtybit(args, 11);


}
