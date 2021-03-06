//
// auto-generated by ops.py
//

#include "./MPI_inline/tea_leaf_common.h"


void ops_init_backend() {}

void ops_decl_const_char2(int dim, char const *type,
int size, char *dat, char const *name){
  if (!strcmp(name,"field")) {
    field = *(field_type*)dat;
  }
  else
  if (!strcmp(name,"grid")) {
    grid = *(grid_type*)dat;
  }
  else
  if (!strcmp(name,"number_of_states")) {
    number_of_states = *(int*)dat;
  }
  else
  if (!strcmp(name,"states")) {
    states = (state_type*)dat;
  }
  else
  if (!strcmp(name,"g_circ")) {
    g_circ = *(int*)dat;
  }
  else
  if (!strcmp(name,"g_point")) {
    g_point = *(int*)dat;
  }
  else
  if (!strcmp(name,"g_rect")) {
    g_rect = *(int*)dat;
  }
  else
  {
    printf("error: unknown const name\n"); exit(1);
  }
}

//user kernel files
#include "field_summary_kernel_mpiinline_kernel.cpp"
#include "generate_chunk_kernel_mpiinline_kernel.cpp"
#include "initialise_chunk_kernel_zero_mpiinline_kernel.cpp"
#include "initialise_chunk_kernel_zero_x_mpiinline_kernel.cpp"
#include "initialise_chunk_kernel_zero_y_mpiinline_kernel.cpp"
#include "initialise_chunk_kernel_xx_mpiinline_kernel.cpp"
#include "initialise_chunk_kernel_yy_mpiinline_kernel.cpp"
#include "initialise_chunk_kernel_x_mpiinline_kernel.cpp"
#include "initialise_chunk_kernel_y_mpiinline_kernel.cpp"
#include "initialise_chunk_kernel_cellx_mpiinline_kernel.cpp"
#include "initialise_chunk_kernel_celly_mpiinline_kernel.cpp"
#include "initialise_chunk_kernel_volume_mpiinline_kernel.cpp"
#include "set_field_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_init_zero2_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_yeqx_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_dot_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_cg_calc_w_reduce_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_axpy_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_cg_calc_ur_r_reduce_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_axpby_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_cheby_init_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_recip3_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_xpy_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_common_init_u_u0_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_recip_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_common_init_Kx_Ky_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_init_zero_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_common_init_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_recip2_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_common_residual_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_norm2_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_common_init_diag_init_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_zeqxty_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_jacobi_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_ppcg_init1_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_ppcg_init2_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_ppcg_inner1_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_ppcg_inner2_kernel_mpiinline_kernel.cpp"
#include "tea_leaf_ppcg_reduce_kernel_mpiinline_kernel.cpp"
#include "update_halo_kernel1_b2_mpiinline_kernel.cpp"
#include "update_halo_kernel1_b1_mpiinline_kernel.cpp"
#include "update_halo_kernel1_t2_mpiinline_kernel.cpp"
#include "update_halo_kernel1_t1_mpiinline_kernel.cpp"
#include "update_halo_kernel1_l2_mpiinline_kernel.cpp"
#include "update_halo_kernel1_l1_mpiinline_kernel.cpp"
#include "update_halo_kernel1_r2_mpiinline_kernel.cpp"
#include "update_halo_kernel1_r1_mpiinline_kernel.cpp"
