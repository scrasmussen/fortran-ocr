module m
  use ISO_C_BINDING
  use :: ocr_fortran_types
  use :: ocr_types
  use :: ocr_interfaces
  implicit none

contains

function appEdt(paramc, paramv, depc, depv) result(returnGuid) BIND(c)
  use ISO_C_BINDING
  use ocr_types
  use :: ocr_interfaces, only : printf_str
  implicit none
  integer(ocrGuid_k) :: returnGuid
  integer(u32), intent(in) :: paramc
  integer(u64), intent(in) :: paramv
  integer(u32), intent(in) :: depc
  type(ocrEdtDep_t), intent(in) :: depv
  call printf_str("Hello from EDT" // C_NULL_CHAR)
  returnGuid = 0
end function appEdt


function mainEdt(paramc, paramv, depc, depv) result(returnGuid) &
     BIND (c, name='mainEdt')
  implicit none
  integer(ocrGuid_k) :: returnGuid
  integer(u32), intent(in) :: paramc
  integer(u64), intent(in) :: paramv
  integer(u32), intent(in) :: depc
  type(ocrEdtDep_t), intent(in) :: depv
  integer(u32) :: fparamc
  integer(u32) :: fdepc
  integer(ocrGuid_k) :: templateGuid, edtGuid, outputEvent
  type(ocrEdt_t) :: ptr 

  ptr%ptr = C_FUNLOC(appEdt)
  fparamc = 0
  fdepc = 0
  ! call printf_i("ptr==%p" // C_NULL_CHAR, C_NULL_PTR)

  call ocrEdtTemplateCreate(templateGuid, ptr, fparamc, fdepc, &
       C_NULL_CHAR)

  outputEvent = C_NULL
  call ocrEdtCreate(edtGuid, templateGuid, EDT_PARAM_DEF, NULL_PARAMV, &
       EDT_PARAM_DEF, NULL_DEPV, EDT_PROP_NONE, NULL_GUID, outputEvent)


  call ocrShutdown()

  returnGuid = 0 ! C_NULL_PTR
end function mainEdt

end module m
