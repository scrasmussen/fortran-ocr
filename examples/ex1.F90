module m
  use ISO_C_BINDING
  use :: ocr_types
  use :: ocr_interfaces, only : ocrShutdown, printf_str, printf_i
  use :: ocr_interfaces, only : ocrEdtTemplateCreate_internal
  use :: ocr_interfaces, only : ocrEdtCreate
  implicit none

contains

function appEdt(paramc, paramv, depc, depv) result(returnGuid) BIND(c)
  use ISO_C_BINDING
  use ocr_types
  use :: ocr_interfaces, only : printf_str
  implicit none
  integer(C_INTPTR_T) :: returnGuid
  integer(C_INT32_T), intent(in) :: paramc
  integer(C_INT64_T), intent(in) :: paramv
  integer(C_INT32_T), intent(in) :: depc
  type(ocrEdtDep_t), intent(in) :: depv
  call printf_str("Hello from EDT" // C_NULL_CHAR)
  returnGuid = 0 ! C_NULL_PTR
end function appEdt


function mainEdt(paramc, paramv, depc, depv) result(returnGuid) &
     BIND (c, name='mainEdt')
  integer(C_INTPTR_T) :: returnGuid
  integer(C_INT32_T), intent(in) :: paramc
  integer(C_INT64_T), intent(in) :: paramv
  integer(C_INT32_T), intent(in) :: depc
  type(ocrEdtDep_t), intent(in) :: depv
  integer(C_INT32_T) :: fparamc
  integer(C_INT32_T) :: fdepc
  integer(C_INTPTR_T) :: templateGuid
  type(C_PTR) :: edtGuid
  type(ocrEdt_t) :: ptr 
  type(C_PTR) :: outputEvent

  ptr%ptr = C_FUNLOC(appEdt)
  fparamc = 0
  fdepc = 0
  ! call printf_str("Hells Angels" // C_NULL_CHAR)
  ! call printf_i("ptr==%p" // C_NULL_CHAR, C_NULL_PTR)

  call ocrEdtTemplateCreate_internal(templateGuid, ptr, fparamc, fdepc, &
       C_NULL_CHAR)

  outputEvent = C_NULL_PTR
  call ocrEdtCreate(edtGuid, templateGuid, EDT_PARAM_DEF, C_NULL_PTR, &
       EDT_PARAM_DEF, C_NULL_PTR, EDT_PROP_NONE, NULL_GUID, outputEvent)


  call ocrShutdown()

  returnGuid = 0 ! C_NULL_PTR
end function mainEdt

end module m

! interface
!    subroutine test() BIND(c)
!      use ISO_C_BINDING
!      use :: ocr_interfaces, only : printf_str
!    end subroutine test
! end interface

