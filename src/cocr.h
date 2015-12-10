#ifndef COCR_H
#define COCR_H
#include <ocr.h>

typedef struct {
  ocrGuid_t evt_next_compute;
  ocrGuid_t evt_next_sync;
  ocrGuid_t *edt_next_sync;
  ocrGuid_t *edt_next_compute;
} neighborSync_t;

typedef struct {
  ocrGuid_t info_guid;
  ocrGuid_t tpl_sync;
  ocrGuid_t tpl_compute;
  ocrGuid_t evt_finalize;

  ocrGuid_t edt_next_sync;
  ocrGuid_t evt_next_sync;
  ocrGuid_t evt_out_next_sync;

  ocrGuid_t edt_next_compute;
  ocrGuid_t evt_next_compute;
  ocrGuid_t evt_out_next_compute;

  neighborSync_t neighbor[];
} info_t;

typedef struct {
  const int placeHolder;
  info_t *thread[];
} SPMD_t;

void SPMD_Add_haloarray(int len, int slot, int *halo, SPMD_t info);
void SPMD_Finalize(info_t info);
void SPMD_Init(int size, int dim, int topology, int np, void *mainF90Edt, 
	       void *syncEdt, void *finalizeEdt, SPMD_t info);
ocrGuid_t fortranMain(u32, u64 *, u32, ocrEdtDep_t[]);
ocrGuid_t recFunc(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[]);


#endif
