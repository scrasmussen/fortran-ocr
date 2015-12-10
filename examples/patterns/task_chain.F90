!------------------------------------------------------------------------------
!
! Chain pattern
!
!   Code segment EDTs will have to be outlined from original code at segment
!   boundaries where communication or synchronization occurs.  This example
!   shows how EDT are chained together by connecting the output event from
!   one segment to an input slot on the succeeding segment EDT.
!
!------------------------------------------------------------------------------
module task_chain_mod
  use :: ocr
  implicit none

contains

function segment1(paramc, paramv, depc, depv) result(rtn) &
   BIND(C, name='segment_1')
   use ISO_C_BINDING
   implicit none
   integer(C_INTPTR_T) :: rtn
   integer(C_INT32_T), intent(in) :: paramc, depc
   integer(C_INT64_T), intent(in) :: paramv(*)
   type(ocrEdtDep_t),  intent(in) :: depv(*)
   integer(C_INT32_t), pointer    :: nseg

   call c_f_pointer(depv(1)%ptr, nseg)
   nseg = nseg + 1
   call printf("Segment 1", nseg)

   nullify(nseg)
   call ocrDbRelease(depv(1)%guid)

   rtn = NULL_GUID
end function segment1

function segment2(paramc, paramv, depc, depv) result(rtn) &
   BIND(C, name='segment_2')
   use ISO_C_BINDING
   implicit none
   integer(C_INTPTR_T) :: rtn
   integer(C_INT32_T), intent(in) :: paramc, depc
   integer(C_INT64_T), intent(in) :: paramv(*)
   type(ocrEdtDep_t),  intent(in) :: depv(*)
   integer(C_INT32_t), pointer    :: nseg

   call c_f_pointer(depv(1)%ptr, nseg)
   nseg = nseg + 1
   call printf("Segment 2", nseg)

   nullify(nseg)
   call ocrDbRelease(depv(1)%guid)

   rtn = NULL_GUID
end function segment2

function segment3(paramc, paramv, depc, depv) result(rtn) &
   BIND(C, name='segment_3')
   use ISO_C_BINDING
   implicit none
   integer(C_INTPTR_T) :: rtn
   integer(C_INT32_T), intent(in) :: paramc, depc
   integer(C_INT64_T), intent(in) :: paramv(*)
   type(ocrEdtDep_t),  intent(in) :: depv(*)
   integer(C_INT32_t), pointer    :: nseg

   call c_f_pointer(depv(1)%ptr, nseg)
   nseg = nseg + 1
   call printf("Segment 3", nseg)

   nullify(nseg)
   call ocrDbRelease(depv(1)%guid)

   rtn = NULL_GUID
end function segment3


function mainEdt(paramc, paramv, depc, depv) result(rtn) &
   BIND (c, name='mainEdt')
   use ISO_C_BINDING
   implicit none
   integer(C_INT32_T), intent(in) :: paramc, depc
   integer(C_INT64_T), intent(in) :: paramv(*)
   type(ocrEdtDep_t), intent(in) :: depv(*)
   integer(C_INTPTR_T) :: rtn
   integer(C_INTPTR_T) :: tpl_segment, edt_segment1, edt_segment2, edt_segment3
   integer(C_INTPTR_T) :: evt_output1, evt_output2, evt_output3
   integer(C_INTPTR_T) :: db_segment
   integer(C_INT8_T)   :: status
   type   (C_PTR)      :: addr
   type(ocrEdtDep_t)   :: dep_segment
   type(ocrEdt_t)      :: func
   integer(C_INT32_t), pointer :: nseg

   call ocrDbCreate(db_segment, addr, 1*4_i64, DB_PROP_NONE, NULL_GUID, NO_ALLOC)
   call c_f_pointer(addr, nseg)
   nseg = 0
   call printf("mainEDT", nseg)

   nullify(nseg)
   call ocrDbRelease(db_segment)

   !! segment 1
   !
   func%ptr = C_FUNLOC(segment1)
   call ocrEdtTemplateCreate(tpl_segment, func, 0, 1)

   call ocrEdtCreate(edt_segment1, tpl_segment, EDT_PARAM_DEF, NULL_PARAMV, &
                     EDT_PARAM_DEF, NULL_DEPV, EDT_PROP_NONE, NULL_GUID, evt_output1)
   call printf("mainEDT: segment_1 created.................................")

   !! segment 2
   !
   func%ptr = C_FUNLOC(segment2)
   call ocrEdtTemplateCreate(tpl_segment, func, 0, 2)

   call ocrEdtCreate(edt_segment2, tpl_segment, EDT_PARAM_DEF, NULL_PARAMV, &
                     EDT_PARAM_DEF, NULL_DEPV, EDT_PROP_NONE, NULL_GUID, evt_output2)
   status = ocrAddDependence(db_segment,  edt_segment2, 0, DB_MODE_RW)
   status = ocrAddDependence(evt_output1, edt_segment2, 1, DB_MODE_CONST)
   call printf("mainEDT: segment_2 created.................................")

   !! segment 3
   !
   func%ptr = C_FUNLOC(segment3)
   call ocrEdtTemplateCreate(tpl_segment, func, 0, 2)

   call ocrEdtCreate(edt_segment3, tpl_segment, EDT_PARAM_DEF, NULL_PARAMV, &
                     EDT_PARAM_DEF, NULL_DEPV, EDT_PROP_NONE, NULL_GUID, evt_output3)
   status = ocrAddDependence(db_segment,  edt_segment3, 0, DB_MODE_RW)
   status = ocrAddDependence(evt_output2, edt_segment3, 1, DB_MODE_CONST)
   call printf("mainEDT: segment_3 created.................................")

   !! Add data block to start execution of the segment chain
   !
   status = ocrAddDependence(db_segment, edt_segment1, 0, DB_MODE_RW)
   call printf("mainEDT: segment_1 runnable................................")

   call printf("mainEDT: shutting down.....................................")
   call sleep(3)

   !! finalize
   !
   call ocrShutdown()

   rtn = NULL_GUID
end function mainEdt

end module task_chain_mod
