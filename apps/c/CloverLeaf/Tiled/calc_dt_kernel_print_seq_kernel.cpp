//
// auto-generated by ops.py
//
#define OPS_ACC0(x, y)                                                         \
  (n_x * 1 + n_y * xdim0_calc_dt_kernel_print * 1 + x +                        \
   xdim0_calc_dt_kernel_print * (y))
#define OPS_ACC1(x, y)                                                         \
  (n_x * 1 + n_y * xdim1_calc_dt_kernel_print * 1 + x +                        \
   xdim1_calc_dt_kernel_print * (y))
#define OPS_ACC2(x, y)                                                         \
  (n_x * 1 + n_y * xdim2_calc_dt_kernel_print * 1 + x +                        \
   xdim2_calc_dt_kernel_print * (y))
#define OPS_ACC3(x, y)                                                         \
  (n_x * 1 + n_y * xdim3_calc_dt_kernel_print * 1 + x +                        \
   xdim3_calc_dt_kernel_print * (y))
#define OPS_ACC4(x, y)                                                         \
  (n_x * 1 + n_y * xdim4_calc_dt_kernel_print * 1 + x +                        \
   xdim4_calc_dt_kernel_print * (y))
#define OPS_ACC5(x, y)                                                         \
  (n_x * 1 + n_y * xdim5_calc_dt_kernel_print * 1 + x +                        \
   xdim5_calc_dt_kernel_print * (y))

// user function

// host stub function
void ops_par_loop_calc_dt_kernel_print_execute(ops_kernel_descriptor *desc) {
  ops_block block = desc->block;
  int dim = desc->dim;
  int *range = desc->range;
  ops_arg arg0 = desc->args[0];
  ops_arg arg1 = desc->args[1];
  ops_arg arg2 = desc->args[2];
  ops_arg arg3 = desc->args[3];
  ops_arg arg4 = desc->args[4];
  ops_arg arg5 = desc->args[5];
  ops_arg arg6 = desc->args[6];

  // Timing
  double t1, t2, c1, c2;

  ops_arg args[7] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6};

#ifdef CHECKPOINTING
  if (!ops_checkpointing_before(args, 7, range, 30))
    return;
#endif

  if (OPS_diags > 1) {
    OPS_kernels[30].count++;
    ops_timers_core(&c2, &t2);
  }

  // compute locally allocated range for the sub-block
  int start[2];
  int end[2];

  for (int n = 0; n < 2; n++) {
    start[n] = range[2 * n];
    end[n] = range[2 * n + 1];
  }

#ifdef OPS_DEBUG
  ops_register_args(args, "calc_dt_kernel_print");
#endif

  // set up initial pointers and exchange halos if necessary
  int base0 = args[0].dat->base_offset;
  const double *__restrict__ xvel0 = (double *)(args[0].data + base0);

  int base1 = args[1].dat->base_offset;
  const double *__restrict__ yvel0 = (double *)(args[1].data + base1);

  int base2 = args[2].dat->base_offset;
  const double *__restrict__ density0 = (double *)(args[2].data + base2);

  int base3 = args[3].dat->base_offset;
  const double *__restrict__ energy0 = (double *)(args[3].data + base3);

  int base4 = args[4].dat->base_offset;
  const double *__restrict__ pressure = (double *)(args[4].data + base4);

  int base5 = args[5].dat->base_offset;
  const double *__restrict__ soundspeed = (double *)(args[5].data + base5);

#ifdef OPS_MPI
  double *__restrict__ p_a6 =
      (double *)(((ops_reduction)args[6].data)->data +
                 ((ops_reduction)args[6].data)->size * block->index);
#else  // OPS_MPI
  double *__restrict__ p_a6 = (double *)((ops_reduction)args[6].data)->data;
#endif // OPS_MPI

  // initialize global variable with the dimension of dats
  int xdim0_calc_dt_kernel_print = args[0].dat->size[0];
  int xdim1_calc_dt_kernel_print = args[1].dat->size[0];
  int xdim2_calc_dt_kernel_print = args[2].dat->size[0];
  int xdim3_calc_dt_kernel_print = args[3].dat->size[0];
  int xdim4_calc_dt_kernel_print = args[4].dat->size[0];
  int xdim5_calc_dt_kernel_print = args[5].dat->size[0];

  if (OPS_diags > 1) {
    ops_timers_core(&c1, &t1);
    OPS_kernels[30].mpi_time += t1 - t2;
  }

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
#pragma omp parallel for reduction(+ : p_a6_0) reduction(                      \
    + : p_a6_1) reduction(+ : p_a6_2) reduction(+ : p_a6_3) reduction(         \
        + : p_a6_4) reduction(+ : p_a6_5) reduction(+ : p_a6_6) reduction(     \
            + : p_a6_7) reduction(+ : p_a6_8) reduction(+ : p_a6_9) reduction( \
                + : p_a6_10) reduction(+ : p_a6_11)
  for (int n_y = start[1]; n_y < end[1]; n_y++) {
#ifdef intel
#pragma loop_count(10000)
#pragma omp simd reduction(+ : p_a6_0) reduction(+ : p_a6_1) reduction(        \
    + : p_a6_2) reduction(+ : p_a6_3) reduction(+ : p_a6_4) reduction(         \
        + : p_a6_5) reduction(+ : p_a6_6) reduction(+ : p_a6_7) reduction(     \
            + : p_a6_8) reduction(+ : p_a6_9) reduction(                       \
                + : p_a6_10) reduction(+ : p_a6_11) aligned(                   \
                    xvel0, yvel0, density0, energy0, pressure, soundspeed)
#else
#pragma simd reduction(+ : p_a6_0) reduction(+ : p_a6_1) reduction(            \
    + : p_a6_2) reduction(+ : p_a6_3) reduction(+ : p_a6_4) reduction(         \
        + : p_a6_5) reduction(+ : p_a6_6) reduction(+ : p_a6_7) reduction(     \
            + : p_a6_8) reduction(+ : p_a6_9) reduction(                       \
                + : p_a6_10) reduction(+ : p_a6_11)
#endif
    for (int n_x = start[0]; n_x < end[0]; n_x++) {
      double output[12];
      output[0] = ZERO_double;
      output[1] = ZERO_double;
      output[2] = ZERO_double;
      output[3] = ZERO_double;
      output[4] = ZERO_double;
      output[5] = ZERO_double;
      output[6] = ZERO_double;
      output[7] = ZERO_double;
      output[8] = ZERO_double;
      output[9] = ZERO_double;
      output[10] = ZERO_double;
      output[11] = ZERO_double;

      output[0] = xvel0[OPS_ACC0(1, 0)];
      output[1] = yvel0[OPS_ACC1(1, 0)];
      output[2] = xvel0[OPS_ACC0(-1, 0)];
      output[3] = yvel0[OPS_ACC1(-1, 0)];
      output[4] = xvel0[OPS_ACC0(0, 1)];
      output[5] = yvel0[OPS_ACC1(0, 1)];
      output[6] = xvel0[OPS_ACC0(0, -1)];
      output[7] = yvel0[OPS_ACC1(0, -1)];
      output[8] = density0[OPS_ACC2(0, 0)];
      output[9] = energy0[OPS_ACC3(0, 0)];
      output[10] = pressure[OPS_ACC4(0, 0)];
      output[11] = soundspeed[OPS_ACC5(0, 0)];

      p_a6_0 += output[0];
      p_a6_1 += output[1];
      p_a6_2 += output[2];
      p_a6_3 += output[3];
      p_a6_4 += output[4];
      p_a6_5 += output[5];
      p_a6_6 += output[6];
      p_a6_7 += output[7];
      p_a6_8 += output[8];
      p_a6_9 += output[9];
      p_a6_10 += output[10];
      p_a6_11 += output[11];
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
  if (OPS_diags > 1) {
    ops_timers_core(&c2, &t2);
    OPS_kernels[30].time += t2 - t1;
  }

  if (OPS_diags > 1) {
    // Update kernel record
    ops_timers_core(&c1, &t1);
    OPS_kernels[30].mpi_time += t1 - t2;
    OPS_kernels[30].transfer += ops_compute_transfer(dim, start, end, &arg0);
    OPS_kernels[30].transfer += ops_compute_transfer(dim, start, end, &arg1);
    OPS_kernels[30].transfer += ops_compute_transfer(dim, start, end, &arg2);
    OPS_kernels[30].transfer += ops_compute_transfer(dim, start, end, &arg3);
    OPS_kernels[30].transfer += ops_compute_transfer(dim, start, end, &arg4);
    OPS_kernels[30].transfer += ops_compute_transfer(dim, start, end, &arg5);
  }
}
#undef OPS_ACC0
#undef OPS_ACC1
#undef OPS_ACC2
#undef OPS_ACC3
#undef OPS_ACC4
#undef OPS_ACC5

void ops_par_loop_calc_dt_kernel_print(char const *name, ops_block block,
                                       int dim, int *range, ops_arg arg0,
                                       ops_arg arg1, ops_arg arg2, ops_arg arg3,
                                       ops_arg arg4, ops_arg arg5,
                                       ops_arg arg6) {
  ops_kernel_descriptor *desc =
      (ops_kernel_descriptor *)malloc(sizeof(ops_kernel_descriptor));
  desc->name = name;
  desc->block = block;
  desc->dim = dim;
  desc->device = 1;
  desc->index = 30;
  desc->hash = 5381;
  desc->hash = ((desc->hash << 5) + desc->hash) + 30;
  for (int i = 0; i < 4; i++) {
    desc->range[i] = range[i];
    desc->orig_range[i] = range[i];
    desc->hash = ((desc->hash << 5) + desc->hash) + range[i];
  }
  desc->nargs = 7;
  desc->args = (ops_arg *)malloc(7 * sizeof(ops_arg));
  desc->args[0] = arg0;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg0.dat->index;
  desc->args[1] = arg1;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg1.dat->index;
  desc->args[2] = arg2;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg2.dat->index;
  desc->args[3] = arg3;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg3.dat->index;
  desc->args[4] = arg4;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg4.dat->index;
  desc->args[5] = arg5;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg5.dat->index;
  desc->args[6] = arg6;
  desc->function = ops_par_loop_calc_dt_kernel_print_execute;
  if (OPS_diags > 1) {
    ops_timing_realloc(30, "calc_dt_kernel_print");
  }
  ops_enqueue_kernel(desc);
}
