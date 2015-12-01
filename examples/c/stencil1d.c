#include <ocr.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include "stencil1d.h"
#include "serialStencil.h"
#include "cocr.h"

#define NT 1
#define NX 4
#define H 1

ocrGuid_t mainEdt(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  PRINTF("==Starting==\n");
  /* u64 np = 2; */
  s64 np = 4, nx = 4, h = H, t = T, numsleep = 0; 
  t = 7;
  PRINTF("np=%u nx=%u h=%u t=%u\n", np, nx, h, t);
  // Initialize array A
  /* USER
  double A[n*M+2];
  for (int i = 0; i < n*m + 2; ++i)
      A = i;
  */
  /* USER FORTRAN 
     !$OFP HALO :: A(H:*:H)
     allocate(A(1-H:NX+H))
     A(1:NX) = 1.0
     A(1-H:) = 0.0
     A(NX+1:NX+H) = 0.0 
  */

  HaloArray_t *haloArrayPtr[np];
  createHaloArrays(haloArrayPtr, np, nx, h, t);
  initHaloArrays(haloArrayPtr, np, nx, t);
 
  int i;
  for (i = 0; i < np; ++i) {
    if (t%2 == 0) {
      ocrEventSatisfy(haloArrayPtr[i]->evenTrigger.event, NULL);
      ocrEventSatisfy(haloArrayPtr[i]->left.evenTriggerEvent, NULL);
      ocrEventSatisfy(haloArrayPtr[i]->right.evenTriggerEvent, NULL);
    }
    else {
      ocrEventSatisfy(haloArrayPtr[i]->oddTrigger.event, NULL);
      ocrEventSatisfy(haloArrayPtr[i]->left.oddTriggerEvent, NULL);
      ocrEventSatisfy(haloArrayPtr[i]->right.oddTriggerEvent, NULL);
    }
  }

  PRINTF("Fin\n");
  return NULL_GUID;
}
