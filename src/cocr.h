#ifndef COCR_H
#define COCR_H


#include <ocr.h>
#include "labeling.h"
/* #define ENABLE_EXTENSION_LABELING */
/* #include <extensions/ocr-labeling.h> */

typedef struct {
  ocrGuid_t evt_next_compute;
  ocrGuid_t evt_next_sync;
  ocrGuid_t *edt_next_sync;
  ocrGuid_t *edt_next_compute;
} neighborSync_t;

typedef struct {
  ocrGuid_t map_guid;
  ocrGuid_t tpl_sync;
  ocrGuid_t tpl_compute;


  ocrGuid_t info_guid;
  ocrGuid_t evt_finalize;

  ocrGuid_t edt_next_sync;
  ocrGuid_t evt_next_sync;
  ocrGuid_t evt_out_next_sync;

  ocrGuid_t edt_next_compute;
  ocrGuid_t evt_next_compute;
  ocrGuid_t evt_out_next_compute;
} info_t;

typedef struct {
  const int placeHolder;
  info_t *thread[];
} SPMD_t;

void SPMD_Add_haloarray(int len, int slot, int *halo, SPMD_t info);
void SPMD_Finalize(info_t info);
void SPMD_Init(u64 size, u64 dim, u64 topology, u64 np, ocrEdt_t mainF90Edt, 
	       ocrEdt_t syncEdt, ocrEdt_t finalizeEdt, SPMD_t info);
void SPMD_Labeled_Init(u64 size, u64 dim, u64 topology, u64 np, ocrEdt_t mainF90Edt, 
	       ocrEdt_t syncEdt, ocrEdt_t finalizeEdt, SPMD_t info);
ocrGuid_t fortranMain(u32, u64 *, u32, ocrEdtDep_t[]);
ocrGuid_t recFunc(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[]);


#endif
