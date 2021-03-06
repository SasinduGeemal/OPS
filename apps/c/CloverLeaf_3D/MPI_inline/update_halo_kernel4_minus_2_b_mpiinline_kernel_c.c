//
// auto-generated by ops.py
//

int xdim0_update_halo_kernel4_minus_2_b;
int ydim0_update_halo_kernel4_minus_2_b;
int xdim1_update_halo_kernel4_minus_2_b;
int ydim1_update_halo_kernel4_minus_2_b;


#define OPS_ACC0(x,y,z) (n_x*1+n_y*xdim0_update_halo_kernel4_minus_2_b*1+n_z*xdim0_update_halo_kernel4_minus_2_b*ydim0_update_halo_kernel4_minus_2_b*1+x+xdim0_update_halo_kernel4_minus_2_b*(y)+xdim0_update_halo_kernel4_minus_2_b*ydim0_update_halo_kernel4_minus_2_b*(z))
#define OPS_ACC1(x,y,z) (n_x*1+n_y*xdim1_update_halo_kernel4_minus_2_b*1+n_z*xdim1_update_halo_kernel4_minus_2_b*ydim1_update_halo_kernel4_minus_2_b*1+x+xdim1_update_halo_kernel4_minus_2_b*(y)+xdim1_update_halo_kernel4_minus_2_b*ydim1_update_halo_kernel4_minus_2_b*(z))

//user function



void update_halo_kernel4_minus_2_b_c_wrapper(
  double * restrict vol_flux_y,
  double * restrict mass_flux_y,
  const int * restrict fields,
  int x_size, int y_size, int z_size) {
  #pragma omp parallel for
  for ( int n_z=0; n_z<z_size; n_z++ ){
    for ( int n_y=0; n_y<y_size; n_y++ ){
      for ( int n_x=0; n_x<x_size; n_x++ ){
        
  if(fields[FIELD_VOL_FLUX_Y] == 1) vol_flux_y[OPS_ACC0(0,0,0)] = -(vol_flux_y[OPS_ACC0(0,-2,0)]);
  if(fields[FIELD_MASS_FLUX_Y] == 1) mass_flux_y[OPS_ACC1(0,0,0)] = -(mass_flux_y[OPS_ACC1(0,-2,0)]);

      }
    }
  }
}
#undef OPS_ACC0
#undef OPS_ACC1

