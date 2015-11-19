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

// globalData(rank, size)
typedef struct{
  u64 timestep;
  u64 rank;
  ocrGuid_t leftbucket;
  ocrGuid_t rightbucket;
  ocrGuid_t leftlist;
  ocrGuid_t rightlist;
} private_t;

ocrGuid_t fortranMain(u32, u64 *, u32, ocrEdtDep_t[]);

void initialize(double *, double);
void printArray(double *);

ocrGuid_t mainEdt(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  PRINTF("==Starting==\n");
  PRINTF("ranks=%d ranksize=%d timesteps=%d\n", N, M, T);
  u64 np = 2;
  haloArray_t haloArray[np];
  createHaloArrays(haloArray, NX);
  // SERIAL IMPLEMENTATION
  /* double A[N*M+2]; */
  /* initialize(A, 1); */
  /* serialCompute(A); */
  /* printArray(A); */

  // Create Templates
  ocrGuid_t fortranMainTemplate, endTemplate;
  u64 i;
  ocrEdtTemplateCreate(&fortranMainTemplate, fortranMain, 2, 1);
  // Create db
  float *dataArray;
  ocrGuid_t dataGuid;
  ocrDbCreate(&dataGuid, (void **) &dataArray, sizeof(float)*N*M, DB_PROP_NONE, NULL_GUID, NO_ALLOC);

  for (i = 0; i < N*M; ++i)
    dataArray[i] = (i+1)*0.2;

  // Create EDTs
  ocrGuid_t dummy;
  u64 nparamv[2*N];
  for (i = 0; i < N; ++i)
    {
      nparamv[2*i] = i*M;
      nparamv[2*i+1] = N*M;
      ocrEdtCreate(&dummy, fortranMainTemplate, 2, &nparamv[2*i], 1, 
		   &dataGuid, EDT_PROP_NONE, NULL_GUID, NULL);
    }

  // Add Dependencies

  ocrShutdown();
  return NULL_GUID;
}


ocrGuid_t fortranMain(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  /* PRINTF("HELLO FROM FORTRAN MAIN\n"); */
  float *data = depv[0].ptr;
  u64 rank = paramv[0], size = paramv[1], i;
  PRINTF("rank=%u size=%u dataArray[%u]=%.1f\n", rank, size, rank, data[rank]);

  /* allocate array  */
  /* This should trigger ocr_createDB for the array and the halo regions. */
  /* u64 unitSize = N / M; */
  /* float *lHalo, *rHalo; */
  /* ocrGuid_t lHaloGuid, rHaloGuid; */
  /* ocrDbCreate(&lHaloGuid, (void **) &lHalo, sizeof(float)*N, DB_PROP_NONE, \ */
  /* 	      NULL_GUID, NO_ALLOC); */
  /* ocrDbCreate(&rHaloGuid, (void **) &rHalo, sizeof(float)*N, DB_PROP_NONE, \ */
  /* 	      NULL_GUID, NO_ALLOC); */

  // initialize Center and Halo
  float *test1Data, *rtest1Data, *test2Data, *ltest2Data; ; 
  ocrGuid_t test1Guid, rtest1Guid, test2Guid, ltest2Guid;
  ocrDbCreate(&test1Guid, (void **) &test1Data, sizeof(float), 0, NULL_GUID, NO_ALLOC);
  ocrDbCreate(&rtest1Guid, (void **) &rtest1Data, sizeof(float), 0, NULL_GUID, NO_ALLOC);
  ocrDbCreate(&test2Guid, (void **) &test2Data, sizeof(float), 0, NULL_GUID, NO_ALLOC);
  ocrDbCreate(&ltest2Guid, (void **) &ltest2Data, sizeof(float), 0, NULL_GUID, NO_ALLOC);
  // fill in data


  // Dependencies






  return NULL_GUID;
}

void initialize(double *A, double boundary)
{
  int i, end = N*M;
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
