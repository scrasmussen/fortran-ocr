MODULE test_ocr_mod

CONTAINS

FUNCTION mainEdt(paramc,paramv,depc,depv) RESULT(rtn) BIND(C,NAME='mainEdt')
USE ISO_C_BINDING
USE ocr_interfaces
IMPLICIT NONE
INTEGER, PARAMETER :: L = 32, M = 64
INTEGER(KIND=C_INTPTR_T) :: rtn
INTEGER(KIND=C_INT32_T), INTENT(IN) :: paramc
INTEGER(KIND=C_INT64_T), INTENT(IN) :: paramv(*)
INTEGER(KIND=C_INT32_T), INTENT(IN) :: depc
TYPE(ocrEdtDep_t), INTENT(IN) :: depv(*)
CALL printf("Hellno, world!",L)
CALL ocrShutdown()
rtn = NULL_GUID
END FUNCTION mainEdt
END MODULE test_ocr_mod
