module m
  use ISO_C_BINDING
  use :: ocr_types
  use :: ocr_interfaces, only : ocrShutdown, printf_str, printf_i, printf_p
  use :: ocr_interfaces, only : ocrEdtTemplateCreate, printf_pi, printf_i32
  use :: ocr_interfaces, only : ocrEdtCreate, ocrDbCreate
  implicit none

contains

function appEdt(paramc, paramv, depc, depv) result(returnGuid) BIND(c)
  use ISO_C_BINDING
  use ocr_types
  use :: ocr_interfaces, only : printf_str
  implicit none
  integer(ocrGuid_k) :: returnGuid, dataguid
  integer(u32), intent(in) :: paramc
  integer(u64), intent(in), dimension(*) :: paramv
  integer(u32), intent(in) :: depc
  type(ocrEdtDep_t), intent(in), dimension(*), target :: depv
  type(C_PTR) :: data
  integer(u64), pointer :: test(:)
  integer(u64) :: i, val
  integer(u64), pointer :: a(:)

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
  integer(ocrGuid_k) :: returnGuid
  integer(u32), intent(in) :: paramc
  integer(u64), intent(in) :: paramv
  integer(u32), intent(in) :: depc
  type(ocrEdtDep_t), intent(in) :: depv
  ! params
  integer(u32) :: fparamc
  integer(u32) :: fdepc
  integer(ocrGuid_k) :: templateGuid, edtGuid
  type(ocrEdt_t) :: ptr 
  type(C_PTR) :: outputEvent
  integer(u64) :: arraySize, index
  integer(u64), dimension(2) :: nparamv
  integer(u64) :: u64
  integer(u32) :: allocator
  ! integer(C_INT64_T) :: dataArray
  type(C_PTR) :: dataArray
  integer(u16) :: flags
  integer(ocrGuid_k) :: dataGuid
  integer(ocrGuid_k), dimension(1) :: dataGuidArray
  ! type(C_PTR) :: dataGuid
  integer(ocrGuid_k) :: affinity
  integer(C_INT64_T), pointer :: a(:)
  ! integer(u64) :: i
  type(C_PTR) :: locval
  integer(ocrGuid_k) :: test


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
  ! outputEvent = C_NULL_PTR
  outputEvent = C_NULL_PTR
  test = C_NULL

  call ocrEdtCreate(edtGuid, templateGuid, EDT_PARAM_DEF, nparamv, &
       1_C_INT, dataGuidArray, EDT_PROP_NONE, NULL_GUID, test)

  ! call c_f_pointer(edtGuid, test, 1);
  call printf_i("===edtGuid" // C_NULL_CHAR , edtGuid)

  returnGuid = 0 ! C_NULL_PTR
end function mainEdt

end module m
