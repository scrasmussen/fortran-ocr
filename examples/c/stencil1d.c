#include <ocr.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include "stencil1d.h"
#include "serialStencil.h"
#include "cocr.h"

//REMOVE THESE LATER
#include <unistd.h>

#define NT 1
#define NX 4
#define H 1

void printArray(double *);

ocrGuid_t mainEdt(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  PRINTF("==Starting==\n");
  /* u64 np = 2; */
  s64 np = 3, nx = 4, h = H, t = T, numsleep = 0; 
  t = 2;
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
  // Become
  HaloArray_t haloArray[np];

  /* createHaloArrays(haloArray, np, nx, h, t); */
  /* initHaloArrays(haloArray, np, nx); */
  /* startRecursion(haloArray, np, nx, h, t); */

  // second way to do this 

  HaloArray_t *haloArrayPtr[np];
  createSingleHaloArrays(haloArrayPtr, np, nx, h, t);
  initSingleHaloArrays(haloArrayPtr, np, nx, t);

  /* HaloArray_t * ptr = haloArray[1].ptr; */
  /* PRINTF("rank = %u for ptr = %p\n", ptr->rank, ptr); */

  int i;

  for (i = 0; i < np; ++i)
    ocrEventSatisfy(haloArrayPtr[i]->evenTriggerEvent, NULL);


  numsleep = 1;
  PRINTF("==sleep %u==\n", numsleep);
  sleep(numsleep);
  PRINTF("ocrShutdown\n");
  ocrShutdown();
  PRINTF("Fin\n");
  return NULL_GUID;
}


ocrGuid_t stickyEvent(buffer_t *in, double *A, ocrGuid_t *event,
		 ocrEdtDep_t *depv, int i, int j)
{
  ocrGuid_t inEvent = in->control, sticky;
  in->halo = A[i];
  ocrEventCreate(&sticky, OCR_EVENT_STICKY_T, true);
  in->control = sticky;
  ocrDbRelease(depv[j].guid);
  ocrEventSatisfy(*event, depv[j].guid);
  *event = sticky;
  return inEvent;
}


void printArray(double *A)
{
  int i, end = N*M;
  PRINTF("A=[%.1f ", A[0]);
  for (i = 1; i <= N*M; ++i)
    PRINTF("%.2f  ", A[i]);
  PRINTF("%.1f]\n", A[N*M+1]);
  return;
}


  // SERIAL IMPLEMENTATION
  /* double A[N*M+2]; */
  /* initialize(A, 1); */
  /* serialCompute(A); */
  /* printArray(A); */

