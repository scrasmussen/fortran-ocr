#include <ocr.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include "daveStencil1d.h"
// globalData(rank, size)
typedef struct{
  u64 rank;
} rank_t;

typedef struct{
  u64 timestep;
  u64 mynode;
  ocrGuid_t left;
  ocrGuid_t right;
  ocrGuid_t leftold;
  ocrGuid_t rightold;
  ocrGuid_t template;
} private_t;

ocrGuid_t startFunc(u32, u64 *, u32, ocrEdtDep_t[]);
ocrGuid_t endFunc(u32, u64 *, u32, ocrEdtDep_t[]);
ocrGuid_t stencilFunc(u32, u64 *, u32, ocrEdtDep_t[]);
void initialize(double *, buffer_t *, buffer_t *);
void compute(double *, double *, buffer_t *, buffer_t *);
ocrGuid_t stickyEvent(buffer_t *, double *, ocrGuid_t *, ocrEdtDep_t *, int, 
		      int);

// stencilTask
// wrapupTask
// realmainTask



ocrGuid_t mainEdt(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  /* What they did here: create DB arrays, create template and edtcreate, add dependence */
     /*realMainTask: create sticky events, create n stencil edts, pass in data */
     /*stecilTask: given own DB and halo regions, and private block */
     /*wrapupTask: waits till realMain is done and ends and shutsdown  */
  /* Fork Join: template and edt, add dependence, funcs had db create and destroyes */
  /*   */
  /* PRINTF("HELLO FROM MAIN \n"); */
  PRINTF("==Starting==\nnumber of workers=%d   data on each worker=%d number of timesteps=%d\n", N, M, T);

  // Create Templates
  ocrGuid_t startTemplate, endTemplate;
  ocrEdtTemplateCreate(&startTemplate, startFunc, 0, 4 * N - 2);
  ocrEdtTemplateCreate(&endTemplate, endFunc, 0, N + 1);
  // Create DBs
  u64 i, *dummy;
  ocrGuid_t dataDb[N], privateDb[N], bufferDb[2 * N - 2];
  for (i = 0; i < 2 * N - 2; ++i)
    ocrDbCreate(&(bufferDb[i]), (void**) &dummy, sizeof(buffer_t), 0, \ 
		NULL_GUID, NO_ALLOC);
  for (i = 0; i < N; ++i) {
    ocrDbCreate(&(privateDb[i]), (void**) &dummy, sizeof(private_t), 0, \
		NULL_GUID, NO_ALLOC);
    ocrDbCreate(&(dataDb[i]), (void**) &dummy, 2*M*sizeof(double), 0, \
		NULL_GUID, NO_ALLOC);
    }
  // Create EDTs
  ocrGuid_t startEdt, endEdt, startOutput;
  ocrEdtCreate(&endEdt, endTemplate, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, NULL,\
	       EDT_PROP_NONE, NULL_GUID, NULL);
  ocrEdtCreate(&startEdt, startTemplate, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, \
	       NULL, EDT_PROP_FINISH, NULL_GUID, startOutput);
  // Add Dependencies
  for (i = 0; i < N; ++i)
    ocrAddDependence(dataDb[i], endEdt, i, DB_MODE_RW);
  ocrAddDependence(startOutput, endEdt, N, DB_MODE_RW);
  for (i = 0; i < N; ++i)
    ocrAddDependence(dataDb[i], startEdt, i, DB_MODE_RW);
  for (i = 0; i < 2 * N - 2; ++i)
    ocrAddDependence(bufferDb[i], startEdt, N+i, DB_MODE_RW);
  for (i = 0; i < N; ++i)
    ocrAddDependence(privateDb[i], startEdt, (3 * N - 2) + i, DB_MODE_RW);


  /* ocrShutdown(); */

  return NULL_GUID;
}


ocrGuid_t startFunc(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  /* PRINTF("HELLO FROM START\n"); */
  ocrGuid_t stencilTemplate;
  ocrEdtTemplateCreate(&stencilTemplate, stencilFunc, 0, 4);

  int i;
  double *A[N];
  buffer_t *buffer[2 * N - 2];
  private_t *private[N];
  for (i = 0; i < N; ++i) 
    A[i] = depv[i].ptr;
  for (i = 0; i < 3 * N - 2; ++i) 
    buffer[i - N] = depv[i].ptr;
  for (i = 0; i < N; ++i) 
    private[i] = depv[3 * N - 2 + i].ptr;

  u64 lin = 0, rin = lin + N - 1;
  private[0]->left = NULL_GUID;
  private[0]->leftold = NULL_GUID;

  ocrGuid_t sticky;
  for (i = 0; i < N - 1; ++i) 
    {
      private[i]->mynode = i;
      private[i]->timestep = 0;
      private[i]->template = stencilTemplate;

      ocrEventCreate(&sticky, OCR_EVENT_STICKY_T, true);
      private[i]->right = sticky;
      buffer[lin++]->control = sticky;
      private[i]->rightold = NULL_GUID;

      ocrEventCreate(&sticky, OCR_EVENT_STICKY_T, true);    
      private[i + 1]->left = sticky;
      buffer[rin++]->control = sticky;
      private[i + 1]->leftold = NULL_GUID;
    }

  private[N - 1]->mynode = N-1;
  private[N - 1]->timestep = 0;
  private[N - 1]->template = stencilTemplate;
  private[N - 1]->right = NULL_GUID;
  private[N - 1]->rightold = NULL_GUID;

  // create N stencil init events, attach the data db
  ocrGuid_t stencilEdt[N];
  for (i = 0; i < 4 * N - 2; ++i) 
    ocrDbRelease(depv[i].guid); //release all dbs
  for (i = 0; i < N; ++i) 
    ocrEdtCreate(&stencilEdt[i], stencilTemplate, EDT_PARAM_DEF, NULL, \
		 EDT_PARAM_DEF, NULL, EDT_PROP_NONE, NULL_GUID, NULL_GUID);
  // attach correct data blocks
  lin = N;
  rin = lin + N - 1;
  u64 pbuffer = 3 * N - 2;
  ocrAddDependence(NULL_GUID, stencilEdt[0], 1, DB_MODE_CONST);
  for (i = 0; i < N - 1; ++i)
    {
      ocrAddDependence(depv[i].guid, stencilEdt[i], 0, DB_MODE_RW);
      ocrAddDependence(depv[rin++].guid, stencilEdt[i], 2, DB_MODE_RW);
      ocrAddDependence(depv[pbuffer++].guid, stencilEdt[i], 3, DB_MODE_RW);
      ocrAddDependence(depv[lin++].guid, stencilEdt[i + 1], 1, DB_MODE_RW);
    }
  ocrAddDependence(depv[N - 1].guid, stencilEdt[N - 1], 0, DB_MODE_RW);
  ocrAddDependence(NULL_GUID, stencilEdt[N - 1], 2, DB_MODE_RW);
  ocrAddDependence(depv[pbuffer++].guid, stencilEdt[N - 1], 3, DB_MODE_RW);

  return NULL_GUID;
}

ocrGuid_t endFunc(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  /* PRINTF("HELLO FROM END\n"); */
  /* waits until startFunc is done; prints out answer  */
  u64 i, j;
  double *data[N];
  for (i=0; i<N; ++i) 
    {
      data[i] = depv[i].ptr;
      for (j=0; j<M; j++) 
	PRINTF("%d %d %f \n", i, j, data[i][j]);
    }

  ocrShutdown();
  return NULL_GUID;
}

ocrGuid_t stencilFunc(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  /* PRINTF("HELLO FROM STENCIL\n"); */

  double *A = depv[0].ptr;
  double *ATemp = A + M;
  buffer_t *leftin = depv[1].ptr;
  buffer_t *rightin = depv[2].ptr;
  private_t *private = depv[3].ptr;

  if (private->leftold != NULL_GUID) 
    ocrEventDestroy(private->leftold);
  if (private->rightold != NULL_GUID) 
    ocrEventDestroy(private->rightold);
  u64 timestep = private->timestep;
  // Initilize
  if (timestep == 0)
      initialize(A, leftin, rightin);

  ocrGuid_t leftInEvent, rightInEvent, sticky;

  if (timestep < T)
    {
      compute(A, ATemp, leftin, rightin);

      /* leftInEvent = stickyEvent(leftin, A, private->left, depv, 0, 1); */
      /*rightInEvent = stickyEvent(rightin, A, private->right, depv, M-1,2); */
      if(leftin != NULL) { //leftin is NULL too
      	leftInEvent = leftin->control;
      	leftin->halo = A[0];
      	ocrEventCreate(&sticky, OCR_EVENT_STICKY_T, true);
      	leftin->control = sticky;
      	ocrDbRelease(depv[1].guid);
      	ocrEventSatisfy(private->left, depv[1].guid);
      	private->left = sticky;
      }

      if(rightin != NULL) {
	rightInEvent = rightin->control;
	rightin->halo = A[M-1];
	ocrEventCreate(&sticky, OCR_EVENT_STICKY_T, true);
	rightin->control = sticky;
	ocrDbRelease(depv[2].guid);
	ocrEventSatisfy(private->right, depv[2].guid);
	private->right = sticky;
      }
      
      private->timestep = timestep + 1;
      private->leftold = leftInEvent;
      private->rightold = rightInEvent;

      //create clone
      ocrGuid_t stencilEdt;
      ocrEdtCreate(&stencilEdt, private->template, EDT_PARAM_DEF, NULL,
		   EDT_PARAM_DEF, NULL, EDT_PROP_NONE, NULL_GUID, NULL_GUID);
      ocrDbRelease(depv[0].guid);
      ocrAddDependence(depv[0].guid, stencilEdt, 0, DB_MODE_RW);
      ocrAddDependence(leftInEvent, stencilEdt, 1, DB_MODE_RW);
      ocrAddDependence(rightInEvent, stencilEdt, 2, DB_MODE_RW);
      ocrDbRelease(depv[3].guid);
      ocrAddDependence(depv[3].guid, stencilEdt, 3, DB_MODE_RW);
    }

  return NULL_GUID;
}

void compute(double *A, double *ATemp, buffer_t *leftin, buffer_t *rightin)
{
  int i;
  if (M == 1)
    {
      if (leftin == NULL || rightin == NULL)
	ATemp[0] = A[0];
      else
	ATemp[0] = 0.5 * A[0] + 0.25 * (leftin->halo + rightin->halo);
    }
  else
    {
      if (leftin != NULL)
	ATemp[0] = 0.5 * A[0] + 0.25 * (A[1] + leftin->halo);
      else
	ATemp[0] = A[0];
      for (i = 1; i < M - 1; ++i)
	ATemp[i] = 0.5 * A[i] + 0.25 * (A[i-1]+A[i+1]);
      if (rightin != NULL)
	ATemp[M - 1] = 0.5 * A[M-1] + 0.25 * (rightin->halo + A[M-2]);
      else
	ATemp[M - 1] = A[M - 1];
    }
  for (i = 0; i < M; ++i)
    A[i] = ATemp[i];
  return;
}

void initialize(double *A, buffer_t *leftin, buffer_t *rightin)
{
  int i;
  /* if (leftin == NULL) */
  /*   PRINTF("NULLLLLLLLLLLLLLLLL\n"); */
  if (M == 1)
    {
      if (leftin == NULL || rightin == NULL)
	A[0] = 1;
      else
	A[0] = 0;
    }
  else
    {
      if (leftin == NULL) 
	A[0] = 1;
      else 
	{
	  A[0] = 0;
	  leftin->halo = 0;
	  for (i = 1; i < M-1; ++i)
	    A[i] = 0;
	  if (rightin == NULL)
	    A[M-1] = 1;
	  else
	    {
	      A[M-1] = 0;
	      rightin->halo = 0;
	    }
	}
    }

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

