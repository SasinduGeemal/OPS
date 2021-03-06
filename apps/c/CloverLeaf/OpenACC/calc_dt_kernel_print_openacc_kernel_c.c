//
// auto-generated by ops.py
//

int xdim0_calc_dt_kernel_print;
int xdim1_calc_dt_kernel_print;
int xdim2_calc_dt_kernel_print;
int xdim3_calc_dt_kernel_print;
int xdim4_calc_dt_kernel_print;
int xdim5_calc_dt_kernel_print;


#undef OPS_ACC0
#undef OPS_ACC1
#undef OPS_ACC2
#undef OPS_ACC3
#undef OPS_ACC4
#undef OPS_ACC5


#define OPS_ACC0(x,y) (x+xdim0_calc_dt_kernel_print*(y))
#define OPS_ACC1(x,y) (x+xdim1_calc_dt_kernel_print*(y))
#define OPS_ACC2(x,y) (x+xdim2_calc_dt_kernel_print*(y))
#define OPS_ACC3(x,y) (x+xdim3_calc_dt_kernel_print*(y))
#define OPS_ACC4(x,y) (x+xdim4_calc_dt_kernel_print*(y))
#define OPS_ACC5(x,y) (x+xdim5_calc_dt_kernel_print*(y))

//user function
inline 
void calc_dt_kernel_print(const double *xvel0, const double *yvel0,
                        const double *density0, const double *energy0,
                        const double *pressure, const double *soundspeed, double *output) {
  output[0] = xvel0[OPS_ACC0(1,0)];
  output[1] = yvel0[OPS_ACC1(1,0)];
  output[2] = xvel0[OPS_ACC0(-1,0)];
  output[3] = yvel0[OPS_ACC1(-1,0)];
  output[4] = xvel0[OPS_ACC0(0,1)];
  output[5] = yvel0[OPS_ACC1(0,1)];
  output[6] = xvel0[OPS_ACC0(0,-1)];
  output[7] = yvel0[OPS_ACC1(0,-1)];
  output[8] = density0[OPS_ACC2(0,0)];
  output[9] = energy0[OPS_ACC3(0,0)];
  output[10]= pressure[OPS_ACC4(0,0)];
  output[11]= soundspeed[OPS_ACC5(0,0)];

}


#undef OPS_ACC0
#undef OPS_ACC1
#undef OPS_ACC2
#undef OPS_ACC3
#undef OPS_ACC4
#undef OPS_ACC5



void calc_dt_kernel_print_c_wrapper(
  double *p_a0,
  double *p_a1,
  double *p_a2,
  double *p_a3,
  double *p_a4,
  double *p_a5,
  double *p_a6,
  int x_size, int y_size) {
  double p_a6_0 = p_a6[0];
  double p_a6_1 = p_a6[1];
  double p_a6_2 = p_a6[2];
  double p_a6_3 = p_a6[3];
  double p_a6_4 = p_a6[4];
  double p_a6_5 = p_a6[5];
  double p_a6_6 = p_a6[6];
  double p_a6_7 = p_a6[7];
  double p_a6_8 = p_a6[8];
  double p_a6_9 = p_a6[9];
  double p_a6_10 = p_a6[10];
  double p_a6_11 = p_a6[11];
  #ifdef OPS_GPU
  #pragma acc parallel deviceptr(p_a0,p_a1,p_a2,p_a3,p_a4,p_a5) reduction(+:p_a6_0) reduction(+:p_a6_1) reduction(+:p_a6_2) reduction(+:p_a6_3) reduction(+:p_a6_4) reduction(+:p_a6_5) reduction(+:p_a6_6) reduction(+:p_a6_7) reduction(+:p_a6_8) reduction(+:p_a6_9) reduction(+:p_a6_10) reduction(+:p_a6_11)
  #pragma acc loop reduction(+:p_a6_0) reduction(+:p_a6_1) reduction(+:p_a6_2) reduction(+:p_a6_3) reduction(+:p_a6_4) reduction(+:p_a6_5) reduction(+:p_a6_6) reduction(+:p_a6_7) reduction(+:p_a6_8) reduction(+:p_a6_9) reduction(+:p_a6_10) reduction(+:p_a6_11)
  #endif
  for ( int n_y=0; n_y<y_size; n_y++ ){
    #ifdef OPS_GPU
    #pragma acc loop reduction(+:p_a6_0) reduction(+:p_a6_1) reduction(+:p_a6_2) reduction(+:p_a6_3) reduction(+:p_a6_4) reduction(+:p_a6_5) reduction(+:p_a6_6) reduction(+:p_a6_7) reduction(+:p_a6_8) reduction(+:p_a6_9) reduction(+:p_a6_10) reduction(+:p_a6_11)
    #endif
    for ( int n_x=0; n_x<x_size; n_x++ ){
      double p_a6_local[12];
      p_a6_local[0] = ZERO_double;
      p_a6_local[1] = ZERO_double;
      p_a6_local[2] = ZERO_double;
      p_a6_local[3] = ZERO_double;
      p_a6_local[4] = ZERO_double;
      p_a6_local[5] = ZERO_double;
      p_a6_local[6] = ZERO_double;
      p_a6_local[7] = ZERO_double;
      p_a6_local[8] = ZERO_double;
      p_a6_local[9] = ZERO_double;
      p_a6_local[10] = ZERO_double;
      p_a6_local[11] = ZERO_double;
      calc_dt_kernel_print(  p_a0 + n_x*1*1 + n_y*xdim0_calc_dt_kernel_print*1*1,
           p_a1 + n_x*1*1 + n_y*xdim1_calc_dt_kernel_print*1*1, p_a2 + n_x*1*1 + n_y*xdim2_calc_dt_kernel_print*1*1,
           p_a3 + n_x*1*1 + n_y*xdim3_calc_dt_kernel_print*1*1, p_a4 + n_x*1*1 + n_y*xdim4_calc_dt_kernel_print*1*1,
           p_a5 + n_x*1*1 + n_y*xdim5_calc_dt_kernel_print*1*1, p_a6_local );

      p_a6_0 +=p_a6_local[0];
      p_a6_1 +=p_a6_local[1];
      p_a6_2 +=p_a6_local[2];
      p_a6_3 +=p_a6_local[3];
      p_a6_4 +=p_a6_local[4];
      p_a6_5 +=p_a6_local[5];
      p_a6_6 +=p_a6_local[6];
      p_a6_7 +=p_a6_local[7];
      p_a6_8 +=p_a6_local[8];
      p_a6_9 +=p_a6_local[9];
      p_a6_10 +=p_a6_local[10];
      p_a6_11 +=p_a6_local[11];
    }
  }
  p_a6[0] = p_a6_0;
  p_a6[1] = p_a6_1;
  p_a6[2] = p_a6_2;
  p_a6[3] = p_a6_3;
  p_a6[4] = p_a6_4;
  p_a6[5] = p_a6_5;
  p_a6[6] = p_a6_6;
  p_a6[7] = p_a6_7;
  p_a6[8] = p_a6_8;
  p_a6[9] = p_a6_9;
  p_a6[10] = p_a6_10;
  p_a6[11] = p_a6_11;
}
