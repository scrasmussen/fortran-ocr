!
! Author: Soren Rasmussen
! University of Oregon
!

module ocr_interfaces
  use ISO_C_BINDING
  use ocr_types

  !! These types should be moved to the ocr_types module?
  !

  !! Each process/thread will have a private data block of this type
  !
  type :: spmd_common_t
     integer(C_INT64_T) :: rank          ! the "rank" of this thread
     integer(C_INT64_T) :: size          ! the number of concurrent threads
  end type spmd_common_t

!! OCR interfaces
!
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
  integer(ocrGuid_k), intent(OUT) :: guid
  integer(C_INT32_T), intent(IN), value :: eventType
  integer(C_INT16_T), intent(IN), value :: flags
end subroutine ocrEventCreate

subroutine ocrEventSatisfy(eventGuid, dataGuid) &
  BIND (c, name='ocrEventSatisfy')
  use :: ISO_C_BINDING
  use :: ocr_types
  implicit none
  integer(ocrGuid_k), intent(IN), value :: eventGuid, dataGuid
end subroutine ocrEventSatisfy

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
  BIND (c, name='wocrEdtTemplateCreate')
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

end interface  ! OCR interfaces


!! Fortran wrappers for ocrAbort
!
interface ocrAbort

subroutine ocrAbort_u8(errorCode)
  use ISO_C_BINDING
  implicit none
  integer(C_INT8_T), intent(in), value :: errorCode
end subroutine ocrAbort_u8

subroutine ocrAbort_str(errorStr)
  character(len=*), intent(in) :: errorStr
end subroutine ocrAbort_str

end interface ocrAbort

!! Fortran wrappers for OCR getArgc and getArgv
!
interface

function getArgc(dbPtr) result(argNum) &
  BIND (c, name='wgetArgc')
  use ISO_C_BINDING
  implicit none
  type(C_PTR), intent(IN), value :: dbPtr
  integer(C_INT64_T) :: argNum
end function getArgc

function getArgv_i64(dbPtr, str, err) result(rtn)
  use ISO_C_BINDING
  implicit none
  type(C_PTR), intent(in), value :: dbPtr
  character(len=*), intent(in)   :: str
  logical, intent(out)           :: err
  integer(C_INT64_T) :: rtn
end function getArgv_i64

end interface  ! OCR getArgc and getArgv


!! interfaces to OFP SPMD procedures
!
interface

function create_spmd_edt(i, size, funcPtr, edt_finalize) result (rtn) &
  BIND (c, name='create_spmd_edt')
  use :: ISO_C_BINDING
  use :: ocr_types
  implicit none
  integer(C_INT64_t), intent(in), value :: i, size
  type(C_FUNPTR),     intent(in), value :: funcPtr
  integer(ocrGuid_k), intent(in), value :: edt_finalize
  integer(ocrGuid_k) :: rtn
end function create_spmd_edt

function spmd_create_finalize_edt(size) result (rtn) &
  BIND (c, name='spmd_create_finalize_edt')
  use :: ISO_C_BINDING
  use :: ocr_types
  implicit none
  integer(C_INT64_t), intent(in), value :: size
  integer(ocrGuid_k) :: rtn
end function spmd_create_finalize_edt

end interface  ! OFP SPMD procedures


!! interfaces to OCR PRINTF
!
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

