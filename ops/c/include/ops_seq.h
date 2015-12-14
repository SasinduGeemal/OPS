
/*
* Open source copyright declaration based on BSD open source template:
* http://www.opensource.org/licenses/bsd-license.php
*
* This file is part of the OPS distribution.
*
* Copyright (c) 2013, Mike Giles and others. Please see the AUTHORS file in
* the main source directory for a full list of copyright holders.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
* * Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* * The name of Mike Giles may not be used to endorse or promote products
* derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY Mike Giles ''AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL Mike Giles BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/** @brief header file declaring the functions for the ops sequential backend
  * @author Gihan Mudalige, Istvan Reguly
  * @details Declares the OPS API calls for the sequential backend
  */

#include "ops_lib_cpp.h"
#ifdef OPS_MPI
#include "ops_mpi_core.h"
#endif

static int arg_idx[OPS_MAX_DIM];

inline int mult(int* size, int dim)
{
  int result = 1;
  if(dim > 0) {
    for(int i = 0; i<dim;i++) result *= size[i];
  }
  return result;
}

inline int add(int* coords, int* size, int dim)
{
  int result = coords[0];
  for(int i = 1; i<=dim;i++) result += coords[i]*mult(size,i);
  return result;
}


inline int off(int ndim, int dim , int* start, int* end, int* size, int* stride)
{

  int i = 0;
  int c1[3];
  int c2[3];

  for(i=0; i<=dim; i++) c1[i] = start[i]+1;
  for(i=dim+1; i<ndim; i++) c1[i] = start[i];

  for(i = 0; i<dim; i++) c2[i] = end[i];
  for(i=dim; i<ndim; i++) c2[i] = start[i];

  for (i = 0; i < ndim; i++) {
    c1[i] *= stride[i];
    c2[i] *= stride[i];
  }
  int off =  add(c1, size, dim) - add(c2, size, dim);

  return off;
}

inline int address(int ndim, int dat_size, int* start, int* size, int* stride, int* base_off, int *d_m)
{
  int base = 0;
  for(int i=0; i<ndim; i++) {
    base = base + dat_size * mult(size, i) * (start[i] * stride[i] - base_off[i] - d_m[i]);
  }
  return base;
}

inline void stencil_depth(ops_stencil sten, int* d_pos, int* d_neg)
{
  for(int dim = 0;dim<sten->dims;dim++){
    d_pos[dim] = 0; d_neg[dim] = 0;
  }
  for(int p=0;p<sten->points; p++) {
    for(int dim = 0;dim<sten->dims;dim++){
    d_pos[dim] = MAX(d_pos[dim],sten->stencil[sten->dims*p + dim]);
    d_neg[dim] = MIN(d_neg[dim],sten->stencil[sten->dims*p + dim]);
    }
  }
}



//
//ops_par_loop routine for 1 arguments
//
template <class T0>
void ops_par_loop(void (*kernel)(T0*),
     char const * name, ops_block block, int dim, int *range,
     ops_arg arg0) {

  char *p_a[1];
  int  offs[1][OPS_MAX_DIM];

  int  count[dim];
  ops_arg args[1] = { arg0};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_name_before(args,1,range,name)) return;
  #endif

  int start[OPS_MAX_DIM];
  int end[OPS_MAX_DIM];

  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  //compute locally allocated range for the sub-block
  int ndim = sb->ndim;
  for (int n=0; n<ndim; n++) {
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) start[n] = 0;
    else start[n] = range[2*n] - start[n];
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) end[n] = range[2*n+1] - sb->decomp_disp[n];
    else end[n] = sb->decomp_size[n];
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else //!OPS_MPI
  int ndim = block->dims;
  for (int n=0; n<ndim; n++) {
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif //OPS_MPI

  #ifdef OPS_DEBUG
  ops_register_args(args, name);
  #endif

  for (int i = 0; i<1;i++) {
    if(args[i].stencil!=NULL) {
      offs[i][0] = args[i].stencil->stride[0]*1;  //unit step in x dimension
      for(int n=1; n<ndim; n++) {
        offs[i][n] = off(ndim, n, &start[0], &end[0],
                         args[i].dat->size, args[i].stencil->stride);
      }
    }
  }

  //set up initial pointers
  for (int i = 0; i < 1; i++) {
    if (args[i].argtype == OPS_ARG_DAT) {
      int d_m[OPS_MAX_DIM];
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d] + OPS_sub_dat_list[args[i].dat->index]->d_im[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d];
  #endif //OPS_MPI
      p_a[i] = (char *)args[i].data //base of 2D array
      + address(ndim, args[i].dat->elem_size, &start[0],
        args[i].dat->size, args[i].stencil->stride, args[i].dat->base,
        d_m);
    }
    else if (args[i].argtype == OPS_ARG_GBL) {
      if (args[i].acc == OPS_READ) p_a[i] = args[i].data;
      else
  #ifdef OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data + ((ops_reduction)args[i].data)->size * block->index;
  #else //OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data;
  #endif //OPS_MPI
    } else if (args[i].argtype == OPS_ARG_IDX) {
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      p_a[i] = (char *)arg_idx;
    }
  }

  int total_range = 1;
  for (int n=0; n<ndim; n++) {
    count[n] = end[n]-start[n];  // number in each dimension
    total_range *= count[n];
    total_range *= (count[n]<0?0:1);
  }
  count[dim-1]++;     // extra in last to ensure correct termination

  if (args[0].argtype == OPS_ARG_DAT) {
    xdim0 = args[0].dat->size[0];
    multi_d0 = args[0].dat->dim;
    #ifdef OPS_3D
    ydim0 = args[0].dat->size[1];
    #endif
  }

  ops_H_D_exchanges_host(args, 1);
  ops_halo_exchanges(args,1,range);
  for (int nt=0; nt<total_range; nt++) {
    // call kernel function, passing in pointers to data

    kernel(  (T0 *)p_a[0] );

    count[0]--;   // decrement counter
    int m = 0;    // max dimension with changed index
    while (count[m]==0) {
      count[m] =  end[m]-start[m];// reset counter
      m++;                        // next dimension
      count[m]--;                 // decrement counter
    }

    // shift pointers to data
    for (int i=0; i<1; i++) {
      if (args[i].argtype == OPS_ARG_DAT)
        p_a[i] = p_a[i] + (args[i].dat->elem_size * offs[i][m]);
      else if (args[i].argtype == OPS_ARG_IDX) {
        arg_idx[m]++;
  #ifdef OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      }
    }
  }

  #ifdef OPS_DEBUG_DUMP
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ) ops_dump3(args[0].dat,name);
  #endif
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[0],range);
  ops_set_dirtybit_host(args, 1);
}


//
//ops_par_loop routine for 2 arguments
//
template <class T0,class T1>
void ops_par_loop(void (*kernel)(T0*, T1*),
     char const * name, ops_block block, int dim, int *range,
     ops_arg arg0, ops_arg arg1) {

  char *p_a[2];
  int  offs[2][OPS_MAX_DIM];

  int  count[dim];
  ops_arg args[2] = { arg0, arg1};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_name_before(args,2,range,name)) return;
  #endif

  int start[OPS_MAX_DIM];
  int end[OPS_MAX_DIM];

  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  //compute locally allocated range for the sub-block
  int ndim = sb->ndim;
  for (int n=0; n<ndim; n++) {
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) start[n] = 0;
    else start[n] = range[2*n] - start[n];
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) end[n] = range[2*n+1] - sb->decomp_disp[n];
    else end[n] = sb->decomp_size[n];
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else //!OPS_MPI
  int ndim = block->dims;
  for (int n=0; n<ndim; n++) {
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif //OPS_MPI

  #ifdef OPS_DEBUG
  ops_register_args(args, name);
  #endif

  for (int i = 0; i<2;i++) {
    if(args[i].stencil!=NULL) {
      offs[i][0] = args[i].stencil->stride[0]*1;  //unit step in x dimension
      for(int n=1; n<ndim; n++) {
        offs[i][n] = off(ndim, n, &start[0], &end[0],
                         args[i].dat->size, args[i].stencil->stride);
      }
    }
  }

  //set up initial pointers
  for (int i = 0; i < 2; i++) {
    if (args[i].argtype == OPS_ARG_DAT) {
      int d_m[OPS_MAX_DIM];
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d] + OPS_sub_dat_list[args[i].dat->index]->d_im[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d];
  #endif //OPS_MPI
      p_a[i] = (char *)args[i].data //base of 2D array
      + address(ndim, args[i].dat->elem_size, &start[0],
        args[i].dat->size, args[i].stencil->stride, args[i].dat->base,
        d_m);
    }
    else if (args[i].argtype == OPS_ARG_GBL) {
      if (args[i].acc == OPS_READ) p_a[i] = args[i].data;
      else
  #ifdef OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data + ((ops_reduction)args[i].data)->size * block->index;
  #else //OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data;
  #endif //OPS_MPI
    } else if (args[i].argtype == OPS_ARG_IDX) {
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      p_a[i] = (char *)arg_idx;
    }
  }

  int total_range = 1;
  for (int n=0; n<ndim; n++) {
    count[n] = end[n]-start[n];  // number in each dimension
    total_range *= count[n];
    total_range *= (count[n]<0?0:1);
  }
  count[dim-1]++;     // extra in last to ensure correct termination

  if (args[0].argtype == OPS_ARG_DAT) {
    xdim0 = args[0].dat->size[0];
    multi_d0 = args[0].dat->dim;
    #ifdef OPS_3D
    ydim0 = args[0].dat->size[1];
    #endif
  }
  if (args[1].argtype == OPS_ARG_DAT) {
    xdim1 = args[1].dat->size[0];
    multi_d1 = args[1].dat->dim;
    #ifdef OPS_3D
    ydim1 = args[1].dat->size[1];
    #endif
  }

  ops_H_D_exchanges_host(args, 2);
  ops_halo_exchanges(args,2,range);
  for (int nt=0; nt<total_range; nt++) {
    // call kernel function, passing in pointers to data

    kernel(  (T0 *)p_a[0], (T1 *)p_a[1] );

    count[0]--;   // decrement counter
    int m = 0;    // max dimension with changed index
    while (count[m]==0) {
      count[m] =  end[m]-start[m];// reset counter
      m++;                        // next dimension
      count[m]--;                 // decrement counter
    }

    // shift pointers to data
    for (int i=0; i<2; i++) {
      if (args[i].argtype == OPS_ARG_DAT)
        p_a[i] = p_a[i] + (args[i].dat->elem_size * offs[i][m]);
      else if (args[i].argtype == OPS_ARG_IDX) {
        arg_idx[m]++;
  #ifdef OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      }
    }
  }

  #ifdef OPS_DEBUG_DUMP
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ) ops_dump3(args[0].dat,name);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ) ops_dump3(args[1].dat,name);
  #endif
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[0],range);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[1],range);
  ops_set_dirtybit_host(args, 2);
}


//
//ops_par_loop routine for 3 arguments
//
template <class T0,class T1,class T2>
void ops_par_loop(void (*kernel)(T0*, T1*, T2*),
     char const * name, ops_block block, int dim, int *range,
     ops_arg arg0, ops_arg arg1, ops_arg arg2) {

  char *p_a[3];
  int  offs[3][OPS_MAX_DIM];

  int  count[dim];
  ops_arg args[3] = { arg0, arg1, arg2};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_name_before(args,3,range,name)) return;
  #endif

  int start[OPS_MAX_DIM];
  int end[OPS_MAX_DIM];

  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  //compute locally allocated range for the sub-block
  int ndim = sb->ndim;
  for (int n=0; n<ndim; n++) {
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) start[n] = 0;
    else start[n] = range[2*n] - start[n];
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) end[n] = range[2*n+1] - sb->decomp_disp[n];
    else end[n] = sb->decomp_size[n];
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else //!OPS_MPI
  int ndim = block->dims;
  for (int n=0; n<ndim; n++) {
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif //OPS_MPI

  #ifdef OPS_DEBUG
  ops_register_args(args, name);
  #endif

  for (int i = 0; i<3;i++) {
    if(args[i].stencil!=NULL) {
      offs[i][0] = args[i].stencil->stride[0]*1;  //unit step in x dimension
      for(int n=1; n<ndim; n++) {
        offs[i][n] = off(ndim, n, &start[0], &end[0],
                         args[i].dat->size, args[i].stencil->stride);
      }
    }
  }

  //set up initial pointers
  for (int i = 0; i < 3; i++) {
    if (args[i].argtype == OPS_ARG_DAT) {
      int d_m[OPS_MAX_DIM];
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d] + OPS_sub_dat_list[args[i].dat->index]->d_im[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d];
  #endif //OPS_MPI
      p_a[i] = (char *)args[i].data //base of 2D array
      + address(ndim, args[i].dat->elem_size, &start[0],
        args[i].dat->size, args[i].stencil->stride, args[i].dat->base,
        d_m);
    }
    else if (args[i].argtype == OPS_ARG_GBL) {
      if (args[i].acc == OPS_READ) p_a[i] = args[i].data;
      else
  #ifdef OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data + ((ops_reduction)args[i].data)->size * block->index;
  #else //OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data;
  #endif //OPS_MPI
    } else if (args[i].argtype == OPS_ARG_IDX) {
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      p_a[i] = (char *)arg_idx;
    }
  }

  int total_range = 1;
  for (int n=0; n<ndim; n++) {
    count[n] = end[n]-start[n];  // number in each dimension
    total_range *= count[n];
    total_range *= (count[n]<0?0:1);
  }
  count[dim-1]++;     // extra in last to ensure correct termination

  if (args[0].argtype == OPS_ARG_DAT) {
    xdim0 = args[0].dat->size[0];
    multi_d0 = args[0].dat->dim;
    #ifdef OPS_3D
    ydim0 = args[0].dat->size[1];
    #endif
  }
  if (args[1].argtype == OPS_ARG_DAT) {
    xdim1 = args[1].dat->size[0];
    multi_d1 = args[1].dat->dim;
    #ifdef OPS_3D
    ydim1 = args[1].dat->size[1];
    #endif
  }
  if (args[2].argtype == OPS_ARG_DAT) {
    xdim2 = args[2].dat->size[0];
    multi_d2 = args[2].dat->dim;
    #ifdef OPS_3D
    ydim2 = args[2].dat->size[1];
    #endif
  }

  ops_H_D_exchanges_host(args, 3);
  ops_halo_exchanges(args,3,range);
  for (int nt=0; nt<total_range; nt++) {
    // call kernel function, passing in pointers to data

    kernel(  (T0 *)p_a[0], (T1 *)p_a[1], (T2 *)p_a[2] );

    count[0]--;   // decrement counter
    int m = 0;    // max dimension with changed index
    while (count[m]==0) {
      count[m] =  end[m]-start[m];// reset counter
      m++;                        // next dimension
      count[m]--;                 // decrement counter
    }

    // shift pointers to data
    for (int i=0; i<3; i++) {
      if (args[i].argtype == OPS_ARG_DAT)
        p_a[i] = p_a[i] + (args[i].dat->elem_size * offs[i][m]);
      else if (args[i].argtype == OPS_ARG_IDX) {
        arg_idx[m]++;
  #ifdef OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      }
    }
  }

  #ifdef OPS_DEBUG_DUMP
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ) ops_dump3(args[0].dat,name);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ) ops_dump3(args[1].dat,name);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ) ops_dump3(args[2].dat,name);
  #endif
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[0],range);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[1],range);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[2],range);
  ops_set_dirtybit_host(args, 3);
}


//
//ops_par_loop routine for 4 arguments
//
template <class T0,class T1,class T2,class T3>
void ops_par_loop(void (*kernel)(T0*, T1*, T2*, T3*),
     char const * name, ops_block block, int dim, int *range,
     ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3) {

  char *p_a[4];
  int  offs[4][OPS_MAX_DIM];

  int  count[dim];
  ops_arg args[4] = { arg0, arg1, arg2, arg3};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_name_before(args,4,range,name)) return;
  #endif

  int start[OPS_MAX_DIM];
  int end[OPS_MAX_DIM];

  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  //compute locally allocated range for the sub-block
  int ndim = sb->ndim;
  for (int n=0; n<ndim; n++) {
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) start[n] = 0;
    else start[n] = range[2*n] - start[n];
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) end[n] = range[2*n+1] - sb->decomp_disp[n];
    else end[n] = sb->decomp_size[n];
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else //!OPS_MPI
  int ndim = block->dims;
  for (int n=0; n<ndim; n++) {
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif //OPS_MPI

  #ifdef OPS_DEBUG
  ops_register_args(args, name);
  #endif

  for (int i = 0; i<4;i++) {
    if(args[i].stencil!=NULL) {
      offs[i][0] = args[i].stencil->stride[0]*1;  //unit step in x dimension
      for(int n=1; n<ndim; n++) {
        offs[i][n] = off(ndim, n, &start[0], &end[0],
                         args[i].dat->size, args[i].stencil->stride);
      }
    }
  }

  //set up initial pointers
  for (int i = 0; i < 4; i++) {
    if (args[i].argtype == OPS_ARG_DAT) {
      int d_m[OPS_MAX_DIM];
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d] + OPS_sub_dat_list[args[i].dat->index]->d_im[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d];
  #endif //OPS_MPI
      p_a[i] = (char *)args[i].data //base of 2D array
      + address(ndim, args[i].dat->elem_size, &start[0],
        args[i].dat->size, args[i].stencil->stride, args[i].dat->base,
        d_m);
    }
    else if (args[i].argtype == OPS_ARG_GBL) {
      if (args[i].acc == OPS_READ) p_a[i] = args[i].data;
      else
  #ifdef OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data + ((ops_reduction)args[i].data)->size * block->index;
  #else //OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data;
  #endif //OPS_MPI
    } else if (args[i].argtype == OPS_ARG_IDX) {
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      p_a[i] = (char *)arg_idx;
    }
  }

  int total_range = 1;
  for (int n=0; n<ndim; n++) {
    count[n] = end[n]-start[n];  // number in each dimension
    total_range *= count[n];
    total_range *= (count[n]<0?0:1);
  }
  count[dim-1]++;     // extra in last to ensure correct termination

  if (args[0].argtype == OPS_ARG_DAT) {
    xdim0 = args[0].dat->size[0];
    multi_d0 = args[0].dat->dim;
    #ifdef OPS_3D
    ydim0 = args[0].dat->size[1];
    #endif
  }
  if (args[1].argtype == OPS_ARG_DAT) {
    xdim1 = args[1].dat->size[0];
    multi_d1 = args[1].dat->dim;
    #ifdef OPS_3D
    ydim1 = args[1].dat->size[1];
    #endif
  }
  if (args[2].argtype == OPS_ARG_DAT) {
    xdim2 = args[2].dat->size[0];
    multi_d2 = args[2].dat->dim;
    #ifdef OPS_3D
    ydim2 = args[2].dat->size[1];
    #endif
  }
  if (args[3].argtype == OPS_ARG_DAT) {
    xdim3 = args[3].dat->size[0];
    multi_d3 = args[3].dat->dim;
    #ifdef OPS_3D
    ydim3 = args[3].dat->size[1];
    #endif
  }

  ops_H_D_exchanges_host(args, 4);
  ops_halo_exchanges(args,4,range);
  for (int nt=0; nt<total_range; nt++) {
    // call kernel function, passing in pointers to data

    kernel(  (T0 *)p_a[0], (T1 *)p_a[1], (T2 *)p_a[2], (T3 *)p_a[3] );

    count[0]--;   // decrement counter
    int m = 0;    // max dimension with changed index
    while (count[m]==0) {
      count[m] =  end[m]-start[m];// reset counter
      m++;                        // next dimension
      count[m]--;                 // decrement counter
    }

    // shift pointers to data
    for (int i=0; i<4; i++) {
      if (args[i].argtype == OPS_ARG_DAT)
        p_a[i] = p_a[i] + (args[i].dat->elem_size * offs[i][m]);
      else if (args[i].argtype == OPS_ARG_IDX) {
        arg_idx[m]++;
  #ifdef OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      }
    }
  }

  #ifdef OPS_DEBUG_DUMP
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ) ops_dump3(args[0].dat,name);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ) ops_dump3(args[1].dat,name);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ) ops_dump3(args[2].dat,name);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ) ops_dump3(args[3].dat,name);
  #endif
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[0],range);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[1],range);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[2],range);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[3],range);
  ops_set_dirtybit_host(args, 4);
}


//
//ops_par_loop routine for 5 arguments
//
template <class T0,class T1,class T2,class T3,
class T4>
void ops_par_loop(void (*kernel)(T0*, T1*, T2*, T3*,
                           T4*),
     char const * name, ops_block block, int dim, int *range,
     ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
     ops_arg arg4) {

  char *p_a[5];
  int  offs[5][OPS_MAX_DIM];

  int  count[dim];
  ops_arg args[5] = { arg0, arg1, arg2, arg3,
                     arg4};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_name_before(args,5,range,name)) return;
  #endif

  int start[OPS_MAX_DIM];
  int end[OPS_MAX_DIM];

  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  //compute locally allocated range for the sub-block
  int ndim = sb->ndim;
  for (int n=0; n<ndim; n++) {
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) start[n] = 0;
    else start[n] = range[2*n] - start[n];
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) end[n] = range[2*n+1] - sb->decomp_disp[n];
    else end[n] = sb->decomp_size[n];
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else //!OPS_MPI
  int ndim = block->dims;
  for (int n=0; n<ndim; n++) {
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif //OPS_MPI

  #ifdef OPS_DEBUG
  ops_register_args(args, name);
  #endif

  for (int i = 0; i<5;i++) {
    if(args[i].stencil!=NULL) {
      offs[i][0] = args[i].stencil->stride[0]*1;  //unit step in x dimension
      for(int n=1; n<ndim; n++) {
        offs[i][n] = off(ndim, n, &start[0], &end[0],
                         args[i].dat->size, args[i].stencil->stride);
      }
    }
  }

  //set up initial pointers
  for (int i = 0; i < 5; i++) {
    if (args[i].argtype == OPS_ARG_DAT) {
      int d_m[OPS_MAX_DIM];
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d] + OPS_sub_dat_list[args[i].dat->index]->d_im[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d];
  #endif //OPS_MPI
      p_a[i] = (char *)args[i].data //base of 2D array
      + address(ndim, args[i].dat->elem_size, &start[0],
        args[i].dat->size, args[i].stencil->stride, args[i].dat->base,
        d_m);
    }
    else if (args[i].argtype == OPS_ARG_GBL) {
      if (args[i].acc == OPS_READ) p_a[i] = args[i].data;
      else
  #ifdef OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data + ((ops_reduction)args[i].data)->size * block->index;
  #else //OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data;
  #endif //OPS_MPI
    } else if (args[i].argtype == OPS_ARG_IDX) {
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      p_a[i] = (char *)arg_idx;
    }
  }

  int total_range = 1;
  for (int n=0; n<ndim; n++) {
    count[n] = end[n]-start[n];  // number in each dimension
    total_range *= count[n];
    total_range *= (count[n]<0?0:1);
  }
  count[dim-1]++;     // extra in last to ensure correct termination

  if (args[0].argtype == OPS_ARG_DAT) {
    xdim0 = args[0].dat->size[0];
    multi_d0 = args[0].dat->dim;
    #ifdef OPS_3D
    ydim0 = args[0].dat->size[1];
    #endif
  }
  if (args[1].argtype == OPS_ARG_DAT) {
    xdim1 = args[1].dat->size[0];
    multi_d1 = args[1].dat->dim;
    #ifdef OPS_3D
    ydim1 = args[1].dat->size[1];
    #endif
  }
  if (args[2].argtype == OPS_ARG_DAT) {
    xdim2 = args[2].dat->size[0];
    multi_d2 = args[2].dat->dim;
    #ifdef OPS_3D
    ydim2 = args[2].dat->size[1];
    #endif
  }
  if (args[3].argtype == OPS_ARG_DAT) {
    xdim3 = args[3].dat->size[0];
    multi_d3 = args[3].dat->dim;
    #ifdef OPS_3D
    ydim3 = args[3].dat->size[1];
    #endif
  }
  if (args[4].argtype == OPS_ARG_DAT) {
    xdim4 = args[4].dat->size[0];
    multi_d4 = args[4].dat->dim;
    #ifdef OPS_3D
    ydim4 = args[4].dat->size[1];
    #endif
  }

  ops_H_D_exchanges_host(args, 5);
  ops_halo_exchanges(args,5,range);
  for (int nt=0; nt<total_range; nt++) {
    // call kernel function, passing in pointers to data

    kernel(  (T0 *)p_a[0], (T1 *)p_a[1], (T2 *)p_a[2], (T3 *)p_a[3],
           (T4 *)p_a[4] );

    count[0]--;   // decrement counter
    int m = 0;    // max dimension with changed index
    while (count[m]==0) {
      count[m] =  end[m]-start[m];// reset counter
      m++;                        // next dimension
      count[m]--;                 // decrement counter
    }

    // shift pointers to data
    for (int i=0; i<5; i++) {
      if (args[i].argtype == OPS_ARG_DAT)
        p_a[i] = p_a[i] + (args[i].dat->elem_size * offs[i][m]);
      else if (args[i].argtype == OPS_ARG_IDX) {
        arg_idx[m]++;
  #ifdef OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      }
    }
  }

  #ifdef OPS_DEBUG_DUMP
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ) ops_dump3(args[0].dat,name);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ) ops_dump3(args[1].dat,name);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ) ops_dump3(args[2].dat,name);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ) ops_dump3(args[3].dat,name);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ) ops_dump3(args[4].dat,name);
  #endif
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[0],range);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[1],range);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[2],range);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[3],range);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[4],range);
  ops_set_dirtybit_host(args, 5);
}


//
//ops_par_loop routine for 6 arguments
//
template <class T0,class T1,class T2,class T3,
class T4,class T5>
void ops_par_loop(void (*kernel)(T0*, T1*, T2*, T3*,
                           T4*, T5*),
     char const * name, ops_block block, int dim, int *range,
     ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
     ops_arg arg4, ops_arg arg5) {

  char *p_a[6];
  int  offs[6][OPS_MAX_DIM];

  int  count[dim];
  ops_arg args[6] = { arg0, arg1, arg2, arg3,
                     arg4, arg5};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_name_before(args,6,range,name)) return;
  #endif

  int start[OPS_MAX_DIM];
  int end[OPS_MAX_DIM];

  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  //compute locally allocated range for the sub-block
  int ndim = sb->ndim;
  for (int n=0; n<ndim; n++) {
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) start[n] = 0;
    else start[n] = range[2*n] - start[n];
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) end[n] = range[2*n+1] - sb->decomp_disp[n];
    else end[n] = sb->decomp_size[n];
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else //!OPS_MPI
  int ndim = block->dims;
  for (int n=0; n<ndim; n++) {
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif //OPS_MPI

  #ifdef OPS_DEBUG
  ops_register_args(args, name);
  #endif

  for (int i = 0; i<6;i++) {
    if(args[i].stencil!=NULL) {
      offs[i][0] = args[i].stencil->stride[0]*1;  //unit step in x dimension
      for(int n=1; n<ndim; n++) {
        offs[i][n] = off(ndim, n, &start[0], &end[0],
                         args[i].dat->size, args[i].stencil->stride);
      }
    }
  }

  //set up initial pointers
  for (int i = 0; i < 6; i++) {
    if (args[i].argtype == OPS_ARG_DAT) {
      int d_m[OPS_MAX_DIM];
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d] + OPS_sub_dat_list[args[i].dat->index]->d_im[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d];
  #endif //OPS_MPI
      p_a[i] = (char *)args[i].data //base of 2D array
      + address(ndim, args[i].dat->elem_size, &start[0],
        args[i].dat->size, args[i].stencil->stride, args[i].dat->base,
        d_m);
    }
    else if (args[i].argtype == OPS_ARG_GBL) {
      if (args[i].acc == OPS_READ) p_a[i] = args[i].data;
      else
  #ifdef OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data + ((ops_reduction)args[i].data)->size * block->index;
  #else //OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data;
  #endif //OPS_MPI
    } else if (args[i].argtype == OPS_ARG_IDX) {
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      p_a[i] = (char *)arg_idx;
    }
  }

  int total_range = 1;
  for (int n=0; n<ndim; n++) {
    count[n] = end[n]-start[n];  // number in each dimension
    total_range *= count[n];
    total_range *= (count[n]<0?0:1);
  }
  count[dim-1]++;     // extra in last to ensure correct termination

  if (args[0].argtype == OPS_ARG_DAT) {
    xdim0 = args[0].dat->size[0];
    multi_d0 = args[0].dat->dim;
    #ifdef OPS_3D
    ydim0 = args[0].dat->size[1];
    #endif
  }
  if (args[1].argtype == OPS_ARG_DAT) {
    xdim1 = args[1].dat->size[0];
    multi_d1 = args[1].dat->dim;
    #ifdef OPS_3D
    ydim1 = args[1].dat->size[1];
    #endif
  }
  if (args[2].argtype == OPS_ARG_DAT) {
    xdim2 = args[2].dat->size[0];
    multi_d2 = args[2].dat->dim;
    #ifdef OPS_3D
    ydim2 = args[2].dat->size[1];
    #endif
  }
  if (args[3].argtype == OPS_ARG_DAT) {
    xdim3 = args[3].dat->size[0];
    multi_d3 = args[3].dat->dim;
    #ifdef OPS_3D
    ydim3 = args[3].dat->size[1];
    #endif
  }
  if (args[4].argtype == OPS_ARG_DAT) {
    xdim4 = args[4].dat->size[0];
    multi_d4 = args[4].dat->dim;
    #ifdef OPS_3D
    ydim4 = args[4].dat->size[1];
    #endif
  }
  if (args[5].argtype == OPS_ARG_DAT) {
    xdim5 = args[5].dat->size[0];
    multi_d5 = args[5].dat->dim;
    #ifdef OPS_3D
    ydim5 = args[5].dat->size[1];
    #endif
  }

  ops_H_D_exchanges_host(args, 6);
  ops_halo_exchanges(args,6,range);
  for (int nt=0; nt<total_range; nt++) {
    // call kernel function, passing in pointers to data

    kernel(  (T0 *)p_a[0], (T1 *)p_a[1], (T2 *)p_a[2], (T3 *)p_a[3],
           (T4 *)p_a[4], (T5 *)p_a[5] );

    count[0]--;   // decrement counter
    int m = 0;    // max dimension with changed index
    while (count[m]==0) {
      count[m] =  end[m]-start[m];// reset counter
      m++;                        // next dimension
      count[m]--;                 // decrement counter
    }

    // shift pointers to data
    for (int i=0; i<6; i++) {
      if (args[i].argtype == OPS_ARG_DAT)
        p_a[i] = p_a[i] + (args[i].dat->elem_size * offs[i][m]);
      else if (args[i].argtype == OPS_ARG_IDX) {
        arg_idx[m]++;
  #ifdef OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      }
    }
  }

  #ifdef OPS_DEBUG_DUMP
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ) ops_dump3(args[0].dat,name);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ) ops_dump3(args[1].dat,name);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ) ops_dump3(args[2].dat,name);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ) ops_dump3(args[3].dat,name);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ) ops_dump3(args[4].dat,name);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ) ops_dump3(args[5].dat,name);
  #endif
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[0],range);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[1],range);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[2],range);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[3],range);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[4],range);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[5],range);
  ops_set_dirtybit_host(args, 6);
}


//
//ops_par_loop routine for 7 arguments
//
template <class T0,class T1,class T2,class T3,
class T4,class T5,class T6>
void ops_par_loop(void (*kernel)(T0*, T1*, T2*, T3*,
                           T4*, T5*, T6*),
     char const * name, ops_block block, int dim, int *range,
     ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
     ops_arg arg4, ops_arg arg5, ops_arg arg6) {

  char *p_a[7];
  int  offs[7][OPS_MAX_DIM];

  int  count[dim];
  ops_arg args[7] = { arg0, arg1, arg2, arg3,
                     arg4, arg5, arg6};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_name_before(args,7,range,name)) return;
  #endif

  int start[OPS_MAX_DIM];
  int end[OPS_MAX_DIM];

  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  //compute locally allocated range for the sub-block
  int ndim = sb->ndim;
  for (int n=0; n<ndim; n++) {
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) start[n] = 0;
    else start[n] = range[2*n] - start[n];
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) end[n] = range[2*n+1] - sb->decomp_disp[n];
    else end[n] = sb->decomp_size[n];
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else //!OPS_MPI
  int ndim = block->dims;
  for (int n=0; n<ndim; n++) {
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif //OPS_MPI

  #ifdef OPS_DEBUG
  ops_register_args(args, name);
  #endif

  for (int i = 0; i<7;i++) {
    if(args[i].stencil!=NULL) {
      offs[i][0] = args[i].stencil->stride[0]*1;  //unit step in x dimension
      for(int n=1; n<ndim; n++) {
        offs[i][n] = off(ndim, n, &start[0], &end[0],
                         args[i].dat->size, args[i].stencil->stride);
      }
    }
  }

  //set up initial pointers
  for (int i = 0; i < 7; i++) {
    if (args[i].argtype == OPS_ARG_DAT) {
      int d_m[OPS_MAX_DIM];
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d] + OPS_sub_dat_list[args[i].dat->index]->d_im[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d];
  #endif //OPS_MPI
      p_a[i] = (char *)args[i].data //base of 2D array
      + address(ndim, args[i].dat->elem_size, &start[0],
        args[i].dat->size, args[i].stencil->stride, args[i].dat->base,
        d_m);
    }
    else if (args[i].argtype == OPS_ARG_GBL) {
      if (args[i].acc == OPS_READ) p_a[i] = args[i].data;
      else
  #ifdef OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data + ((ops_reduction)args[i].data)->size * block->index;
  #else //OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data;
  #endif //OPS_MPI
    } else if (args[i].argtype == OPS_ARG_IDX) {
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      p_a[i] = (char *)arg_idx;
    }
  }

  int total_range = 1;
  for (int n=0; n<ndim; n++) {
    count[n] = end[n]-start[n];  // number in each dimension
    total_range *= count[n];
    total_range *= (count[n]<0?0:1);
  }
  count[dim-1]++;     // extra in last to ensure correct termination

  if (args[0].argtype == OPS_ARG_DAT) {
    xdim0 = args[0].dat->size[0];
    multi_d0 = args[0].dat->dim;
    #ifdef OPS_3D
    ydim0 = args[0].dat->size[1];
    #endif
  }
  if (args[1].argtype == OPS_ARG_DAT) {
    xdim1 = args[1].dat->size[0];
    multi_d1 = args[1].dat->dim;
    #ifdef OPS_3D
    ydim1 = args[1].dat->size[1];
    #endif
  }
  if (args[2].argtype == OPS_ARG_DAT) {
    xdim2 = args[2].dat->size[0];
    multi_d2 = args[2].dat->dim;
    #ifdef OPS_3D
    ydim2 = args[2].dat->size[1];
    #endif
  }
  if (args[3].argtype == OPS_ARG_DAT) {
    xdim3 = args[3].dat->size[0];
    multi_d3 = args[3].dat->dim;
    #ifdef OPS_3D
    ydim3 = args[3].dat->size[1];
    #endif
  }
  if (args[4].argtype == OPS_ARG_DAT) {
    xdim4 = args[4].dat->size[0];
    multi_d4 = args[4].dat->dim;
    #ifdef OPS_3D
    ydim4 = args[4].dat->size[1];
    #endif
  }
  if (args[5].argtype == OPS_ARG_DAT) {
    xdim5 = args[5].dat->size[0];
    multi_d5 = args[5].dat->dim;
    #ifdef OPS_3D
    ydim5 = args[5].dat->size[1];
    #endif
  }
  if (args[6].argtype == OPS_ARG_DAT) {
    xdim6 = args[6].dat->size[0];
    multi_d6 = args[6].dat->dim;
    #ifdef OPS_3D
    ydim6 = args[6].dat->size[1];
    #endif
  }

  ops_H_D_exchanges_host(args, 7);
  ops_halo_exchanges(args,7,range);
  for (int nt=0; nt<total_range; nt++) {
    // call kernel function, passing in pointers to data

    kernel(  (T0 *)p_a[0], (T1 *)p_a[1], (T2 *)p_a[2], (T3 *)p_a[3],
           (T4 *)p_a[4], (T5 *)p_a[5], (T6 *)p_a[6] );

    count[0]--;   // decrement counter
    int m = 0;    // max dimension with changed index
    while (count[m]==0) {
      count[m] =  end[m]-start[m];// reset counter
      m++;                        // next dimension
      count[m]--;                 // decrement counter
    }

    // shift pointers to data
    for (int i=0; i<7; i++) {
      if (args[i].argtype == OPS_ARG_DAT)
        p_a[i] = p_a[i] + (args[i].dat->elem_size * offs[i][m]);
      else if (args[i].argtype == OPS_ARG_IDX) {
        arg_idx[m]++;
  #ifdef OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      }
    }
  }

  #ifdef OPS_DEBUG_DUMP
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ) ops_dump3(args[0].dat,name);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ) ops_dump3(args[1].dat,name);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ) ops_dump3(args[2].dat,name);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ) ops_dump3(args[3].dat,name);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ) ops_dump3(args[4].dat,name);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ) ops_dump3(args[5].dat,name);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ) ops_dump3(args[6].dat,name);
  #endif
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[0],range);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[1],range);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[2],range);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[3],range);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[4],range);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[5],range);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[6],range);
  ops_set_dirtybit_host(args, 7);
}


//
//ops_par_loop routine for 8 arguments
//
template <class T0,class T1,class T2,class T3,
class T4,class T5,class T6,class T7>
void ops_par_loop(void (*kernel)(T0*, T1*, T2*, T3*,
                           T4*, T5*, T6*, T7*),
     char const * name, ops_block block, int dim, int *range,
     ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
     ops_arg arg4, ops_arg arg5, ops_arg arg6, ops_arg arg7) {

  char *p_a[8];
  int  offs[8][OPS_MAX_DIM];

  int  count[dim];
  ops_arg args[8] = { arg0, arg1, arg2, arg3,
                     arg4, arg5, arg6, arg7};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_name_before(args,8,range,name)) return;
  #endif

  int start[OPS_MAX_DIM];
  int end[OPS_MAX_DIM];

  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  //compute locally allocated range for the sub-block
  int ndim = sb->ndim;
  for (int n=0; n<ndim; n++) {
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) start[n] = 0;
    else start[n] = range[2*n] - start[n];
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) end[n] = range[2*n+1] - sb->decomp_disp[n];
    else end[n] = sb->decomp_size[n];
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else //!OPS_MPI
  int ndim = block->dims;
  for (int n=0; n<ndim; n++) {
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif //OPS_MPI

  #ifdef OPS_DEBUG
  ops_register_args(args, name);
  #endif

  for (int i = 0; i<8;i++) {
    if(args[i].stencil!=NULL) {
      offs[i][0] = args[i].stencil->stride[0]*1;  //unit step in x dimension
      for(int n=1; n<ndim; n++) {
        offs[i][n] = off(ndim, n, &start[0], &end[0],
                         args[i].dat->size, args[i].stencil->stride);
      }
    }
  }

  //set up initial pointers
  for (int i = 0; i < 8; i++) {
    if (args[i].argtype == OPS_ARG_DAT) {
      int d_m[OPS_MAX_DIM];
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d] + OPS_sub_dat_list[args[i].dat->index]->d_im[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d];
  #endif //OPS_MPI
      p_a[i] = (char *)args[i].data //base of 2D array
      + address(ndim, args[i].dat->elem_size, &start[0],
        args[i].dat->size, args[i].stencil->stride, args[i].dat->base,
        d_m);
    }
    else if (args[i].argtype == OPS_ARG_GBL) {
      if (args[i].acc == OPS_READ) p_a[i] = args[i].data;
      else
  #ifdef OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data + ((ops_reduction)args[i].data)->size * block->index;
  #else //OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data;
  #endif //OPS_MPI
    } else if (args[i].argtype == OPS_ARG_IDX) {
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      p_a[i] = (char *)arg_idx;
    }
  }

  int total_range = 1;
  for (int n=0; n<ndim; n++) {
    count[n] = end[n]-start[n];  // number in each dimension
    total_range *= count[n];
    total_range *= (count[n]<0?0:1);
  }
  count[dim-1]++;     // extra in last to ensure correct termination

  if (args[0].argtype == OPS_ARG_DAT) {
    xdim0 = args[0].dat->size[0];
    multi_d0 = args[0].dat->dim;
    #ifdef OPS_3D
    ydim0 = args[0].dat->size[1];
    #endif
  }
  if (args[1].argtype == OPS_ARG_DAT) {
    xdim1 = args[1].dat->size[0];
    multi_d1 = args[1].dat->dim;
    #ifdef OPS_3D
    ydim1 = args[1].dat->size[1];
    #endif
  }
  if (args[2].argtype == OPS_ARG_DAT) {
    xdim2 = args[2].dat->size[0];
    multi_d2 = args[2].dat->dim;
    #ifdef OPS_3D
    ydim2 = args[2].dat->size[1];
    #endif
  }
  if (args[3].argtype == OPS_ARG_DAT) {
    xdim3 = args[3].dat->size[0];
    multi_d3 = args[3].dat->dim;
    #ifdef OPS_3D
    ydim3 = args[3].dat->size[1];
    #endif
  }
  if (args[4].argtype == OPS_ARG_DAT) {
    xdim4 = args[4].dat->size[0];
    multi_d4 = args[4].dat->dim;
    #ifdef OPS_3D
    ydim4 = args[4].dat->size[1];
    #endif
  }
  if (args[5].argtype == OPS_ARG_DAT) {
    xdim5 = args[5].dat->size[0];
    multi_d5 = args[5].dat->dim;
    #ifdef OPS_3D
    ydim5 = args[5].dat->size[1];
    #endif
  }
  if (args[6].argtype == OPS_ARG_DAT) {
    xdim6 = args[6].dat->size[0];
    multi_d6 = args[6].dat->dim;
    #ifdef OPS_3D
    ydim6 = args[6].dat->size[1];
    #endif
  }
  if (args[7].argtype == OPS_ARG_DAT) {
    xdim7 = args[7].dat->size[0];
    multi_d7 = args[7].dat->dim;
    #ifdef OPS_3D
    ydim7 = args[7].dat->size[1];
    #endif
  }

  ops_H_D_exchanges_host(args, 8);
  ops_halo_exchanges(args,8,range);
  for (int nt=0; nt<total_range; nt++) {
    // call kernel function, passing in pointers to data

    kernel(  (T0 *)p_a[0], (T1 *)p_a[1], (T2 *)p_a[2], (T3 *)p_a[3],
           (T4 *)p_a[4], (T5 *)p_a[5], (T6 *)p_a[6], (T7 *)p_a[7] );

    count[0]--;   // decrement counter
    int m = 0;    // max dimension with changed index
    while (count[m]==0) {
      count[m] =  end[m]-start[m];// reset counter
      m++;                        // next dimension
      count[m]--;                 // decrement counter
    }

    // shift pointers to data
    for (int i=0; i<8; i++) {
      if (args[i].argtype == OPS_ARG_DAT)
        p_a[i] = p_a[i] + (args[i].dat->elem_size * offs[i][m]);
      else if (args[i].argtype == OPS_ARG_IDX) {
        arg_idx[m]++;
  #ifdef OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      }
    }
  }

  #ifdef OPS_DEBUG_DUMP
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ) ops_dump3(args[0].dat,name);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ) ops_dump3(args[1].dat,name);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ) ops_dump3(args[2].dat,name);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ) ops_dump3(args[3].dat,name);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ) ops_dump3(args[4].dat,name);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ) ops_dump3(args[5].dat,name);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ) ops_dump3(args[6].dat,name);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ) ops_dump3(args[7].dat,name);
  #endif
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[0],range);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[1],range);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[2],range);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[3],range);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[4],range);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[5],range);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[6],range);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[7],range);
  ops_set_dirtybit_host(args, 8);
}


//
//ops_par_loop routine for 9 arguments
//
template <class T0,class T1,class T2,class T3,
class T4,class T5,class T6,class T7,
class T8>
void ops_par_loop(void (*kernel)(T0*, T1*, T2*, T3*,
                           T4*, T5*, T6*, T7*,
                           T8*),
     char const * name, ops_block block, int dim, int *range,
     ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
     ops_arg arg4, ops_arg arg5, ops_arg arg6, ops_arg arg7,
     ops_arg arg8) {

  char *p_a[9];
  int  offs[9][OPS_MAX_DIM];

  int  count[dim];
  ops_arg args[9] = { arg0, arg1, arg2, arg3,
                     arg4, arg5, arg6, arg7,
                     arg8};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_name_before(args,9,range,name)) return;
  #endif

  int start[OPS_MAX_DIM];
  int end[OPS_MAX_DIM];

  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  //compute locally allocated range for the sub-block
  int ndim = sb->ndim;
  for (int n=0; n<ndim; n++) {
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) start[n] = 0;
    else start[n] = range[2*n] - start[n];
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) end[n] = range[2*n+1] - sb->decomp_disp[n];
    else end[n] = sb->decomp_size[n];
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else //!OPS_MPI
  int ndim = block->dims;
  for (int n=0; n<ndim; n++) {
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif //OPS_MPI

  #ifdef OPS_DEBUG
  ops_register_args(args, name);
  #endif

  for (int i = 0; i<9;i++) {
    if(args[i].stencil!=NULL) {
      offs[i][0] = args[i].stencil->stride[0]*1;  //unit step in x dimension
      for(int n=1; n<ndim; n++) {
        offs[i][n] = off(ndim, n, &start[0], &end[0],
                         args[i].dat->size, args[i].stencil->stride);
      }
    }
  }

  //set up initial pointers
  for (int i = 0; i < 9; i++) {
    if (args[i].argtype == OPS_ARG_DAT) {
      int d_m[OPS_MAX_DIM];
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d] + OPS_sub_dat_list[args[i].dat->index]->d_im[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d];
  #endif //OPS_MPI
      p_a[i] = (char *)args[i].data //base of 2D array
      + address(ndim, args[i].dat->elem_size, &start[0],
        args[i].dat->size, args[i].stencil->stride, args[i].dat->base,
        d_m);
    }
    else if (args[i].argtype == OPS_ARG_GBL) {
      if (args[i].acc == OPS_READ) p_a[i] = args[i].data;
      else
  #ifdef OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data + ((ops_reduction)args[i].data)->size * block->index;
  #else //OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data;
  #endif //OPS_MPI
    } else if (args[i].argtype == OPS_ARG_IDX) {
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      p_a[i] = (char *)arg_idx;
    }
  }

  int total_range = 1;
  for (int n=0; n<ndim; n++) {
    count[n] = end[n]-start[n];  // number in each dimension
    total_range *= count[n];
    total_range *= (count[n]<0?0:1);
  }
  count[dim-1]++;     // extra in last to ensure correct termination

  if (args[0].argtype == OPS_ARG_DAT) {
    xdim0 = args[0].dat->size[0];
    multi_d0 = args[0].dat->dim;
    #ifdef OPS_3D
    ydim0 = args[0].dat->size[1];
    #endif
  }
  if (args[1].argtype == OPS_ARG_DAT) {
    xdim1 = args[1].dat->size[0];
    multi_d1 = args[1].dat->dim;
    #ifdef OPS_3D
    ydim1 = args[1].dat->size[1];
    #endif
  }
  if (args[2].argtype == OPS_ARG_DAT) {
    xdim2 = args[2].dat->size[0];
    multi_d2 = args[2].dat->dim;
    #ifdef OPS_3D
    ydim2 = args[2].dat->size[1];
    #endif
  }
  if (args[3].argtype == OPS_ARG_DAT) {
    xdim3 = args[3].dat->size[0];
    multi_d3 = args[3].dat->dim;
    #ifdef OPS_3D
    ydim3 = args[3].dat->size[1];
    #endif
  }
  if (args[4].argtype == OPS_ARG_DAT) {
    xdim4 = args[4].dat->size[0];
    multi_d4 = args[4].dat->dim;
    #ifdef OPS_3D
    ydim4 = args[4].dat->size[1];
    #endif
  }
  if (args[5].argtype == OPS_ARG_DAT) {
    xdim5 = args[5].dat->size[0];
    multi_d5 = args[5].dat->dim;
    #ifdef OPS_3D
    ydim5 = args[5].dat->size[1];
    #endif
  }
  if (args[6].argtype == OPS_ARG_DAT) {
    xdim6 = args[6].dat->size[0];
    multi_d6 = args[6].dat->dim;
    #ifdef OPS_3D
    ydim6 = args[6].dat->size[1];
    #endif
  }
  if (args[7].argtype == OPS_ARG_DAT) {
    xdim7 = args[7].dat->size[0];
    multi_d7 = args[7].dat->dim;
    #ifdef OPS_3D
    ydim7 = args[7].dat->size[1];
    #endif
  }
  if (args[8].argtype == OPS_ARG_DAT) {
    xdim8 = args[8].dat->size[0];
    multi_d8 = args[8].dat->dim;
    #ifdef OPS_3D
    ydim8 = args[8].dat->size[1];
    #endif
  }

  ops_H_D_exchanges_host(args, 9);
  ops_halo_exchanges(args,9,range);
  for (int nt=0; nt<total_range; nt++) {
    // call kernel function, passing in pointers to data

    kernel(  (T0 *)p_a[0], (T1 *)p_a[1], (T2 *)p_a[2], (T3 *)p_a[3],
           (T4 *)p_a[4], (T5 *)p_a[5], (T6 *)p_a[6], (T7 *)p_a[7],
           (T8 *)p_a[8] );

    count[0]--;   // decrement counter
    int m = 0;    // max dimension with changed index
    while (count[m]==0) {
      count[m] =  end[m]-start[m];// reset counter
      m++;                        // next dimension
      count[m]--;                 // decrement counter
    }

    // shift pointers to data
    for (int i=0; i<9; i++) {
      if (args[i].argtype == OPS_ARG_DAT)
        p_a[i] = p_a[i] + (args[i].dat->elem_size * offs[i][m]);
      else if (args[i].argtype == OPS_ARG_IDX) {
        arg_idx[m]++;
  #ifdef OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      }
    }
  }

  #ifdef OPS_DEBUG_DUMP
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ) ops_dump3(args[0].dat,name);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ) ops_dump3(args[1].dat,name);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ) ops_dump3(args[2].dat,name);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ) ops_dump3(args[3].dat,name);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ) ops_dump3(args[4].dat,name);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ) ops_dump3(args[5].dat,name);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ) ops_dump3(args[6].dat,name);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ) ops_dump3(args[7].dat,name);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ) ops_dump3(args[8].dat,name);
  #endif
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[0],range);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[1],range);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[2],range);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[3],range);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[4],range);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[5],range);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[6],range);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[7],range);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[8],range);
  ops_set_dirtybit_host(args, 9);
}


//
//ops_par_loop routine for 10 arguments
//
template <class T0,class T1,class T2,class T3,
class T4,class T5,class T6,class T7,
class T8,class T9>
void ops_par_loop(void (*kernel)(T0*, T1*, T2*, T3*,
                           T4*, T5*, T6*, T7*,
                           T8*, T9*),
     char const * name, ops_block block, int dim, int *range,
     ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
     ops_arg arg4, ops_arg arg5, ops_arg arg6, ops_arg arg7,
     ops_arg arg8, ops_arg arg9) {

  char *p_a[10];
  int  offs[10][OPS_MAX_DIM];

  int  count[dim];
  ops_arg args[10] = { arg0, arg1, arg2, arg3,
                     arg4, arg5, arg6, arg7,
                     arg8, arg9};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_name_before(args,10,range,name)) return;
  #endif

  int start[OPS_MAX_DIM];
  int end[OPS_MAX_DIM];

  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  //compute locally allocated range for the sub-block
  int ndim = sb->ndim;
  for (int n=0; n<ndim; n++) {
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) start[n] = 0;
    else start[n] = range[2*n] - start[n];
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) end[n] = range[2*n+1] - sb->decomp_disp[n];
    else end[n] = sb->decomp_size[n];
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else //!OPS_MPI
  int ndim = block->dims;
  for (int n=0; n<ndim; n++) {
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif //OPS_MPI

  #ifdef OPS_DEBUG
  ops_register_args(args, name);
  #endif

  for (int i = 0; i<10;i++) {
    if(args[i].stencil!=NULL) {
      offs[i][0] = args[i].stencil->stride[0]*1;  //unit step in x dimension
      for(int n=1; n<ndim; n++) {
        offs[i][n] = off(ndim, n, &start[0], &end[0],
                         args[i].dat->size, args[i].stencil->stride);
      }
    }
  }

  //set up initial pointers
  for (int i = 0; i < 10; i++) {
    if (args[i].argtype == OPS_ARG_DAT) {
      int d_m[OPS_MAX_DIM];
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d] + OPS_sub_dat_list[args[i].dat->index]->d_im[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d];
  #endif //OPS_MPI
      p_a[i] = (char *)args[i].data //base of 2D array
      + address(ndim, args[i].dat->elem_size, &start[0],
        args[i].dat->size, args[i].stencil->stride, args[i].dat->base,
        d_m);
    }
    else if (args[i].argtype == OPS_ARG_GBL) {
      if (args[i].acc == OPS_READ) p_a[i] = args[i].data;
      else
  #ifdef OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data + ((ops_reduction)args[i].data)->size * block->index;
  #else //OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data;
  #endif //OPS_MPI
    } else if (args[i].argtype == OPS_ARG_IDX) {
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      p_a[i] = (char *)arg_idx;
    }
  }

  int total_range = 1;
  for (int n=0; n<ndim; n++) {
    count[n] = end[n]-start[n];  // number in each dimension
    total_range *= count[n];
    total_range *= (count[n]<0?0:1);
  }
  count[dim-1]++;     // extra in last to ensure correct termination

  if (args[0].argtype == OPS_ARG_DAT) {
    xdim0 = args[0].dat->size[0];
    multi_d0 = args[0].dat->dim;
    #ifdef OPS_3D
    ydim0 = args[0].dat->size[1];
    #endif
  }
  if (args[1].argtype == OPS_ARG_DAT) {
    xdim1 = args[1].dat->size[0];
    multi_d1 = args[1].dat->dim;
    #ifdef OPS_3D
    ydim1 = args[1].dat->size[1];
    #endif
  }
  if (args[2].argtype == OPS_ARG_DAT) {
    xdim2 = args[2].dat->size[0];
    multi_d2 = args[2].dat->dim;
    #ifdef OPS_3D
    ydim2 = args[2].dat->size[1];
    #endif
  }
  if (args[3].argtype == OPS_ARG_DAT) {
    xdim3 = args[3].dat->size[0];
    multi_d3 = args[3].dat->dim;
    #ifdef OPS_3D
    ydim3 = args[3].dat->size[1];
    #endif
  }
  if (args[4].argtype == OPS_ARG_DAT) {
    xdim4 = args[4].dat->size[0];
    multi_d4 = args[4].dat->dim;
    #ifdef OPS_3D
    ydim4 = args[4].dat->size[1];
    #endif
  }
  if (args[5].argtype == OPS_ARG_DAT) {
    xdim5 = args[5].dat->size[0];
    multi_d5 = args[5].dat->dim;
    #ifdef OPS_3D
    ydim5 = args[5].dat->size[1];
    #endif
  }
  if (args[6].argtype == OPS_ARG_DAT) {
    xdim6 = args[6].dat->size[0];
    multi_d6 = args[6].dat->dim;
    #ifdef OPS_3D
    ydim6 = args[6].dat->size[1];
    #endif
  }
  if (args[7].argtype == OPS_ARG_DAT) {
    xdim7 = args[7].dat->size[0];
    multi_d7 = args[7].dat->dim;
    #ifdef OPS_3D
    ydim7 = args[7].dat->size[1];
    #endif
  }
  if (args[8].argtype == OPS_ARG_DAT) {
    xdim8 = args[8].dat->size[0];
    multi_d8 = args[8].dat->dim;
    #ifdef OPS_3D
    ydim8 = args[8].dat->size[1];
    #endif
  }
  if (args[9].argtype == OPS_ARG_DAT) {
    xdim9 = args[9].dat->size[0];
    multi_d9 = args[9].dat->dim;
    #ifdef OPS_3D
    ydim9 = args[9].dat->size[1];
    #endif
  }

  ops_H_D_exchanges_host(args, 10);
  ops_halo_exchanges(args,10,range);
  for (int nt=0; nt<total_range; nt++) {
    // call kernel function, passing in pointers to data

    kernel(  (T0 *)p_a[0], (T1 *)p_a[1], (T2 *)p_a[2], (T3 *)p_a[3],
           (T4 *)p_a[4], (T5 *)p_a[5], (T6 *)p_a[6], (T7 *)p_a[7],
           (T8 *)p_a[8], (T9 *)p_a[9] );

    count[0]--;   // decrement counter
    int m = 0;    // max dimension with changed index
    while (count[m]==0) {
      count[m] =  end[m]-start[m];// reset counter
      m++;                        // next dimension
      count[m]--;                 // decrement counter
    }

    // shift pointers to data
    for (int i=0; i<10; i++) {
      if (args[i].argtype == OPS_ARG_DAT)
        p_a[i] = p_a[i] + (args[i].dat->elem_size * offs[i][m]);
      else if (args[i].argtype == OPS_ARG_IDX) {
        arg_idx[m]++;
  #ifdef OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      }
    }
  }

  #ifdef OPS_DEBUG_DUMP
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ) ops_dump3(args[0].dat,name);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ) ops_dump3(args[1].dat,name);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ) ops_dump3(args[2].dat,name);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ) ops_dump3(args[3].dat,name);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ) ops_dump3(args[4].dat,name);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ) ops_dump3(args[5].dat,name);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ) ops_dump3(args[6].dat,name);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ) ops_dump3(args[7].dat,name);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ) ops_dump3(args[8].dat,name);
  if (args[9].argtype == OPS_ARG_DAT && args[9].acc != OPS_READ) ops_dump3(args[9].dat,name);
  #endif
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[0],range);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[1],range);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[2],range);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[3],range);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[4],range);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[5],range);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[6],range);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[7],range);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[8],range);
  if (args[9].argtype == OPS_ARG_DAT && args[9].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[9],range);
  ops_set_dirtybit_host(args, 10);
}


//
//ops_par_loop routine for 11 arguments
//
template <class T0,class T1,class T2,class T3,
class T4,class T5,class T6,class T7,
class T8,class T9,class T10>
void ops_par_loop(void (*kernel)(T0*, T1*, T2*, T3*,
                           T4*, T5*, T6*, T7*,
                           T8*, T9*, T10*),
     char const * name, ops_block block, int dim, int *range,
     ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
     ops_arg arg4, ops_arg arg5, ops_arg arg6, ops_arg arg7,
     ops_arg arg8, ops_arg arg9, ops_arg arg10) {

  char *p_a[11];
  int  offs[11][OPS_MAX_DIM];

  int  count[dim];
  ops_arg args[11] = { arg0, arg1, arg2, arg3,
                     arg4, arg5, arg6, arg7,
                     arg8, arg9, arg10};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_name_before(args,11,range,name)) return;
  #endif

  int start[OPS_MAX_DIM];
  int end[OPS_MAX_DIM];

  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  //compute locally allocated range for the sub-block
  int ndim = sb->ndim;
  for (int n=0; n<ndim; n++) {
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) start[n] = 0;
    else start[n] = range[2*n] - start[n];
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) end[n] = range[2*n+1] - sb->decomp_disp[n];
    else end[n] = sb->decomp_size[n];
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else //!OPS_MPI
  int ndim = block->dims;
  for (int n=0; n<ndim; n++) {
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif //OPS_MPI

  #ifdef OPS_DEBUG
  ops_register_args(args, name);
  #endif

  for (int i = 0; i<11;i++) {
    if(args[i].stencil!=NULL) {
      offs[i][0] = args[i].stencil->stride[0]*1;  //unit step in x dimension
      for(int n=1; n<ndim; n++) {
        offs[i][n] = off(ndim, n, &start[0], &end[0],
                         args[i].dat->size, args[i].stencil->stride);
      }
    }
  }

  //set up initial pointers
  for (int i = 0; i < 11; i++) {
    if (args[i].argtype == OPS_ARG_DAT) {
      int d_m[OPS_MAX_DIM];
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d] + OPS_sub_dat_list[args[i].dat->index]->d_im[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d];
  #endif //OPS_MPI
      p_a[i] = (char *)args[i].data //base of 2D array
      + address(ndim, args[i].dat->elem_size, &start[0],
        args[i].dat->size, args[i].stencil->stride, args[i].dat->base,
        d_m);
    }
    else if (args[i].argtype == OPS_ARG_GBL) {
      if (args[i].acc == OPS_READ) p_a[i] = args[i].data;
      else
  #ifdef OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data + ((ops_reduction)args[i].data)->size * block->index;
  #else //OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data;
  #endif //OPS_MPI
    } else if (args[i].argtype == OPS_ARG_IDX) {
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      p_a[i] = (char *)arg_idx;
    }
  }

  int total_range = 1;
  for (int n=0; n<ndim; n++) {
    count[n] = end[n]-start[n];  // number in each dimension
    total_range *= count[n];
    total_range *= (count[n]<0?0:1);
  }
  count[dim-1]++;     // extra in last to ensure correct termination

  if (args[0].argtype == OPS_ARG_DAT) {
    xdim0 = args[0].dat->size[0];
    multi_d0 = args[0].dat->dim;
    #ifdef OPS_3D
    ydim0 = args[0].dat->size[1];
    #endif
  }
  if (args[1].argtype == OPS_ARG_DAT) {
    xdim1 = args[1].dat->size[0];
    multi_d1 = args[1].dat->dim;
    #ifdef OPS_3D
    ydim1 = args[1].dat->size[1];
    #endif
  }
  if (args[2].argtype == OPS_ARG_DAT) {
    xdim2 = args[2].dat->size[0];
    multi_d2 = args[2].dat->dim;
    #ifdef OPS_3D
    ydim2 = args[2].dat->size[1];
    #endif
  }
  if (args[3].argtype == OPS_ARG_DAT) {
    xdim3 = args[3].dat->size[0];
    multi_d3 = args[3].dat->dim;
    #ifdef OPS_3D
    ydim3 = args[3].dat->size[1];
    #endif
  }
  if (args[4].argtype == OPS_ARG_DAT) {
    xdim4 = args[4].dat->size[0];
    multi_d4 = args[4].dat->dim;
    #ifdef OPS_3D
    ydim4 = args[4].dat->size[1];
    #endif
  }
  if (args[5].argtype == OPS_ARG_DAT) {
    xdim5 = args[5].dat->size[0];
    multi_d5 = args[5].dat->dim;
    #ifdef OPS_3D
    ydim5 = args[5].dat->size[1];
    #endif
  }
  if (args[6].argtype == OPS_ARG_DAT) {
    xdim6 = args[6].dat->size[0];
    multi_d6 = args[6].dat->dim;
    #ifdef OPS_3D
    ydim6 = args[6].dat->size[1];
    #endif
  }
  if (args[7].argtype == OPS_ARG_DAT) {
    xdim7 = args[7].dat->size[0];
    multi_d7 = args[7].dat->dim;
    #ifdef OPS_3D
    ydim7 = args[7].dat->size[1];
    #endif
  }
  if (args[8].argtype == OPS_ARG_DAT) {
    xdim8 = args[8].dat->size[0];
    multi_d8 = args[8].dat->dim;
    #ifdef OPS_3D
    ydim8 = args[8].dat->size[1];
    #endif
  }
  if (args[9].argtype == OPS_ARG_DAT) {
    xdim9 = args[9].dat->size[0];
    multi_d9 = args[9].dat->dim;
    #ifdef OPS_3D
    ydim9 = args[9].dat->size[1];
    #endif
  }
  if (args[10].argtype == OPS_ARG_DAT) {
    xdim10 = args[10].dat->size[0];
    multi_d10 = args[10].dat->dim;
    #ifdef OPS_3D
    ydim10 = args[10].dat->size[1];
    #endif
  }

  ops_H_D_exchanges_host(args, 11);
  ops_halo_exchanges(args,11,range);
  for (int nt=0; nt<total_range; nt++) {
    // call kernel function, passing in pointers to data

    kernel(  (T0 *)p_a[0], (T1 *)p_a[1], (T2 *)p_a[2], (T3 *)p_a[3],
           (T4 *)p_a[4], (T5 *)p_a[5], (T6 *)p_a[6], (T7 *)p_a[7],
           (T8 *)p_a[8], (T9 *)p_a[9], (T10 *)p_a[10] );

    count[0]--;   // decrement counter
    int m = 0;    // max dimension with changed index
    while (count[m]==0) {
      count[m] =  end[m]-start[m];// reset counter
      m++;                        // next dimension
      count[m]--;                 // decrement counter
    }

    // shift pointers to data
    for (int i=0; i<11; i++) {
      if (args[i].argtype == OPS_ARG_DAT)
        p_a[i] = p_a[i] + (args[i].dat->elem_size * offs[i][m]);
      else if (args[i].argtype == OPS_ARG_IDX) {
        arg_idx[m]++;
  #ifdef OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      }
    }
  }

  #ifdef OPS_DEBUG_DUMP
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ) ops_dump3(args[0].dat,name);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ) ops_dump3(args[1].dat,name);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ) ops_dump3(args[2].dat,name);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ) ops_dump3(args[3].dat,name);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ) ops_dump3(args[4].dat,name);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ) ops_dump3(args[5].dat,name);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ) ops_dump3(args[6].dat,name);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ) ops_dump3(args[7].dat,name);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ) ops_dump3(args[8].dat,name);
  if (args[9].argtype == OPS_ARG_DAT && args[9].acc != OPS_READ) ops_dump3(args[9].dat,name);
  if (args[10].argtype == OPS_ARG_DAT && args[10].acc != OPS_READ) ops_dump3(args[10].dat,name);
  #endif
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[0],range);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[1],range);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[2],range);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[3],range);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[4],range);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[5],range);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[6],range);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[7],range);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[8],range);
  if (args[9].argtype == OPS_ARG_DAT && args[9].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[9],range);
  if (args[10].argtype == OPS_ARG_DAT && args[10].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[10],range);
  ops_set_dirtybit_host(args, 11);
}


//
//ops_par_loop routine for 12 arguments
//
template <class T0,class T1,class T2,class T3,
class T4,class T5,class T6,class T7,
class T8,class T9,class T10,class T11>
void ops_par_loop(void (*kernel)(T0*, T1*, T2*, T3*,
                           T4*, T5*, T6*, T7*,
                           T8*, T9*, T10*, T11*),
     char const * name, ops_block block, int dim, int *range,
     ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
     ops_arg arg4, ops_arg arg5, ops_arg arg6, ops_arg arg7,
     ops_arg arg8, ops_arg arg9, ops_arg arg10, ops_arg arg11) {

  char *p_a[12];
  int  offs[12][OPS_MAX_DIM];

  int  count[dim];
  ops_arg args[12] = { arg0, arg1, arg2, arg3,
                     arg4, arg5, arg6, arg7,
                     arg8, arg9, arg10, arg11};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_name_before(args,12,range,name)) return;
  #endif

  int start[OPS_MAX_DIM];
  int end[OPS_MAX_DIM];

  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  //compute locally allocated range for the sub-block
  int ndim = sb->ndim;
  for (int n=0; n<ndim; n++) {
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) start[n] = 0;
    else start[n] = range[2*n] - start[n];
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) end[n] = range[2*n+1] - sb->decomp_disp[n];
    else end[n] = sb->decomp_size[n];
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else //!OPS_MPI
  int ndim = block->dims;
  for (int n=0; n<ndim; n++) {
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif //OPS_MPI

  #ifdef OPS_DEBUG
  ops_register_args(args, name);
  #endif

  for (int i = 0; i<12;i++) {
    if(args[i].stencil!=NULL) {
      offs[i][0] = args[i].stencil->stride[0]*1;  //unit step in x dimension
      for(int n=1; n<ndim; n++) {
        offs[i][n] = off(ndim, n, &start[0], &end[0],
                         args[i].dat->size, args[i].stencil->stride);
      }
    }
  }

  //set up initial pointers
  for (int i = 0; i < 12; i++) {
    if (args[i].argtype == OPS_ARG_DAT) {
      int d_m[OPS_MAX_DIM];
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d] + OPS_sub_dat_list[args[i].dat->index]->d_im[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d];
  #endif //OPS_MPI
      p_a[i] = (char *)args[i].data //base of 2D array
      + address(ndim, args[i].dat->elem_size, &start[0],
        args[i].dat->size, args[i].stencil->stride, args[i].dat->base,
        d_m);
    }
    else if (args[i].argtype == OPS_ARG_GBL) {
      if (args[i].acc == OPS_READ) p_a[i] = args[i].data;
      else
  #ifdef OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data + ((ops_reduction)args[i].data)->size * block->index;
  #else //OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data;
  #endif //OPS_MPI
    } else if (args[i].argtype == OPS_ARG_IDX) {
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      p_a[i] = (char *)arg_idx;
    }
  }

  int total_range = 1;
  for (int n=0; n<ndim; n++) {
    count[n] = end[n]-start[n];  // number in each dimension
    total_range *= count[n];
    total_range *= (count[n]<0?0:1);
  }
  count[dim-1]++;     // extra in last to ensure correct termination

  if (args[0].argtype == OPS_ARG_DAT) {
    xdim0 = args[0].dat->size[0];
    multi_d0 = args[0].dat->dim;
    #ifdef OPS_3D
    ydim0 = args[0].dat->size[1];
    #endif
  }
  if (args[1].argtype == OPS_ARG_DAT) {
    xdim1 = args[1].dat->size[0];
    multi_d1 = args[1].dat->dim;
    #ifdef OPS_3D
    ydim1 = args[1].dat->size[1];
    #endif
  }
  if (args[2].argtype == OPS_ARG_DAT) {
    xdim2 = args[2].dat->size[0];
    multi_d2 = args[2].dat->dim;
    #ifdef OPS_3D
    ydim2 = args[2].dat->size[1];
    #endif
  }
  if (args[3].argtype == OPS_ARG_DAT) {
    xdim3 = args[3].dat->size[0];
    multi_d3 = args[3].dat->dim;
    #ifdef OPS_3D
    ydim3 = args[3].dat->size[1];
    #endif
  }
  if (args[4].argtype == OPS_ARG_DAT) {
    xdim4 = args[4].dat->size[0];
    multi_d4 = args[4].dat->dim;
    #ifdef OPS_3D
    ydim4 = args[4].dat->size[1];
    #endif
  }
  if (args[5].argtype == OPS_ARG_DAT) {
    xdim5 = args[5].dat->size[0];
    multi_d5 = args[5].dat->dim;
    #ifdef OPS_3D
    ydim5 = args[5].dat->size[1];
    #endif
  }
  if (args[6].argtype == OPS_ARG_DAT) {
    xdim6 = args[6].dat->size[0];
    multi_d6 = args[6].dat->dim;
    #ifdef OPS_3D
    ydim6 = args[6].dat->size[1];
    #endif
  }
  if (args[7].argtype == OPS_ARG_DAT) {
    xdim7 = args[7].dat->size[0];
    multi_d7 = args[7].dat->dim;
    #ifdef OPS_3D
    ydim7 = args[7].dat->size[1];
    #endif
  }
  if (args[8].argtype == OPS_ARG_DAT) {
    xdim8 = args[8].dat->size[0];
    multi_d8 = args[8].dat->dim;
    #ifdef OPS_3D
    ydim8 = args[8].dat->size[1];
    #endif
  }
  if (args[9].argtype == OPS_ARG_DAT) {
    xdim9 = args[9].dat->size[0];
    multi_d9 = args[9].dat->dim;
    #ifdef OPS_3D
    ydim9 = args[9].dat->size[1];
    #endif
  }
  if (args[10].argtype == OPS_ARG_DAT) {
    xdim10 = args[10].dat->size[0];
    multi_d10 = args[10].dat->dim;
    #ifdef OPS_3D
    ydim10 = args[10].dat->size[1];
    #endif
  }
  if (args[11].argtype == OPS_ARG_DAT) {
    xdim11 = args[11].dat->size[0];
    multi_d11 = args[11].dat->dim;
    #ifdef OPS_3D
    ydim11 = args[11].dat->size[1];
    #endif
  }

  ops_H_D_exchanges_host(args, 12);
  ops_halo_exchanges(args,12,range);
  for (int nt=0; nt<total_range; nt++) {
    // call kernel function, passing in pointers to data

    kernel(  (T0 *)p_a[0], (T1 *)p_a[1], (T2 *)p_a[2], (T3 *)p_a[3],
           (T4 *)p_a[4], (T5 *)p_a[5], (T6 *)p_a[6], (T7 *)p_a[7],
           (T8 *)p_a[8], (T9 *)p_a[9], (T10 *)p_a[10], (T11 *)p_a[11] );

    count[0]--;   // decrement counter
    int m = 0;    // max dimension with changed index
    while (count[m]==0) {
      count[m] =  end[m]-start[m];// reset counter
      m++;                        // next dimension
      count[m]--;                 // decrement counter
    }

    // shift pointers to data
    for (int i=0; i<12; i++) {
      if (args[i].argtype == OPS_ARG_DAT)
        p_a[i] = p_a[i] + (args[i].dat->elem_size * offs[i][m]);
      else if (args[i].argtype == OPS_ARG_IDX) {
        arg_idx[m]++;
  #ifdef OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      }
    }
  }

  #ifdef OPS_DEBUG_DUMP
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ) ops_dump3(args[0].dat,name);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ) ops_dump3(args[1].dat,name);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ) ops_dump3(args[2].dat,name);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ) ops_dump3(args[3].dat,name);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ) ops_dump3(args[4].dat,name);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ) ops_dump3(args[5].dat,name);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ) ops_dump3(args[6].dat,name);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ) ops_dump3(args[7].dat,name);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ) ops_dump3(args[8].dat,name);
  if (args[9].argtype == OPS_ARG_DAT && args[9].acc != OPS_READ) ops_dump3(args[9].dat,name);
  if (args[10].argtype == OPS_ARG_DAT && args[10].acc != OPS_READ) ops_dump3(args[10].dat,name);
  if (args[11].argtype == OPS_ARG_DAT && args[11].acc != OPS_READ) ops_dump3(args[11].dat,name);
  #endif
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[0],range);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[1],range);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[2],range);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[3],range);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[4],range);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[5],range);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[6],range);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[7],range);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[8],range);
  if (args[9].argtype == OPS_ARG_DAT && args[9].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[9],range);
  if (args[10].argtype == OPS_ARG_DAT && args[10].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[10],range);
  if (args[11].argtype == OPS_ARG_DAT && args[11].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[11],range);
  ops_set_dirtybit_host(args, 12);
}


//
//ops_par_loop routine for 13 arguments
//
template <class T0,class T1,class T2,class T3,
class T4,class T5,class T6,class T7,
class T8,class T9,class T10,class T11,
class T12>
void ops_par_loop(void (*kernel)(T0*, T1*, T2*, T3*,
                           T4*, T5*, T6*, T7*,
                           T8*, T9*, T10*, T11*,
                           T12*),
     char const * name, ops_block block, int dim, int *range,
     ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
     ops_arg arg4, ops_arg arg5, ops_arg arg6, ops_arg arg7,
     ops_arg arg8, ops_arg arg9, ops_arg arg10, ops_arg arg11,
     ops_arg arg12) {

  char *p_a[13];
  int  offs[13][OPS_MAX_DIM];

  int  count[dim];
  ops_arg args[13] = { arg0, arg1, arg2, arg3,
                     arg4, arg5, arg6, arg7,
                     arg8, arg9, arg10, arg11,
                     arg12};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_name_before(args,13,range,name)) return;
  #endif

  int start[OPS_MAX_DIM];
  int end[OPS_MAX_DIM];

  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  //compute locally allocated range for the sub-block
  int ndim = sb->ndim;
  for (int n=0; n<ndim; n++) {
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) start[n] = 0;
    else start[n] = range[2*n] - start[n];
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) end[n] = range[2*n+1] - sb->decomp_disp[n];
    else end[n] = sb->decomp_size[n];
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else //!OPS_MPI
  int ndim = block->dims;
  for (int n=0; n<ndim; n++) {
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif //OPS_MPI

  #ifdef OPS_DEBUG
  ops_register_args(args, name);
  #endif

  for (int i = 0; i<13;i++) {
    if(args[i].stencil!=NULL) {
      offs[i][0] = args[i].stencil->stride[0]*1;  //unit step in x dimension
      for(int n=1; n<ndim; n++) {
        offs[i][n] = off(ndim, n, &start[0], &end[0],
                         args[i].dat->size, args[i].stencil->stride);
      }
    }
  }

  //set up initial pointers
  for (int i = 0; i < 13; i++) {
    if (args[i].argtype == OPS_ARG_DAT) {
      int d_m[OPS_MAX_DIM];
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d] + OPS_sub_dat_list[args[i].dat->index]->d_im[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d];
  #endif //OPS_MPI
      p_a[i] = (char *)args[i].data //base of 2D array
      + address(ndim, args[i].dat->elem_size, &start[0],
        args[i].dat->size, args[i].stencil->stride, args[i].dat->base,
        d_m);
    }
    else if (args[i].argtype == OPS_ARG_GBL) {
      if (args[i].acc == OPS_READ) p_a[i] = args[i].data;
      else
  #ifdef OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data + ((ops_reduction)args[i].data)->size * block->index;
  #else //OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data;
  #endif //OPS_MPI
    } else if (args[i].argtype == OPS_ARG_IDX) {
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      p_a[i] = (char *)arg_idx;
    }
  }

  int total_range = 1;
  for (int n=0; n<ndim; n++) {
    count[n] = end[n]-start[n];  // number in each dimension
    total_range *= count[n];
    total_range *= (count[n]<0?0:1);
  }
  count[dim-1]++;     // extra in last to ensure correct termination

  if (args[0].argtype == OPS_ARG_DAT) {
    xdim0 = args[0].dat->size[0];
    multi_d0 = args[0].dat->dim;
    #ifdef OPS_3D
    ydim0 = args[0].dat->size[1];
    #endif
  }
  if (args[1].argtype == OPS_ARG_DAT) {
    xdim1 = args[1].dat->size[0];
    multi_d1 = args[1].dat->dim;
    #ifdef OPS_3D
    ydim1 = args[1].dat->size[1];
    #endif
  }
  if (args[2].argtype == OPS_ARG_DAT) {
    xdim2 = args[2].dat->size[0];
    multi_d2 = args[2].dat->dim;
    #ifdef OPS_3D
    ydim2 = args[2].dat->size[1];
    #endif
  }
  if (args[3].argtype == OPS_ARG_DAT) {
    xdim3 = args[3].dat->size[0];
    multi_d3 = args[3].dat->dim;
    #ifdef OPS_3D
    ydim3 = args[3].dat->size[1];
    #endif
  }
  if (args[4].argtype == OPS_ARG_DAT) {
    xdim4 = args[4].dat->size[0];
    multi_d4 = args[4].dat->dim;
    #ifdef OPS_3D
    ydim4 = args[4].dat->size[1];
    #endif
  }
  if (args[5].argtype == OPS_ARG_DAT) {
    xdim5 = args[5].dat->size[0];
    multi_d5 = args[5].dat->dim;
    #ifdef OPS_3D
    ydim5 = args[5].dat->size[1];
    #endif
  }
  if (args[6].argtype == OPS_ARG_DAT) {
    xdim6 = args[6].dat->size[0];
    multi_d6 = args[6].dat->dim;
    #ifdef OPS_3D
    ydim6 = args[6].dat->size[1];
    #endif
  }
  if (args[7].argtype == OPS_ARG_DAT) {
    xdim7 = args[7].dat->size[0];
    multi_d7 = args[7].dat->dim;
    #ifdef OPS_3D
    ydim7 = args[7].dat->size[1];
    #endif
  }
  if (args[8].argtype == OPS_ARG_DAT) {
    xdim8 = args[8].dat->size[0];
    multi_d8 = args[8].dat->dim;
    #ifdef OPS_3D
    ydim8 = args[8].dat->size[1];
    #endif
  }
  if (args[9].argtype == OPS_ARG_DAT) {
    xdim9 = args[9].dat->size[0];
    multi_d9 = args[9].dat->dim;
    #ifdef OPS_3D
    ydim9 = args[9].dat->size[1];
    #endif
  }
  if (args[10].argtype == OPS_ARG_DAT) {
    xdim10 = args[10].dat->size[0];
    multi_d10 = args[10].dat->dim;
    #ifdef OPS_3D
    ydim10 = args[10].dat->size[1];
    #endif
  }
  if (args[11].argtype == OPS_ARG_DAT) {
    xdim11 = args[11].dat->size[0];
    multi_d11 = args[11].dat->dim;
    #ifdef OPS_3D
    ydim11 = args[11].dat->size[1];
    #endif
  }
  if (args[12].argtype == OPS_ARG_DAT) {
    xdim12 = args[12].dat->size[0];
    multi_d12 = args[12].dat->dim;
    #ifdef OPS_3D
    ydim12 = args[12].dat->size[1];
    #endif
  }

  ops_H_D_exchanges_host(args, 13);
  ops_halo_exchanges(args,13,range);
  for (int nt=0; nt<total_range; nt++) {
    // call kernel function, passing in pointers to data

    kernel(  (T0 *)p_a[0], (T1 *)p_a[1], (T2 *)p_a[2], (T3 *)p_a[3],
           (T4 *)p_a[4], (T5 *)p_a[5], (T6 *)p_a[6], (T7 *)p_a[7],
           (T8 *)p_a[8], (T9 *)p_a[9], (T10 *)p_a[10], (T11 *)p_a[11],
           (T12 *)p_a[12] );

    count[0]--;   // decrement counter
    int m = 0;    // max dimension with changed index
    while (count[m]==0) {
      count[m] =  end[m]-start[m];// reset counter
      m++;                        // next dimension
      count[m]--;                 // decrement counter
    }

    // shift pointers to data
    for (int i=0; i<13; i++) {
      if (args[i].argtype == OPS_ARG_DAT)
        p_a[i] = p_a[i] + (args[i].dat->elem_size * offs[i][m]);
      else if (args[i].argtype == OPS_ARG_IDX) {
        arg_idx[m]++;
  #ifdef OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      }
    }
  }

  #ifdef OPS_DEBUG_DUMP
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ) ops_dump3(args[0].dat,name);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ) ops_dump3(args[1].dat,name);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ) ops_dump3(args[2].dat,name);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ) ops_dump3(args[3].dat,name);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ) ops_dump3(args[4].dat,name);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ) ops_dump3(args[5].dat,name);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ) ops_dump3(args[6].dat,name);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ) ops_dump3(args[7].dat,name);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ) ops_dump3(args[8].dat,name);
  if (args[9].argtype == OPS_ARG_DAT && args[9].acc != OPS_READ) ops_dump3(args[9].dat,name);
  if (args[10].argtype == OPS_ARG_DAT && args[10].acc != OPS_READ) ops_dump3(args[10].dat,name);
  if (args[11].argtype == OPS_ARG_DAT && args[11].acc != OPS_READ) ops_dump3(args[11].dat,name);
  if (args[12].argtype == OPS_ARG_DAT && args[12].acc != OPS_READ) ops_dump3(args[12].dat,name);
  #endif
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[0],range);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[1],range);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[2],range);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[3],range);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[4],range);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[5],range);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[6],range);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[7],range);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[8],range);
  if (args[9].argtype == OPS_ARG_DAT && args[9].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[9],range);
  if (args[10].argtype == OPS_ARG_DAT && args[10].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[10],range);
  if (args[11].argtype == OPS_ARG_DAT && args[11].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[11],range);
  if (args[12].argtype == OPS_ARG_DAT && args[12].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[12],range);
  ops_set_dirtybit_host(args, 13);
}


//
//ops_par_loop routine for 14 arguments
//
template <class T0,class T1,class T2,class T3,
class T4,class T5,class T6,class T7,
class T8,class T9,class T10,class T11,
class T12,class T13>
void ops_par_loop(void (*kernel)(T0*, T1*, T2*, T3*,
                           T4*, T5*, T6*, T7*,
                           T8*, T9*, T10*, T11*,
                           T12*, T13*),
     char const * name, ops_block block, int dim, int *range,
     ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
     ops_arg arg4, ops_arg arg5, ops_arg arg6, ops_arg arg7,
     ops_arg arg8, ops_arg arg9, ops_arg arg10, ops_arg arg11,
     ops_arg arg12, ops_arg arg13) {

  char *p_a[14];
  int  offs[14][OPS_MAX_DIM];

  int  count[dim];
  ops_arg args[14] = { arg0, arg1, arg2, arg3,
                     arg4, arg5, arg6, arg7,
                     arg8, arg9, arg10, arg11,
                     arg12, arg13};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_name_before(args,14,range,name)) return;
  #endif

  int start[OPS_MAX_DIM];
  int end[OPS_MAX_DIM];

  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  //compute locally allocated range for the sub-block
  int ndim = sb->ndim;
  for (int n=0; n<ndim; n++) {
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) start[n] = 0;
    else start[n] = range[2*n] - start[n];
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) end[n] = range[2*n+1] - sb->decomp_disp[n];
    else end[n] = sb->decomp_size[n];
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else //!OPS_MPI
  int ndim = block->dims;
  for (int n=0; n<ndim; n++) {
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif //OPS_MPI

  #ifdef OPS_DEBUG
  ops_register_args(args, name);
  #endif

  for (int i = 0; i<14;i++) {
    if(args[i].stencil!=NULL) {
      offs[i][0] = args[i].stencil->stride[0]*1;  //unit step in x dimension
      for(int n=1; n<ndim; n++) {
        offs[i][n] = off(ndim, n, &start[0], &end[0],
                         args[i].dat->size, args[i].stencil->stride);
      }
    }
  }

  //set up initial pointers
  for (int i = 0; i < 14; i++) {
    if (args[i].argtype == OPS_ARG_DAT) {
      int d_m[OPS_MAX_DIM];
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d] + OPS_sub_dat_list[args[i].dat->index]->d_im[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d];
  #endif //OPS_MPI
      p_a[i] = (char *)args[i].data //base of 2D array
      + address(ndim, args[i].dat->elem_size, &start[0],
        args[i].dat->size, args[i].stencil->stride, args[i].dat->base,
        d_m);
    }
    else if (args[i].argtype == OPS_ARG_GBL) {
      if (args[i].acc == OPS_READ) p_a[i] = args[i].data;
      else
  #ifdef OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data + ((ops_reduction)args[i].data)->size * block->index;
  #else //OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data;
  #endif //OPS_MPI
    } else if (args[i].argtype == OPS_ARG_IDX) {
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      p_a[i] = (char *)arg_idx;
    }
  }

  int total_range = 1;
  for (int n=0; n<ndim; n++) {
    count[n] = end[n]-start[n];  // number in each dimension
    total_range *= count[n];
    total_range *= (count[n]<0?0:1);
  }
  count[dim-1]++;     // extra in last to ensure correct termination

  if (args[0].argtype == OPS_ARG_DAT) {
    xdim0 = args[0].dat->size[0];
    multi_d0 = args[0].dat->dim;
    #ifdef OPS_3D
    ydim0 = args[0].dat->size[1];
    #endif
  }
  if (args[1].argtype == OPS_ARG_DAT) {
    xdim1 = args[1].dat->size[0];
    multi_d1 = args[1].dat->dim;
    #ifdef OPS_3D
    ydim1 = args[1].dat->size[1];
    #endif
  }
  if (args[2].argtype == OPS_ARG_DAT) {
    xdim2 = args[2].dat->size[0];
    multi_d2 = args[2].dat->dim;
    #ifdef OPS_3D
    ydim2 = args[2].dat->size[1];
    #endif
  }
  if (args[3].argtype == OPS_ARG_DAT) {
    xdim3 = args[3].dat->size[0];
    multi_d3 = args[3].dat->dim;
    #ifdef OPS_3D
    ydim3 = args[3].dat->size[1];
    #endif
  }
  if (args[4].argtype == OPS_ARG_DAT) {
    xdim4 = args[4].dat->size[0];
    multi_d4 = args[4].dat->dim;
    #ifdef OPS_3D
    ydim4 = args[4].dat->size[1];
    #endif
  }
  if (args[5].argtype == OPS_ARG_DAT) {
    xdim5 = args[5].dat->size[0];
    multi_d5 = args[5].dat->dim;
    #ifdef OPS_3D
    ydim5 = args[5].dat->size[1];
    #endif
  }
  if (args[6].argtype == OPS_ARG_DAT) {
    xdim6 = args[6].dat->size[0];
    multi_d6 = args[6].dat->dim;
    #ifdef OPS_3D
    ydim6 = args[6].dat->size[1];
    #endif
  }
  if (args[7].argtype == OPS_ARG_DAT) {
    xdim7 = args[7].dat->size[0];
    multi_d7 = args[7].dat->dim;
    #ifdef OPS_3D
    ydim7 = args[7].dat->size[1];
    #endif
  }
  if (args[8].argtype == OPS_ARG_DAT) {
    xdim8 = args[8].dat->size[0];
    multi_d8 = args[8].dat->dim;
    #ifdef OPS_3D
    ydim8 = args[8].dat->size[1];
    #endif
  }
  if (args[9].argtype == OPS_ARG_DAT) {
    xdim9 = args[9].dat->size[0];
    multi_d9 = args[9].dat->dim;
    #ifdef OPS_3D
    ydim9 = args[9].dat->size[1];
    #endif
  }
  if (args[10].argtype == OPS_ARG_DAT) {
    xdim10 = args[10].dat->size[0];
    multi_d10 = args[10].dat->dim;
    #ifdef OPS_3D
    ydim10 = args[10].dat->size[1];
    #endif
  }
  if (args[11].argtype == OPS_ARG_DAT) {
    xdim11 = args[11].dat->size[0];
    multi_d11 = args[11].dat->dim;
    #ifdef OPS_3D
    ydim11 = args[11].dat->size[1];
    #endif
  }
  if (args[12].argtype == OPS_ARG_DAT) {
    xdim12 = args[12].dat->size[0];
    multi_d12 = args[12].dat->dim;
    #ifdef OPS_3D
    ydim12 = args[12].dat->size[1];
    #endif
  }
  if (args[13].argtype == OPS_ARG_DAT) {
    xdim13 = args[13].dat->size[0];
    multi_d13 = args[13].dat->dim;
    #ifdef OPS_3D
    ydim13 = args[13].dat->size[1];
    #endif
  }

  ops_H_D_exchanges_host(args, 14);
  ops_halo_exchanges(args,14,range);
  for (int nt=0; nt<total_range; nt++) {
    // call kernel function, passing in pointers to data

    kernel(  (T0 *)p_a[0], (T1 *)p_a[1], (T2 *)p_a[2], (T3 *)p_a[3],
           (T4 *)p_a[4], (T5 *)p_a[5], (T6 *)p_a[6], (T7 *)p_a[7],
           (T8 *)p_a[8], (T9 *)p_a[9], (T10 *)p_a[10], (T11 *)p_a[11],
           (T12 *)p_a[12], (T13 *)p_a[13] );

    count[0]--;   // decrement counter
    int m = 0;    // max dimension with changed index
    while (count[m]==0) {
      count[m] =  end[m]-start[m];// reset counter
      m++;                        // next dimension
      count[m]--;                 // decrement counter
    }

    // shift pointers to data
    for (int i=0; i<14; i++) {
      if (args[i].argtype == OPS_ARG_DAT)
        p_a[i] = p_a[i] + (args[i].dat->elem_size * offs[i][m]);
      else if (args[i].argtype == OPS_ARG_IDX) {
        arg_idx[m]++;
  #ifdef OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      }
    }
  }

  #ifdef OPS_DEBUG_DUMP
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ) ops_dump3(args[0].dat,name);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ) ops_dump3(args[1].dat,name);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ) ops_dump3(args[2].dat,name);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ) ops_dump3(args[3].dat,name);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ) ops_dump3(args[4].dat,name);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ) ops_dump3(args[5].dat,name);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ) ops_dump3(args[6].dat,name);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ) ops_dump3(args[7].dat,name);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ) ops_dump3(args[8].dat,name);
  if (args[9].argtype == OPS_ARG_DAT && args[9].acc != OPS_READ) ops_dump3(args[9].dat,name);
  if (args[10].argtype == OPS_ARG_DAT && args[10].acc != OPS_READ) ops_dump3(args[10].dat,name);
  if (args[11].argtype == OPS_ARG_DAT && args[11].acc != OPS_READ) ops_dump3(args[11].dat,name);
  if (args[12].argtype == OPS_ARG_DAT && args[12].acc != OPS_READ) ops_dump3(args[12].dat,name);
  if (args[13].argtype == OPS_ARG_DAT && args[13].acc != OPS_READ) ops_dump3(args[13].dat,name);
  #endif
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[0],range);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[1],range);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[2],range);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[3],range);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[4],range);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[5],range);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[6],range);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[7],range);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[8],range);
  if (args[9].argtype == OPS_ARG_DAT && args[9].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[9],range);
  if (args[10].argtype == OPS_ARG_DAT && args[10].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[10],range);
  if (args[11].argtype == OPS_ARG_DAT && args[11].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[11],range);
  if (args[12].argtype == OPS_ARG_DAT && args[12].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[12],range);
  if (args[13].argtype == OPS_ARG_DAT && args[13].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[13],range);
  ops_set_dirtybit_host(args, 14);
}


//
//ops_par_loop routine for 15 arguments
//
template <class T0,class T1,class T2,class T3,
class T4,class T5,class T6,class T7,
class T8,class T9,class T10,class T11,
class T12,class T13,class T14>
void ops_par_loop(void (*kernel)(T0*, T1*, T2*, T3*,
                           T4*, T5*, T6*, T7*,
                           T8*, T9*, T10*, T11*,
                           T12*, T13*, T14*),
     char const * name, ops_block block, int dim, int *range,
     ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
     ops_arg arg4, ops_arg arg5, ops_arg arg6, ops_arg arg7,
     ops_arg arg8, ops_arg arg9, ops_arg arg10, ops_arg arg11,
     ops_arg arg12, ops_arg arg13, ops_arg arg14) {

  char *p_a[15];
  int  offs[15][OPS_MAX_DIM];

  int  count[dim];
  ops_arg args[15] = { arg0, arg1, arg2, arg3,
                     arg4, arg5, arg6, arg7,
                     arg8, arg9, arg10, arg11,
                     arg12, arg13, arg14};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_name_before(args,15,range,name)) return;
  #endif

  int start[OPS_MAX_DIM];
  int end[OPS_MAX_DIM];

  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  //compute locally allocated range for the sub-block
  int ndim = sb->ndim;
  for (int n=0; n<ndim; n++) {
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) start[n] = 0;
    else start[n] = range[2*n] - start[n];
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) end[n] = range[2*n+1] - sb->decomp_disp[n];
    else end[n] = sb->decomp_size[n];
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else //!OPS_MPI
  int ndim = block->dims;
  for (int n=0; n<ndim; n++) {
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif //OPS_MPI

  #ifdef OPS_DEBUG
  ops_register_args(args, name);
  #endif

  for (int i = 0; i<15;i++) {
    if(args[i].stencil!=NULL) {
      offs[i][0] = args[i].stencil->stride[0]*1;  //unit step in x dimension
      for(int n=1; n<ndim; n++) {
        offs[i][n] = off(ndim, n, &start[0], &end[0],
                         args[i].dat->size, args[i].stencil->stride);
      }
    }
  }

  //set up initial pointers
  for (int i = 0; i < 15; i++) {
    if (args[i].argtype == OPS_ARG_DAT) {
      int d_m[OPS_MAX_DIM];
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d] + OPS_sub_dat_list[args[i].dat->index]->d_im[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d];
  #endif //OPS_MPI
      p_a[i] = (char *)args[i].data //base of 2D array
      + address(ndim, args[i].dat->elem_size, &start[0],
        args[i].dat->size, args[i].stencil->stride, args[i].dat->base,
        d_m);
    }
    else if (args[i].argtype == OPS_ARG_GBL) {
      if (args[i].acc == OPS_READ) p_a[i] = args[i].data;
      else
  #ifdef OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data + ((ops_reduction)args[i].data)->size * block->index;
  #else //OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data;
  #endif //OPS_MPI
    } else if (args[i].argtype == OPS_ARG_IDX) {
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      p_a[i] = (char *)arg_idx;
    }
  }

  int total_range = 1;
  for (int n=0; n<ndim; n++) {
    count[n] = end[n]-start[n];  // number in each dimension
    total_range *= count[n];
    total_range *= (count[n]<0?0:1);
  }
  count[dim-1]++;     // extra in last to ensure correct termination

  if (args[0].argtype == OPS_ARG_DAT) {
    xdim0 = args[0].dat->size[0];
    multi_d0 = args[0].dat->dim;
    #ifdef OPS_3D
    ydim0 = args[0].dat->size[1];
    #endif
  }
  if (args[1].argtype == OPS_ARG_DAT) {
    xdim1 = args[1].dat->size[0];
    multi_d1 = args[1].dat->dim;
    #ifdef OPS_3D
    ydim1 = args[1].dat->size[1];
    #endif
  }
  if (args[2].argtype == OPS_ARG_DAT) {
    xdim2 = args[2].dat->size[0];
    multi_d2 = args[2].dat->dim;
    #ifdef OPS_3D
    ydim2 = args[2].dat->size[1];
    #endif
  }
  if (args[3].argtype == OPS_ARG_DAT) {
    xdim3 = args[3].dat->size[0];
    multi_d3 = args[3].dat->dim;
    #ifdef OPS_3D
    ydim3 = args[3].dat->size[1];
    #endif
  }
  if (args[4].argtype == OPS_ARG_DAT) {
    xdim4 = args[4].dat->size[0];
    multi_d4 = args[4].dat->dim;
    #ifdef OPS_3D
    ydim4 = args[4].dat->size[1];
    #endif
  }
  if (args[5].argtype == OPS_ARG_DAT) {
    xdim5 = args[5].dat->size[0];
    multi_d5 = args[5].dat->dim;
    #ifdef OPS_3D
    ydim5 = args[5].dat->size[1];
    #endif
  }
  if (args[6].argtype == OPS_ARG_DAT) {
    xdim6 = args[6].dat->size[0];
    multi_d6 = args[6].dat->dim;
    #ifdef OPS_3D
    ydim6 = args[6].dat->size[1];
    #endif
  }
  if (args[7].argtype == OPS_ARG_DAT) {
    xdim7 = args[7].dat->size[0];
    multi_d7 = args[7].dat->dim;
    #ifdef OPS_3D
    ydim7 = args[7].dat->size[1];
    #endif
  }
  if (args[8].argtype == OPS_ARG_DAT) {
    xdim8 = args[8].dat->size[0];
    multi_d8 = args[8].dat->dim;
    #ifdef OPS_3D
    ydim8 = args[8].dat->size[1];
    #endif
  }
  if (args[9].argtype == OPS_ARG_DAT) {
    xdim9 = args[9].dat->size[0];
    multi_d9 = args[9].dat->dim;
    #ifdef OPS_3D
    ydim9 = args[9].dat->size[1];
    #endif
  }
  if (args[10].argtype == OPS_ARG_DAT) {
    xdim10 = args[10].dat->size[0];
    multi_d10 = args[10].dat->dim;
    #ifdef OPS_3D
    ydim10 = args[10].dat->size[1];
    #endif
  }
  if (args[11].argtype == OPS_ARG_DAT) {
    xdim11 = args[11].dat->size[0];
    multi_d11 = args[11].dat->dim;
    #ifdef OPS_3D
    ydim11 = args[11].dat->size[1];
    #endif
  }
  if (args[12].argtype == OPS_ARG_DAT) {
    xdim12 = args[12].dat->size[0];
    multi_d12 = args[12].dat->dim;
    #ifdef OPS_3D
    ydim12 = args[12].dat->size[1];
    #endif
  }
  if (args[13].argtype == OPS_ARG_DAT) {
    xdim13 = args[13].dat->size[0];
    multi_d13 = args[13].dat->dim;
    #ifdef OPS_3D
    ydim13 = args[13].dat->size[1];
    #endif
  }
  if (args[14].argtype == OPS_ARG_DAT) {
    xdim14 = args[14].dat->size[0];
    multi_d14 = args[14].dat->dim;
    #ifdef OPS_3D
    ydim14 = args[14].dat->size[1];
    #endif
  }

  ops_H_D_exchanges_host(args, 15);
  ops_halo_exchanges(args,15,range);
  for (int nt=0; nt<total_range; nt++) {
    // call kernel function, passing in pointers to data

    kernel(  (T0 *)p_a[0], (T1 *)p_a[1], (T2 *)p_a[2], (T3 *)p_a[3],
           (T4 *)p_a[4], (T5 *)p_a[5], (T6 *)p_a[6], (T7 *)p_a[7],
           (T8 *)p_a[8], (T9 *)p_a[9], (T10 *)p_a[10], (T11 *)p_a[11],
           (T12 *)p_a[12], (T13 *)p_a[13], (T14 *)p_a[14] );

    count[0]--;   // decrement counter
    int m = 0;    // max dimension with changed index
    while (count[m]==0) {
      count[m] =  end[m]-start[m];// reset counter
      m++;                        // next dimension
      count[m]--;                 // decrement counter
    }

    // shift pointers to data
    for (int i=0; i<15; i++) {
      if (args[i].argtype == OPS_ARG_DAT)
        p_a[i] = p_a[i] + (args[i].dat->elem_size * offs[i][m]);
      else if (args[i].argtype == OPS_ARG_IDX) {
        arg_idx[m]++;
  #ifdef OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      }
    }
  }

  #ifdef OPS_DEBUG_DUMP
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ) ops_dump3(args[0].dat,name);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ) ops_dump3(args[1].dat,name);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ) ops_dump3(args[2].dat,name);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ) ops_dump3(args[3].dat,name);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ) ops_dump3(args[4].dat,name);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ) ops_dump3(args[5].dat,name);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ) ops_dump3(args[6].dat,name);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ) ops_dump3(args[7].dat,name);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ) ops_dump3(args[8].dat,name);
  if (args[9].argtype == OPS_ARG_DAT && args[9].acc != OPS_READ) ops_dump3(args[9].dat,name);
  if (args[10].argtype == OPS_ARG_DAT && args[10].acc != OPS_READ) ops_dump3(args[10].dat,name);
  if (args[11].argtype == OPS_ARG_DAT && args[11].acc != OPS_READ) ops_dump3(args[11].dat,name);
  if (args[12].argtype == OPS_ARG_DAT && args[12].acc != OPS_READ) ops_dump3(args[12].dat,name);
  if (args[13].argtype == OPS_ARG_DAT && args[13].acc != OPS_READ) ops_dump3(args[13].dat,name);
  if (args[14].argtype == OPS_ARG_DAT && args[14].acc != OPS_READ) ops_dump3(args[14].dat,name);
  #endif
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[0],range);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[1],range);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[2],range);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[3],range);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[4],range);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[5],range);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[6],range);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[7],range);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[8],range);
  if (args[9].argtype == OPS_ARG_DAT && args[9].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[9],range);
  if (args[10].argtype == OPS_ARG_DAT && args[10].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[10],range);
  if (args[11].argtype == OPS_ARG_DAT && args[11].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[11],range);
  if (args[12].argtype == OPS_ARG_DAT && args[12].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[12],range);
  if (args[13].argtype == OPS_ARG_DAT && args[13].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[13],range);
  if (args[14].argtype == OPS_ARG_DAT && args[14].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[14],range);
  ops_set_dirtybit_host(args, 15);
}


//
//ops_par_loop routine for 16 arguments
//
template <class T0,class T1,class T2,class T3,
class T4,class T5,class T6,class T7,
class T8,class T9,class T10,class T11,
class T12,class T13,class T14,class T15>
void ops_par_loop(void (*kernel)(T0*, T1*, T2*, T3*,
                           T4*, T5*, T6*, T7*,
                           T8*, T9*, T10*, T11*,
                           T12*, T13*, T14*, T15*),
     char const * name, ops_block block, int dim, int *range,
     ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
     ops_arg arg4, ops_arg arg5, ops_arg arg6, ops_arg arg7,
     ops_arg arg8, ops_arg arg9, ops_arg arg10, ops_arg arg11,
     ops_arg arg12, ops_arg arg13, ops_arg arg14, ops_arg arg15) {

  char *p_a[16];
  int  offs[16][OPS_MAX_DIM];

  int  count[dim];
  ops_arg args[16] = { arg0, arg1, arg2, arg3,
                     arg4, arg5, arg6, arg7,
                     arg8, arg9, arg10, arg11,
                     arg12, arg13, arg14, arg15};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_name_before(args,16,range,name)) return;
  #endif

  int start[OPS_MAX_DIM];
  int end[OPS_MAX_DIM];

  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  //compute locally allocated range for the sub-block
  int ndim = sb->ndim;
  for (int n=0; n<ndim; n++) {
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) start[n] = 0;
    else start[n] = range[2*n] - start[n];
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) end[n] = range[2*n+1] - sb->decomp_disp[n];
    else end[n] = sb->decomp_size[n];
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else //!OPS_MPI
  int ndim = block->dims;
  for (int n=0; n<ndim; n++) {
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif //OPS_MPI

  #ifdef OPS_DEBUG
  ops_register_args(args, name);
  #endif

  for (int i = 0; i<16;i++) {
    if(args[i].stencil!=NULL) {
      offs[i][0] = args[i].stencil->stride[0]*1;  //unit step in x dimension
      for(int n=1; n<ndim; n++) {
        offs[i][n] = off(ndim, n, &start[0], &end[0],
                         args[i].dat->size, args[i].stencil->stride);
      }
    }
  }

  //set up initial pointers
  for (int i = 0; i < 16; i++) {
    if (args[i].argtype == OPS_ARG_DAT) {
      int d_m[OPS_MAX_DIM];
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d] + OPS_sub_dat_list[args[i].dat->index]->d_im[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d];
  #endif //OPS_MPI
      p_a[i] = (char *)args[i].data //base of 2D array
      + address(ndim, args[i].dat->elem_size, &start[0],
        args[i].dat->size, args[i].stencil->stride, args[i].dat->base,
        d_m);
    }
    else if (args[i].argtype == OPS_ARG_GBL) {
      if (args[i].acc == OPS_READ) p_a[i] = args[i].data;
      else
  #ifdef OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data + ((ops_reduction)args[i].data)->size * block->index;
  #else //OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data;
  #endif //OPS_MPI
    } else if (args[i].argtype == OPS_ARG_IDX) {
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      p_a[i] = (char *)arg_idx;
    }
  }

  int total_range = 1;
  for (int n=0; n<ndim; n++) {
    count[n] = end[n]-start[n];  // number in each dimension
    total_range *= count[n];
    total_range *= (count[n]<0?0:1);
  }
  count[dim-1]++;     // extra in last to ensure correct termination

  if (args[0].argtype == OPS_ARG_DAT) {
    xdim0 = args[0].dat->size[0];
    multi_d0 = args[0].dat->dim;
    #ifdef OPS_3D
    ydim0 = args[0].dat->size[1];
    #endif
  }
  if (args[1].argtype == OPS_ARG_DAT) {
    xdim1 = args[1].dat->size[0];
    multi_d1 = args[1].dat->dim;
    #ifdef OPS_3D
    ydim1 = args[1].dat->size[1];
    #endif
  }
  if (args[2].argtype == OPS_ARG_DAT) {
    xdim2 = args[2].dat->size[0];
    multi_d2 = args[2].dat->dim;
    #ifdef OPS_3D
    ydim2 = args[2].dat->size[1];
    #endif
  }
  if (args[3].argtype == OPS_ARG_DAT) {
    xdim3 = args[3].dat->size[0];
    multi_d3 = args[3].dat->dim;
    #ifdef OPS_3D
    ydim3 = args[3].dat->size[1];
    #endif
  }
  if (args[4].argtype == OPS_ARG_DAT) {
    xdim4 = args[4].dat->size[0];
    multi_d4 = args[4].dat->dim;
    #ifdef OPS_3D
    ydim4 = args[4].dat->size[1];
    #endif
  }
  if (args[5].argtype == OPS_ARG_DAT) {
    xdim5 = args[5].dat->size[0];
    multi_d5 = args[5].dat->dim;
    #ifdef OPS_3D
    ydim5 = args[5].dat->size[1];
    #endif
  }
  if (args[6].argtype == OPS_ARG_DAT) {
    xdim6 = args[6].dat->size[0];
    multi_d6 = args[6].dat->dim;
    #ifdef OPS_3D
    ydim6 = args[6].dat->size[1];
    #endif
  }
  if (args[7].argtype == OPS_ARG_DAT) {
    xdim7 = args[7].dat->size[0];
    multi_d7 = args[7].dat->dim;
    #ifdef OPS_3D
    ydim7 = args[7].dat->size[1];
    #endif
  }
  if (args[8].argtype == OPS_ARG_DAT) {
    xdim8 = args[8].dat->size[0];
    multi_d8 = args[8].dat->dim;
    #ifdef OPS_3D
    ydim8 = args[8].dat->size[1];
    #endif
  }
  if (args[9].argtype == OPS_ARG_DAT) {
    xdim9 = args[9].dat->size[0];
    multi_d9 = args[9].dat->dim;
    #ifdef OPS_3D
    ydim9 = args[9].dat->size[1];
    #endif
  }
  if (args[10].argtype == OPS_ARG_DAT) {
    xdim10 = args[10].dat->size[0];
    multi_d10 = args[10].dat->dim;
    #ifdef OPS_3D
    ydim10 = args[10].dat->size[1];
    #endif
  }
  if (args[11].argtype == OPS_ARG_DAT) {
    xdim11 = args[11].dat->size[0];
    multi_d11 = args[11].dat->dim;
    #ifdef OPS_3D
    ydim11 = args[11].dat->size[1];
    #endif
  }
  if (args[12].argtype == OPS_ARG_DAT) {
    xdim12 = args[12].dat->size[0];
    multi_d12 = args[12].dat->dim;
    #ifdef OPS_3D
    ydim12 = args[12].dat->size[1];
    #endif
  }
  if (args[13].argtype == OPS_ARG_DAT) {
    xdim13 = args[13].dat->size[0];
    multi_d13 = args[13].dat->dim;
    #ifdef OPS_3D
    ydim13 = args[13].dat->size[1];
    #endif
  }
  if (args[14].argtype == OPS_ARG_DAT) {
    xdim14 = args[14].dat->size[0];
    multi_d14 = args[14].dat->dim;
    #ifdef OPS_3D
    ydim14 = args[14].dat->size[1];
    #endif
  }
  if (args[15].argtype == OPS_ARG_DAT) {
    xdim15 = args[15].dat->size[0];
    multi_d15 = args[15].dat->dim;
    #ifdef OPS_3D
    ydim15 = args[15].dat->size[1];
    #endif
  }

  ops_H_D_exchanges_host(args, 16);
  ops_halo_exchanges(args,16,range);
  for (int nt=0; nt<total_range; nt++) {
    // call kernel function, passing in pointers to data

    kernel(  (T0 *)p_a[0], (T1 *)p_a[1], (T2 *)p_a[2], (T3 *)p_a[3],
           (T4 *)p_a[4], (T5 *)p_a[5], (T6 *)p_a[6], (T7 *)p_a[7],
           (T8 *)p_a[8], (T9 *)p_a[9], (T10 *)p_a[10], (T11 *)p_a[11],
           (T12 *)p_a[12], (T13 *)p_a[13], (T14 *)p_a[14], (T15 *)p_a[15] );

    count[0]--;   // decrement counter
    int m = 0;    // max dimension with changed index
    while (count[m]==0) {
      count[m] =  end[m]-start[m];// reset counter
      m++;                        // next dimension
      count[m]--;                 // decrement counter
    }

    // shift pointers to data
    for (int i=0; i<16; i++) {
      if (args[i].argtype == OPS_ARG_DAT)
        p_a[i] = p_a[i] + (args[i].dat->elem_size * offs[i][m]);
      else if (args[i].argtype == OPS_ARG_IDX) {
        arg_idx[m]++;
  #ifdef OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      }
    }
  }

  #ifdef OPS_DEBUG_DUMP
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ) ops_dump3(args[0].dat,name);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ) ops_dump3(args[1].dat,name);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ) ops_dump3(args[2].dat,name);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ) ops_dump3(args[3].dat,name);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ) ops_dump3(args[4].dat,name);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ) ops_dump3(args[5].dat,name);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ) ops_dump3(args[6].dat,name);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ) ops_dump3(args[7].dat,name);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ) ops_dump3(args[8].dat,name);
  if (args[9].argtype == OPS_ARG_DAT && args[9].acc != OPS_READ) ops_dump3(args[9].dat,name);
  if (args[10].argtype == OPS_ARG_DAT && args[10].acc != OPS_READ) ops_dump3(args[10].dat,name);
  if (args[11].argtype == OPS_ARG_DAT && args[11].acc != OPS_READ) ops_dump3(args[11].dat,name);
  if (args[12].argtype == OPS_ARG_DAT && args[12].acc != OPS_READ) ops_dump3(args[12].dat,name);
  if (args[13].argtype == OPS_ARG_DAT && args[13].acc != OPS_READ) ops_dump3(args[13].dat,name);
  if (args[14].argtype == OPS_ARG_DAT && args[14].acc != OPS_READ) ops_dump3(args[14].dat,name);
  if (args[15].argtype == OPS_ARG_DAT && args[15].acc != OPS_READ) ops_dump3(args[15].dat,name);
  #endif
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[0],range);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[1],range);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[2],range);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[3],range);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[4],range);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[5],range);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[6],range);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[7],range);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[8],range);
  if (args[9].argtype == OPS_ARG_DAT && args[9].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[9],range);
  if (args[10].argtype == OPS_ARG_DAT && args[10].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[10],range);
  if (args[11].argtype == OPS_ARG_DAT && args[11].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[11],range);
  if (args[12].argtype == OPS_ARG_DAT && args[12].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[12],range);
  if (args[13].argtype == OPS_ARG_DAT && args[13].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[13],range);
  if (args[14].argtype == OPS_ARG_DAT && args[14].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[14],range);
  if (args[15].argtype == OPS_ARG_DAT && args[15].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[15],range);
  ops_set_dirtybit_host(args, 16);
}


//
//ops_par_loop routine for 17 arguments
//
template <class T0,class T1,class T2,class T3,
class T4,class T5,class T6,class T7,
class T8,class T9,class T10,class T11,
class T12,class T13,class T14,class T15,
class T16>
void ops_par_loop(void (*kernel)(T0*, T1*, T2*, T3*,
                           T4*, T5*, T6*, T7*,
                           T8*, T9*, T10*, T11*,
                           T12*, T13*, T14*, T15*,
                           T16*),
     char const * name, ops_block block, int dim, int *range,
     ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
     ops_arg arg4, ops_arg arg5, ops_arg arg6, ops_arg arg7,
     ops_arg arg8, ops_arg arg9, ops_arg arg10, ops_arg arg11,
     ops_arg arg12, ops_arg arg13, ops_arg arg14, ops_arg arg15,
     ops_arg arg16) {

  char *p_a[17];
  int  offs[17][OPS_MAX_DIM];

  int  count[dim];
  ops_arg args[17] = { arg0, arg1, arg2, arg3,
                     arg4, arg5, arg6, arg7,
                     arg8, arg9, arg10, arg11,
                     arg12, arg13, arg14, arg15,
                     arg16};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_name_before(args,17,range,name)) return;
  #endif

  int start[OPS_MAX_DIM];
  int end[OPS_MAX_DIM];

  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  //compute locally allocated range for the sub-block
  int ndim = sb->ndim;
  for (int n=0; n<ndim; n++) {
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) start[n] = 0;
    else start[n] = range[2*n] - start[n];
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) end[n] = range[2*n+1] - sb->decomp_disp[n];
    else end[n] = sb->decomp_size[n];
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else //!OPS_MPI
  int ndim = block->dims;
  for (int n=0; n<ndim; n++) {
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif //OPS_MPI

  #ifdef OPS_DEBUG
  ops_register_args(args, name);
  #endif

  for (int i = 0; i<17;i++) {
    if(args[i].stencil!=NULL) {
      offs[i][0] = args[i].stencil->stride[0]*1;  //unit step in x dimension
      for(int n=1; n<ndim; n++) {
        offs[i][n] = off(ndim, n, &start[0], &end[0],
                         args[i].dat->size, args[i].stencil->stride);
      }
    }
  }

  //set up initial pointers
  for (int i = 0; i < 17; i++) {
    if (args[i].argtype == OPS_ARG_DAT) {
      int d_m[OPS_MAX_DIM];
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d] + OPS_sub_dat_list[args[i].dat->index]->d_im[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d];
  #endif //OPS_MPI
      p_a[i] = (char *)args[i].data //base of 2D array
      + address(ndim, args[i].dat->elem_size, &start[0],
        args[i].dat->size, args[i].stencil->stride, args[i].dat->base,
        d_m);
    }
    else if (args[i].argtype == OPS_ARG_GBL) {
      if (args[i].acc == OPS_READ) p_a[i] = args[i].data;
      else
  #ifdef OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data + ((ops_reduction)args[i].data)->size * block->index;
  #else //OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data;
  #endif //OPS_MPI
    } else if (args[i].argtype == OPS_ARG_IDX) {
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      p_a[i] = (char *)arg_idx;
    }
  }

  int total_range = 1;
  for (int n=0; n<ndim; n++) {
    count[n] = end[n]-start[n];  // number in each dimension
    total_range *= count[n];
    total_range *= (count[n]<0?0:1);
  }
  count[dim-1]++;     // extra in last to ensure correct termination

  if (args[0].argtype == OPS_ARG_DAT) {
    xdim0 = args[0].dat->size[0];
    multi_d0 = args[0].dat->dim;
    #ifdef OPS_3D
    ydim0 = args[0].dat->size[1];
    #endif
  }
  if (args[1].argtype == OPS_ARG_DAT) {
    xdim1 = args[1].dat->size[0];
    multi_d1 = args[1].dat->dim;
    #ifdef OPS_3D
    ydim1 = args[1].dat->size[1];
    #endif
  }
  if (args[2].argtype == OPS_ARG_DAT) {
    xdim2 = args[2].dat->size[0];
    multi_d2 = args[2].dat->dim;
    #ifdef OPS_3D
    ydim2 = args[2].dat->size[1];
    #endif
  }
  if (args[3].argtype == OPS_ARG_DAT) {
    xdim3 = args[3].dat->size[0];
    multi_d3 = args[3].dat->dim;
    #ifdef OPS_3D
    ydim3 = args[3].dat->size[1];
    #endif
  }
  if (args[4].argtype == OPS_ARG_DAT) {
    xdim4 = args[4].dat->size[0];
    multi_d4 = args[4].dat->dim;
    #ifdef OPS_3D
    ydim4 = args[4].dat->size[1];
    #endif
  }
  if (args[5].argtype == OPS_ARG_DAT) {
    xdim5 = args[5].dat->size[0];
    multi_d5 = args[5].dat->dim;
    #ifdef OPS_3D
    ydim5 = args[5].dat->size[1];
    #endif
  }
  if (args[6].argtype == OPS_ARG_DAT) {
    xdim6 = args[6].dat->size[0];
    multi_d6 = args[6].dat->dim;
    #ifdef OPS_3D
    ydim6 = args[6].dat->size[1];
    #endif
  }
  if (args[7].argtype == OPS_ARG_DAT) {
    xdim7 = args[7].dat->size[0];
    multi_d7 = args[7].dat->dim;
    #ifdef OPS_3D
    ydim7 = args[7].dat->size[1];
    #endif
  }
  if (args[8].argtype == OPS_ARG_DAT) {
    xdim8 = args[8].dat->size[0];
    multi_d8 = args[8].dat->dim;
    #ifdef OPS_3D
    ydim8 = args[8].dat->size[1];
    #endif
  }
  if (args[9].argtype == OPS_ARG_DAT) {
    xdim9 = args[9].dat->size[0];
    multi_d9 = args[9].dat->dim;
    #ifdef OPS_3D
    ydim9 = args[9].dat->size[1];
    #endif
  }
  if (args[10].argtype == OPS_ARG_DAT) {
    xdim10 = args[10].dat->size[0];
    multi_d10 = args[10].dat->dim;
    #ifdef OPS_3D
    ydim10 = args[10].dat->size[1];
    #endif
  }
  if (args[11].argtype == OPS_ARG_DAT) {
    xdim11 = args[11].dat->size[0];
    multi_d11 = args[11].dat->dim;
    #ifdef OPS_3D
    ydim11 = args[11].dat->size[1];
    #endif
  }
  if (args[12].argtype == OPS_ARG_DAT) {
    xdim12 = args[12].dat->size[0];
    multi_d12 = args[12].dat->dim;
    #ifdef OPS_3D
    ydim12 = args[12].dat->size[1];
    #endif
  }
  if (args[13].argtype == OPS_ARG_DAT) {
    xdim13 = args[13].dat->size[0];
    multi_d13 = args[13].dat->dim;
    #ifdef OPS_3D
    ydim13 = args[13].dat->size[1];
    #endif
  }
  if (args[14].argtype == OPS_ARG_DAT) {
    xdim14 = args[14].dat->size[0];
    multi_d14 = args[14].dat->dim;
    #ifdef OPS_3D
    ydim14 = args[14].dat->size[1];
    #endif
  }
  if (args[15].argtype == OPS_ARG_DAT) {
    xdim15 = args[15].dat->size[0];
    multi_d15 = args[15].dat->dim;
    #ifdef OPS_3D
    ydim15 = args[15].dat->size[1];
    #endif
  }
  if (args[16].argtype == OPS_ARG_DAT) {
    xdim16 = args[16].dat->size[0];
    multi_d16 = args[16].dat->dim;
    #ifdef OPS_3D
    ydim16 = args[16].dat->size[1];
    #endif
  }

  ops_H_D_exchanges_host(args, 17);
  ops_halo_exchanges(args,17,range);
  for (int nt=0; nt<total_range; nt++) {
    // call kernel function, passing in pointers to data

    kernel(  (T0 *)p_a[0], (T1 *)p_a[1], (T2 *)p_a[2], (T3 *)p_a[3],
           (T4 *)p_a[4], (T5 *)p_a[5], (T6 *)p_a[6], (T7 *)p_a[7],
           (T8 *)p_a[8], (T9 *)p_a[9], (T10 *)p_a[10], (T11 *)p_a[11],
           (T12 *)p_a[12], (T13 *)p_a[13], (T14 *)p_a[14], (T15 *)p_a[15],
           (T16 *)p_a[16] );

    count[0]--;   // decrement counter
    int m = 0;    // max dimension with changed index
    while (count[m]==0) {
      count[m] =  end[m]-start[m];// reset counter
      m++;                        // next dimension
      count[m]--;                 // decrement counter
    }

    // shift pointers to data
    for (int i=0; i<17; i++) {
      if (args[i].argtype == OPS_ARG_DAT)
        p_a[i] = p_a[i] + (args[i].dat->elem_size * offs[i][m]);
      else if (args[i].argtype == OPS_ARG_IDX) {
        arg_idx[m]++;
  #ifdef OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      }
    }
  }

  #ifdef OPS_DEBUG_DUMP
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ) ops_dump3(args[0].dat,name);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ) ops_dump3(args[1].dat,name);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ) ops_dump3(args[2].dat,name);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ) ops_dump3(args[3].dat,name);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ) ops_dump3(args[4].dat,name);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ) ops_dump3(args[5].dat,name);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ) ops_dump3(args[6].dat,name);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ) ops_dump3(args[7].dat,name);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ) ops_dump3(args[8].dat,name);
  if (args[9].argtype == OPS_ARG_DAT && args[9].acc != OPS_READ) ops_dump3(args[9].dat,name);
  if (args[10].argtype == OPS_ARG_DAT && args[10].acc != OPS_READ) ops_dump3(args[10].dat,name);
  if (args[11].argtype == OPS_ARG_DAT && args[11].acc != OPS_READ) ops_dump3(args[11].dat,name);
  if (args[12].argtype == OPS_ARG_DAT && args[12].acc != OPS_READ) ops_dump3(args[12].dat,name);
  if (args[13].argtype == OPS_ARG_DAT && args[13].acc != OPS_READ) ops_dump3(args[13].dat,name);
  if (args[14].argtype == OPS_ARG_DAT && args[14].acc != OPS_READ) ops_dump3(args[14].dat,name);
  if (args[15].argtype == OPS_ARG_DAT && args[15].acc != OPS_READ) ops_dump3(args[15].dat,name);
  if (args[16].argtype == OPS_ARG_DAT && args[16].acc != OPS_READ) ops_dump3(args[16].dat,name);
  #endif
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[0],range);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[1],range);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[2],range);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[3],range);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[4],range);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[5],range);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[6],range);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[7],range);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[8],range);
  if (args[9].argtype == OPS_ARG_DAT && args[9].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[9],range);
  if (args[10].argtype == OPS_ARG_DAT && args[10].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[10],range);
  if (args[11].argtype == OPS_ARG_DAT && args[11].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[11],range);
  if (args[12].argtype == OPS_ARG_DAT && args[12].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[12],range);
  if (args[13].argtype == OPS_ARG_DAT && args[13].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[13],range);
  if (args[14].argtype == OPS_ARG_DAT && args[14].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[14],range);
  if (args[15].argtype == OPS_ARG_DAT && args[15].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[15],range);
  if (args[16].argtype == OPS_ARG_DAT && args[16].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[16],range);
  ops_set_dirtybit_host(args, 17);
}


//
//ops_par_loop routine for 18 arguments
//
template <class T0,class T1,class T2,class T3,
class T4,class T5,class T6,class T7,
class T8,class T9,class T10,class T11,
class T12,class T13,class T14,class T15,
class T16,class T17>
void ops_par_loop(void (*kernel)(T0*, T1*, T2*, T3*,
                           T4*, T5*, T6*, T7*,
                           T8*, T9*, T10*, T11*,
                           T12*, T13*, T14*, T15*,
                           T16*, T17*),
     char const * name, ops_block block, int dim, int *range,
     ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
     ops_arg arg4, ops_arg arg5, ops_arg arg6, ops_arg arg7,
     ops_arg arg8, ops_arg arg9, ops_arg arg10, ops_arg arg11,
     ops_arg arg12, ops_arg arg13, ops_arg arg14, ops_arg arg15,
     ops_arg arg16, ops_arg arg17) {

  char *p_a[18];
  int  offs[18][OPS_MAX_DIM];

  int  count[dim];
  ops_arg args[18] = { arg0, arg1, arg2, arg3,
                     arg4, arg5, arg6, arg7,
                     arg8, arg9, arg10, arg11,
                     arg12, arg13, arg14, arg15,
                     arg16, arg17};


  #ifdef CHECKPOINTING
  if (!ops_checkpointing_name_before(args,18,range,name)) return;
  #endif

  int start[OPS_MAX_DIM];
  int end[OPS_MAX_DIM];

  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  //compute locally allocated range for the sub-block
  int ndim = sb->ndim;
  for (int n=0; n<ndim; n++) {
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) start[n] = 0;
    else start[n] = range[2*n] - start[n];
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) end[n] = range[2*n+1] - sb->decomp_disp[n];
    else end[n] = sb->decomp_size[n];
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else //!OPS_MPI
  int ndim = block->dims;
  for (int n=0; n<ndim; n++) {
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif //OPS_MPI

  #ifdef OPS_DEBUG
  ops_register_args(args, name);
  #endif

  for (int i = 0; i<18;i++) {
    if(args[i].stencil!=NULL) {
      offs[i][0] = args[i].stencil->stride[0]*1;  //unit step in x dimension
      for(int n=1; n<ndim; n++) {
        offs[i][n] = off(ndim, n, &start[0], &end[0],
                         args[i].dat->size, args[i].stencil->stride);
      }
    }
  }

  //set up initial pointers
  for (int i = 0; i < 18; i++) {
    if (args[i].argtype == OPS_ARG_DAT) {
      int d_m[OPS_MAX_DIM];
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d] + OPS_sub_dat_list[args[i].dat->index]->d_im[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) d_m[d] = args[i].dat->d_m[d];
  #endif //OPS_MPI
      p_a[i] = (char *)args[i].data //base of 2D array
      + address(ndim, args[i].dat->elem_size, &start[0],
        args[i].dat->size, args[i].stencil->stride, args[i].dat->base,
        d_m);
    }
    else if (args[i].argtype == OPS_ARG_GBL) {
      if (args[i].acc == OPS_READ) p_a[i] = args[i].data;
      else
  #ifdef OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data + ((ops_reduction)args[i].data)->size * block->index;
  #else //OPS_MPI
        p_a[i] = ((ops_reduction)args[i].data)->data;
  #endif //OPS_MPI
    } else if (args[i].argtype == OPS_ARG_IDX) {
  #ifdef OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
      for (int d = 0; d < dim; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      p_a[i] = (char *)arg_idx;
    }
  }

  int total_range = 1;
  for (int n=0; n<ndim; n++) {
    count[n] = end[n]-start[n];  // number in each dimension
    total_range *= count[n];
    total_range *= (count[n]<0?0:1);
  }
  count[dim-1]++;     // extra in last to ensure correct termination

  if (args[0].argtype == OPS_ARG_DAT) {
    xdim0 = args[0].dat->size[0];
    multi_d0 = args[0].dat->dim;
    #ifdef OPS_3D
    ydim0 = args[0].dat->size[1];
    #endif
  }
  if (args[1].argtype == OPS_ARG_DAT) {
    xdim1 = args[1].dat->size[0];
    multi_d1 = args[1].dat->dim;
    #ifdef OPS_3D
    ydim1 = args[1].dat->size[1];
    #endif
  }
  if (args[2].argtype == OPS_ARG_DAT) {
    xdim2 = args[2].dat->size[0];
    multi_d2 = args[2].dat->dim;
    #ifdef OPS_3D
    ydim2 = args[2].dat->size[1];
    #endif
  }
  if (args[3].argtype == OPS_ARG_DAT) {
    xdim3 = args[3].dat->size[0];
    multi_d3 = args[3].dat->dim;
    #ifdef OPS_3D
    ydim3 = args[3].dat->size[1];
    #endif
  }
  if (args[4].argtype == OPS_ARG_DAT) {
    xdim4 = args[4].dat->size[0];
    multi_d4 = args[4].dat->dim;
    #ifdef OPS_3D
    ydim4 = args[4].dat->size[1];
    #endif
  }
  if (args[5].argtype == OPS_ARG_DAT) {
    xdim5 = args[5].dat->size[0];
    multi_d5 = args[5].dat->dim;
    #ifdef OPS_3D
    ydim5 = args[5].dat->size[1];
    #endif
  }
  if (args[6].argtype == OPS_ARG_DAT) {
    xdim6 = args[6].dat->size[0];
    multi_d6 = args[6].dat->dim;
    #ifdef OPS_3D
    ydim6 = args[6].dat->size[1];
    #endif
  }
  if (args[7].argtype == OPS_ARG_DAT) {
    xdim7 = args[7].dat->size[0];
    multi_d7 = args[7].dat->dim;
    #ifdef OPS_3D
    ydim7 = args[7].dat->size[1];
    #endif
  }
  if (args[8].argtype == OPS_ARG_DAT) {
    xdim8 = args[8].dat->size[0];
    multi_d8 = args[8].dat->dim;
    #ifdef OPS_3D
    ydim8 = args[8].dat->size[1];
    #endif
  }
  if (args[9].argtype == OPS_ARG_DAT) {
    xdim9 = args[9].dat->size[0];
    multi_d9 = args[9].dat->dim;
    #ifdef OPS_3D
    ydim9 = args[9].dat->size[1];
    #endif
  }
  if (args[10].argtype == OPS_ARG_DAT) {
    xdim10 = args[10].dat->size[0];
    multi_d10 = args[10].dat->dim;
    #ifdef OPS_3D
    ydim10 = args[10].dat->size[1];
    #endif
  }
  if (args[11].argtype == OPS_ARG_DAT) {
    xdim11 = args[11].dat->size[0];
    multi_d11 = args[11].dat->dim;
    #ifdef OPS_3D
    ydim11 = args[11].dat->size[1];
    #endif
  }
  if (args[12].argtype == OPS_ARG_DAT) {
    xdim12 = args[12].dat->size[0];
    multi_d12 = args[12].dat->dim;
    #ifdef OPS_3D
    ydim12 = args[12].dat->size[1];
    #endif
  }
  if (args[13].argtype == OPS_ARG_DAT) {
    xdim13 = args[13].dat->size[0];
    multi_d13 = args[13].dat->dim;
    #ifdef OPS_3D
    ydim13 = args[13].dat->size[1];
    #endif
  }
  if (args[14].argtype == OPS_ARG_DAT) {
    xdim14 = args[14].dat->size[0];
    multi_d14 = args[14].dat->dim;
    #ifdef OPS_3D
    ydim14 = args[14].dat->size[1];
    #endif
  }
  if (args[15].argtype == OPS_ARG_DAT) {
    xdim15 = args[15].dat->size[0];
    multi_d15 = args[15].dat->dim;
    #ifdef OPS_3D
    ydim15 = args[15].dat->size[1];
    #endif
  }
  if (args[16].argtype == OPS_ARG_DAT) {
    xdim16 = args[16].dat->size[0];
    multi_d16 = args[16].dat->dim;
    #ifdef OPS_3D
    ydim16 = args[16].dat->size[1];
    #endif
  }
  if (args[17].argtype == OPS_ARG_DAT) {
    xdim17 = args[17].dat->size[0];
    multi_d17 = args[17].dat->dim;
    #ifdef OPS_3D
    ydim17 = args[17].dat->size[1];
    #endif
  }

  ops_H_D_exchanges_host(args, 18);
  ops_halo_exchanges(args,18,range);
  for (int nt=0; nt<total_range; nt++) {
    // call kernel function, passing in pointers to data

    kernel(  (T0 *)p_a[0], (T1 *)p_a[1], (T2 *)p_a[2], (T3 *)p_a[3],
           (T4 *)p_a[4], (T5 *)p_a[5], (T6 *)p_a[6], (T7 *)p_a[7],
           (T8 *)p_a[8], (T9 *)p_a[9], (T10 *)p_a[10], (T11 *)p_a[11],
           (T12 *)p_a[12], (T13 *)p_a[13], (T14 *)p_a[14], (T15 *)p_a[15],
           (T16 *)p_a[16], (T17 *)p_a[17] );

    count[0]--;   // decrement counter
    int m = 0;    // max dimension with changed index
    while (count[m]==0) {
      count[m] =  end[m]-start[m];// reset counter
      m++;                        // next dimension
      count[m]--;                 // decrement counter
    }

    // shift pointers to data
    for (int i=0; i<18; i++) {
      if (args[i].argtype == OPS_ARG_DAT)
        p_a[i] = p_a[i] + (args[i].dat->elem_size * offs[i][m]);
      else if (args[i].argtype == OPS_ARG_IDX) {
        arg_idx[m]++;
  #ifdef OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = sb->decomp_disp[d] + start[d];
  #else //OPS_MPI
        for (int d = 0; d < m; d++) arg_idx[d] = start[d];
  #endif //OPS_MPI
      }
    }
  }

  #ifdef OPS_DEBUG_DUMP
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ) ops_dump3(args[0].dat,name);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ) ops_dump3(args[1].dat,name);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ) ops_dump3(args[2].dat,name);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ) ops_dump3(args[3].dat,name);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ) ops_dump3(args[4].dat,name);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ) ops_dump3(args[5].dat,name);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ) ops_dump3(args[6].dat,name);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ) ops_dump3(args[7].dat,name);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ) ops_dump3(args[8].dat,name);
  if (args[9].argtype == OPS_ARG_DAT && args[9].acc != OPS_READ) ops_dump3(args[9].dat,name);
  if (args[10].argtype == OPS_ARG_DAT && args[10].acc != OPS_READ) ops_dump3(args[10].dat,name);
  if (args[11].argtype == OPS_ARG_DAT && args[11].acc != OPS_READ) ops_dump3(args[11].dat,name);
  if (args[12].argtype == OPS_ARG_DAT && args[12].acc != OPS_READ) ops_dump3(args[12].dat,name);
  if (args[13].argtype == OPS_ARG_DAT && args[13].acc != OPS_READ) ops_dump3(args[13].dat,name);
  if (args[14].argtype == OPS_ARG_DAT && args[14].acc != OPS_READ) ops_dump3(args[14].dat,name);
  if (args[15].argtype == OPS_ARG_DAT && args[15].acc != OPS_READ) ops_dump3(args[15].dat,name);
  if (args[16].argtype == OPS_ARG_DAT && args[16].acc != OPS_READ) ops_dump3(args[16].dat,name);
  if (args[17].argtype == OPS_ARG_DAT && args[17].acc != OPS_READ) ops_dump3(args[17].dat,name);
  #endif
  if (args[0].argtype == OPS_ARG_DAT && args[0].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[0],range);
  if (args[1].argtype == OPS_ARG_DAT && args[1].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[1],range);
  if (args[2].argtype == OPS_ARG_DAT && args[2].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[2],range);
  if (args[3].argtype == OPS_ARG_DAT && args[3].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[3],range);
  if (args[4].argtype == OPS_ARG_DAT && args[4].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[4],range);
  if (args[5].argtype == OPS_ARG_DAT && args[5].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[5],range);
  if (args[6].argtype == OPS_ARG_DAT && args[6].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[6],range);
  if (args[7].argtype == OPS_ARG_DAT && args[7].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[7],range);
  if (args[8].argtype == OPS_ARG_DAT && args[8].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[8],range);
  if (args[9].argtype == OPS_ARG_DAT && args[9].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[9],range);
  if (args[10].argtype == OPS_ARG_DAT && args[10].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[10],range);
  if (args[11].argtype == OPS_ARG_DAT && args[11].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[11],range);
  if (args[12].argtype == OPS_ARG_DAT && args[12].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[12],range);
  if (args[13].argtype == OPS_ARG_DAT && args[13].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[13],range);
  if (args[14].argtype == OPS_ARG_DAT && args[14].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[14],range);
  if (args[15].argtype == OPS_ARG_DAT && args[15].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[15],range);
  if (args[16].argtype == OPS_ARG_DAT && args[16].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[16],range);
  if (args[17].argtype == OPS_ARG_DAT && args[17].acc != OPS_READ)  ops_set_halo_dirtybit3(&args[17],range);
  ops_set_dirtybit_host(args, 18);
}
