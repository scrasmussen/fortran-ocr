! program main
!   call hello
! end program

module helloMod
  use, intrinsic :: ISO_C_BINDING
  use :: ocr_types
  use :: ocr_interfaces
  implicit none

  integer(C_INTPTR_T), dimension(1), parameter :: C_NULL_ARRAY = 0
  integer(C_INT64_T),  dimension(1), parameter :: C64_NULL_ARRAY = 0

contains

function hello_Edt(paramc, paramv, depc, depv) result(rtn)
  use ISO_C_BINDING
  use ocr_types
  use :: ocr_interfaces, only : printf_str
  implicit none
  integer(C_INTPTR_T) :: rtn
  integer(C_INT32_T), intent(in) :: paramc, depc
  integer(C_INT64_T), intent(in) :: paramv(*)
  type(ocrEdtDep_t),  intent(in) :: depv(*)

  call printf_str("Hellno, world!" // C_NULL_CHAR)

  rtn = NULL_GUID
end function hello_Edt


function mainEdt(paramc, paramv, depc, depv) result(returnGuid) &
     BIND (c, name='mainEdt')
  integer(C_INTPTR_T) :: returnGuid
  integer(C_INT32_T), intent(in) :: paramc, depc
  integer(C_INT64_T), intent(in) :: paramv(*)
  type(ocrEdtDep_t), intent(in) :: depv(*)
  integer(C_INTPTR_T) :: templateGuid, edtGuid
  type(ocrEdt_t) :: ptr 
  integer(C_INTPTR_T) :: outputEvent

  !! main variables
  !
  real :: x

  !! main exec statements
  !
  x = cos(0.0)
  call printf_f("cos(0)", x)

  !! subroutine call
  !
  ptr%ptr = C_FUNLOC(hello_Edt)
  call ocrEdtTemplateCreate(templateGuid, ptr, 0, 0, C_NULL_CHAR)

  outputEvent = NULL_GUID
  call ocrEdtCreate(edtGuid, templateGuid, EDT_PARAM_DEF, C64_NULL_ARRAY, &
       EDT_PARAM_DEF, C_NULL_ARRAY, EDT_PROP_NONE, NULL_GUID, outputEvent)

  !! finalize
  !
  call ocrShutdown()

  returnGuid = 0 ! C_NULL_PTR
end function mainEdt

end module helloMod
