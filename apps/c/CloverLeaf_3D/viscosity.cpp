/* Crown Copyright 2012 AWE.

 This file is part of CloverLeaf.

 CloverLeaf is free software: you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the
 Free Software Foundation, either version 3 of the License, or (at your option)
 any later version.

 CloverLeaf is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 details.

 You should have received a copy of the GNU General Public License along with
 CloverLeaf. If not, see http://www.gnu.org/licenses/. */

/** @brief call the viscosity kernels
 *  @author Wayne Gaudin
 *  @details Calculates an artificial viscosity using the Wilkin's method to
 *  smooth out shock front and prevent oscillations around discontinuities.
 *  Only cells in compression will have a non-zero value.
**/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// OPS header file

#define OPS_3D
#include "ops_seq.h"

#include "data.h"
#include "definitions.h"

#include "viscosity_kernel.h"


void viscosity_func()
{
  //initialize sizes using global values
  int x_min = field.x_min;
  int x_max = field.x_max;
  int y_min = field.y_min;
  int y_max = field.y_max;
  int z_min = field.z_min;
  int z_max = field.z_max;

  int rangexyz_inner[] = {x_min,x_max,y_min,y_max,z_min,z_max}; // inner range without border

  ops_par_loop(viscosity_kernel, "viscosity_kernel", clover_grid, 3, rangexyz_inner,
      ops_arg_dat(xvel0, 1, S3D_000_fP1P1P1, "double", OPS_READ),
      ops_arg_dat(yvel0, 1, S3D_000_fP1P1P1, "double", OPS_READ),
      ops_arg_dat(celldx, 1, S3D_000_P100_STRID3D_X, "double", OPS_READ),
      ops_arg_dat(celldy, 1, S3D_000_0P10_STRID3D_Y, "double", OPS_READ),
      ops_arg_dat(pressure, 1, S3D_P100_M100_0P10_0M10_00P1_00M1, "double", OPS_READ),
      ops_arg_dat(density0, 1, S3D_000, "double", OPS_READ),
      ops_arg_dat(viscosity, 1, S3D_000, "double", OPS_WRITE),
      ops_arg_dat(zvel0, 1, S3D_000_fP1P1P1, "double", OPS_READ),
      ops_arg_dat(celldz, 1, S3D_000_00P1_STRID3D_Z, "double", OPS_READ),
      ops_arg_dat(xarea, 1, S3D_000, "double", OPS_READ),
      ops_arg_dat(yarea, 1, S3D_000, "double", OPS_READ),
      ops_arg_dat(zarea, 1, S3D_000, "double", OPS_READ));
}
