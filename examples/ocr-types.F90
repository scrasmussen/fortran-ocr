!
! This file is used to define Fortran types that have equivalent size as the
! OCR types. This allows the C OCR types to be declared in Fortran
!

module OCR_types
  use :: ISO_C_BINDING
  implicit none

  include "ocr-type-size.h"

  ! type, bind(C) :: s64
  !    integer(C_INT64_T) :: i
  ! end type s64

  ! type, bind(C) :: s32
  !    integer(C_INT32_T) :: i
  ! end type s32

  ! type, bind(C) :: s8
  !    integer(C_INT8_T) :: i
  ! end type s8

  ! type, bind(C) :: u64
  !    integer(C_INT64_T) :: i
  ! end type u64

  ! type, bind(C) :: u32
  !    integer(C_INT32_t) :: i
  ! end type u32

  ! type, bind(C) :: u8
  !    integer(C_INT8_T) :: i
  ! end type u8

  ! type, bind(C) :: ocrGuid_t
  !    integer(C_INTPTR_T) :: guid
  ! end type ocrGuid_t
  
  type, bind(C) :: ocrEdtDep_t
     integer(C_INTPTR_T) :: guid
     type(c_ptr) :: ptr
  end type ocrEdtDep_t

  ! In ocr-types.h this is more explicitly defined to be a function pointer
  ! to a function with arguments of type u32, u64*, depc, ocrEdtDep_t 
  type, bind(C) :: ocrEdt_t
     type(C_FUNPTR) :: ptr
  end type ocrEdt_t

  ! type(C_INTPTR_T), parameter :: NULL_GUID%guid = C_NULL_PTR
  integer(C_INTPTR_T), parameter :: NULL_GUID=0

end module OCR_types
