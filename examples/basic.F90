function mainEdt(paramc, paramv, depc, depv) result(returnGuid) &
   BIND (c, name='mainEdt')
   use :: ocr_types  !, only : s64
   use :: ocr_interfaces, only : ocrShutdown, printf_str, printf_i
   implicit none
   type(ocrGuid_t) :: returnGuid
   type(u32), intent(in) :: paramc
   type(u64), intent(in) :: paramv
   type(u32), intent(in) :: depc
   type(ocrEdtDep_t), intent(in) :: depv

   call printf_str("Hellno" // C_NULL_CHAR)
   call printf_i("Hellyes" // C_NULL_CHAR, 13)

   call ocrShutdown()

   returnGuid%guid = C_NULL_PTR

 end function mainEdt
