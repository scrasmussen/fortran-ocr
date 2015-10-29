module ocr_interfaces
interface

subroutine ocrDbCreate(db, addr, len, flags, affinity, allocator) &
     BIND (c, name='wocrDbCreate')
  use :: ISO_C_BINDING
  use :: ocr_types
  implicit none
  ! type(C_PTR), intent(OUT) :: db
  integer(C_INTPTR_T), intent(OUT) :: db
  ! integer(C_INTPTR_T), intent(OUT)  :: addr
  type(C_PTR), intent(OUT)  :: addr
  integer(C_INT64_T), intent(IN), value :: len
  integer(C_INT16_T), intent(IN), value :: flags
  integer(C_INTPTR_T), intent(INOUT) :: affinity
  integer(C_INT32_T), intent(IN), value :: allocator
end subroutine ocrDbCreate

subroutine ocrEdtCreate(guid, templateGuid, paramc, paramv, depc, depv, &
     properties, affinity, outputEvent) &
     BIND (c, name='wocrEdtCreate')
  use :: ISO_C_BINDING
  use :: ocr_types
  implicit none
  type(C_PTR), intent(OUT) :: guid
  integer(C_INTPTR_T), intent(IN), value :: templateGuid
  integer(C_INT32_T), intent(IN), value :: paramc
  ! type(C_PTR), intent(IN), value  :: paramv
  integer(C_INT64_T), intent(IN), dimension(*) :: paramv
  integer(C_INT32_T), intent(IN), value :: depc
  integer(C_INTPTR_T), intent(IN), dimension(*) :: depv
  ! type(C_PTR), intent(IN), dimension(*) :: depv
  integer(C_INT16_T), intent(IN), value :: properties
  integer(C_INTPTR_T), intent(IN), value :: affinity
  type(C_PTR), intent(INOUT) :: outputEvent
end subroutine ocrEdtCreate

! ocrEdtTemplateCreate, _internal added so symbols match
subroutine ocrEdtTemplateCreate(guid, funcPtr, paramc, depc, &
     funcName) BIND (c, name='wocrEdtTemplateCreate_internal')
  use :: ISO_C_BINDING
  use :: ocr_types
  implicit none
  integer(C_INTPTR_T), intent(OUT) :: guid
  type(ocrEdt_t), intent(IN), value  :: funcPtr
  integer(C_INT32_T), intent(IN), value :: paramc
  integer(C_INT32_T), intent(IN), value :: depc
  character(C_CHAR), intent(IN), value :: funcName
end subroutine ocrEdtTemplateCreate

subroutine ocrShutdown() &
  BIND (c, name='wocrShutdown')
  implicit none
end subroutine ocrShutdown

function mainEdt(paramc, paramv, depc, depv) result(returnGuid) &
     BIND (c, name='wmainEdt')
  use ISO_C_BINDING
  use :: ocr_types
  implicit none
  integer(C_INTPTR_T) :: returnGuid
  integer(C_INT32_T), intent(IN) :: paramc
  integer(C_INT64_T), intent(IN) :: paramv
  ! type(C_PTR), intent(IN) :: paramv
  integer(C_INT32_T), intent(IN) :: depc
  type(ocrEdtDep_t), intent(IN) :: depv
end function mainEdt

subroutine printf_str(str) bind(C, name="printf_str")
  character(len=1) :: str(*)
end subroutine printf_str

subroutine printf_i(str, i) bind(C, name="printf_i")
  use ISO_C_BINDING
  character(len=1) :: str(*)
  integer(C_INT64_t), value :: i
end subroutine printf_i

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
  character(len=1) :: str(*)
  integer(C_INTPTR_T), value :: i
end subroutine printf_pi

end interface

end module ocr_interfaces

