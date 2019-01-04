//
// auto-generated by ops.py
//
#define OPS_ACC0(x,y,z) (n_x*1+n_y*xdim0_field_summary_kernel*1+n_z*xdim0_field_summary_kernel*ydim0_field_summary_kernel*1+x+xdim0_field_summary_kernel*(y)+xdim0_field_summary_kernel*ydim0_field_summary_kernel*(z))
#define OPS_ACC1(x,y,z) (n_x*1+n_y*xdim1_field_summary_kernel*1+n_z*xdim1_field_summary_kernel*ydim1_field_summary_kernel*1+x+xdim1_field_summary_kernel*(y)+xdim1_field_summary_kernel*ydim1_field_summary_kernel*(z))
#define OPS_ACC2(x,y,z) (n_x*1+n_y*xdim2_field_summary_kernel*1+n_z*xdim2_field_summary_kernel*ydim2_field_summary_kernel*1+x+xdim2_field_summary_kernel*(y)+xdim2_field_summary_kernel*ydim2_field_summary_kernel*(z))
#define OPS_ACC3(x,y,z) (n_x*1+n_y*xdim3_field_summary_kernel*1+n_z*xdim3_field_summary_kernel*ydim3_field_summary_kernel*1+x+xdim3_field_summary_kernel*(y)+xdim3_field_summary_kernel*ydim3_field_summary_kernel*(z))
#define OPS_ACC4(x,y,z) (n_x*1+n_y*xdim4_field_summary_kernel*1+n_z*xdim4_field_summary_kernel*ydim4_field_summary_kernel*1+x+xdim4_field_summary_kernel*(y)+xdim4_field_summary_kernel*ydim4_field_summary_kernel*(z))
#define OPS_ACC5(x,y,z) (n_x*1+n_y*xdim5_field_summary_kernel*1+n_z*xdim5_field_summary_kernel*ydim5_field_summary_kernel*1+x+xdim5_field_summary_kernel*(y)+xdim5_field_summary_kernel*ydim5_field_summary_kernel*(z))
#define OPS_ACC6(x,y,z) (n_x*1+n_y*xdim6_field_summary_kernel*1+n_z*xdim6_field_summary_kernel*ydim6_field_summary_kernel*1+x+xdim6_field_summary_kernel*(y)+xdim6_field_summary_kernel*ydim6_field_summary_kernel*(z))


//user function

// host stub function
void ops_par_loop_field_summary_kernel_execute(ops_kernel_descriptor *desc) {
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
  ops_arg arg7 = desc->args[7];
  ops_arg arg8 = desc->args[8];
  ops_arg arg9 = desc->args[9];
  ops_arg arg10 = desc->args[10];
  ops_arg arg11 = desc->args[11];

  //Timing
  double t1,t2,c1,c2;

  ops_arg args[12] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11};



  #ifdef CHECKPOINTING
  if (!ops_checkpointing_before(args, 12, range, 96))
    return;
  #endif

  if (OPS_diags > 1) {
    OPS_kernels[96].count++;
    ops_timers_core(&c2,&t2);
  }

  //compute locally allocated range for the sub-block
  int start[3];
  int end[3];

  for ( int n=0; n<3; n++ ){
    start[n] = range[2*n];end[n] = range[2*n+1];
  }

  #ifdef OPS_DEBUG
  ops_register_args(args, "field_summary_kernel");
  #endif



  //set up initial pointers and exchange halos if necessary
  int base0 = args[0].dat->base_offset;
  const double * __restrict__ volume = (double *)(args[0].data + base0);

  int base1 = args[1].dat->base_offset;
  const double * __restrict__ density0 = (double *)(args[1].data + base1);

  int base2 = args[2].dat->base_offset;
  const double * __restrict__ energy0 = (double *)(args[2].data + base2);

  int base3 = args[3].dat->base_offset;
  const double * __restrict__ pressure = (double *)(args[3].data + base3);

  int base4 = args[4].dat->base_offset;
  const double * __restrict__ xvel0 = (double *)(args[4].data + base4);

  int base5 = args[5].dat->base_offset;
  const double * __restrict__ yvel0 = (double *)(args[5].data + base5);

  int base6 = args[6].dat->base_offset;
  const double * __restrict__ zvel0 = (double *)(args[6].data + base6);

  #ifdef OPS_MPI
  double * __restrict__ p_a7 = (double *)(((ops_reduction)args[7].data)->data + ((ops_reduction)args[7].data)->size * block->index);
  #else //OPS_MPI
  double * __restrict__ p_a7 = (double *)((ops_reduction)args[7].data)->data;
  #endif //OPS_MPI


  #ifdef OPS_MPI
  double * __restrict__ p_a8 = (double *)(((ops_reduction)args[8].data)->data + ((ops_reduction)args[8].data)->size * block->index);
  #else //OPS_MPI
  double * __restrict__ p_a8 = (double *)((ops_reduction)args[8].data)->data;
  #endif //OPS_MPI


  #ifdef OPS_MPI
  double * __restrict__ p_a9 = (double *)(((ops_reduction)args[9].data)->data + ((ops_reduction)args[9].data)->size * block->index);
  #else //OPS_MPI
  double * __restrict__ p_a9 = (double *)((ops_reduction)args[9].data)->data;
  #endif //OPS_MPI


  #ifdef OPS_MPI
  double * __restrict__ p_a10 = (double *)(((ops_reduction)args[10].data)->data + ((ops_reduction)args[10].data)->size * block->index);
  #else //OPS_MPI
  double * __restrict__ p_a10 = (double *)((ops_reduction)args[10].data)->data;
  #endif //OPS_MPI


  #ifdef OPS_MPI
  double * __restrict__ p_a11 = (double *)(((ops_reduction)args[11].data)->data + ((ops_reduction)args[11].data)->size * block->index);
  #else //OPS_MPI
  double * __restrict__ p_a11 = (double *)((ops_reduction)args[11].data)->data;
  #endif //OPS_MPI



  //initialize global variable with the dimension of dats
  int xdim0_field_summary_kernel = args[0].dat->size[0];
  int ydim0_field_summary_kernel = args[0].dat->size[1];
  int xdim1_field_summary_kernel = args[1].dat->size[0];
  int ydim1_field_summary_kernel = args[1].dat->size[1];
  int xdim2_field_summary_kernel = args[2].dat->size[0];
  int ydim2_field_summary_kernel = args[2].dat->size[1];
  int xdim3_field_summary_kernel = args[3].dat->size[0];
  int ydim3_field_summary_kernel = args[3].dat->size[1];
  int xdim4_field_summary_kernel = args[4].dat->size[0];
  int ydim4_field_summary_kernel = args[4].dat->size[1];
  int xdim5_field_summary_kernel = args[5].dat->size[0];
  int ydim5_field_summary_kernel = args[5].dat->size[1];
  int xdim6_field_summary_kernel = args[6].dat->size[0];
  int ydim6_field_summary_kernel = args[6].dat->size[1];

  if (OPS_diags > 1) {
    ops_timers_core(&c1,&t1);
    OPS_kernels[96].mpi_time += t1 - t2;
  }

  double p_a7_0 = p_a7[0];
  double p_a8_0 = p_a8[0];
  double p_a9_0 = p_a9[0];
  double p_a10_0 = p_a10[0];
  double p_a11_0 = p_a11[0];
  #pragma omp parallel for reduction(+:p_a7_0) reduction(+:p_a8_0) reduction(+:p_a9_0) reduction(+:p_a10_0) reduction(+:p_a11_0)
  for ( int n_z=start[2]; n_z<end[2]; n_z++ ){
    for ( int n_y=start[1]; n_y<end[1]; n_y++ ){
      #ifdef intel
      #pragma loop_count(10000)
      #pragma omp simd reduction(+:p_a7_0) reduction(+:p_a8_0) reduction(+:p_a9_0) reduction(+:p_a10_0) reduction(+:p_a11_0) aligned(volume,density0,energy0,pressure,xvel0,yvel0,zvel0)
      #else
      #pragma simd reduction(+:p_a7_0) reduction(+:p_a8_0) reduction(+:p_a9_0) reduction(+:p_a10_0) reduction(+:p_a11_0)
      #endif
      for ( int n_x=start[0]; n_x<end[0]; n_x++ ){
        double *vol = &p_a7_0;
        double *mass = &p_a8_0;
        double *ie = &p_a9_0;
        double *ke = &p_a10_0;
        double *press = &p_a11_0;
        

  double vsqrd, cell_vol, cell_mass;

  vsqrd = 0.0;
  vsqrd+=0.125*( xvel0[OPS_ACC4(0,0,0)] * xvel0[OPS_ACC4(0,0,0)] +
                 yvel0[OPS_ACC5(0,0,0)] * yvel0[OPS_ACC5(0,0,0)] +
                 zvel0[OPS_ACC6(0,0,0)] * zvel0[OPS_ACC6(0,0,0)]);
  vsqrd+=0.125*( xvel0[OPS_ACC4(1,0,0)] * xvel0[OPS_ACC4(1,0,0)] +
                 yvel0[OPS_ACC5(1,0,0)] * yvel0[OPS_ACC5(1,0,0)] +
                 zvel0[OPS_ACC6(1,0,0)] * zvel0[OPS_ACC6(1,0,0)]);
  vsqrd+=0.125*( xvel0[OPS_ACC4(0,1,0)] * xvel0[OPS_ACC4(0,1,0)] +
                 yvel0[OPS_ACC5(0,1,0)] * yvel0[OPS_ACC5(0,1,0)] +
                 zvel0[OPS_ACC6(0,1,0)] * zvel0[OPS_ACC6(0,1,0)]);
  vsqrd+=0.125*( xvel0[OPS_ACC4(1,1,0)] * xvel0[OPS_ACC4(1,1,0)] +
                 yvel0[OPS_ACC5(1,1,0)] * yvel0[OPS_ACC5(1,1,0)] +
                 zvel0[OPS_ACC6(1,1,0)] * zvel0[OPS_ACC6(1,1,0)]);
  vsqrd+=0.125*( xvel0[OPS_ACC4(0,0,1)] * xvel0[OPS_ACC4(0,0,1)] +
                 yvel0[OPS_ACC5(0,0,1)] * yvel0[OPS_ACC5(0,0,1)] +
                 zvel0[OPS_ACC6(0,0,1)] * zvel0[OPS_ACC6(0,0,1)]);
  vsqrd+=0.125*( xvel0[OPS_ACC4(1,0,1)] * xvel0[OPS_ACC4(1,0,1)] +
                 yvel0[OPS_ACC5(1,0,1)] * yvel0[OPS_ACC5(1,0,1)] +
                 zvel0[OPS_ACC6(1,0,1)] * zvel0[OPS_ACC6(1,0,1)]);
  vsqrd+=0.125*( xvel0[OPS_ACC4(0,1,1)] * xvel0[OPS_ACC4(0,1,1)] +
                 yvel0[OPS_ACC5(0,1,1)] * yvel0[OPS_ACC5(0,1,1)] +
                 zvel0[OPS_ACC6(0,1,1)] * zvel0[OPS_ACC6(0,1,1)]);
  vsqrd+=0.125*( xvel0[OPS_ACC4(1,1,1)] * xvel0[OPS_ACC4(1,1,1)] +
                 yvel0[OPS_ACC5(1,1,1)] * yvel0[OPS_ACC5(1,1,1)] +
                 zvel0[OPS_ACC6(1,1,1)] * zvel0[OPS_ACC6(1,1,1)]);

  cell_vol = volume[OPS_ACC0(0,0,0)];
  cell_mass = cell_vol * density0[OPS_ACC1(0,0,0)];
  *vol = *vol + cell_vol;
  *mass = *mass + cell_mass;
  *ie = *ie + cell_mass * energy0[OPS_ACC2(0,0,0)];
  *ke = *ke + cell_mass * 0.5 * vsqrd;
  *press = *press + cell_vol * pressure[OPS_ACC3(0,0,0)];


      }
    }
  }
  p_a7[0] = p_a7_0;
  p_a8[0] = p_a8_0;
  p_a9[0] = p_a9_0;
  p_a10[0] = p_a10_0;
  p_a11[0] = p_a11_0;
  if (OPS_diags > 1) {
    ops_timers_core(&c2,&t2);
    OPS_kernels[96].time += t2 - t1;
  }

  if (OPS_diags > 1) {
    //Update kernel record
    ops_timers_core(&c1,&t1);
    OPS_kernels[96].mpi_time += t1 - t2;
    OPS_kernels[96].transfer += ops_compute_transfer(dim, start, end, &arg0);
    OPS_kernels[96].transfer += ops_compute_transfer(dim, start, end, &arg1);
    OPS_kernels[96].transfer += ops_compute_transfer(dim, start, end, &arg2);
    OPS_kernels[96].transfer += ops_compute_transfer(dim, start, end, &arg3);
    OPS_kernels[96].transfer += ops_compute_transfer(dim, start, end, &arg4);
    OPS_kernels[96].transfer += ops_compute_transfer(dim, start, end, &arg5);
    OPS_kernels[96].transfer += ops_compute_transfer(dim, start, end, &arg6);
  }
}
#undef OPS_ACC0
#undef OPS_ACC1
#undef OPS_ACC2
#undef OPS_ACC3
#undef OPS_ACC4
#undef OPS_ACC5
#undef OPS_ACC6


void ops_par_loop_field_summary_kernel(char const *name, ops_block block, int dim, int* range,
 ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
 ops_arg arg4, ops_arg arg5, ops_arg arg6, ops_arg arg7,
 ops_arg arg8, ops_arg arg9, ops_arg arg10, ops_arg arg11) {
  ops_kernel_descriptor *desc = (ops_kernel_descriptor *)malloc(sizeof(ops_kernel_descriptor));
  desc->name = name;
  desc->block = block;
  desc->dim = dim;
  desc->device = 1;
  desc->index = 96;
  desc->hash = 5381;
  desc->hash = ((desc->hash << 5) + desc->hash) + 96;
  for ( int i=0; i<6; i++ ){
    desc->range[i] = range[i];
    desc->orig_range[i] = range[i];
    desc->hash = ((desc->hash << 5) + desc->hash) + range[i];
  }
  desc->nargs = 12;
  desc->args = (ops_arg*)malloc(12*sizeof(ops_arg));
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
  desc->hash = ((desc->hash << 5) + desc->hash) + arg6.dat->index;
  desc->args[7] = arg7;
  desc->args[8] = arg8;
  desc->args[9] = arg9;
  desc->args[10] = arg10;
  desc->args[11] = arg11;
  desc->function = ops_par_loop_field_summary_kernel_execute;
  if (OPS_diags > 1) {
    ops_timing_realloc(96, "field_summary_kernel");
  }
  ops_enqueue_kernel(desc);
  }
