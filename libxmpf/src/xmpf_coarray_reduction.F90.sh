#!/bin/bash

#-------------------------------------------------------
#  generator for xmp_coarray_reduction.F90
#-------------------------------------------------------

template='
subroutine co_%op%_%t%%k%(source, result)
  include "mpif.h"
  %type%(kind=%k%) :: source, result
  integer ierr

#ifdef _XMP_GASNET
  call mpi_barrier(mpi_comm_world, ierr, source, result)
  if (ierr /= 0) then
     call xmpf_coarray_fatal("CO_%OP% failed before mpi_allreduce")
  end if
#endif
  call mpi_allreduce(source, result, 1, %mpitype%, &
       mpi_%op%, mpi_comm_world, ierr)
  if (ierr /= 0) then
     call xmpf_coarray_fatal("CO_%OP% failed at mpi_allreduce")
  end if
#ifdef _XMP_GASNET
  call mpi_barrier(mpi_comm_world, ierr, source, result)
  if (ierr /= 0) then
     call xmpf_coarray_fatal("CO_%OP% failed after mpi_allreduce")
  end if
#endif
  return
end subroutine'

print_procedure() {
    echo "$template" | sed '
s/%op%/'$1'/g
s/%OP%/'$2'/g
s/%type%/'$3'/g
s/%t%/'$4'/g
s/%k%/'$5'/g
s/%mpitype%/'$6'/g'
}

print_reduction() {
    echo '
!-------------------------------
!  coarray intrinsic co_'$1'
!-------------------------------'

    print_procedure "$@" integer i 4 mpi_integer
    print_procedure "$@" real    r 4 mpi_real4
    print_procedure "$@" real    r 8 mpi_real8
}


#############
# main 
#############

echo "!! This file is automatically generated by $0"
print_reduction sum SUM
print_reduction max MAX
print_reduction min MIN

exit 0

