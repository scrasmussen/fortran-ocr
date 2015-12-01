function mainEdt(paramc, paramv, depc, depv) result(returnGuid) &
   BIND (c, name='mainEdt')
   use :: ocr_interfaces
   implicit none
   integer(C_INTPTR_T) :: returnGuid
   integer(C_INT32_T), intent(in) :: paramc, depc
   integer(C_INT64_T), intent(in) :: paramv(*)
   type(ocrEdtDep_t),  intent(in) :: depv(*)

   call printf("Hellno")
   call printf("An integer", 13)

   call ocrShutdown()

   returnGuid = NULL_GUID

 end function mainEdt
