//
// auto-generated by ops.py//

//header
#define OPS_ACC_MD_MACROS
#define OPS_3D
#include "ops_lib_cpp.h"

#include "ops_cuda_rt_support.h"
#include "ops_cuda_reduction.h"

#include <cuComplex.h>

#ifdef OPS_MPI
#include "ops_mpi_core.h"
#endif
// global constants

void ops_init_backend() {}

void ops_decl_const_char(int dim, char const *type,
int size, char *dat, char const *name){
  {
    printf("error: unknown const name\n"); exit(1);
  }
}


//user kernel files
#include "write_kernel_cuda_kernel.cu"
