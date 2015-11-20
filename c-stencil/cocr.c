#include <stdio.h>
#include <stdlib.h>
#include <ocr.h>
#include "cocr.h"
#include "stencil1d.h"

ocrGuid_t test(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[]);

ocrGuid_t test(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  PRINTF("ENTERED TEST\n");  
  double *data = (double*) depv[0].ptr;
  PRINTF("data pointer = %p\n", &data);
  PRINTF("A[0] = %.1f\n", data[0]);
  PRINTF("A[1] = %.1f\n", data[1]);

  return NULL_GUID;
}


void createHaloArrays(haloArray_t haloArray[], u64 np, u64 nx, u64 t)
{
  u64 i=0, j;
  PRINTF("In lib np=%u nx=%u t=%u\n", np, nx, t);
  PRINTF("pre ocrDbCreate\n");
  PRINTF("addr = %p\n", &haloArray[i].data.guid);
  ocrGuid_t dataArrayGuid;

  //  ocrDbCreate(&haloArray[i].data.guid, (void **) &haloArray[i].data.ptr, sizeof(double)*nx, DB_PROP_NONE, NULL_GUID, NO_ALLOC);
  ocrDbCreate(&haloArray[i].data.guid, &haloArray[i].data.ptr, sizeof(double)*nx, DB_PROP_NONE, NULL_GUID, NO_ALLOC);

  PRINTF(" dataARRAYPOINTER = %p\n", &haloArray[i].data.ptr);

  /* ocrDbCreate(dataArrayGuid, (void **) &haloArray[x]->dataArray, sizeof(double)*nx, DB_PROP_NONE, NULL_GUID, NO_ALLOC); */
  PRINTF("post ocrDbCreate\n");

  /* 1. Set up np datablocks */
  /* for (i = 0; i < np; ++i) */
  /*   { */
  /*     PRINTF("i=%u\n", i); */
  /*     ocrDbCreate(&haloArray[i]->dataArrayGuid, (void **) &haloArray[i]->dataArray, sizeof(double)*nx, DB_PROP_NONE, NULL_GUID, NO_ALLOC); */
  /*     PRINTF("post ocrDbCreate\n"); */
  /*     haloArray[i]->dataArray[0] = i+8; */
  /*   } */

  double* tmp =  (double*) haloArray[0].data.ptr;
  tmp[1] = -3;
  ((double*) haloArray[0].data.ptr)[0] = 4;




  ocrGuid_t testTemplate, testGuid;
  ocrEdtTemplateCreate(&testTemplate, test, 0, 1);
  ocrEdtCreate(&testGuid, testTemplate, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, &haloArray[0].data.guid, EDT_PROP_NONE, NULL_GUID, NULL);


  /* ocrEdtCreate(&testGuid, testTemplate, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, &dataArrayGuid, EDT_PROP_NONE, NULL_GUID, NULL); */




  /* for (i = 0; i < np; ++i) */
    /* ocrEdtCreate(&testGuid, testTemplate, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, &haloArray[i]->dataArrayGuid, EDT_PROP_NONE, NULL_GUID, NULL); */
  PRINTF("post ocrEdtCreate\n");


  return;
}



