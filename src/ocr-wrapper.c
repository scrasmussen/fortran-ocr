#include <stdio.h>
#include <stdlib.h>
#include "ocr.h"
#include "ocr-std.h"

#undef VERBOSE
#undef VERBOSEARG

void wocrAddDependence(ocrGuid_t source, ocrGuid_t destination, u32 slot, ocrDbAccessMode_t mode)
{
#if defined(VERBOSE)
  PRINTF("Pre-ocrAddDependence\n");
  PRINTF("-source = %p\n", source);
  PRINTF("-destination = %p\n", destination);
#endif

  ocrAddDependence(source, destination, slot, mode);

#if defined(VERBOSE)
  PRINTF("Post-ocrAddDependence\n");
#endif
}

void wocrEventCreate(ocrGuid_t* guid, ocrEventTypes_t eventType, u16 flags)
{
  u8 statusCode;
#if defined(VERBOSE)
  PRINTF("Pre-ocrEventCreate\n");
#endif
  
  statusCode = ocrEventCreate(guid, eventType, flags);

#if defined(VERBOSE)
  PRINTF("argNum = %d\n", argNum);
  PRINTF("Post-ocrEventCreate\n");
#endif
  return; //statusCode
}

u64 wgetArgc(void* dbPtr)
{
  u64 argNum;
#if defined(VERBOSE)
  PRINTF("Pre-getArgc\n");
#endif
  argNum = getArgc(dbPtr);
#if defined(VERBOSE)
  PRINTF("argNum = %d\n", argNum);
  PRINTF("Post-getArgc\n");
#endif
  return argNum;
}

/*
 * WARNING: The affinity argument type was changed to ocrGuid_t from ocrGuid_t*
 *          and the Fortran interface changed to INTENT(IN), VALUE so that
 *          NULL_GUID could be passed.  Otherwise a variable is required.  This is
 *          temporary.  When affinity is actually used by the library this will have
 *          to be fixed.
 */
void wocrDbCreate(ocrGuid_t * db, void ** addr, u64 len, u16 flags, ocrGuid_t affinity, ocrInDbAllocator_t allocator)
{
#if defined(VERBOSE)
  PRINTF("Pre-ocrDbCreate\n");
  PRINTF("Size of GSETTING = %u\n", len);
#endif

  ocrDbCreate(db, addr, len, flags, affinity, allocator);

  /* ((u64*)*addr)[1] = (u64) 316; */

#if defined(VERBOSE)
  /* PRINTF("DB ADDRESS = %p\n", db); */
  PRINTF("Post-ocrDbCreate\n");
  /* PRINTF("Post-ocrDbCreate %p %ld\n", *addr, ((u64*)*addr)[1]); */
#endif
  return;
}

/* these are declared in ocr-types.F90 */
extern u64 OFP_NULL_PARAMV;
extern ocrGuid_t OFP_NULL_DEPV;

ocrGuid_t* test_NULL_DEPV(ocrGuid_t *addr)
{
   return (addr == &OFP_NULL_DEPV) ? NULL : addr;
}

u64* test_NULL_PARAMV(u64 *addr)
{
   return (addr == &OFP_NULL_PARAMV) ? NULL : addr;
}

void wocrEdtCreate(ocrGuid_t *guid, ocrGuid_t templateGuid, u32 paramc, u64 * paramv, u32 depc, ocrGuid_t *depv, u16 flags, ocrGuid_t affinity, ocrGuid_t * outputEvent)
{
#if defined(VERBOSE)
  PRINTF("Pre-ocrEdtCreate\n");
  PRINTF("paramc, depc: %d %d\n", paramc, depc);
  PRINTF("      params: %d %d\n", paramv[0], paramv[1]);
  PRINTF("      paramv: %p %p\n", paramv, &OFP_NULL_PARAMV);
  PRINTF("        depv: %p %p\n", depv, &OFP_NULL_DEPV);
#endif

  depv = test_NULL_DEPV(depv);
  paramv = test_NULL_PARAMV(paramv);

  ocrEdtCreate(guid, templateGuid, paramc, paramv, depc, depv, flags, affinity, outputEvent);

#if defined(VERBOSE)
  PRINTF("Post-ocrEdtCreate\n");
#endif
  return;
}

void wocrEdtTemplateCreate(ocrGuid_t *guid, ocrEdt_t funcPtr, u32 paramc, u32 depc)
{
#if defined(VERBOSE)
  PRINTF("Pre-ocrEdtTemplateCreate_internal\n");
#endif
#if defined(VERBOSEARG)
  PRINTF("guid=%p\n", guid);
  PRINTF("funcPtr=%p\n", funcPtr);
  PRINTF("paramc=%u\n", paramc);
  PRINTF("depc=%u\n", depc);
  PRINTF("EDT_PARAM_UNK=%u\n", EDT_PARAM_UNK);
  PRINTF("EDT_PARAM_UNK=%u\n", (u32)-1);
  PRINTF("EDT_PARAM_DEF=%u\n", EDT_PARAM_DEF);
#endif

  ocrEdtTemplateCreate(guid, funcPtr, paramc, depc);
#if defined(VERBOSE)
  PRINTF("Post-ocrEdtTemplateCreate_internal\n");
#endif
  return;
}

void wocrShutdown()
{
#if defined(VERBOSE)
  PRINTF("Pre-ocrShutdown\n");
#endif
  ocrShutdown();
#if defined(VERBOSE)
  PRINTF("Post-ocrShutdown\n");
#endif
  return;
}
