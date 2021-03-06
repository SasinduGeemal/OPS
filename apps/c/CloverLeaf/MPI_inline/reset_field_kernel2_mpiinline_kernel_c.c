//
// auto-generated by ops.py
//

int xdim0_reset_field_kernel2;
int xdim1_reset_field_kernel2;
int xdim2_reset_field_kernel2;
int xdim3_reset_field_kernel2;


#define OPS_ACC0(x,y) (n_x*1+n_y*xdim0_reset_field_kernel2*1+x+xdim0_reset_field_kernel2*(y))
#define OPS_ACC1(x,y) (n_x*1+n_y*xdim1_reset_field_kernel2*1+x+xdim1_reset_field_kernel2*(y))
#define OPS_ACC2(x,y) (n_x*1+n_y*xdim2_reset_field_kernel2*1+x+xdim2_reset_field_kernel2*(y))
#define OPS_ACC3(x,y) (n_x*1+n_y*xdim3_reset_field_kernel2*1+x+xdim3_reset_field_kernel2*(y))

//user function



void reset_field_kernel2_c_wrapper(
  double * restrict xvel0,
  const double * restrict xvel1,
  double * restrict yvel0,
  const double * restrict yvel1,
  int x_size, int y_size) {
  #pragma omp parallel for
  for ( int n_y=0; n_y<y_size; n_y++ ){
    for ( int n_x=0; n_x<x_size; n_x++ ){
      

  xvel0[OPS_ACC0(0,0)]  = xvel1[OPS_ACC1(0,0)] ;
  yvel0[OPS_ACC2(0,0)]  = yvel1[OPS_ACC3(0,0)] ;


    }
  }
}
#undef OPS_ACC0
#undef OPS_ACC1
#undef OPS_ACC2
#undef OPS_ACC3

