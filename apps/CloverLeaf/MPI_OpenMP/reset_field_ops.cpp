//
// auto-generated by ops.py on 2014-03-07 16:00
//



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#include  "ops_lib_cpp.h"

//
// ops_par_loop declarations
//

void ops_par_loop_reset_field_kernel1(char const *, ops_block, int , int*,
  ops_arg,
  ops_arg,
  ops_arg,
  ops_arg );

void ops_par_loop_reset_field_kernel2(char const *, ops_block, int , int*,
  ops_arg,
  ops_arg,
  ops_arg,
  ops_arg );



#include "data.h"
#include "definitions.h"

//#include "reset_field_kernel.h"

void reset_field()
{
  error_condition = 0;

  int x_cells = grid->x_cells;
  int y_cells = grid->y_cells;
  int x_min = field->x_min;
  int x_max = field->x_max;
  int y_min = field->y_min;
  int y_max = field->y_max;

  int rangexy_inner[] = {x_min,x_max,y_min,y_max};

  ops_par_loop_reset_field_kernel1("reset_field_kernel1", clover_grid, 2, rangexy_inner,
               ops_arg_dat(density0, S2D_00, "double", OPS_WRITE),
               ops_arg_dat(density1, S2D_00, "double", OPS_READ),
               ops_arg_dat(energy0, S2D_00, "double", OPS_WRITE),
               ops_arg_dat(energy1, S2D_00, "double", OPS_READ));

  int rangexy_inner_plus1xy[] = {x_min,x_max+1,y_min,y_max+1};

  ops_par_loop_reset_field_kernel2("reset_field_kernel2", clover_grid, 2, rangexy_inner_plus1xy,
               ops_arg_dat(xvel0, S2D_00, "double", OPS_WRITE),
               ops_arg_dat(xvel1, S2D_00, "double", OPS_READ),
               ops_arg_dat(yvel0, S2D_00, "double", OPS_WRITE),
               ops_arg_dat(yvel1, S2D_00, "double", OPS_READ));

}
