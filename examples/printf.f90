!------------------------------------------------------------------------------
!
! This file contains Fortran wrapper functions that trim Fortran character
! strings and appends C_NULL_CHAR before printing by the C printf functions
! (in wprintf.c) and eventually by OCR's PRINTF facility.
!
!------------------------------------------------------------------------------

subroutine printf_str(str)
  use, intrinsic :: ISO_C_BINDING, only : C_NULL_CHAR
  interface
    subroutine wprintf_str(str) bind(C, name="wprintf_str")
      character(len=1) :: str(*)
    end subroutine wprintf_str
  end interface
  character(len=*) :: str

  call wprintf_str( TRIM(str)//C_NULL_CHAR )

end subroutine printf_str

subroutine printf_i32(str, i)
  use, intrinsic :: ISO_C_BINDING, only : C_NULL_CHAR, C_INT32_T
  interface
    subroutine wprintf_i32(str, i) bind(C, name="wprintf_i32")
      use ISO_C_BINDING
      character(len=1) :: str(*)
      integer(C_INT32_t), value :: i
    end subroutine wprintf_i32
  end interface
  character(len=*) :: str
  integer(C_INT32_t), value :: i

  call wprintf_i32( TRIM(str)//C_NULL_CHAR,  i)

end subroutine printf_i32

subroutine printf_i64(str, i)
  use, intrinsic :: ISO_C_BINDING, only : C_NULL_CHAR, C_INT64_T
  interface
    subroutine wprintf_i64(str, i) bind(C, name="wprintf_i64")
      use ISO_C_BINDING
      character(len=1) :: str(*)
      integer(C_INT64_t), value :: i
    end subroutine wprintf_i64
  end interface
  character(len=*) :: str
  integer(C_INT64_t), value :: i

  call wprintf_i64( TRIM(str)//C_NULL_CHAR,  i)

end subroutine printf_i64

subroutine printf_r(str, x)
  use, intrinsic :: ISO_C_BINDING, only : C_NULL_CHAR
  interface
    subroutine wprintf_f(str, x) bind(C, name="wprintf_f")
      use ISO_C_BINDING
      character(len=1) :: str(*)
      real(C_FLOAT), value :: x
    end subroutine wprintf_f
  end interface
  character(len=*) :: str
  real, value :: x

  call wprintf_f( TRIM(str)//C_NULL_CHAR,  x)

end subroutine printf_r

subroutine printf_p(str, p)
  use, intrinsic :: ISO_C_BINDING, only : C_NULL_CHAR, C_PTR
  interface
    subroutine wprintf_p(str, p) bind(C, name="wprintf_p")
      use ISO_C_BINDING
      character(len=1) :: str(*)
      type(C_PTR), value :: p
    end subroutine wprintf_p
  end interface
  character(len=*) :: str
  type(C_PTR), value :: p

  call wprintf_p( TRIM(str)//C_NULL_CHAR,  p)

end subroutine printf_p

