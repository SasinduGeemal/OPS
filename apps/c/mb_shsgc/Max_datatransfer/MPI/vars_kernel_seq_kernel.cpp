//
// auto-generated by ops.py
//
#define OPS_ACC_MD0(d,x) ((x)*3+(d))
#define OPS_ACC_MD1(d,x) ((x)*3+(d))
#define OPS_ACC_MD2(d,x) ((x)*3+(d))
#define OPS_ACC_MD3(d,x) ((x)*3+(d))
#define OPS_ACC_MD4(d,x) ((x)*3+(d))

//user function
inline void vars_kernel(const double* alam, const double* al, const double *gt,
                 double* cmp,  double* cf) {
  double  anu, aaa, ga, qf, ww;
  for (int m=0; m < 3 ;m++) {
    anu = alam[OPS_ACC_MD0(m,0)];
    aaa = al[OPS_ACC_MD1(m,0)];
    ga = aaa * ( gt[OPS_ACC_MD2(m,1)] - gt[OPS_ACC_MD2(m,0)]) / (pow(aaa,2.0) + del2);
    qf = sqrt ( con + pow(anu,2.0));
    cmp[OPS_ACC_MD3(m,0)] = 0.50 * qf;
    ww = anu + cmp[OPS_ACC_MD3(m,0)] * ga;
    qf = sqrt(con + pow(ww,2.0));
    cf[OPS_ACC_MD4(m,0)] = qf;
  }
}



#undef OPS_ACC_MD0
#undef OPS_ACC_MD1
#undef OPS_ACC_MD2
#undef OPS_ACC_MD3
#undef OPS_ACC_MD4


// host stub function
void ops_par_loop_vars_kernel(char const *name, ops_block block, int dim, int* range,
 ops_arg arg0, ops_arg arg1, ops_arg arg2, ops_arg arg3,
 ops_arg arg4) {

  //Timing
  double t1,t2,c1,c2;

  char *p_a[5];
  int  offs[5][1];
  ops_arg args[5] = { arg0, arg1, arg2, arg3, arg4};



  #ifdef CHECKPOINTING
  if (!ops_checkpointing_before(args,5,range,10)) return;
  #endif

  if (OPS_diags > 1) {
    ops_timing_realloc(10,"vars_kernel");
    OPS_kernels[10].count++;
    ops_timers_core(&c2,&t2);
  }

  //compute locally allocated range for the sub-block
  int start[1];
  int end[1];

  #ifdef OPS_MPI
  sub_block_list sb = OPS_sub_block_list[block->index];
  if (!sb->owned) return;
  for ( int n=0; n<1; n++ ){
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
  for ( int n=0; n<1; n++ ){
    start[n] = range[2*n];end[n] = range[2*n+1];
  }
  #endif
  #ifdef OPS_DEBUG
  ops_register_args(args, "vars_kernel");
  #endif

  offs[0][0] = args[0].stencil->stride[0]*1;  //unit step in x dimension

  offs[1][0] = args[1].stencil->stride[0]*1;  //unit step in x dimension

  offs[2][0] = args[2].stencil->stride[0]*1;  //unit step in x dimension

  offs[3][0] = args[3].stencil->stride[0]*1;  //unit step in x dimension

  offs[4][0] = args[4].stencil->stride[0]*1;  //unit step in x dimension



  int off0_0 = offs[0][0];
  int dat0 = (OPS_soa ? args[0].dat->type_size : args[0].dat->elem_size);
  int off1_0 = offs[1][0];
  int dat1 = (OPS_soa ? args[1].dat->type_size : args[1].dat->elem_size);
  int off2_0 = offs[2][0];
  int dat2 = (OPS_soa ? args[2].dat->type_size : args[2].dat->elem_size);
  int off3_0 = offs[3][0];
  int dat3 = (OPS_soa ? args[3].dat->type_size : args[3].dat->elem_size);
  int off4_0 = offs[4][0];
  int dat4 = (OPS_soa ? args[4].dat->type_size : args[4].dat->elem_size);

  //set up initial pointers and exchange halos if necessary
  int base0 = args[0].dat->base_offset + (OPS_soa ? args[0].dat->type_size : args[0].dat->elem_size) * start[0] * args[0].stencil->stride[0];
  p_a[0] = (char *)args[0].data + base0;

  int base1 = args[1].dat->base_offset + (OPS_soa ? args[1].dat->type_size : args[1].dat->elem_size) * start[0] * args[1].stencil->stride[0];
  p_a[1] = (char *)args[1].data + base1;

  int base2 = args[2].dat->base_offset + (OPS_soa ? args[2].dat->type_size : args[2].dat->elem_size) * start[0] * args[2].stencil->stride[0];
  p_a[2] = (char *)args[2].data + base2;

  int base3 = args[3].dat->base_offset + (OPS_soa ? args[3].dat->type_size : args[3].dat->elem_size) * start[0] * args[3].stencil->stride[0];
  p_a[3] = (char *)args[3].data + base3;

  int base4 = args[4].dat->base_offset + (OPS_soa ? args[4].dat->type_size : args[4].dat->elem_size) * start[0] * args[4].stencil->stride[0];
  p_a[4] = (char *)args[4].data + base4;


  //initialize global variable with the dimension of dats
  xdim0 = args[0].dat->size[0];
  xdim1 = args[1].dat->size[0];
  xdim2 = args[2].dat->size[0];
  xdim3 = args[3].dat->size[0];
  xdim4 = args[4].dat->size[0];

  //Halo Exchanges
  ops_H_D_exchanges_host(args, 5);
  ops_halo_exchanges(args,5,range);
  ops_H_D_exchanges_host(args, 5);

  if (OPS_diags > 1) {
    ops_timers_core(&c1,&t1);
    OPS_kernels[10].mpi_time += t1-t2;
  }

  int n_x;
  #pragma novector
  for( n_x=start[0]; n_x<start[0]+((end[0]-start[0])/SIMD_VEC)*SIMD_VEC; n_x+=SIMD_VEC ) {
    //call kernel function, passing in pointers to data -vectorised
    #pragma simd
    for ( int i=0; i<SIMD_VEC; i++ ){
      vars_kernel(  (double *)p_a[0]+ i*1*3, (double *)p_a[1]+ i*1*3, (double *)p_a[2]+ i*1*3,
           (double *)p_a[3]+ i*1*3, (double *)p_a[4]+ i*1*3 );

    }

    //shift pointers to data x direction
    p_a[0]= p_a[0] + (dat0 * off0_0)*SIMD_VEC;
    p_a[1]= p_a[1] + (dat1 * off1_0)*SIMD_VEC;
    p_a[2]= p_a[2] + (dat2 * off2_0)*SIMD_VEC;
    p_a[3]= p_a[3] + (dat3 * off3_0)*SIMD_VEC;
    p_a[4]= p_a[4] + (dat4 * off4_0)*SIMD_VEC;
  }

  for ( int n_x=start[0]+((end[0]-start[0])/SIMD_VEC)*SIMD_VEC; n_x<end[0]; n_x++ ){
    //call kernel function, passing in pointers to data - remainder
    vars_kernel(  (double *)p_a[0], (double *)p_a[1], (double *)p_a[2],
           (double *)p_a[3], (double *)p_a[4] );


    //shift pointers to data x direction
    p_a[0]= p_a[0] + (dat0 * off0_0);
    p_a[1]= p_a[1] + (dat1 * off1_0);
    p_a[2]= p_a[2] + (dat2 * off2_0);
    p_a[3]= p_a[3] + (dat3 * off3_0);
    p_a[4]= p_a[4] + (dat4 * off4_0);
  }

  if (OPS_diags > 1) {
    ops_timers_core(&c2,&t2);
    OPS_kernels[10].time += t2-t1;
  }
  ops_set_dirtybit_host(args, 5);
  ops_set_halo_dirtybit3(&args[3],range);
  ops_set_halo_dirtybit3(&args[4],range);

  if (OPS_diags > 1) {
    //Update kernel record
    ops_timers_core(&c1,&t1);
    OPS_kernels[10].mpi_time += t1-t2;
    OPS_kernels[10].transfer += ops_compute_transfer(dim, start, end, &arg0);
    OPS_kernels[10].transfer += ops_compute_transfer(dim, start, end, &arg1);
    OPS_kernels[10].transfer += ops_compute_transfer(dim, start, end, &arg2);
    OPS_kernels[10].transfer += ops_compute_transfer(dim, start, end, &arg3);
    OPS_kernels[10].transfer += ops_compute_transfer(dim, start, end, &arg4);
  }
}
