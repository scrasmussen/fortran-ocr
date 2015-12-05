module ocr_fortran_types
use ISO_C_BINDING
implicit none

integer, parameter :: ocrGuid_k = C_INTPTR_T

integer, parameter :: u64 = C_INT64_T
integer, parameter :: s64 = C_INT64_T
integer, parameter :: u32 = C_INT32_T
integer, parameter :: s32 = C_INT32_T
integer, parameter :: u16 = C_INT16_T
integer, parameter :: s16 = C_INT16_T
integer, parameter :: u8 = C_INT8_T
integer, parameter :: s8 = C_INT8_T

integer(ocrGuid_k), parameter :: NULL_GUID = 0
integer(ocrGuid_k), parameter :: NULL_GUID_ARRAY(1) = 0
integer(u64), bind(c, name="OFP_NULL_PARAMV") :: NULL_PARAMV(1) = [-1]
integer(ocrGuid_k), bind(c, name="OFP_NULL_DEPV") :: NULL_DEPV(1) = [-1]
integer(u64), parameter :: C_NULL = 0

type, bind(C) :: ocrEdt_t
   type(C_FUNPTR) :: ptr
end type ocrEdt_t

type, bind(C) :: ocrEdtDep_t
   integer(ocrGuid_k) :: guid
   type(c_ptr) :: ptr
end type ocrEdtDep_t

type, bind(C) :: dbParams_t
  integer(u64) :: rank
  integer(u64) :: size
  integer(ocrGuid_k) ::  evt_finalize_guid
end type dbParams_t

end module ocr_fortran_types

module ocr_fortran
use ISO_C_BINDING
use ocr_fortran_types
implicit none

contains

  ! function shutdownFnc(paramc, paramv, depc, depv) result(rtn)
  !   integer(u32), intent(in) :: paramc, depc
  !   integer(u64), intent(in) :: paramv(*)
  !   type(ocrEdtDep_t), intent(in) :: depv(*)
  !   integer(ocrGuid_k) :: rtn

  !   rtn = NULL_GUID
  ! end function shutdownFnc

  function getArgv_i64(ptr, str, err) result(np)
    ! integer(C_INTPTR_T), intent(IN) :: ptr
    type(C_PTR), intent(IN) :: ptr
    character(len=*) :: str
    logical, intent(OUT) :: err
    integer(u64) :: np, start, end

    start = index(str, '-np')
    if (start .eq. 0) then
       err = .true.
       return
    else
       err = .false.
    end if

    start = start + 2
    do while (str(start+1:start+1) .eq. ' ')
       start = start + 1
    end do

    end = start
    do while (str(end+1:end+1) .ne. ' ')
       end = end + 1
    end do

    ! If string ends with the np value, a runtime error occurs
    read (str(start+1:end), '(I10)') np
    return
  end function getArgv_i64
  
  function spmd_create_finalize_edt(size) result(edt_finalize)
    integer(u64), intent(IN) :: size
    integer(ocrGuid_k) :: edt_finalize
    integer(u32) :: i
    call printf_str("entered")

    do i = 1, 10
       call print_i32(i)
    end do
       
    ! ocrEdtCreate(edt_finalize, 
    
  end function spmd_create_finalize_edt

  function create_spmd_edt(i, size, addr, edt_finalize) result (evt_start)
    integer(u64) :: i, size
    type(C_FUNPTR) :: addr
    integer(ocrGuid_k) :: edt_finalize
    integer(ocrGuid_k) :: evt_start
    
    
  end function create_spmd_edt

end module ocr_fortran
