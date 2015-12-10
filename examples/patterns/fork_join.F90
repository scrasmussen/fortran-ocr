!------------------------------------------------------------------------------
!
! Fork-Join pattern
!
!   This examples shows how multiple EDTs are spawned to create a fork-join
!   pattern.  All threads run the same program (SPMD) mainF90Edt.
!
!------------------------------------------------------------------------------
module fork_join_mod
  use :: OCR
  implicit none

contains


!------------------------------------------------------------------------------
! syncEdt
!   1. Replicate
!   2. Chain decendent onto the next compute EDT
!   3. Exchange data
!------------------------------------------------------------------------------
function syncEdt(paramc, paramv, depc, depv) result(rtn) &
   BIND(C, name='syncEdt')
   implicit none
   integer(u32), intent(in) :: paramc, depc
   integer(u64), intent(in) :: paramv(*)
   type(ocrEdtDep_t), intent(in) :: depv(*)
   integer(ocrGuid_k) :: rtn

   ! local variables
   type(SPMD_t) :: info
   real, pointer :: A(:), A_new(:), halo_l(:), halo_r(:)

   call SPMD_Get_info(depv(1), info)

   call printf("syncEdt:: rank", info%rank)
   call printf("syncEdt:: size", info%size)

   !! replicate and connect
   !

   call SPMD_Replicate_sync(info)

   !! halo exchange
   !
   call c_f_pointer(get_data       (depv(2)), A)
   call c_f_pointer(get_halo_write (depv(2)), LEFT,  halo_l)
   call c_f_pointer(get_halo_write (depv(2)), RIGHT, halo_r)

   halo_l(1) = A(1 )
   halo_r(1) = A(NX)

   call start_neighbor_compute(depv(1))

   rtn = NULL_GUID
end function syncEdt

!------------------------------------------------------------------------------
!------------------------------------------------------------------------------
function computeEdt(paramc, paramv, depc, depv) result(rtn) &
   BIND(C, name='computeEdt')
   implicit none
   integer(u32), intent(in) :: paramc, depc
   integer(u64), intent(in) :: paramv(*)
   type(ocrEdtDep_t), intent(in) :: depv(*)
   integer(ocrGuid_k) :: rtn

   ! local variables
   type(SPMD_t) :: info
   real, pointer :: A(:)

   call SPMD_Get_info(depv(1), info)

   call printf("mainF90Edt:: rank", info%rank)
   call printf("mainF90Edt:: size", info%rank)

   call c_f_pointer(depv(2)%ptr, A)
   call c_f_pointer(get_data_buffer(depv(2)), A_new)

   call exchange_halo(depv(2:4))

   call SPMD_Finalize(info)

   rtn = NULL_GUID
end function computeEdt

function mainF90Edt(paramc, paramv, depc, depv) result(rtn) &
   BIND(C, name='mainF90Edt')
   implicit none
   integer(u32), intent(in) :: paramc, depc
   integer(u64), intent(in) :: paramv(*)
   type(ocrEdtDep_t), intent(in) :: depv(*)
   integer(ocrGuid_k) :: rtn
   
   ! local variables
   type(SPMD_t) :: info
   real, pointer :: A(:)

   call SPMD_Get_info(depv(1), info)

   call printf("mainF90Edt:: rank", info%rank)
   call printf("mainF90Edt:: size", info%rank)

   call c_f_pointer(depv(2)%ptr, A)

   call exchange_halo(depv(2:4))

   call SPMD_Finalize(info)

   rtn = NULL_GUID
end function mainF90Edt

function mainEdt(paramc, paramv, depc, depv) result(rtn) &
   BIND (c, name='mainEdt')
   implicit none
   integer(u32), intent(in) :: paramc, depc
   integer(u64), intent(in) :: paramv(*)
   type(ocrEdtDep_t), intent(in) :: depv(*)
   integer(ocrGuid_k) :: rtn

   ! local variables
   type(SPMD_t) :: info
   integer(u64) :: argc, size, dim
   logical      :: err

   argc = getArgc(depv(1)%ptr)
   size = getArgv_i64(depv(1)%ptr, "-np", err)     ! for example "smpd_run" -np 4

   if (err) call ocrAbort("-np command line parameter not present", 1_u8)

   dim = 1
   call SPMD_Init(size, dim, topology=[size], nDeps=2,                             &
                  C_FUNLOC(mainF90Edto), C_FUNLOC(syncEdt), C_FUNLOC(finalizeEdt), &
                  info_array)

   !! Create array data blocks with associated halos
   !
   call SPMD_Add_haloarray(len, slot=1, halo=[1,1], info_array)

   !! Start the spawned SPMD EDTs
   !
   call SPMD_Run(info_array)

   !! mainEDT can go away
   !
   rtn = NULL_GUID
end function mainEdt

end module fork_join_mod
