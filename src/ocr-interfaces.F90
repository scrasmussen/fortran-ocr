!
! Author: Soren Rasmussen
! University of Oregon
!

module ocr_interfaces
use ISO_C_BINDING
use :: ocr_types

interface

subroutine ocrAbort(errorCode) &
     BIND (c, name='wocrAbort')
  use :: ISO_C_BINDING
  use :: ocr_types
  implicit none
  integer(u8), intent(IN) :: errorCode
end subroutine ocrAbort

function ocrAddDependence(source, destination, slot, mode) result(status) &
     BIND (c, name='wocrAddDependence')
  use :: ISO_C_BINDING
  use :: ocr_types
  implicit none
  integer(ocrGuid_k), intent(IN), value :: source, destination
  integer(u32), intent(IN), value :: slot, mode
  integer(u8) :: status
end function ocrAddDependence

subroutine ocrDbCreate(db, addr, len, flags, affinity, allocator) &
     BIND (c, name='wocrDbCreate')
  use :: ISO_C_BINDING
  use :: ocr_types
  implicit none
  integer(ocrGuid_k), intent(OUT) :: db
  type(C_PTR), intent(OUT)  :: addr
  integer(u64), intent(IN), value :: len
  integer(u16), intent(IN), value :: flags
  integer(ocrGuid_k), intent(INOUT) :: affinity
  integer(u32), intent(IN), value :: allocator
end subroutine ocrDbCreate

subroutine ocrEdtCreate(guid, templateGuid, paramc, paramv, depc, depv, &
     flags, affinity, outputEvent) &
     BIND (c, name='wocrEdtCreate')
  use :: ISO_C_BINDING
  use :: ocr_types
  implicit none
  integer(ocrGuid_k), intent(OUT) :: guid
  integer(ocrGuid_k), intent(IN), value :: templateGuid
  integer(u32), intent(IN), value :: paramc
  integer(u64), intent(IN), dimension(*) :: paramv
  integer(u32), intent(IN), value :: depc
  integer(ocrGuid_k), intent(IN), dimension(*) :: depv
  integer(u16), intent(IN), value :: flags
  integer(ocrGuid_k), intent(IN), value :: affinity
  integer(ocrGuid_k), intent(INOUT) :: outputEvent
end subroutine ocrEdtCreate

! ocrEdtTemplateCreate, _internal added so symbols match
subroutine ocrEdtTemplateCreate(guid, funcPtr, paramc, depc, &
     funcName) BIND (c, name='wocrEdtTemplateCreate_internal')
  use :: ISO_C_BINDING
  use :: ocr_types
  use :: ocr_fortran_types
  implicit none
  integer(ocrGuid_k), intent(OUT) :: guid
  type(ocrEdt_t), intent(IN), value  :: funcPtr
  integer(u32), intent(IN), value :: paramc
  integer(u32), intent(IN), value :: depc
  character(C_CHAR), intent(IN), value :: funcName
end subroutine ocrEdtTemplateCreate

subroutine ocrEventCreate(guid, eventType, flags) &
     BIND (c, name='wocrEventCreate')
  use :: ISO_C_BINDING
  use :: ocr_types
  implicit none
  integer(ocrGuid_k), intent(OUT) :: guid
  integer(u32), intent(IN), value :: eventType
  integer(u16), intent(IN), value :: flags
end subroutine ocrEventCreate

subroutine ocrEventSatisfy(eventGuid, dataGuid)  &
     BIND (c, name='wocrEventSatisfy')
  use :: ISO_C_BINDING
  use :: ocr_types
  implicit none
  integer(ocrGuid_k), intent(IN), value :: eventGuid
  integer(ocrGuid_k), intent(IN), value :: dataGuid
end subroutine ocrEventSatisfy

subroutine ocrShutdown() &
  BIND (c, name='wocrShutdown')
  implicit none
end subroutine ocrShutdown

function getArgc(dbPtr) result(argNum) &
     BIND (c, name='wgetArgc')
  use ISO_C_BINDING
  use :: ocr_types
  type(C_PTR), intent(IN) :: dbPtr
  integer(u64) :: argNum
end function getArgc

! function mainEdt(paramc, paramv, depc, depv) result(returnGuid) &
!      BIND (c, name='wmainEdt')
!   use ISO_C_BINDING
!   use :: ocr_types
!   implicit none
!   integer(ocrGuid_k) :: returnGuid
!   integer(u32), intent(IN) :: paramc
!   integer(u64), intent(IN) :: paramv
!   ! type(C_PTR), intent(IN) :: paramv
!   integer(u32), intent(IN) :: depc
!   type(ocrEdtDep_t), intent(IN) :: depv
! end function mainEdt

subroutine printf_str(str) bind(C, name="printf_str")
  character(len=1) :: str(*)
end subroutine printf_str

subroutine printf_i(str, i) bind(C, name="printf_i")
  use ISO_C_BINDING
  character(len=1) :: str(*)
  integer(C_INT64_t), value :: i
end subroutine printf_i

subroutine printf_f(str, i) bind(C, name="printf_f")
  use ISO_C_BINDING
  character(len=1) :: str(*)
  real(C_FLOAT), value :: i
end subroutine printf_f

subroutine printf_i32(str, i) bind(C, name="printf_i32")
  use ISO_C_BINDING
  character(len=1) :: str(*)
  integer(C_INT32_t), value :: i
end subroutine printf_i32

subroutine printf_p(str, i) bind(C, name="printf_p")
  use ISO_C_BINDING
  character(len=1) :: str(*)
  type(C_PTR), value :: i
end subroutine printf_p

subroutine printf_pi(str, i) bind(C, name="printf_pi")
  use ISO_C_BINDING
  use :: ocr_types
  character(len=1) :: str(*)
  integer(ocrGuid_k), value :: i
end subroutine printf_pi

end interface
end module ocr_interfaces

