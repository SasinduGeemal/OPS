//
// auto-generated by ops.py
//
__constant__ int xdim0_tea_leaf_jacobi_kernel;
int xdim0_tea_leaf_jacobi_kernel_h = -1;
int ydim0_tea_leaf_jacobi_kernel_h = -1;
__constant__ int xdim1_tea_leaf_jacobi_kernel;
int xdim1_tea_leaf_jacobi_kernel_h = -1;
int ydim1_tea_leaf_jacobi_kernel_h = -1;
__constant__ int xdim2_tea_leaf_jacobi_kernel;
int xdim2_tea_leaf_jacobi_kernel_h = -1;
int ydim2_tea_leaf_jacobi_kernel_h = -1;
__constant__ int xdim3_tea_leaf_jacobi_kernel;
int xdim3_tea_leaf_jacobi_kernel_h = -1;
int ydim3_tea_leaf_jacobi_kernel_h = -1;
__constant__ int xdim4_tea_leaf_jacobi_kernel;
int xdim4_tea_leaf_jacobi_kernel_h = -1;
int ydim4_tea_leaf_jacobi_kernel_h = -1;

#undef OPS_ACC0
#undef OPS_ACC1
#undef OPS_ACC2
#undef OPS_ACC3
#undef OPS_ACC4

#define OPS_ACC0(x, y) (x + xdim0_tea_leaf_jacobi_kernel * (y))
#define OPS_ACC1(x, y) (x + xdim1_tea_leaf_jacobi_kernel * (y))
#define OPS_ACC2(x, y) (x + xdim2_tea_leaf_jacobi_kernel * (y))
#define OPS_ACC3(x, y) (x + xdim3_tea_leaf_jacobi_kernel * (y))
#define OPS_ACC4(x, y) (x + xdim4_tea_leaf_jacobi_kernel * (y))

// user function
__device__

    void
    tea_leaf_jacobi_kernel_gpu(double *u1, const double *Kx, const double *Ky,
                               const double *un, const double *u0,
                               const double *rx, const double *ry,
                               double *error) {
  u1[OPS_ACC0(0, 0)] =
      (u0[OPS_ACC4(0, 0)] +
       (*rx) * (Kx[OPS_ACC1(1, 0)] * un[OPS_ACC3(1, 0)] +
                Kx[OPS_ACC1(0, 0)] * un[OPS_ACC3(-1, 0)]) +
       (*ry) * (Ky[OPS_ACC2(0, 1)] * un[OPS_ACC3(0, 1)] +
                Ky[OPS_ACC2(0, 0)] * un[OPS_ACC3(0, -1)])) /
      (1.0 + (*rx) * (Kx[OPS_ACC1(1, 0)] + Kx[OPS_ACC1(0, 0)]) +
       (*ry) * (Ky[OPS_ACC2(0, 1)] + Ky[OPS_ACC2(0, 0)]));

  *error = *error + fabs(u1[OPS_ACC0(0, 0)] - un[OPS_ACC3(0, 0)]);
}

#undef OPS_ACC0
#undef OPS_ACC1
#undef OPS_ACC2
#undef OPS_ACC3
#undef OPS_ACC4

__global__ void ops_tea_leaf_jacobi_kernel(
    double *__restrict arg0, const double *__restrict arg1,
    const double *__restrict arg2, const double *__restrict arg3,
    const double *__restrict arg4, const double arg5, const double arg6,
    double *__restrict arg7, int size0, int size1) {

  double arg7_l[1];
  for (int d = 0; d < 1; d++)
    arg7_l[d] = ZERO_double;

  int idx_y = blockDim.y * blockIdx.y + threadIdx.y;
  int idx_x = blockDim.x * blockIdx.x + threadIdx.x;

  arg0 += idx_x * 1 * 1 + idx_y * 1 * 1 * xdim0_tea_leaf_jacobi_kernel;
  arg1 += idx_x * 1 * 1 + idx_y * 1 * 1 * xdim1_tea_leaf_jacobi_kernel;
  arg2 += idx_x * 1 * 1 + idx_y * 1 * 1 * xdim2_tea_leaf_jacobi_kernel;
  arg3 += idx_x * 1 * 1 + idx_y * 1 * 1 * xdim3_tea_leaf_jacobi_kernel;
  arg4 += idx_x * 1 * 1 + idx_y * 1 * 1 * xdim4_tea_leaf_jacobi_kernel;

  if (idx_x < size0 && idx_y < size1) {
    tea_leaf_jacobi_kernel_gpu(arg0, arg1, arg2, arg3, arg4, &arg5, &arg6,
                               arg7_l);
  }
  for (int d = 0; d < 1; d++)
    ops_reduction_cuda<OPS_INC>(
        &arg7[d + (blockIdx.x + blockIdx.y * gridDim.x) * 1], arg7_l[d]);
}

// host stub function
void ops_par_loop_tea_leaf_jacobi_kernel(char const *name, ops_block block,
                                         int dim, int *range, ops_arg arg0,
                                         ops_arg arg1, ops_arg arg2,
                                         ops_arg arg3, ops_arg arg4,
                                         ops_arg arg5, ops_arg arg6,
                                         ops_arg arg7) {

  // Timing
  double t1, t2, c1, c2;

  ops_arg args[8] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7};

#ifdef CHECKPOINTING
  if (!ops_checkpointing_before(args, 8, range, 42))
    return;
#endif

  if (OPS_diags > 1) {
    ops_timing_realloc(42, "tea_leaf_jacobi_kernel");
    OPS_kernels[42].count++;
    ops_timers_core(&c1, &t1);
  }

  // compute locally allocated range for the sub-block
  int start[2];
  int end[2];
#ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned)
    return;
  for (int n = 0; n < 2; n++) {
    start[n] = sb->decomp_disp[n];
    end[n] = sb->decomp_disp[n] + sb->decomp_size[n];
    if (start[n] >= range[2 * n]) {
      start[n] = 0;
    } else {
      start[n] = range[2 * n] - start[n];
    }
    if (sb->id_m[n] == MPI_PROC_NULL && range[2 * n] < 0)
      start[n] = range[2 * n];
    if (end[n] >= range[2 * n + 1]) {
      end[n] = range[2 * n + 1] - sb->decomp_disp[n];
    } else {
      end[n] = sb->decomp_size[n];
    }
    if (sb->id_p[n] == MPI_PROC_NULL &&
        (range[2 * n + 1] > sb->decomp_disp[n] + sb->decomp_size[n]))
      end[n] += (range[2 * n + 1] - sb->decomp_disp[n] - sb->decomp_size[n]);
  }
#else
  for (int n = 0; n < 2; n++) {
    start[n] = range[2 * n];
    end[n] = range[2 * n + 1];
  }
#endif

  int x_size = MAX(0, end[0] - start[0]);
  int y_size = MAX(0, end[1] - start[1]);

  int xdim0 = args[0].dat->size[0];
  int xdim1 = args[1].dat->size[0];
  int xdim2 = args[2].dat->size[0];
  int xdim3 = args[3].dat->size[0];
  int xdim4 = args[4].dat->size[0];

  if (xdim0 != xdim0_tea_leaf_jacobi_kernel_h ||
      xdim1 != xdim1_tea_leaf_jacobi_kernel_h ||
      xdim2 != xdim2_tea_leaf_jacobi_kernel_h ||
      xdim3 != xdim3_tea_leaf_jacobi_kernel_h ||
      xdim4 != xdim4_tea_leaf_jacobi_kernel_h) {
    cudaMemcpyToSymbol(xdim0_tea_leaf_jacobi_kernel, &xdim0, sizeof(int));
    xdim0_tea_leaf_jacobi_kernel_h = xdim0;
    cudaMemcpyToSymbol(xdim1_tea_leaf_jacobi_kernel, &xdim1, sizeof(int));
    xdim1_tea_leaf_jacobi_kernel_h = xdim1;
    cudaMemcpyToSymbol(xdim2_tea_leaf_jacobi_kernel, &xdim2, sizeof(int));
    xdim2_tea_leaf_jacobi_kernel_h = xdim2;
    cudaMemcpyToSymbol(xdim3_tea_leaf_jacobi_kernel, &xdim3, sizeof(int));
    xdim3_tea_leaf_jacobi_kernel_h = xdim3;
    cudaMemcpyToSymbol(xdim4_tea_leaf_jacobi_kernel, &xdim4, sizeof(int));
    xdim4_tea_leaf_jacobi_kernel_h = xdim4;
  }

#ifdef OPS_MPI
  double *arg7h =
      (double *)(((ops_reduction)args[7].data)->data +
                 ((ops_reduction)args[7].data)->size * block->index);
#else
  double *arg7h = (double *)(((ops_reduction)args[7].data)->data);
#endif

  dim3 grid((x_size - 1) / OPS_block_size_x + 1,
            (y_size - 1) / OPS_block_size_y + 1, 1);
  dim3 tblock(OPS_block_size_x, OPS_block_size_y, 1);

  int nblocks = ((x_size - 1) / OPS_block_size_x + 1) *
                ((y_size - 1) / OPS_block_size_y + 1);
  int maxblocks = nblocks;
  int reduct_bytes = 0;
  int reduct_size = 0;

  reduct_bytes += ROUND_UP(maxblocks * 1 * sizeof(double));
  reduct_size = MAX(reduct_size, sizeof(double) * 1);

  reallocReductArrays(reduct_bytes);
  reduct_bytes = 0;

  arg7.data = OPS_reduct_h + reduct_bytes;
  arg7.data_d = OPS_reduct_d + reduct_bytes;
  for (int b = 0; b < maxblocks; b++)
    for (int d = 0; d < 1; d++)
      ((double *)arg7.data)[d + b * 1] = ZERO_double;
  reduct_bytes += ROUND_UP(maxblocks * 1 * sizeof(double));

  mvReductArraysToDevice(reduct_bytes);
  int dat0 = args[0].dat->elem_size;
  int dat1 = args[1].dat->elem_size;
  int dat2 = args[2].dat->elem_size;
  int dat3 = args[3].dat->elem_size;
  int dat4 = args[4].dat->elem_size;

  char *p_a[8];

  // set up initial pointers
  int d_m[OPS_MAX_DIM];
#ifdef OPS_MPI
  for (int d = 0; d < dim; d++)
    d_m[d] =
        args[0].dat->d_m[d] + OPS_sub_dat_list[args[0].dat->index]->d_im[d];
#else
  for (int d = 0; d < dim; d++)
    d_m[d] = args[0].dat->d_m[d];
#endif
  int base0 = dat0 * 1 * (start[0] * args[0].stencil->stride[0] -
                          args[0].dat->base[0] - d_m[0]);
  base0 = base0 +
          dat0 * args[0].dat->size[0] * (start[1] * args[0].stencil->stride[1] -
                                         args[0].dat->base[1] - d_m[1]);
  p_a[0] = (char *)args[0].data_d + base0;

#ifdef OPS_MPI
  for (int d = 0; d < dim; d++)
    d_m[d] =
        args[1].dat->d_m[d] + OPS_sub_dat_list[args[1].dat->index]->d_im[d];
#else
  for (int d = 0; d < dim; d++)
    d_m[d] = args[1].dat->d_m[d];
#endif
  int base1 = dat1 * 1 * (start[0] * args[1].stencil->stride[0] -
                          args[1].dat->base[0] - d_m[0]);
  base1 = base1 +
          dat1 * args[1].dat->size[0] * (start[1] * args[1].stencil->stride[1] -
                                         args[1].dat->base[1] - d_m[1]);
  p_a[1] = (char *)args[1].data_d + base1;

#ifdef OPS_MPI
  for (int d = 0; d < dim; d++)
    d_m[d] =
        args[2].dat->d_m[d] + OPS_sub_dat_list[args[2].dat->index]->d_im[d];
#else
  for (int d = 0; d < dim; d++)
    d_m[d] = args[2].dat->d_m[d];
#endif
  int base2 = dat2 * 1 * (start[0] * args[2].stencil->stride[0] -
                          args[2].dat->base[0] - d_m[0]);
  base2 = base2 +
          dat2 * args[2].dat->size[0] * (start[1] * args[2].stencil->stride[1] -
                                         args[2].dat->base[1] - d_m[1]);
  p_a[2] = (char *)args[2].data_d + base2;

#ifdef OPS_MPI
  for (int d = 0; d < dim; d++)
    d_m[d] =
        args[3].dat->d_m[d] + OPS_sub_dat_list[args[3].dat->index]->d_im[d];
#else
  for (int d = 0; d < dim; d++)
    d_m[d] = args[3].dat->d_m[d];
#endif
  int base3 = dat3 * 1 * (start[0] * args[3].stencil->stride[0] -
                          args[3].dat->base[0] - d_m[0]);
  base3 = base3 +
          dat3 * args[3].dat->size[0] * (start[1] * args[3].stencil->stride[1] -
                                         args[3].dat->base[1] - d_m[1]);
  p_a[3] = (char *)args[3].data_d + base3;

#ifdef OPS_MPI
  for (int d = 0; d < dim; d++)
    d_m[d] =
        args[4].dat->d_m[d] + OPS_sub_dat_list[args[4].dat->index]->d_im[d];
#else
  for (int d = 0; d < dim; d++)
    d_m[d] = args[4].dat->d_m[d];
#endif
  int base4 = dat4 * 1 * (start[0] * args[4].stencil->stride[0] -
                          args[4].dat->base[0] - d_m[0]);
  base4 = base4 +
          dat4 * args[4].dat->size[0] * (start[1] * args[4].stencil->stride[1] -
                                         args[4].dat->base[1] - d_m[1]);
  p_a[4] = (char *)args[4].data_d + base4;

  ops_H_D_exchanges_device(args, 8);
  ops_halo_exchanges(args, 8, range);

  if (OPS_diags > 1) {
    ops_timers_core(&c2, &t2);
    OPS_kernels[42].mpi_time += t2 - t1;
  }

  int nshared = 0;
  int nthread = OPS_block_size_x * OPS_block_size_y;

  nshared = MAX(nshared, sizeof(double) * 1);

  nshared = MAX(nshared * nthread, reduct_size * nthread);

  // call kernel wrapper function, passing in pointers to data
  ops_tea_leaf_jacobi_kernel<<<grid, tblock, nshared>>>(
      (double *)p_a[0], (double *)p_a[1], (double *)p_a[2], (double *)p_a[3],
      (double *)p_a[4], *(double *)arg5.data, *(double *)arg6.data,
      (double *)arg7.data_d, x_size, y_size);

  mvReductArraysToHost(reduct_bytes);
  for (int b = 0; b < maxblocks; b++) {
    for (int d = 0; d < 1; d++) {
      arg7h[d] = arg7h[d] + ((double *)arg7.data)[d + b * 1];
    }
  }
  arg7.data = (char *)arg7h;

  if (OPS_diags > 1) {
    cutilSafeCall(cudaDeviceSynchronize());
    ops_timers_core(&c1, &t1);
    OPS_kernels[42].time += t1 - t2;
  }

  ops_set_dirtybit_device(args, 8);
  ops_set_halo_dirtybit3(&args[0], range);

  if (OPS_diags > 1) {
    // Update kernel record
    ops_timers_core(&c2, &t2);
    OPS_kernels[42].mpi_time += t2 - t1;
    OPS_kernels[42].transfer += ops_compute_transfer(dim, start, end, &arg0);
    OPS_kernels[42].transfer += ops_compute_transfer(dim, start, end, &arg1);
    OPS_kernels[42].transfer += ops_compute_transfer(dim, start, end, &arg2);
    OPS_kernels[42].transfer += ops_compute_transfer(dim, start, end, &arg3);
    OPS_kernels[42].transfer += ops_compute_transfer(dim, start, end, &arg4);
  }
}
