//
// auto-generated by ops.py
//
#include "./MPI_inline/laplace2d_common.h"

int xdim0_copy;
int xdim1_copy;


#define OPS_ACC0(x,y) (n_x*1+n_y*xdim0_copy*1+x+xdim0_copy*(y))
#define OPS_ACC1(x,y) (n_x*1+n_y*xdim1_copy*1+x+xdim1_copy*(y))

//user function



void copy_c_wrapper(
  double * restrict A,
  const double * restrict Anew,
  int x_size, int y_size) {
  #pragma omp parallel for
  for ( int n_y=0; n_y<y_size; n_y++ ){
    for ( int n_x=0; n_x<x_size; n_x++ ){
      
  A[OPS_ACC0(0,0)] = Anew[OPS_ACC1(0,0)];

    }
  }
}
#undef OPS_ACC0
#undef OPS_ACC1

