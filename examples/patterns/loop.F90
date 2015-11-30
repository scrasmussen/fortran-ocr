!------------------------------------------------------------------------------
!
! Loop pattern
!
!   This example shows how EDT can be created to for a loop construct.  It
!   uses the follow parameters to pass information to each succeeding iteration:
!
!       params(1) = 1           ! iteration counter
!       params(2) = 13          ! iteration limit
!       params(3) = tpl_loop    ! template for the EDT of the next iteration
!       params(4) = evt_end     ! EDT to chain to when loop limit reached
!
!------------------------------------------------------------------------------
module loop_mod
  use :: ocr_interfaces
  implicit none

  integer, parameter :: N_PARAMS = 4

contains

function endEDT(paramc, paramv, depc, depv) result(rtn) &
   BIND(C, name='endEDT')
   use ISO_C_BINDING
   implicit none
   integer(C_INTPTR_T) :: rtn
   integer(C_INT32_T), intent(in) :: paramc, depc
   integer(C_INT64_T), intent(in) :: paramv(*)
   type(ocrEdtDep_t),  intent(in) :: depv(*)

   call printf("endEDT : shutting down ....................................")

   !! finalize
   !
   call ocrShutdown()

   rtn = NULL_GUID
end function endEDT

function loopEDT(paramc, paramv, depc, depv) result(rtn) &
   BIND(C, name='loopEDT')
   use ISO_C_BINDING
   implicit none
   integer(C_INTPTR_T) :: rtn
   integer(C_INT32_T), intent(in) :: paramc, depc
   integer(C_INT64_T), intent(in) :: paramv(*)
   type(ocrEdtDep_t),  intent(in) :: depv(*)
   integer(C_INTPTR_T) :: edt_loop, evt_output
   integer(C_INT64_T)  :: params(N_PARAMS)

   if (paramv(1) == 1) then
      call printf("Loop   limit", paramv(2))
   end if
   call printf("Loop counter", paramv(1))

   !! Do some work
   !

   !! Create next loop iteration (or finish)
   !
   params(1) = paramv(1) + 1
   params(2) = paramv(2)
   params(3) = paramv(3)
   params(4) = paramv(4)

   if (params(1) > params(2)) then
      call printf("loopEDT: finishing ........................................")
      call ocrEventSatisfy(paramv(4), NULL_GUID)
   else
      call ocrEdtCreate(edt_loop, paramv(3), EDT_PARAM_DEF, params, &
                        EDT_PARAM_DEF, NULL_DEPV, EDT_PROP_NONE, NULL_GUID, evt_output)
   end if

   rtn = NULL_GUID
end function loopEDT

function mainEdt(paramc, paramv, depc, depv) result(rtn) &
   BIND (c, name='mainEdt')
   use ISO_C_BINDING
   implicit none
   integer(C_INT32_T), intent(in) :: paramc, depc
   integer(C_INT64_T), intent(in) :: paramv(*)
   type(ocrEdtDep_t), intent(in) :: depv(*)
   integer(C_INTPTR_T) :: rtn
   integer(C_INTPTR_T) :: tpl_loop, edt_loop, evt_loop_output
   integer(C_INTPTR_T) :: tpl_end,  edt_end,  evt_end_output, evt_end
   integer(C_INT64_T)  :: params(N_PARAMS)
   integer(C_INT8_T)   :: status
   type(ocrEdt_t)      :: func

   !! Create end event
   !
   call ocrEventCreate(evt_end, OCR_EVENT_ONCE_T, EVT_PROP_NONE)

   !! loop EDT setup
   !
   func%ptr = C_FUNLOC(loopEDT)
   call ocrEdtTemplateCreate(tpl_loop, func, N_PARAMS, 0)

   params(1) = 1           ! iteration counter
   params(2) = 13          ! iteration limit
   params(3) = tpl_loop
   params(4) = evt_end

   call ocrEdtCreate(edt_loop, tpl_loop, EDT_PARAM_DEF, params, EDT_PARAM_DEF, &
                     NULL_DEPV, EDT_PROP_NONE, NULL_GUID, evt_loop_output)
   call printf("mainEDT: loopEDT created ..................................")

   !! end EDT setup
   !
   func%ptr = C_FUNLOC(endEDT)
   call ocrEdtTemplateCreate(tpl_end, func, 0, 1)

   call ocrEdtCreate(edt_end, tpl_end, EDT_PARAM_DEF, NULL_PARAMV, EDT_PARAM_DEF, &
                     NULL_DEPV, EDT_PROP_NONE, NULL_GUID, evt_end_output)
   status = ocrAddDependence(evt_end, edt_end, 0, DB_MODE_CONST)

   call printf("mainEDT: finishing ........................................")

   rtn = NULL_GUID
end function mainEdt

end module loop_mod
