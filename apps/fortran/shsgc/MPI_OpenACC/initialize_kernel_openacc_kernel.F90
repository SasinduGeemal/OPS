!
! auto-generated by ops_fortran.py
!
MODULE INITIALIZE_KERNEL_MODULE
USE OPS_FORTRAN_DECLARATIONS
USE OPS_FORTRAN_RT_SUPPORT

USE OPS_CONSTANTS
USE ISO_C_BINDING

INTEGER(KIND=4) xdim1
#define OPS_ACC1(x) (x+1)
INTEGER(KIND=4) xdim2
#define OPS_ACC2(x) (x+1)
INTEGER(KIND=4) xdim3
#define OPS_ACC3(x) (x+1)
INTEGER(KIND=4) xdim4
#define OPS_ACC4(x) (x+1)
INTEGER(KIND=4) xdim5
#define OPS_ACC5(x) (x+1)


contains

!$ACC ROUTINE(initialize_kernel) SEQ
!user function
subroutine initialize_kernel(x, rho_new, rhou_new, rhoE_new, rhoin, idx)
  real (kind=8) , DIMENSION(1) :: x, rho_new, rhou_new, rhoE_new, rhoin
  INTEGER(kind=4), DIMENSION(1), INTENT(IN) :: idx

  x(OPS_ACC1(0)) = xmin + (idx(1)-2.0_8-1.0_8) * dx
  if (x(OPS_ACC1(0)) .ge. -4.0_8) then
    rho_new(OPS_ACC2(0)) = 1.0_8 + eps * dsin(lambda * x(OPS_ACC1(0)))
    rhou_new(OPS_ACC3(0)) = ur * rho_new(OPS_ACC2(0))
    rhoE_new(OPS_ACC4(0)) = (pr / gam1) + 0.5_8 * (rhou_new(OPS_ACC3(0))**2_8)/rho_new(OPS_ACC2(0))
  else
    rho_new(OPS_ACC2(0)) = rhol
    rhou_new(OPS_ACC3(0)) = ul * rho_new(OPS_ACC2(0))
    rhoE_new(OPS_ACC4(0)) = (pl / gam1) + 0.5_8 * (rhou_new(OPS_ACC3(0))**2_8)/rho_new(OPS_ACC2(0))
  end if

  rhoin(OPS_ACC5(0)) = gam1 * (rhoE_new(OPS_ACC4(0)) - 0.5_8 * rhou_new(OPS_ACC3(0)) * rhou_new(OPS_ACC3(0)) / rho_new(OPS_ACC2(0)))

end subroutine

#undef OPS_ACC1
#undef OPS_ACC2
#undef OPS_ACC3
#undef OPS_ACC4
#undef OPS_ACC5



subroutine initialize_kernel_wrap( &
& opsDat1Local, &
& opsDat2Local, &
& opsDat3Local, &
& opsDat4Local, &
& opsDat5Local, &
& idx, &
& dat1_base, &
& dat2_base, &
& dat3_base, &
& dat4_base, &
& dat5_base, &
& start, &
& end )
  IMPLICIT NONE
  real(8) :: opsDat1Local(*)
  real(8) :: opsDat2Local(*)
  real(8) :: opsDat3Local(*)
  real(8) :: opsDat4Local(*)
  real(8) :: opsDat5Local(*)
  integer(4) idx(1)
  integer(4) :: idx_local(1)
  integer :: dat1_base
  integer :: dat2_base
  integer :: dat3_base
  integer :: dat4_base
  integer :: dat5_base
  integer(4) start(1)
  integer(4) end(1)
  integer n_x


  !$acc parallel deviceptr(opsDat1Local,opsDat2Local,opsDat3Local,opsDat4Local,opsDat5Local)  
  !$acc loop 
  DO n_x = 1, end(1)-start(1)+1
    idx_local(1) = idx(1) + n_x - 1
    call initialize_kernel( &
    & opsDat1Local(dat1_base+(n_x-1)*1), &
    & opsDat2Local(dat2_base+(n_x-1)*1), &
    & opsDat3Local(dat3_base+(n_x-1)*1), &
    & opsDat4Local(dat4_base+(n_x-1)*1), &
    & opsDat5Local(dat5_base+(n_x-1)*1), &
    & idx_local )
  END DO
  !$acc end parallel

end subroutine

!host subroutine
subroutine initialize_kernel_host( userSubroutine, block, dim, range, &
& opsArg1, &
& opsArg2, &
& opsArg3, &
& opsArg4, &
& opsArg5, &
& opsArg6)
  IMPLICIT NONE
  character(kind=c_char,len=*), INTENT(IN) :: userSubroutine
  type ( ops_block ), INTENT(IN) :: block
  integer(kind=4), INTENT(IN):: dim
  integer(kind=4)   , DIMENSION(dim), INTENT(IN) :: range
  real(kind=8) t1,t2,t3
  real(kind=4) transfer_total, transfer

  type ( ops_arg )  , INTENT(IN) :: opsArg1
  real(8), DIMENSION(:), POINTER :: opsDat1Local
  integer(kind=4) :: opsDat1Cardinality
  integer(kind=4), POINTER, DIMENSION(:)  :: dat1_size
  integer(kind=4) :: dat1_base

  type ( ops_arg )  , INTENT(IN) :: opsArg2
  real(8), DIMENSION(:), POINTER :: opsDat2Local
  integer(kind=4) :: opsDat2Cardinality
  integer(kind=4), POINTER, DIMENSION(:)  :: dat2_size
  integer(kind=4) :: dat2_base

  type ( ops_arg )  , INTENT(IN) :: opsArg3
  real(8), DIMENSION(:), POINTER :: opsDat3Local
  integer(kind=4) :: opsDat3Cardinality
  integer(kind=4), POINTER, DIMENSION(:)  :: dat3_size
  integer(kind=4) :: dat3_base

  type ( ops_arg )  , INTENT(IN) :: opsArg4
  real(8), DIMENSION(:), POINTER :: opsDat4Local
  integer(kind=4) :: opsDat4Cardinality
  integer(kind=4), POINTER, DIMENSION(:)  :: dat4_size
  integer(kind=4) :: dat4_base

  type ( ops_arg )  , INTENT(IN) :: opsArg5
  real(8), DIMENSION(:), POINTER :: opsDat5Local
  integer(kind=4) :: opsDat5Cardinality
  integer(kind=4), POINTER, DIMENSION(:)  :: dat5_size
  integer(kind=4) :: dat5_base

  type ( ops_arg )  , INTENT(IN) :: opsArg6

  integer n_x
  integer start(1)
  integer end(1)
  integer idx(1)
  integer(kind=4) :: n

  type ( ops_arg ) , DIMENSION(6) :: opsArgArray

  opsArgArray(1) = opsArg1
  opsArgArray(2) = opsArg2
  opsArgArray(3) = opsArg3
  opsArgArray(4) = opsArg4
  opsArgArray(5) = opsArg5
  opsArgArray(6) = opsArg6

  call setKernelTime(0,userSubroutine//char(0),0.0_8,0.0_8,0.0_4,0)
  call ops_timers_core(t1)

#ifdef OPS_MPI
  IF (getRange(block, start, end, range) < 0) THEN
    return
  ENDIF
#else
  DO n = 1, 1
    start(n) = range(2*n-1)
    end(n) = range(2*n);
  END DO
#endif

#ifdef OPS_MPI
  call getIdx(block,start,idx)
#else
  idx(1) = start(1)
#endif

  call c_f_pointer(getDatSizeFromOpsArg(opsArg1),dat1_size,(/dim/))
  xdim1 = dat1_size(1)
  opsDat1Cardinality = opsArg1%dim * xdim1
  dat1_base = getDatBaseFromOpsArg1D(opsArg1,start,1)
  call c_f_pointer(opsArg1%data_d,opsDat1Local,(/opsDat1Cardinality/))

  call c_f_pointer(getDatSizeFromOpsArg(opsArg2),dat2_size,(/dim/))
  xdim2 = dat2_size(1)
  opsDat2Cardinality = opsArg2%dim * xdim2
  dat2_base = getDatBaseFromOpsArg1D(opsArg2,start,1)
  call c_f_pointer(opsArg2%data_d,opsDat2Local,(/opsDat2Cardinality/))

  call c_f_pointer(getDatSizeFromOpsArg(opsArg3),dat3_size,(/dim/))
  xdim3 = dat3_size(1)
  opsDat3Cardinality = opsArg3%dim * xdim3
  dat3_base = getDatBaseFromOpsArg1D(opsArg3,start,1)
  call c_f_pointer(opsArg3%data_d,opsDat3Local,(/opsDat3Cardinality/))

  call c_f_pointer(getDatSizeFromOpsArg(opsArg4),dat4_size,(/dim/))
  xdim4 = dat4_size(1)
  opsDat4Cardinality = opsArg4%dim * xdim4
  dat4_base = getDatBaseFromOpsArg1D(opsArg4,start,1)
  call c_f_pointer(opsArg4%data_d,opsDat4Local,(/opsDat4Cardinality/))

  call c_f_pointer(getDatSizeFromOpsArg(opsArg5),dat5_size,(/dim/))
  xdim5 = dat5_size(1)
  opsDat5Cardinality = opsArg5%dim * xdim5
  dat5_base = getDatBaseFromOpsArg1D(opsArg5,start,1)
  call c_f_pointer(opsArg5%data_d,opsDat5Local,(/opsDat5Cardinality/))

  call ops_H_D_exchanges_device(opsArgArray,6)
  call ops_halo_exchanges(opsArgArray,6,range)
  call ops_H_D_exchanges_device(opsArgArray,6)

  call ops_timers_core(t2)

  call initialize_kernel_wrap( &
  & opsDat1Local, &
  & opsDat2Local, &
  & opsDat3Local, &
  & opsDat4Local, &
  & opsDat5Local, &
  & idx, &
  & dat1_base, &
  & dat2_base, &
  & dat3_base, &
  & dat4_base, &
  & dat5_base, &
  & start, &
  & end )

  call ops_timers_core(t3)
  call ops_set_dirtybit_device(opsArgArray, 6)
  call ops_set_halo_dirtybit3(opsArg1,range)
  call ops_set_halo_dirtybit3(opsArg2,range)
  call ops_set_halo_dirtybit3(opsArg3,range)
  call ops_set_halo_dirtybit3(opsArg4,range)
  call ops_set_halo_dirtybit3(opsArg5,range)

  !Timing and data movement
  transfer_total = 0.0_4
  call ops_compute_transfer(1, start, end, opsArg1,transfer)
  transfer_total = transfer_total + transfer
  call ops_compute_transfer(1, start, end, opsArg2,transfer)
  transfer_total = transfer_total + transfer
  call ops_compute_transfer(1, start, end, opsArg3,transfer)
  transfer_total = transfer_total + transfer
  call ops_compute_transfer(1, start, end, opsArg4,transfer)
  transfer_total = transfer_total + transfer
  call ops_compute_transfer(1, start, end, opsArg5,transfer)
  transfer_total = transfer_total + transfer
  call setKernelTime(0,userSubroutine,t3-t2,t2-t1,transfer_total,1)
end subroutine
END MODULE
