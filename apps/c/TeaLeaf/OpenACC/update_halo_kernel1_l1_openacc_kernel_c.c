//
// auto-generated by ops.py
//
#include "./OpenACC/tea_leaf_common.h"

#define OPS_GPU

int xdim0_update_halo_kernel1_l1;
int xdim1_update_halo_kernel1_l1;
int xdim2_update_halo_kernel1_l1;
int xdim3_update_halo_kernel1_l1;
int xdim4_update_halo_kernel1_l1;
int xdim5_update_halo_kernel1_l1;

#undef OPS_ACC0
#undef OPS_ACC1
#undef OPS_ACC2
#undef OPS_ACC3
#undef OPS_ACC4
#undef OPS_ACC5

#define OPS_ACC0(x, y) (x + xdim0_update_halo_kernel1_l1 * (y))
#define OPS_ACC1(x, y) (x + xdim1_update_halo_kernel1_l1 * (y))
#define OPS_ACC2(x, y) (x + xdim2_update_halo_kernel1_l1 * (y))
#define OPS_ACC3(x, y) (x + xdim3_update_halo_kernel1_l1 * (y))
#define OPS_ACC4(x, y) (x + xdim4_update_halo_kernel1_l1 * (y))
#define OPS_ACC5(x, y) (x + xdim5_update_halo_kernel1_l1 * (y))

// user function

inline void update_halo_kernel1_l1(double *density0, double *energy0,
                                   double *energy1, double *u, double *p,
                                   double *sd, const int *fields) {
  if (fields[FIELD_DENSITY] == 1)
    density0[OPS_ACC0(0, 0)] = density0[OPS_ACC0(1, 0)];
  if (fields[FIELD_ENERGY0] == 1)
    energy0[OPS_ACC1(0, 0)] = energy0[OPS_ACC1(1, 0)];
  if (fields[FIELD_ENERGY1] == 1)
    energy1[OPS_ACC2(0, 0)] = energy1[OPS_ACC2(1, 0)];
  if (fields[FIELD_U] == 1)
    u[OPS_ACC3(0, 0)] = u[OPS_ACC3(1, 0)];
  if (fields[FIELD_P] == 1)
    p[OPS_ACC4(0, 0)] = p[OPS_ACC4(1, 0)];
  if (fields[FIELD_SD] == 1)
    sd[OPS_ACC5(0, 0)] = sd[OPS_ACC5(1, 0)];
}

#undef OPS_ACC0
#undef OPS_ACC1
#undef OPS_ACC2
#undef OPS_ACC3
#undef OPS_ACC4
#undef OPS_ACC5

void update_halo_kernel1_l1_c_wrapper(double *p_a0, double *p_a1, double *p_a2,
                                      double *p_a3, double *p_a4, double *p_a5,
                                      int *p_a6, int x_size, int y_size) {
#ifdef OPS_GPU
#pragma acc parallel deviceptr(p_a0, p_a1, p_a2, p_a3, p_a4, p_a5, p_a6)
#pragma acc loop
#endif
  for (int n_y = 0; n_y < y_size; n_y++) {
#ifdef OPS_GPU
#pragma acc loop
#endif
    for (int n_x = 0; n_x < x_size; n_x++) {
      update_halo_kernel1_l1(
          p_a0 + n_x * 1 * 1 + n_y * xdim0_update_halo_kernel1_l1 * 1 * 1,
          p_a1 + n_x * 1 * 1 + n_y * xdim1_update_halo_kernel1_l1 * 1 * 1,
          p_a2 + n_x * 1 * 1 + n_y * xdim2_update_halo_kernel1_l1 * 1 * 1,
          p_a3 + n_x * 1 * 1 + n_y * xdim3_update_halo_kernel1_l1 * 1 * 1,
          p_a4 + n_x * 1 * 1 + n_y * xdim4_update_halo_kernel1_l1 * 1 * 1,
          p_a5 + n_x * 1 * 1 + n_y * xdim5_update_halo_kernel1_l1 * 1 * 1,
          p_a6);
    }
  }
}
