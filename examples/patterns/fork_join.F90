!------------------------------------------------------------------------------
!
! Fork-Join pattern
!
!   This examples shows how multiple EDTs are spawned to create a fork-join
!   pattern.  All threads run the same program (SPMD) mainF90Edt.
!
!------------------------------------------------------------------------------
module fork_join_mod
  use :: ocr_interfaces
  use :: ocr_types
  use :: ocr_fortran_types
  use :: ocr_fortran
  implicit none

   integer(C_INT32_T), parameter :: NPARAMS = 1

contains

function mainF90Edt(paramc, paramv, depc, depv) result(rtn) &
   BIND(C, name='mainF90Edt')
   use ISO_C_BINDING
   implicit none
   integer(C_INT32_T), intent(in) :: paramc, depc
   integer(C_INT64_T), intent(in) :: paramv(*)
   type(ocrEdtDep_t),  intent(in) :: depv(*)
   integer(ocrGuid_k)  :: rtn
   
   type(dbParams_t) :: db_params

   ! call spmd_get_params(depc, depv, db_params)

   ! call printf("mainF90Edt:: rank", db_params%rank)

   call printf_str('Fin mainF90Edt')
   rtn = NULL_GUID
end function mainF90Edt

function mainEdt(paramc, paramv, depc, depv) result(rtn) &
   BIND (c, name='mainEdt')
   use ISO_C_BINDING
   implicit none
   integer(u32), intent(in) :: paramc, depc
   integer(u64), intent(in) :: paramv(*)
   type(ocrEdtDep_t),  intent(in) :: depv(*)
   integer(ocrGuid_k) :: rtn
   ! integer(C_INTPTR_T)  :: rtn
   integer(u8) :: errcode

   ! local variables
   ! integer(ocrGuid_k) :: edt_finalize, evt_start
   integer(ocrGuid_k) :: edt_finalize, evt_start
   integer(u64) :: argc, size, i
   type(C_PTR)        :: args
   logical            :: err

   argc = getArgc(depv(1)%ptr);

   !! Read size from params
   ! Need to figure out out to imput string from command line
   size = getArgv_i64(depv(1)%ptr, "test -np 4 ", err)

   errcode = 1
   if (err) call ocrAbort(errcode)
   ! ! if (err) call ocrAbort("-np command line parameter not present")

   edt_finalize = spmd_create_finalize_edt(size)
   
   ! !! Fork SPMD processes/threads
   ! !
   ! do i = 1, size-1
   !    !! set params as needed
   !    !
   !    evt_start = create_spmd_edt(i, size, C_FUNLOC(mainF90Edt), edt_finalize)
   !    !! add deps as needed
   !    !
   !    call ocrEventSatisfy(evt_start, NULL_GUID)
   ! end do

   rtn = NULL_GUID
   call ocrShutdown()
   call printf_str('Fin')
end function mainEdt

end module fork_join_mod
