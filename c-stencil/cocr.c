#include <stdio.h>
#include <ocr.h>
#include "cocr.h"
#include "stencil1d.h"

ocrGuid_t printDB(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[]);

void createHaloArrays(haloArray_t* haloArray, double *A, u64 n, u64 np)
{
  u64 i, sizeOfArray = n / np, j;

  // 1. Set up np datablocks

  ocrGuid_t dataGuid[np];
  double **dataArray[np];
  for (i = 0; i < np; ++i)
    {
      double data[sizeOfArray];
      ocrDbCreate(&dataGuid[i], (void **) &data, sizeof(double)*N*M, DB_PROP_NONE, NULL_GUID, NO_ALLOC);
      for (j = 0; j < sizeOfArray; ++j)
	data[j] = (j+1)*0.2+i*7;
      dataArray[i] = &data;
    }

  double *data;
  for (i = 0; i < np; ++i)
    {
      data = dataArray[i];
      for (j = 0; j < sizeOfArray; ++j)
	PRINTF("data[%u][%u] = %f", i,j, data[j]);
    }

  /* ocrGuid_t printDBTemplate; */
  /* ocrEdtTemplateCreate(&printDBTemplate, printDB, 0, 1);   */

  /* // Create db */


  /* // Create EDTs */
  /* ocrGuid_t dummy; */
  /* u64 nparamv[2*N]; */
  /* for (i = 0; i < N; ++i) */
  /*   { */
  /*     nparamv[2*i] = i*M; */
  /*     nparamv[2*i+1] = N*M; */
  /*     ocrEdtCreate(&dummy, fortranMainTemplate, 2, &nparamv[2*i], 1,  */
  /* 		   &dataGuid, EDT_PROP_NONE, NULL_GUID, NULL); */
  /*   } */

  // Add Dependencies

 

  PRINTF("n = %u\n", n);
  return;
}

ocrGuid_t printDB(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  
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
