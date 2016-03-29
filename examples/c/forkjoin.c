#include <ocr.h>
#include <stdio.h>
#include "cocr.h"
#include "labeling.h"

ocrGuid_t syncEdt(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  return NULL_GUID;
}

ocrGuid_t finalizeEdt(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  return NULL_GUID;
}


ocrGuid_t mainF90Edt(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  return NULL_GUID;
}

ocrGuid_t mainEdt(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  PRINTF("=Start=\n");
  u64 size = 3, dim = 2, topology = 2, nDeps = 2;

  SPMD_t info;

  SPMD_Init(size, dim, topology, nDeps, mainF90Edt, syncEdt, finalizeEdt, info);

  return NULL_GUID;
}
