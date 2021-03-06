//
// auto-generated by ops.py
//
#define OPS_ACC0(x,y) (n_x*1+n_y*xdim0_initialise_chunk_kernel_cellx*0+x+xdim0_initialise_chunk_kernel_cellx*(y))
#define OPS_ACC1(x,y) (n_x*1+n_y*xdim1_initialise_chunk_kernel_cellx*0+x+xdim1_initialise_chunk_kernel_cellx*(y))
#define OPS_ACC2(x,y) (n_x*1+n_y*xdim2_initialise_chunk_kernel_cellx*0+x+xdim2_initialise_chunk_kernel_cellx*(y))


//user function

// host stub function
void ops_par_loop_initialise_chunk_kernel_cellx_execute(ops_kernel_descriptor *desc) {
  ops_block block = desc->block;
  int dim = desc->dim;
  int *range = desc->range;
  ops_arg arg0 = desc->args[0];
  ops_arg arg1 = desc->args[1];
  ops_arg arg2 = desc->args[2];

  //Timing
  double t1,t2,c1,c2;

  ops_arg args[3] = { arg0, arg1, arg2};



  #ifdef CHECKPOINTING
  if (!ops_checkpointing_before(args,3,range,4)) return;
  #endif

  if (OPS_diags > 1) {
    OPS_kernels[4].count++;
    ops_timers_core(&c2,&t2);
  }

  //compute locally allocated range for the sub-block
  int start[2];
  int end[2];

  for ( int n=0; n<2; n++ ){
    start[n] = range[2*n];end[n] = range[2*n+1];
  }

  #ifdef OPS_DEBUG
  ops_register_args(args, "initialise_chunk_kernel_cellx");
  #endif



  //set up initial pointers and exchange halos if necessary
  int base0 = args[0].dat->base_offset;
  const double * __restrict__ vertexx = (double *)(args[0].data + base0);

  int base1 = args[1].dat->base_offset;
  double * __restrict__ cellx = (double *)(args[1].data + base1);

  int base2 = args[2].dat->base_offset;
  double * __restrict__ celldx = (double *)(args[2].data + base2);


  //initialize global variable with the dimension of dats
  int xdim0_initialise_chunk_kernel_cellx = args[0].dat->size[0];
  int xdim1_initialise_chunk_kernel_cellx = args[1].dat->size[0];
  int xdim2_initialise_chunk_kernel_cellx = args[2].dat->size[0];

  if (OPS_diags > 1) {
    ops_timers_core(&c1,&t1);
    OPS_kernels[4].mpi_time += t1-t2;
  }

  #pragma omp parallel for
  for ( int n_y=start[1]; n_y<end[1]; n_y++ ){
    #ifdef intel
    #pragma loop_count(10000)
    #pragma omp simd aligned(vertexx,cellx,celldx)
    #else
    #pragma simd
    #endif
    for ( int n_x=start[0]; n_x<end[0]; n_x++ ){
      

  double d_x;
  d_x = (grid.xmax - grid.xmin)/(double)grid.x_cells;

  cellx[OPS_ACC1(0,0)]  = 0.5*( vertexx[OPS_ACC0(0,0)] + vertexx[OPS_ACC0(1,0)] );
  celldx[OPS_ACC2(0,0)]  = d_x;


    }
  }
  if (OPS_diags > 1) {
    ops_timers_core(&c2,&t2);
    OPS_kernels[4].time += t2-t1;
  }

  if (OPS_diags > 1) {
    //Update kernel record
    ops_timers_core(&c1,&t1);
    OPS_kernels[4].mpi_time += t1-t2;
    OPS_kernels[4].transfer += ops_compute_transfer(dim, start, end, &arg0);
    OPS_kernels[4].transfer += ops_compute_transfer(dim, start, end, &arg1);
    OPS_kernels[4].transfer += ops_compute_transfer(dim, start, end, &arg2);
  }
}
#undef OPS_ACC0
#undef OPS_ACC1
#undef OPS_ACC2


void ops_par_loop_initialise_chunk_kernel_cellx(char const *name, ops_block block, int dim, int* range,
 ops_arg arg0, ops_arg arg1, ops_arg arg2) {
  ops_kernel_descriptor *desc = (ops_kernel_descriptor *)malloc(sizeof(ops_kernel_descriptor));
  desc->name = name;
  desc->block = block;
  desc->dim = dim;
  desc->device = 1;
  desc->index = 4;
  desc->hash = 5381;
  desc->hash = ((desc->hash << 5) + desc->hash) + 4;
  for ( int i=0; i<4; i++ ){
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
  desc->hash = ((desc->hash << 5) + desc->hash) + arg2.dat->index;
  desc->function = ops_par_loop_initialise_chunk_kernel_cellx_execute;
  if (OPS_diags > 1) {
    ops_timing_realloc(4,"initialise_chunk_kernel_cellx");
  }
  ops_enqueue_kernel(desc);
  }
