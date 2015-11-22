!
! Author: Soren Rasmussen
! University of Oregon
!

module ocr_interfaces
use ISO_C_BINDING
use ocr_types

interface

function ocrAddDependence(source, destination, slot, mode) result(status) &
     BIND (c, name='wocrAddDependence')
  use :: ISO_C_BINDING
  use :: ocr_types
  implicit none
  integer(C_INTPTR_T), intent(IN), value :: source, destination
  integer(C_INT32_T), intent(IN), value :: slot, mode
  integer(C_INT8_T) :: status
end function ocrAddDependence

subroutine ocrEventCreate(guid, eventType, flags) &
     BIND (c, name='wocrEventCreate')
  use :: ISO_C_BINDING
  use :: ocr_types
  implicit none
  integer(C_INTPTR_T), intent(OUT) :: guid
  integer(C_INT32_T), intent(IN), value :: eventType
  integer(C_INT16_T), intent(IN), value :: flags
end subroutine ocrEventCreate

subroutine ocrDbCreate(db, addr, len, flags, affinity, allocator) &
     BIND (c, name='wocrDbCreate')
  use :: ISO_C_BINDING
  use :: ocr_types
  implicit none
  integer(C_INTPTR_T), intent(OUT) :: db
  type(C_PTR), intent(OUT)  :: addr
  integer(C_INT64_T), intent(IN), value :: len
  integer(C_INT16_T), intent(IN), value :: flags
  integer(C_INTPTR_T), intent(IN), value :: affinity  ! NOTE standard says INOUT variable?
  integer(C_INT32_T), intent(IN), value :: allocator
end subroutine ocrDbCreate

subroutine ocrDbRelease(db) BIND(c, name='ocrDbRelease')
  use :: ISO_C_BINDING
  implicit none
  integer(C_INTPTR_T), intent(in), value :: db
end subroutine ocrDbRelease

subroutine ocrEdtCreate(guid, templateGuid, paramc, paramv, depc, depv, &
     properties, affinity, outputEvent) &
     BIND (c, name='wocrEdtCreate')
  use :: ISO_C_BINDING
  use :: ocr_types
  implicit none
  integer(C_INTPTR_T), intent(OUT) :: guid
  integer(C_INTPTR_T), intent(IN), value :: templateGuid
  integer(C_INT32_T), intent(IN), value :: paramc
  integer(C_INT64_T), intent(IN), dimension(*) :: paramv
  integer(C_INT32_T), intent(IN), value :: depc
  integer(C_INTPTR_T), intent(IN), dimension(*) :: depv
  integer(C_INT16_T), intent(IN), value :: properties
  integer(C_INTPTR_T), intent(IN), value :: affinity
  integer(C_INTPTR_T), intent(INOUT) :: outputEvent
end subroutine ocrEdtCreate

subroutine ocrEdtTemplateCreate(guid, funcPtr, paramc, depc) &
     BIND (C, name='wocrEdtTemplateCreate')
  use :: ISO_C_BINDING
  use :: ocr_types
  implicit none
  integer(C_INTPTR_T), intent(OUT) :: guid
  type(ocrEdt_t), intent(IN), value  :: funcPtr
  integer(C_INT32_T), intent(IN), value :: paramc, depc
end subroutine ocrEdtTemplateCreate

subroutine ocrShutdown() &
  BIND (c, name='wocrShutdown')
  implicit none
end subroutine ocrShutdown

function getArgc(dbPtr) result(argNum) &
     BIND (c, name='wgetArgc')
  use ISO_C_BINDING
  type(C_PTR), intent(IN) :: dbPtr
  integer(C_INT64_T) :: argNum
end function getArgc

end interface


interface printf

subroutine printf_str(str)
  character(len=*) :: str
end subroutine printf_str

subroutine printf_i32(str, i)
  use ISO_C_BINDING
  character(len=*) :: str
  integer(C_INT32_t), value :: i
end subroutine printf_i32

subroutine printf_i64(str, i)
  use ISO_C_BINDING
  character(len=*) :: str
  integer(C_INT64_t), value :: i
end subroutine printf_i64

subroutine printf_r(str, x)
  character(len=*) :: str
  real, value :: x
end subroutine printf_r

subroutine printf_p(str, p)
  use ISO_C_BINDING
  character(len=1) :: str(*)
  type(C_PTR), value :: p
end subroutine printf_p

end interface printf

end module ocr_interfaces

