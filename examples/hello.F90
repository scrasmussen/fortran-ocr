! program main
!   print *, "Hello, world!"
! end program

module helloMod
  use, intrinsic :: ISO_C_BINDING
  use :: ocr_interfaces
  implicit none

contains

function hello_Edt(paramc, paramv, depc, depv) result(rtn)
  implicit none
  integer(C_INTPTR_T) :: rtn
  integer(C_INT32_T), intent(in) :: paramc, depc
  integer(C_INT64_T), intent(in) :: paramv(*)
  type(ocrEdtDep_t),  intent(in) :: depv(*)

  call printf("Hellno, world!")

  rtn = NULL_GUID
end function hello_Edt


function mainEdt(paramc, paramv, depc, depv) result(returnGuid) &
     BIND (c, name='mainEdt')
  implicit none
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
  call printf("cos(0)", x)

  !! subroutine call
  !
  ptr%ptr = C_FUNLOC(hello_Edt)
  call ocrEdtTemplateCreate(templateGuid, ptr, 0, 0)

  outputEvent = NULL_GUID
  call ocrEdtCreate(edtGuid, templateGuid, EDT_PARAM_DEF, NULL_PARAMV, &
       EDT_PARAM_DEF, NULL_DEPV, EDT_PROP_NONE, NULL_GUID, outputEvent)

  !! finalize
  !
  call ocrShutdown()

  returnGuid = NULL_GUID
end function mainEdt

end module helloMod
