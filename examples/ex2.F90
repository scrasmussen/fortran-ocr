module m
  use ISO_C_BINDING
  use :: ocr_types
  use :: ocr_interfaces, only : ocrShutdown, printf_str, printf_i, printf_p
  use :: ocr_interfaces, only : ocrEdtTemplateCreate, printf_pi, printf_i32
  use :: ocr_interfaces, only : ocrEdtCreate, ocrDbCreate
  use :: test_ocr_types
  implicit none

contains

function appEdt(paramc, paramv, depc, depv) result(returnGuid) BIND(c)
  use ISO_C_BINDING
  use ocr_types
  use :: ocr_interfaces, only : printf_str
  implicit none
  integer(C_INTPTR_T) :: returnGuid, dataguid
  integer(C_INT32_T), intent(in) :: paramc
  integer(C_INT64_T), intent(in), dimension(*) :: paramv
  integer(C_INT32_T), intent(in) :: depc
  type(ocrEdtDep_t), intent(in), dimension(*), target :: depv
  type(C_PTR) :: data
  integer(C_INT64_T), pointer :: test(:)
  integer(C_INT64_T) :: i, val
  integer(C_INT64_T), pointer :: a(:)

  data = depv(1)%ptr
  dataguid = depv(1)%guid
  call printf_i("guid address is" // C_NULL_CHAR , dataguid)
  call printf_p("data address is" // C_NULL_CHAR , data)
  call printf_i("array size = %u" // C_NULL_CHAR, paramv(1))

  i = paramv(2)
  call printf_i("array[i] where i=" // C_NULL_CHAR , i)
  call c_f_pointer(data, a, [100]);
  call printf_i("is " // C_NULL_CHAR , a(i) )

  ! call printf_str("Hello from EDT" // C_NULL_CHAR)
  call ocrShutdown()
  returnGuid = 0 ! C_NULL_PTR
end function appEdt


function mainEdt(paramc, paramv, depc, depv) result(returnGuid) &
     BIND (c, name='mainEdt')
  integer(C_INTPTR_T) :: returnGuid
  integer(C_INT32_T), intent(in) :: paramc
  integer(C_INT64_T), intent(in) :: paramv
  integer(C_INT32_T), intent(in) :: depc
  type(ocrEdtDep_t), intent(in) :: depv
  ! params
  integer(C_INT32_T) :: fparamc
  integer(C_INT32_T) :: fdepc
  integer(C_INTPTR_T) :: templateGuid
  type(C_PTR) :: edtGuid
  type(ocrEdt_t) :: ptr 
  type(C_PTR) :: outputEvent
  integer(C_INT64_T) :: arraySize, index
  integer(C_INT64_T), dimension(2) :: nparamv
  integer(C_INT64_T) :: u64
  integer(C_INT32_T) :: allocator
  ! integer(C_INT64_T) :: dataArray
  type(C_PTR) :: dataArray
  integer(C_INT16_T) :: flags
  integer(C_INTPTR_T) :: dataGuid
  integer(C_INTPTR_T), dimension(1) :: dataGuidArray
  ! type(C_PTR) :: dataGuid
  integer(C_INTPTR_T) :: affinity
  integer(C_INT64_T), pointer :: a(:)
  integer(C_INT64_T) :: i
  type(C_PTR) :: locval
  type(C_PTR), pointer :: test


  ! create template for appEdt
  ptr%ptr = C_FUNLOC(appEdt)
  fparamc = 2
  fdepc = EDT_PARAM_UNK
  call ocrEdtTemplateCreate(templateGuid, ptr, fparamc, fdepc, &
       C_NULL_CHAR)

  ! setup argument vector
  arraySize = 100_C_INT
  index = 2_C_INT !1!cause fortran
  nparamv(1) = arraySize
  nparamv(2) = index

  ! create datablock
  flags = 0
  affinity = NULL_GUID
  allocator = NO_ALLOC

  call ocrDbCreate(dataGuid, dataArray,sizeof(u64)*arraySize, flags, affinity, allocator)

  call printf_i("===dataGuid" // C_NULL_CHAR , dataGuid)
  ! Set element at 'index'
  call c_f_pointer(dataArray, a, [arraySize]);
  call  printf_p("===addr = " // C_NULL_CHAR, dataArray)
  call  printf_str("===index a(2)=10" // C_NULL_CHAR)
  a(index) = 10

  dataGuidArray(1) = dataGuid
  outputEvent = C_NULL_PTR

  call ocrEdtCreate(edtGuid, templateGuid, EDT_PARAM_DEF, nparamv, &
       1_C_INT, dataGuidArray, EDT_PROP_NONE, NULL_GUID, outputEvent)

  ! call c_f_pointer(edtGuid, test, 1);
  call printf_p("===edtGuid" // C_NULL_CHAR , edtGuid)
  ! call printf_p("test" // C_NULL_CHAR , test)


  returnGuid = 0 ! C_NULL_PTR
end function mainEdt

end module m

! interface
!    subroutine test() BIND(c)
!      use ISO_C_BINDING
!      use :: ocr_interfaces, only : printf_str
!    end subroutine test
! end interface

