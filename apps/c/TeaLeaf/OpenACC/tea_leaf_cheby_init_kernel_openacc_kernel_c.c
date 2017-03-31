//
// auto-generated by ops.py
//
#include "./OpenACC/tea_leaf_common.h"

#define OPS_GPU

int xdim0_tea_leaf_cheby_init_kernel;
int xdim1_tea_leaf_cheby_init_kernel;
int xdim2_tea_leaf_cheby_init_kernel;
int xdim3_tea_leaf_cheby_init_kernel;
int xdim4_tea_leaf_cheby_init_kernel;
int xdim5_tea_leaf_cheby_init_kernel;

#undef OPS_ACC0
#undef OPS_ACC1
#undef OPS_ACC2
#undef OPS_ACC3
#undef OPS_ACC4
#undef OPS_ACC5

#define OPS_ACC0(x, y) (x + xdim0_tea_leaf_cheby_init_kernel * (y))
#define OPS_ACC1(x, y) (x + xdim1_tea_leaf_cheby_init_kernel * (y))
#define OPS_ACC2(x, y) (x + xdim2_tea_leaf_cheby_init_kernel * (y))
#define OPS_ACC3(x, y) (x + xdim3_tea_leaf_cheby_init_kernel * (y))
#define OPS_ACC4(x, y) (x + xdim4_tea_leaf_cheby_init_kernel * (y))
#define OPS_ACC5(x, y) (x + xdim5_tea_leaf_cheby_init_kernel * (y))

// user function
inline void tea_leaf_cheby_init_kernel(double *w, double *r, const double *Kx,
                                       const double *Ky, const double *u,
                                       const double *u0, const double *rx,
                                       const double *ry) {
  w[OPS_ACC0(0, 0)] = (1.0 + (*ry) * (Ky[OPS_ACC3(0, 1)] + Ky[OPS_ACC3(0, 0)]) +
                       (*rx) * (Kx[OPS_ACC2(1, 0)] + Kx[OPS_ACC2(0, 0)])) *
                          u[OPS_ACC4(0, 0)] -
                      (*ry) * (Ky[OPS_ACC3(0, 1)] * u[OPS_ACC4(0, 1)] +
                               Ky[OPS_ACC3(0, 0)] * u[OPS_ACC4(0, -1)]) -
                      (*rx) * (Kx[OPS_ACC2(1, 0)] * u[OPS_ACC4(1, 0)] +
                               Kx[OPS_ACC2(0, 0)] * u[OPS_ACC4(-1, 0)]);
  r[OPS_ACC1(0, 0)] = u0[OPS_ACC5(0, 0)] - w[OPS_ACC0(0, 0)];
}

#undef OPS_ACC0
#undef OPS_ACC1
#undef OPS_ACC2
#undef OPS_ACC3
#undef OPS_ACC4
#undef OPS_ACC5

void tea_leaf_cheby_init_kernel_c_wrapper(double *p_a0, double *p_a1,
                                          double *p_a2, double *p_a3,
                                          double *p_a4, double *p_a5,
                                          double p_a6, double p_a7, int x_size,
                                          int y_size) {
#ifdef OPS_GPU
#pragma acc parallel deviceptr(p_a0, p_a1, p_a2, p_a3, p_a4, p_a5)
#pragma acc loop
#endif
  for (int n_y = 0; n_y < y_size; n_y++) {
#ifdef OPS_GPU
#pragma acc loop
#endif
    for (int n_x = 0; n_x < x_size; n_x++) {
      tea_leaf_cheby_init_kernel(
          p_a0 + n_x * 1 * 1 + n_y * xdim0_tea_leaf_cheby_init_kernel * 1 * 1,
          p_a1 + n_x * 1 * 1 + n_y * xdim1_tea_leaf_cheby_init_kernel * 1 * 1,
          p_a2 + n_x * 1 * 1 + n_y * xdim2_tea_leaf_cheby_init_kernel * 1 * 1,
          p_a3 + n_x * 1 * 1 + n_y * xdim3_tea_leaf_cheby_init_kernel * 1 * 1,
          p_a4 + n_x * 1 * 1 + n_y * xdim4_tea_leaf_cheby_init_kernel * 1 * 1,
          p_a5 + n_x * 1 * 1 + n_y * xdim5_tea_leaf_cheby_init_kernel * 1 * 1,
          &p_a6, &p_a7);
    }
  }
}
