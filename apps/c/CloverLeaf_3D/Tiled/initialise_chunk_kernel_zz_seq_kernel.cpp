//
// auto-generated by ops.py
//
#define OPS_ACC0(x,y,z) (n_x*0+n_y*xdim0_initialise_chunk_kernel_zz*0+n_z*xdim0_initialise_chunk_kernel_zz*ydim0_initialise_chunk_kernel_zz*1+x+xdim0_initialise_chunk_kernel_zz*(y)+xdim0_initialise_chunk_kernel_zz*ydim0_initialise_chunk_kernel_zz*(z))


//user function

// host stub function
void ops_par_loop_initialise_chunk_kernel_zz_execute(ops_kernel_descriptor *desc) {
  ops_block block = desc->block;
  int dim = desc->dim;
  int *range = desc->range;
  ops_arg arg0 = desc->args[0];
  ops_arg arg1 = desc->args[1];

  //Timing
  double t1,t2,c1,c2;

  ops_arg args[2] = { arg0, arg1};



  #ifdef CHECKPOINTING
  if (!ops_checkpointing_before(args,2,range,48)) return;
  #endif

  if (OPS_diags > 1) {
    OPS_kernels[48].count++;
    ops_timers_core(&c2,&t2);
  }

  //compute locally allocated range for the sub-block
  int start[3];
  int end[3];

  for ( int n=0; n<3; n++ ){
    start[n] = range[2*n];end[n] = range[2*n+1];
  }

  #ifdef OPS_DEBUG
  ops_register_args(args, "initialise_chunk_kernel_zz");
  #endif


  int arg_idx[3];
  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  arg_idx[0] = sb->decomp_disp[0];
  arg_idx[1] = sb->decomp_disp[1];
  arg_idx[2] = sb->decomp_disp[2];
  #else //OPS_MPI
  arg_idx[0] = 0;
  arg_idx[1] = 0;
  arg_idx[2] = 0;
  #endif //OPS_MPI

  //set up initial pointers and exchange halos if necessary
  int base0 = args[0].dat->base_offset;
  int * __restrict__ zz = (int *)(args[0].data + base0);



  //initialize global variable with the dimension of dats
  int xdim0_initialise_chunk_kernel_zz = args[0].dat->size[0];
  int ydim0_initialise_chunk_kernel_zz = args[0].dat->size[1];

  if (OPS_diags > 1) {
    ops_timers_core(&c1,&t1);
    OPS_kernels[48].mpi_time += t1-t2;
  }

  #pragma omp parallel for collapse(2)
  for ( int n_z=start[2]; n_z<end[2]; n_z++ ){
    for ( int n_y=start[1]; n_y<end[1]; n_y++ ){
      #ifdef intel
      #pragma loop_count(10000)
      #pragma omp simd aligned(zz)
      #else
      #pragma simd
      #endif
      for ( int n_x=start[0]; n_x<end[0]; n_x++ ){
        int idx[] = {arg_idx[0]+n_x, arg_idx[1]+n_y, arg_idx[2]+n_z};
        
  zz[OPS_ACC0(0,0,0)] = idx[2]-2;

      }
    }
  }
  if (OPS_diags > 1) {
    ops_timers_core(&c2,&t2);
    OPS_kernels[48].time += t2-t1;
  }

  if (OPS_diags > 1) {
    //Update kernel record
    ops_timers_core(&c1,&t1);
    OPS_kernels[48].mpi_time += t1-t2;
    OPS_kernels[48].transfer += ops_compute_transfer(dim, start, end, &arg0);
  }
}
#undef OPS_ACC0


void ops_par_loop_initialise_chunk_kernel_zz(char const *name, ops_block block, int dim, int* range,
 ops_arg arg0, ops_arg arg1) {
  ops_kernel_descriptor *desc = (ops_kernel_descriptor *)malloc(sizeof(ops_kernel_descriptor));
  desc->name = name;
  desc->block = block;
  desc->dim = dim;
  desc->index = 48;
  desc->hash = 5381;
  desc->hash = ((desc->hash << 5) + desc->hash) + 48;
  for ( int i=0; i<6; i++ ){
    desc->range[i] = range[i];
    desc->orig_range[i] = range[i];
    desc->hash = ((desc->hash << 5) + desc->hash) + range[i];
  }
  desc->nargs = 2;
  desc->args = (ops_arg*)malloc(2*sizeof(ops_arg));
  desc->args[0] = arg0;
  desc->hash = ((desc->hash << 5) + desc->hash) + arg0.dat->index;
  desc->args[1] = arg1;
  desc->function = ops_par_loop_initialise_chunk_kernel_zz_execute;
  if (OPS_diags > 1) {
    ops_timing_realloc(48,"initialise_chunk_kernel_zz");
  }
  ops_enqueue_kernel(desc);
  }
