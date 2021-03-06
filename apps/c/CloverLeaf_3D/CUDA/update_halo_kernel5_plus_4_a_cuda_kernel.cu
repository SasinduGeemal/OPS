//
// auto-generated by ops.py
//
__constant__ int xdim0_update_halo_kernel5_plus_4_a;
int xdim0_update_halo_kernel5_plus_4_a_h = -1;
__constant__ int ydim0_update_halo_kernel5_plus_4_a;
int ydim0_update_halo_kernel5_plus_4_a_h = -1;
__constant__ int xdim1_update_halo_kernel5_plus_4_a;
int xdim1_update_halo_kernel5_plus_4_a_h = -1;
__constant__ int ydim1_update_halo_kernel5_plus_4_a;
int ydim1_update_halo_kernel5_plus_4_a_h = -1;

#undef OPS_ACC0
#undef OPS_ACC1


#define OPS_ACC0(x,y,z) (x+xdim0_update_halo_kernel5_plus_4_a*(y)+xdim0_update_halo_kernel5_plus_4_a*ydim0_update_halo_kernel5_plus_4_a*(z))
#define OPS_ACC1(x,y,z) (x+xdim1_update_halo_kernel5_plus_4_a*(y)+xdim1_update_halo_kernel5_plus_4_a*ydim1_update_halo_kernel5_plus_4_a*(z))

//user function
__device__

inline void update_halo_kernel5_plus_4_a_gpu(double *vol_flux_z, double *mass_flux_z, const int* fields) {
  if(fields[FIELD_VOL_FLUX_Z] == 1) vol_flux_z[OPS_ACC0(0,0,0)] = vol_flux_z[OPS_ACC0(0,4,0)];
  if(fields[FIELD_MASS_FLUX_Z] == 1) mass_flux_z[OPS_ACC1(0,0,0)] = mass_flux_z[OPS_ACC1(0,4,0)];
}



#undef OPS_ACC0
#undef OPS_ACC1


__global__ void ops_update_halo_kernel5_plus_4_a(
double* __restrict arg0,
double* __restrict arg1,
const int* __restrict arg2,
int size0,
int size1,
int size2 ){


  int idx_z = blockDim.z * blockIdx.z + threadIdx.z;
  int idx_y = blockDim.y * blockIdx.y + threadIdx.y;
  int idx_x = blockDim.x * blockIdx.x + threadIdx.x;

  arg0 += idx_x * 1*1 + idx_y * 1*1 * xdim0_update_halo_kernel5_plus_4_a + idx_z * 1*1 * xdim0_update_halo_kernel5_plus_4_a * ydim0_update_halo_kernel5_plus_4_a;
  arg1 += idx_x * 1*1 + idx_y * 1*1 * xdim1_update_halo_kernel5_plus_4_a + idx_z * 1*1 * xdim1_update_halo_kernel5_plus_4_a * ydim1_update_halo_kernel5_plus_4_a;

  if (idx_x < size0 && idx_y < size1 && idx_z < size2) {
    update_halo_kernel5_plus_4_a_gpu(arg0, arg1, arg2);
  }

}

// host stub function
#ifndef OPS_LAZY
void ops_par_loop_update_halo_kernel5_plus_4_a(char const *name, ops_block block, int dim, int* range,
 ops_arg arg0, ops_arg arg1, ops_arg arg2) {
#else
void ops_par_loop_update_halo_kernel5_plus_4_a_execute(ops_kernel_descriptor *desc) {
  int dim = desc->dim;
  int *range = desc->range;
  ops_arg arg0 = desc->args[0];
  ops_arg arg1 = desc->args[1];
  ops_arg arg2 = desc->args[2];
  #endif

  //Timing
  double t1,t2,c1,c2;

  ops_arg args[3] = { arg0, arg1, arg2};


  #if CHECKPOINTING && !OPS_LAZY
  if (!ops_checkpointing_before(args,3,range,84)) return;
  #endif

  if (OPS_diags > 1) {
    ops_timing_realloc(84,"update_halo_kernel5_plus_4_a");
    OPS_kernels[84].count++;
    ops_timers_core(&c1,&t1);
  }

  //compute locally allocated range for the sub-block
  int start[3];
  int end[3];
  #if OPS_MPI && !OPS_LAZY
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  for ( int n=0; n<3; n++ ){
    start[n] = sb->decomp_disp[n];end[n] = sb->decomp_disp[n]+sb->decomp_size[n];
    if (start[n] >= range[2*n]) {
      start[n] = 0;
    }
    else {
      start[n] = range[2*n] - start[n];
    }
    if (sb->id_m[n]==MPI_PROC_NULL && range[2*n] < 0) start[n] = range[2*n];
    if (end[n] >= range[2*n+1]) {
      end[n] = range[2*n+1] - sb->decomp_disp[n];
    }
    else {
      end[n] = sb->decomp_size[n];
    }
    if (sb->id_p[n]==MPI_PROC_NULL && (range[2*n+1] > sb->decomp_disp[n]+sb->decomp_size[n]))
      end[n] += (range[2*n+1]-sb->decomp_disp[n]-sb->decomp_size[n]);
  }
  #else
  for ( int n=0; n<3; n++ ){
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif

  int x_size = MAX(0,end[0]-start[0]);
  int y_size = MAX(0,end[1]-start[1]);
  int z_size = MAX(0,end[2]-start[2]);

  int xdim0 = args[0].dat->size[0];
  int ydim0 = args[0].dat->size[1];
  int xdim1 = args[1].dat->size[0];
  int ydim1 = args[1].dat->size[1];

  if (xdim0 != xdim0_update_halo_kernel5_plus_4_a_h || ydim0 != ydim0_update_halo_kernel5_plus_4_a_h || xdim1 != xdim1_update_halo_kernel5_plus_4_a_h || ydim1 != ydim1_update_halo_kernel5_plus_4_a_h) {
    cudaMemcpyToSymbol( xdim0_update_halo_kernel5_plus_4_a, &xdim0, sizeof(int) );
    xdim0_update_halo_kernel5_plus_4_a_h = xdim0;
    cudaMemcpyToSymbol( ydim0_update_halo_kernel5_plus_4_a, &ydim0, sizeof(int) );
    ydim0_update_halo_kernel5_plus_4_a_h = ydim0;
    cudaMemcpyToSymbol( xdim1_update_halo_kernel5_plus_4_a, &xdim1, sizeof(int) );
    xdim1_update_halo_kernel5_plus_4_a_h = xdim1;
    cudaMemcpyToSymbol( ydim1_update_halo_kernel5_plus_4_a, &ydim1, sizeof(int) );
    ydim1_update_halo_kernel5_plus_4_a_h = ydim1;
  }


  int *arg2h = (int *)arg2.data;

  dim3 grid( (x_size-1)/OPS_block_size_x+ 1, (y_size-1)/OPS_block_size_y + 1, (z_size-1)/OPS_block_size_z +1);
  dim3 tblock(OPS_block_size_x,OPS_block_size_y,OPS_block_size_z);

  int consts_bytes = 0;

  consts_bytes += ROUND_UP(NUM_FIELDS*sizeof(int));

  reallocConstArrays(consts_bytes);

  consts_bytes = 0;
  arg2.data = OPS_consts_h + consts_bytes;
  arg2.data_d = OPS_consts_d + consts_bytes;
  for (int d=0; d<NUM_FIELDS; d++) ((int *)arg2.data)[d] = arg2h[d];
  consts_bytes += ROUND_UP(NUM_FIELDS*sizeof(int));
  mvConstArraysToDevice(consts_bytes);
  int dat0 = (OPS_soa ? args[0].dat->type_size : args[0].dat->elem_size);
  int dat1 = (OPS_soa ? args[1].dat->type_size : args[1].dat->elem_size);

  char *p_a[3];

  //set up initial pointers
  int base0 = args[0].dat->base_offset + 
           dat0 * 1 * (start[0] * args[0].stencil->stride[0]);
  base0 = base0+ dat0 *
    args[0].dat->size[0] *
    (start[1] * args[0].stencil->stride[1]);
  base0 = base0+ dat0 *
    args[0].dat->size[0] *
    args[0].dat->size[1] *
    (start[2] * args[0].stencil->stride[2]);
  p_a[0] = (char *)args[0].data_d + base0;

  int base1 = args[1].dat->base_offset + 
           dat1 * 1 * (start[0] * args[1].stencil->stride[0]);
  base1 = base1+ dat1 *
    args[1].dat->size[0] *
    (start[1] * args[1].stencil->stride[1]);
  base1 = base1+ dat1 *
    args[1].dat->size[0] *
    args[1].dat->size[1] *
    (start[2] * args[1].stencil->stride[2]);
  p_a[1] = (char *)args[1].data_d + base1;


  #ifndef OPS_LAZY
  ops_H_D_exchanges_device(args, 3);
  ops_halo_exchanges(args,3,range);
  #endif

  if (OPS_diags > 1) {
    ops_timers_core(&c2,&t2);
    OPS_kernels[84].mpi_time += t2-t1;
  }


  //call kernel wrapper function, passing in pointers to data
  if (x_size > 0 && y_size > 0 && z_size > 0)
    ops_update_halo_kernel5_plus_4_a<<<grid, tblock >>> (  (double *)p_a[0], (double *)p_a[1],
           (int *)arg2.data_d,x_size, y_size, z_size);

  cutilSafeCall(cudaGetLastError());

  if (OPS_diags>1) {
    cutilSafeCall(cudaDeviceSynchronize());
    ops_timers_core(&c1,&t1);
    OPS_kernels[84].time += t1-t2;
  }

  #ifndef OPS_LAZY
  ops_set_dirtybit_device(args, 3);
  ops_set_halo_dirtybit3(&args[0],range);
  ops_set_halo_dirtybit3(&args[1],range);
  #endif

  if (OPS_diags > 1) {
    //Update kernel record
    ops_timers_core(&c2,&t2);
    OPS_kernels[84].mpi_time += t2-t1;
    OPS_kernels[84].transfer += ops_compute_transfer(dim, start, end, &arg0);
    OPS_kernels[84].transfer += ops_compute_transfer(dim, start, end, &arg1);
  }
}

#ifdef OPS_LAZY
void ops_par_loop_update_halo_kernel5_plus_4_a(char const *name, ops_block block, int dim, int* range,
 ops_arg arg0, ops_arg arg1, ops_arg arg2) {
  ops_kernel_descriptor *desc = (ops_kernel_descriptor *)malloc(sizeof(ops_kernel_descriptor));
  desc->name = name;
  desc->block = block;
  desc->dim = dim;
  desc->device = 1;
  desc->index = 84;
  desc->hash = 5381;
  desc->hash = ((desc->hash << 5) + desc->hash) + 84;
  for ( int i=0; i<6; i++ ){
    desc->range[i] = range[i];
    desc->orig_range[i] = range[i];
    desc->hash = ((desc->hash << 5) + desc->hash) + range[i];
  }
  desc->nargs = 3;
  desc->args = (ops_arg*)malloc(3*sizeof(ops_arg));
  desc->args[0] = arg0;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg0.dat->index;
  desc->args[1] = arg1;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg1.dat->index;
  desc->args[2] = arg2;
  char *tmp = (char*)malloc(NUM_FIELDS*sizeof(int));
  memcpy(tmp, arg2.data,NUM_FIELDS*sizeof(int));
  desc->args[2].data = tmp;
  desc->function = ops_par_loop_update_halo_kernel5_plus_4_a_execute;
  if (OPS_diags > 1) {
    ops_timing_realloc(84,"update_halo_kernel5_plus_4_a");
  }
  ops_enqueue_kernel(desc);
}
#endif
