#include <ocr.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include "newstencil1d.h"
// globalData(rank, size)
typedef struct{
  u64 timestep;
  u64 rank;
  ocrGuid_t leftbucket;
  ocrGuid_t rightbucket;
  ocrGuid_t leftlist;
  ocrGuid_t rightlist;
} private_t;

ocrGuid_t startFunc(u32, u64 *, u32, ocrEdtDep_t[]);
ocrGuid_t endFunc(u32, u64 *, u32, ocrEdtDep_t[]);
ocrGuid_t stencilFunc(u32, u64 *, u32, ocrEdtDep_t[]);
void initialize(double *, double);
void serialCompute(double *);
double serialStencil(double, double, double);
/* void compute(double *, double *, buffer_t *, buffer_t *); */
void printArray(double *);

ocrGuid_t mainEdt(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  /* What they did here: create DB arrays, create template and edtcreate, add dependence */
  /*realMainTask: create sticky events, create n stencil edts, pass in data */
  /*stecilTask: given own DB and halo regions, and private block */
  /*wrapupTask: waits till realMain is done and ends and shutsdown  */
  /* Fork Join: template and edt, add dependence, funcs had db create and destroyes */
  PRINTF("==Starting==\n");
  PRINTF("ranks=%d ranksize=%d timesteps=%d\n", N, M, T);

  // Create Templates
  ocrGuid_t startTemplate, endTemplate;
  double A[N*M+2];
  initialize(A, 1);
  serialCompute(A);
  printArray(A);


  /* ocrEdtTemplateCreate(&startTemplate, startFunc, 0, 4 * N - 2); */
  // Create DBs

  // Create EDTs

  // Add Dependencies

  ocrShutdown();
  return NULL_GUID;
}


ocrGuid_t startFunc(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  PRINTF("HELLO FROM START\n");
  return NULL_GUID;
}

ocrGuid_t endFunc(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  PRINTF("HELLO FROM END\n");
  /* waits until startFunc is done; prints out answer  */

  return NULL_GUID;
}

ocrGuid_t stencilFunc(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  PRINTF("HELLO FROM STENCIL\n");

  return NULL_GUID;
}

void compute(double *A, double *ATemp, buffer_t *leftin, buffer_t *rightin)
{

  return;
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

void serialCompute(double *A)
{
  int t, i;
  double buf[N*M+2];
  for (i = 0; i<M*N+2; ++i)
      buf[i] = A[i];

  for (t = 1; t<=T; ++t)
    {
      for (i = 1; i<=M*N; ++i)
	A[i] = serialStencil(buf[i-1], A[i], buf[i+1]);
      for (i = 1; i<=M*N; ++i)
	buf[i] = A[i];
    }
  PRINTF("t = %u\n",t);
}


double serialStencil(double iminus, double i, double iplus)
{
  return (0.25*iminus + 0.5*i + 0.25*iplus);
}
