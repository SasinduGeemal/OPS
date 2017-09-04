//
// auto-generated by ops.py
//
#define OPS_ACC0(x, y)                                                         \
  (n_x * 1 + n_y * xdim0_tea_leaf_ppcg_inner1_kernel * 1 + x +                 \
   xdim0_tea_leaf_ppcg_inner1_kernel * (y))
#define OPS_ACC1(x, y)                                                         \
  (n_x * 1 + n_y * xdim1_tea_leaf_ppcg_inner1_kernel * 1 + x +                 \
   xdim1_tea_leaf_ppcg_inner1_kernel * (y))
#define OPS_ACC2(x, y)                                                         \
  (n_x * 1 + n_y * xdim2_tea_leaf_ppcg_inner1_kernel * 1 + x +                 \
   xdim2_tea_leaf_ppcg_inner1_kernel * (y))
#define OPS_ACC3(x, y)                                                         \
  (n_x * 1 + n_y * xdim3_tea_leaf_ppcg_inner1_kernel * 1 + x +                 \
   xdim3_tea_leaf_ppcg_inner1_kernel * (y))

// user function

// host stub function
void ops_par_loop_tea_leaf_ppcg_inner1_kernel_execute(
    ops_kernel_descriptor *desc) {
  ops_block block = desc->block;
  int dim = desc->dim;
  int *range = desc->range;
  ops_arg arg0 = desc->args[0];
  ops_arg arg1 = desc->args[1];
  ops_arg arg2 = desc->args[2];
  ops_arg arg3 = desc->args[3];
  ops_arg arg4 = desc->args[4];
  ops_arg arg5 = desc->args[5];

  // Timing
  double t1, t2, c1, c2;

  ops_arg args[6] = {arg0, arg1, arg2, arg3, arg4, arg5};

#ifdef CHECKPOINTING
  if (!ops_checkpointing_before(args, 6, range, 46))
    return;
#endif

  if (OPS_diags > 1) {
    OPS_kernels[46].count++;
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
  ops_register_args(args, "tea_leaf_ppcg_inner1_kernel");
#endif

  // set up initial pointers and exchange halos if necessary
  int base0 = args[0].dat->base_offset;
  double *__restrict__ rtemp = (double *)(args[0].data + base0);

  int base1 = args[1].dat->base_offset;
  const double *__restrict__ Kx = (double *)(args[1].data + base1);

  int base2 = args[2].dat->base_offset;
  const double *__restrict__ Ky = (double *)(args[2].data + base2);

  int base3 = args[3].dat->base_offset;
  const double *__restrict__ sd = (double *)(args[3].data + base3);

  const double *__restrict__ rx = (double *)args[4].data;

  const double *__restrict__ ry = (double *)args[5].data;

  // initialize global variable with the dimension of dats
  int xdim0_tea_leaf_ppcg_inner1_kernel = args[0].dat->size[0];
  int xdim1_tea_leaf_ppcg_inner1_kernel = args[1].dat->size[0];
  int xdim2_tea_leaf_ppcg_inner1_kernel = args[2].dat->size[0];
  int xdim3_tea_leaf_ppcg_inner1_kernel = args[3].dat->size[0];

  if (OPS_diags > 1) {
    ops_timers_core(&c1, &t1);
    OPS_kernels[46].mpi_time += t1 - t2;
  }

#pragma omp parallel for
  for (int n_y = start[1]; n_y < end[1]; n_y++) {
#ifdef intel
#pragma loop_count(10000)
#pragma omp simd aligned(rtemp, Kx, Ky, sd)
#else
#pragma simd
#endif
    for (int n_x = start[0]; n_x < end[0]; n_x++) {

      double smvp = 0.0;
      smvp = (1.0 + (*ry) * (Ky[OPS_ACC2(0, 1)] + Ky[OPS_ACC2(0, 0)]) +
              (*rx) * (Kx[OPS_ACC1(1, 0)] + Kx[OPS_ACC1(0, 0)])) *
                 sd[OPS_ACC3(0, 0)] -
             (*ry) * (Ky[OPS_ACC2(0, 1)] * sd[OPS_ACC3(0, 1)] +
                      Ky[OPS_ACC2(0, 0)] * sd[OPS_ACC3(0, -1)]) -
             (*rx) * (Kx[OPS_ACC1(1, 0)] * sd[OPS_ACC3(1, 0)] +
                      Kx[OPS_ACC1(0, 0)] * sd[OPS_ACC3(-1, 0)]);
      rtemp[OPS_ACC0(0, 0)] = rtemp[OPS_ACC0(0, 0)] - smvp;
    }
  }
  if (OPS_diags > 1) {
    ops_timers_core(&c2, &t2);
    OPS_kernels[46].time += t2 - t1;
  }

  if (OPS_diags > 1) {
    // Update kernel record
    ops_timers_core(&c1, &t1);
    OPS_kernels[46].mpi_time += t1 - t2;
    OPS_kernels[46].transfer += ops_compute_transfer(dim, start, end, &arg0);
    OPS_kernels[46].transfer += ops_compute_transfer(dim, start, end, &arg1);
    OPS_kernels[46].transfer += ops_compute_transfer(dim, start, end, &arg2);
    OPS_kernels[46].transfer += ops_compute_transfer(dim, start, end, &arg3);
  }
}
#undef OPS_ACC0
#undef OPS_ACC1
#undef OPS_ACC2
#undef OPS_ACC3

void ops_par_loop_tea_leaf_ppcg_inner1_kernel(char const *name, ops_block block,
                                              int dim, int *range, ops_arg arg0,
                                              ops_arg arg1, ops_arg arg2,
                                              ops_arg arg3, ops_arg arg4,
                                              ops_arg arg5) {
  ops_kernel_descriptor *desc =
      (ops_kernel_descriptor *)malloc(sizeof(ops_kernel_descriptor));
  desc->name = name;
  desc->block = block;
  desc->dim = dim;
  desc->device = 1;
  desc->index = 46;
  desc->hash = 5381;
  desc->hash = ((desc->hash << 5) + desc->hash) + 46;
  for (int i = 0; i < 4; i++) {
    desc->range[i] = range[i];
    desc->orig_range[i] = range[i];
    desc->hash = ((desc->hash << 5) + desc->hash) + range[i];
  }
  desc->nargs = 6;
  desc->args = (ops_arg *)malloc(6 * sizeof(ops_arg));
  desc->args[0] = arg0;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg0.dat->index;
  desc->args[1] = arg1;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg1.dat->index;
  desc->args[2] = arg2;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg2.dat->index;
  desc->args[3] = arg3;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg3.dat->index;
  desc->args[4] = arg4;
  char *tmp = (char *)malloc(1 * sizeof(double));
  memcpy(tmp, arg4.data, 1 * sizeof(double));
  desc->args[4].data = tmp;
  desc->args[5] = arg5;
  tmp = (char *)malloc(1 * sizeof(double));
  memcpy(tmp, arg5.data, 1 * sizeof(double));
  desc->args[5].data = tmp;
  desc->function = ops_par_loop_tea_leaf_ppcg_inner1_kernel_execute;
  if (OPS_diags > 1) {
    ops_timing_realloc(46, "tea_leaf_ppcg_inner1_kernel");
  }
  ops_enqueue_kernel(desc);
}
