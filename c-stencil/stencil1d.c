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

void printArray(double *);

ocrGuid_t mainEdt(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  PRINTF("==Starting==\n");
  /* u64 np = 2; */
  s32 np = 2;
  s64 n=2, nx=4, t=T; 
  t = 1;
  PRINTF("np=%u nx=%u t=%u\n", np, nx, t);
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
  haloArray_t haloArray[np];
  createHaloArrays(haloArray, np, nx, t);

  /* for (int i = 0; i < n*m + 2; ++i) */
  /*     A = i; */
  


  ocrShutdown();
  PRINTF("Fin\n");
  return NULL_GUID;
}

void initialize(double *A, u64 n, u64 m, double boundary)
{
  u64 i, end = n*m;
  for (i = 0; i < end; ++i)
    A[i] = 0;
  A[0] = boundary;
  A[end+1] = boundary;
  return;
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

