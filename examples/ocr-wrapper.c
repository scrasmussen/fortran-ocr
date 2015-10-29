#include <stdio.h>
#include <stdlib.h>
#include "ocr.h"

#define VERBOSE
/* #define VERBOSEARG */

void wocrDbCreate(ocrGuid_t * db, void ** addr, u64 len, u16 flags, ocrGuid_t *affinity, ocrInDbAllocator_t allocator)
{
#if defined(VERBOSE)
  PRINTF("Pre-ocrDbCreate\n");
#endif

  ocrDbCreate(db, addr, len, flags, affinity, allocator);
  ((u64*)*addr)[1] = (u64) 316;
  PRINTF("DB ADDRESS = %p\n", db);

#if defined(VERBOSE)
  PRINTF("Post-ocrDbCreate %p %ld\n", *addr, ((u64*)*addr)[1]);
#endif
  return;
}

void wocrEdtCreate(ocrGuid_t *guid, ocrGuid_t templateGuid, u32 paramc, u64 * paramv, u32 depc, ocrGuid_t *depv, u16 flags, ocrGuid_t affinity, ocrGuid_t * outputEvent)
{
#if defined(VERBOSE)
  PRINTF("Pre-ocrEdtCreate\n");
#endif

  /* void * p; */
  /* p = (void * )depv[0]; */
  /* PRINTF("paramv[0] = %u\n", paramv[0]); */
  /* PRINTF("paramv[1] = %u\n", paramv[1]); */
  /* PRINTF("depc = %u\n", depc); */
  /* PRINTF("depv = %p\n", p); */
  /* PRINTF("depv = %p\n", depv[0]); */
  /* PRINTF("outputEvent = %p\n", *outputEvent); */

  ocrEdtCreate(guid, templateGuid, paramc, paramv, depc, depv, flags, affinity, outputEvent);


#if defined(VERBOSE)
  PRINTF("Post-ocrEdtCreate\n");
#endif
  return;
}

void wocrEdtTemplateCreate_internal(ocrGuid_t *guid, ocrEdt_t funcPtr, u32 paramc, u32 depc, char* funcName)
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
  /* PRINTF("Pre-ocrShutdown\n"); */
#endif
  ocrShutdown();
#if defined(VERBOSE)
  /* PRINTF("Post-ocrShutdown\n"); */
#endif
  return;
}
