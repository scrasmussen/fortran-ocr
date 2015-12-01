#include <stdio.h>
#include <stdlib.h>
#include <ocr.h>
#include "cocr.h"
#include "stencil1d.h"


ocrGuid_t testFunc2(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  HaloArray_t *ptr; // = depv[0].ptr;
  ptr = (HaloArray_t*)depv[0].ptr;
  ocrGuid_t dataGuid = depv[0].guid;
  u64 t = ptr->t;
  PRINTF("rank = %u  t = %u\n", ptr->rank, t);
  ptr->t-=1;
  
  if (t == 0)
    {
      PRINTF("=FINISHED=\n");
      return NULL_GUID;
    }


  ocrGuid_t recTemplate, oddTriggerGuid, oddTriggerEvent;
  recTemplate = ptr->template;

  ocrEdtCreate(&oddTriggerGuid, recTemplate, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, NULL, EDT_PROP_NONE, NULL_GUID, NULL);
  ocrAddDependence(dataGuid, oddTriggerGuid, 0, DB_MODE_RW);
  ocrEventCreate(&oddTriggerEvent,OCR_EVENT_ONCE_T,EVT_PROP_NONE);
  ocrAddDependence(oddTriggerEvent, oddTriggerGuid, 1, DB_MODE_RW);
  ocrEventSatisfy(oddTriggerEvent, NULL);

  if (t%2 == 0) {
    /* PRINTF("%u is even\n", t); */
  }
  else {
    /* PRINTF("%u is odd\n", t); */
  }

  return NULL_GUID;
}
void initSingleHaloArrays(HaloArray_t *haloArray[], u64 np, u64 nx, u64 t)
{
  int i, j;
  for (i = 0; i < np; ++i) {
    for (j = 0; j < nx; ++j)
      ((double*) haloArray[i]->data.ptr)[j] = i*7+j+1;
    ((double*) haloArray[i]->left.write.ptr)[0] =
      ((double*)haloArray[i]->data.ptr)[0];
    ((double*) haloArray[i]->right.write.ptr)[0] =
      ((double*)haloArray[i]->data.ptr)[nx-1];
  }
  ocrGuid_t recTemplate, nparamv[1];
  ocrEdtTemplateCreate(&recTemplate, testFunc2, 0, 2);
  nparamv[0] = recTemplate;

  for (i = 0; i < np; ++i) {
    haloArray[i]->t = t;  
    haloArray[i]->template = recTemplate;
  }
  if (t%2 == 0) {
    for (i = 0; i < np; ++i) {
      ocrEdtCreate(&haloArray[i]->evenTriggerGuid, recTemplate, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, NULL, EDT_PROP_NONE, NULL_GUID, NULL);
      ocrAddDependence(haloArray[i]->guid, haloArray[i]->evenTriggerGuid, 0, DB_MODE_RW);
      ocrEventCreate(&haloArray[i]->evenTriggerEvent,OCR_EVENT_ONCE_T,EVT_PROP_NONE);
      ocrAddDependence(haloArray[i]->evenTriggerEvent, haloArray[i]->evenTriggerGuid, 1, DB_MODE_RW);
    }

  
  }




  PRINTF("Init Done\n");
}
void createSingleHaloArrays(HaloArray_t *haloArray[], u64 np, u64 nx, u64 h, u64 t)
{
  u64 i=0;
  ocrGuid_t temp;

  for (i = 0; i < np; ++i) 
    {
      ocrDbCreate(&temp, (void**) &haloArray[i], 
		sizeof(HaloArray_t), DB_PROP_NONE, NULL_GUID, NO_ALLOC);
      haloArray[i]->guid = temp;
    ocrDbCreate(&haloArray[i]->data.guid, (void**) &haloArray[i]->data.ptr, 
		sizeof(double)*nx, DB_PROP_NONE, NULL_GUID, NO_ALLOC);
    ocrDbCreate(&haloArray[i]->left.write.guid, 
		(void**) &haloArray[i]->left.write.ptr, sizeof(double)*h, 
		DB_PROP_NONE, NULL_GUID, NO_ALLOC);
    ocrDbCreate(&haloArray[i]->right.write.guid, 
		(void**) &haloArray[i]->right.write.ptr, sizeof(double)*h, 
		DB_PROP_NONE, NULL_GUID, NO_ALLOC);    
    }
  for (i = 0; i < np; ++i) {
    haloArray[i]->left.read = haloArray[(i-1+np)%np]->right.write;
    haloArray[i]->right.read = haloArray[(i+1)%np]->left.write;
    haloArray[i]->rank = i;
  }
  return;
}

void createHaloArrays(HaloArray_t haloArray[], u64 np, u64 nx, u64 h, u64 t)
{
  u64 i=0, j=0;
  PRINTF("In lib np=%u nx=%u t=%u\n", np, nx, t);

  /* Set up np datablocks */
  for (i = 0; i < np; ++i) {
    ocrDbCreate(&haloArray[i].data.guid, (void**) &haloArray[i].data.ptr, 
		sizeof(double)*nx, DB_PROP_NONE, NULL_GUID, NO_ALLOC);
    ocrDbCreate(&haloArray[i].left.write.guid, 
		(void**) &haloArray[i].left.write.ptr, sizeof(double)*h, 
		DB_PROP_NONE, NULL_GUID, NO_ALLOC);
    ocrDbCreate(&haloArray[i].right.write.guid, 
		(void**) &haloArray[i].right.write.ptr, sizeof(double)*h, 
		DB_PROP_NONE, NULL_GUID, NO_ALLOC);    
    // TEST
    /* ocrDbCreate(&haloArray[i].guid.guid, (void**) &haloArray[i].guid.ptr, */
    /* 		sizeof(ocrGuid_t), DB_PROP_NONE, NULL_GUID, NO_ALLOC); */
    /* ocrEventCreate(&haloArray[i].triggerEvent,OCR_EVENT_STICKY_T, EVT_PROP_NONE); */
    /* ((ocrGuid_t*) haloArray[i].guid.ptr)[0] = haloArray[i].triggerEvent; */
  }
  for (i = 0; i < np; ++i) {
    haloArray[i].left.read = haloArray[(i-1+np)%np].right.write;
    haloArray[i].right.read = haloArray[(i+1)%np].left.write;
    haloArray[i].rank = i;
  }
  // END DB CREATE

  return;
}

void initHaloArrays(HaloArray_t haloArray[], u64 np, u64 nx)
{
  int i, j;
  for (i = 0; i < np; ++i) {
    for (j = 0; j < nx; ++j)
      ((double*) haloArray[i].data.ptr)[j] = i*7+j+1;
    ((double*) haloArray[i].left.write.ptr)[0] = 
      ((double*)haloArray[i].data.ptr)[0];
    ((double*) haloArray[i].right.write.ptr)[0] = 
      ((double*)haloArray[i].data.ptr)[nx-1];
  }
}

ocrGuid_t secondFunc(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  PRINTF("Second !\n");
  double *data = (double*) depv[0].ptr;
  int i;
  for (i = 0; i < 1; ++i)
    PRINTF("S: data[%u] = %.1f\n", i, data[i]);
  
}

void startRecursion(HaloArray_t haloArray[], u64 np, u64 nx, u64 h, u64 t)
{
  u64 nparamv[3];
  nparamv[0] = np;
  nparamv[1] = nx;
  nparamv[2] = t;

  PRINTF("Here \n");
  int i;
  ocrGuid_t recTemplate, recGuid, output[3];
  ocrEdtTemplateCreate(&recTemplate, recFunc, 3, 3);
  for (i = 0; i < np; ++i) {
    ocrEdtCreate(&recGuid, recTemplate, EDT_PARAM_DEF, nparamv, EDT_PARAM_DEF, NULL, EDT_PROP_NONE, NULL_GUID, &output[i]);
    ocrAddDependence(haloArray[i].data.guid, recGuid, 0, DB_MODE_RW);
    ocrAddDependence(haloArray[i].left.read.guid, recGuid, 1, DB_MODE_EW);
    ocrAddDependence(haloArray[i].right.read.guid, recGuid, 2, DB_MODE_EW);
    /* ocrAddDependence(haloArray[i].guid.guid, recGuid, 3, DB_MODE_EW); */
  }
  PRINTF("Here \n");
}

ocrGuid_t recFunc(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  int t = paramv[2];
  ocrGuid_t data, left, right;
  data = depv[0].guid;
  left = depv[1].guid;
  right = depv[2].guid;
  double *ddata, *dleft, *dright;
  ocrGuid_t *event;
  ddata = (double*) depv[0].ptr;
  dleft = (double*) depv[1].ptr;
  dright = (double*) depv[2].ptr;
  /* event = (ocrGuid_t*) depv[3].ptr; */

  PRINTF("Entered Recursion t = %u  %u\n", t, ddata[1]);
  /* PRINTF("data t = %.1f  %u\n", ((double*)depv[0].ptr)[3], (ocrGuid_t*) depv[3].ptr); */

  if (t == 0)
    return NULL_GUID;

  ocrGuid_t recTemplate, recGuid, output[3];
  ocrEdtTemplateCreate(&recTemplate, recFunc, 3, 3);
  paramv[2] -= 1;
  ocrEdtCreate(&recGuid, recTemplate, EDT_PARAM_DEF, paramv, EDT_PARAM_DEF, NULL, EDT_PROP_NONE, NULL_GUID, NULL);
  ocrAddDependence(data, recGuid, 0, DB_MODE_EW);
  ocrAddDependence(left, recGuid, 1, DB_MODE_EW);
  ocrAddDependence(right, recGuid, 2, DB_MODE_EW);
  /* ocrAddDependence(event, recGuid, 3, DB_MODE_EW); */


  PRINTF("                            Exiting Recursion t = %u  %.1f\n", t, ddata[0]);
  // TO DO
  // attempt with sticky
  // a once event

  /* if (t%2 == 0) { */
  /*   PRINTF("%u is even\n", t); */
    
  /* } */
  /* else { */
  /*   PRINTF("%u is odd\n", t); */
  /* } */

  return NULL_GUID;
}

void testHaloArrays(HaloArray_t haloArray[], u64 np)
{
  int i;
  ocrGuid_t testTemplate, testGuid;
  ocrEdtTemplateCreate(&testTemplate, testFunc, 0, 3);
  for (i = 0; i < np; ++i) {
     ocrEdtCreate(&testGuid, testTemplate, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, NULL, EDT_PROP_NONE, NULL_GUID, NULL);
    ocrAddDependence(haloArray[i].data.guid, testGuid, 0, DB_MODE_EW);
    ocrAddDependence(haloArray[i].left.read.guid, testGuid, 1, DB_MODE_EW);
    ocrAddDependence(haloArray[i].right.read.guid, testGuid, 2, DB_MODE_EW);
  }
}

ocrGuid_t testFunc(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  PRINTF("ENTERED TEST\n");  
  double *data = (double*) depv[0].ptr;
  double *left = (double*) depv[1].ptr;
  double *right = (double*) depv[2].ptr;
  int i = 0;
  PRINTF("LEFTWRITE = %.1f\n", left[0]);
  for (i = 0; i < 4; ++i)
    PRINTF("A[%u] = %.1f\n", i, data[i]);
  PRINTF("RIGHTWRITE = %.1f\n", right[0]);

  return NULL_GUID;
}

