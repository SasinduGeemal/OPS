//
// auto-generated by ops.py
//
#include "./OpenACC/clover_leaf_common.h"

#define OPS_GPU

int xdim0_update_halo_kernel2_yvel_plus_4_a;
int xdim1_update_halo_kernel2_yvel_plus_4_a;


#undef OPS_ACC0
#undef OPS_ACC1


#define OPS_ACC0(x,y) (x+xdim0_update_halo_kernel2_yvel_plus_4_a*(y))
#define OPS_ACC1(x,y) (x+xdim1_update_halo_kernel2_yvel_plus_4_a*(y))

//user function

inline void update_halo_kernel2_yvel_plus_4_a(double *yvel0, double *yvel1, const int* fields) {
  if((*fields) & FIELD_YVEL0) yvel0[OPS_ACC0(0,0)] = yvel0[OPS_ACC0(4,0)];
  if((*fields) & FIELD_YVEL1) yvel1[OPS_ACC1(0,0)] = yvel1[OPS_ACC1(4,0)];
}


#undef OPS_ACC0
#undef OPS_ACC1



void update_halo_kernel2_yvel_plus_4_a_c_wrapper(
  double *p_a0,
  double *p_a1,
  int p_a2,
  int x_size, int y_size) {
  #ifdef OPS_GPU
  #pragma acc parallel deviceptr(p_a0,p_a1)
  #pragma acc loop
  #endif
  for ( int n_y=0; n_y<y_size; n_y++ ){
    #ifdef OPS_GPU
    #pragma acc loop
    #endif
    for ( int n_x=0; n_x<x_size; n_x++ ){
      update_halo_kernel2_yvel_plus_4_a(  p_a0 + n_x*1*1 + n_y*xdim0_update_halo_kernel2_yvel_plus_4_a*1*1,
           p_a1 + n_x*1*1 + n_y*xdim1_update_halo_kernel2_yvel_plus_4_a*1*1, &p_a2 );

    }
  }
}
