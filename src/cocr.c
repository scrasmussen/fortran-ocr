#include <stdio.h>
#include <stdlib.h>
#include <ocr.h>
#include "cocr.h"

void SPMD_Finalize(info_t info)
{
  ocrEventSatisfy(info.evt_finalize, NULL_GUID);
}

void SPMD_Init(int size, int dim, int topology, int np, void *mainF90Edt, 
	       void *syncEdt, void *finalizeEdt, Info info)
{
  int i, j;
  ocrGuid_t tmp;
  info_t **thread;
  thread = &info.thread[0];
  
  int numNeighbor = 2;
  for (i = 0; i < np; ++i)
    ocrDbCreate(&tmp, (void **) &thread[i], sizeof(info_t) + sizeof(neighborSync_t) * (numNeighbor - 1), DB_PROP_NONE, NULL_GUID, NO_ALLOC);
  
  // INIT Templates
  ocrGuid_t mainTemplate, syncTemplate, finalizeTemplate;
  ocrEdtTemplateCreate(&mainTemplate, mainF90Edt, 0, 1 + topology);
  ocrEdtTemplateCreate(&syncTemplate, syncEdt, 0, 1 + topology);
  ocrEdtTemplateCreate(&finalizeTemplate, finalizeEdt, 0, np);

  for (i = 0; i < np; ++i) {
    thread[i]->tpl_sync = syncTemplate;
    // NOTE: is this the right template?
    thread[i]->tpl_compute = mainTemplate;
  }
  
  // INIT EDT's
  for (i = 0; i < np; ++i) {
    ocrEdtCreate(&thread[i]->edt_next_sync, syncTemplate, EDT_PARAM_DEF, NULL, 
		 EDT_PARAM_DEF, NULL, EDT_PROP_NONE, NULL_GUID, 
		 &thread[i]->evt_out_next_sync);
    // NOTE: is this the right template?
    ocrEdtCreate(&thread[i]->edt_next_compute, mainTemplate, EDT_PARAM_DEF, 
		 NULL,EDT_PARAM_DEF, NULL, EDT_PROP_NONE, NULL_GUID, 
		 &thread[i]->evt_out_next_compute);
  }
  ocrGuid_t edt_finalize;
  ocrEdtCreate(&edt_finalize, finalizeTemplate, EDT_PARAM_DEF, NULL, 
	       EDT_PARAM_DEF, NULL, EDT_PROP_NONE, NULL_GUID, NULL);

  // INIT Neightbor Events
  for (i = 0; i < np; ++i) {
    // left neighbor
    thread[i]->neighbor[0].edt_next_sync = &thread[(i-1+np)%np]->edt_next_sync;
    thread[i]->neighbor[0].edt_next_compute = &thread[(i-1+np)%np]->edt_next_compute;
    // right neighbor
    thread[i]->neighbor[1].edt_next_sync = &thread[(i+1)%np]->edt_next_sync;
    thread[i]->neighbor[1].edt_next_compute = &thread[(i+1)%np]->edt_next_compute;
  }

  // INIT Events
  for (i = 0; i < np; ++i) {
    // finalize
    ocrEventCreate(&thread[i]->evt_finalize, OCR_EVENT_ONCE_T, EVT_PROP_NONE);
    // self
    ocrEventCreate(&thread[i]->evt_next_sync, OCR_EVENT_ONCE_T, EVT_PROP_NONE);
    ocrEventCreate(&thread[i]->evt_next_compute, OCR_EVENT_ONCE_T, EVT_PROP_NONE);
    // left neighbor
    ocrEventCreate(&thread[i]->neighbor[0].evt_next_sync, OCR_EVENT_ONCE_T, EVT_PROP_NONE);
    ocrEventCreate(&thread[i]->neighbor[0].evt_next_compute, OCR_EVENT_ONCE_T, EVT_PROP_NONE);
    // right neighbor
    ocrEventCreate(&thread[i]->neighbor[1].evt_next_sync, OCR_EVENT_ONCE_T, EVT_PROP_NONE);
    ocrEventCreate(&thread[i]->neighbor[1].evt_next_compute, OCR_EVENT_ONCE_T, EVT_PROP_NONE);

  }

  // INIT Dependencies
  // int start = 1; // start = 0 is for the datablock?
  int start = 0; // until db's are added
  for (i = 0; i < np; ++i) {
    ocrAddDependence(thread[i]->evt_finalize, edt_finalize, i, DB_MODE_RW);
    ocrAddDependence(thread[i]->edt_next_sync, thread[i]->edt_next_sync ,start,DB_MODE_RW);
    ocrAddDependence(thread[i]->neighbor[0].evt_next_sync, *thread[i]->neighbor[0].edt_next_sync, start + 1, DB_MODE_RW); // left neighbor evt
    ocrAddDependence(thread[i]->neighbor[1].evt_next_sync, *thread[i]->neighbor[1].edt_next_sync, start + 2, DB_MODE_RW); // right neighbor evt
  }
  return;
}





/*
ocrGuid_t recFunc(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[])
{
  HaloArray_t *ptr; // = depv[0].ptr;
  ptr = (HaloArray_t*)depv[0].ptr;
  ocrGuid_t dataGuid = depv[0].guid;
  u64 t = ptr->t;
  PRINTF("rank = %u  t = %u \n", ptr->rank, t);
  ptr->t-=1;
  
  if (t == 0)
    {
      PRINTF("=FINISHED=\n");
      if (ptr->rank == 0) {
	PRINTF("=SHUTDOWN=\n");
	ocrShutdown();
      }
      return NULL_GUID;
    }

  // DO CALCULATIONS BEEP-BOOP

  // TRANSFER
  ocrGuid_t recTemplate;
  recTemplate = ptr->template;

  // If even, create evenTrigger Edt Guid, add and satisfy odd Triggers, if 
  // odd do  oppisite.
  if (t%2 == 0) {
    ocrEdtCreate(&ptr->evenTrigger.guid, recTemplate, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, NULL, EDT_PROP_NONE, NULL_GUID, NULL);

    ocrAddDependence(dataGuid, ptr->oddTrigger.guid, 0, DB_MODE_RW);

    ocrEventCreate(&ptr->oddTrigger.event,OCR_EVENT_ONCE_T,EVT_PROP_NONE);
    ocrAddDependence(ptr->oddTrigger.event, ptr->oddTrigger.guid, 1,
    		     DB_MODE_RW);
    ocrEventCreate(&ptr->left.oddTriggerEvent,OCR_EVENT_ONCE_T,EVT_PROP_NONE);
    ocrAddDependence(ptr->left.oddTriggerEvent, *ptr->left.oddTriggerGuid, 2,
		     DB_MODE_RW);
    ocrEventCreate(&ptr->right.oddTriggerEvent,OCR_EVENT_ONCE_T,EVT_PROP_NONE);
    ocrAddDependence(ptr->right.oddTriggerEvent, *ptr->right.oddTriggerGuid, 3,
		     DB_MODE_RW);

    ocrEventSatisfy(ptr->left.oddTriggerEvent, NULL);
    ocrEventSatisfy(ptr->oddTrigger.event, NULL);
    ocrEventSatisfy(ptr->right.oddTriggerEvent, NULL);
  }
  else {
    ocrEdtCreate(&ptr->oddTrigger.guid, recTemplate, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, NULL, EDT_PROP_NONE, NULL_GUID, NULL);
    ocrAddDependence(dataGuid, ptr->evenTrigger.guid, 0, DB_MODE_RW);
    ocrEventCreate(&ptr->evenTrigger.event,OCR_EVENT_ONCE_T,EVT_PROP_NONE);
    ocrAddDependence(ptr->evenTrigger.event, ptr->evenTrigger.guid, 1, DB_MODE_RW);
    ocrEventSatisfy(ptr->evenTrigger.event, NULL);
    ocrEventCreate(&ptr->left.evenTriggerEvent,OCR_EVENT_ONCE_T,EVT_PROP_NONE);
    ocrAddDependence(ptr->left.evenTriggerEvent, *ptr->left.evenTriggerGuid, 2, DB_MODE_RW);
    ocrEventSatisfy(ptr->left.evenTriggerEvent, NULL);
    ocrEventCreate(&ptr->right.evenTriggerEvent,OCR_EVENT_ONCE_T,EVT_PROP_NONE);
    ocrAddDependence(ptr->right.evenTriggerEvent, *ptr->right.evenTriggerGuid, 3, DB_MODE_RW);
    ocrEventSatisfy(ptr->right.evenTriggerEvent, NULL);
  }

  return NULL_GUID;
}
*/

 /*
void initHaloArrays(HaloArray_t *haloArray[], u64 np, u64 nx, u64 t)
{
  int i, j;
  // fill with data
  for (i = 0; i < np; ++i) {
    for (j = 0; j < nx; ++j)
      ((double*) haloArray[i]->data.ptr)[j] = i*7+j+1;
    ((double*) haloArray[i]->left.write.ptr)[0] =
      ((double*)haloArray[i]->data.ptr)[0];
    ((double*) haloArray[i]->right.write.ptr)[0] =
      ((double*)haloArray[i]->data.ptr)[nx-1];
  }

  ocrGuid_t recTemplate;
  ocrEdtTemplateCreate(&recTemplate, recFunc, 0, 4);

  for (i = 0; i < np; ++i) {    
    haloArray[i]->t = t;  
    haloArray[i]->template = recTemplate;
    // create Edt Guids
    ocrEdtCreate(&haloArray[i]->evenTrigger.guid, recTemplate, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, NULL, EDT_PROP_NONE, NULL_GUID, NULL);
    ocrEdtCreate(&haloArray[i]->oddTrigger.guid, recTemplate, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, NULL, EDT_PROP_NONE, NULL_GUID, NULL);
  }

  for (i = 0; i < np; ++i) {
    // setup left/right Trigger Guids
    haloArray[i]->left.evenTriggerGuid = &haloArray[(i-1+np)%np]->evenTrigger.guid;
    haloArray[i]->left.oddTriggerGuid = &haloArray[(i-1+np)%np]->oddTrigger.guid;
    haloArray[i]->right.evenTriggerGuid = &haloArray[(i+1)%np]->evenTrigger.guid;
    haloArray[i]->right.oddTriggerGuid = &haloArray[(i+1)%np]->oddTrigger.guid;
    
    // create Trigger Events
    ocrEventCreate(&haloArray[i]->evenTrigger.event,OCR_EVENT_ONCE_T,EVT_PROP_NONE);
    ocrEventCreate(&haloArray[i]->left.evenTriggerEvent,OCR_EVENT_ONCE_T,EVT_PROP_NONE);
    ocrEventCreate(&haloArray[i]->right.evenTriggerEvent,OCR_EVENT_ONCE_T,EVT_PROP_NONE);
    ocrEventCreate(&haloArray[i]->oddTrigger.event,OCR_EVENT_ONCE_T,EVT_PROP_NONE);
    ocrEventCreate(&haloArray[i]->left.oddTriggerEvent,OCR_EVENT_ONCE_T,EVT_PROP_NONE);
    ocrEventCreate(&haloArray[i]->right.oddTriggerEvent,OCR_EVENT_ONCE_T,EVT_PROP_NONE);
  }

  // add dependencies
  if (t%2 == 0) {
    for (i = 0; i < np; ++i) {
      ocrAddDependence(haloArray[i]->guid, haloArray[i]->evenTrigger.guid, 0, DB_MODE_RW);
      ocrAddDependence(haloArray[i]->evenTrigger.event, haloArray[i]->evenTrigger.guid, 1, DB_MODE_RW);
      ocrAddDependence(haloArray[i]->left.evenTriggerEvent, *haloArray[i]->left.evenTriggerGuid, 2, DB_MODE_RW);
      ocrAddDependence(haloArray[i]->right.evenTriggerEvent, *haloArray[i]->right.evenTriggerGuid, 3, DB_MODE_RW);
    }
  }
  else {
    for (i = 0; i < np; ++i) {
      ocrAddDependence(haloArray[i]->guid, haloArray[i]->oddTrigger.guid, 0, DB_MODE_RW);
      ocrAddDependence(haloArray[i]->oddTrigger.event, haloArray[i]->oddTrigger.guid, 1, DB_MODE_RW);
      ocrAddDependence(haloArray[i]->left.oddTriggerEvent, *haloArray[i]->left.oddTriggerGuid, 2, DB_MODE_RW);
      ocrAddDependence(haloArray[i]->right.oddTriggerEvent, *haloArray[i]->right.oddTriggerGuid, 3, DB_MODE_RW);
    }
  }

  PRINTF("Init Done\n");
}


void createHaloArrays(HaloArray_t *haloArray[], u64 np, u64 nx, u64 h, u64 t)
{
  u64 i=0;
  ocrGuid_t temp;

  for (i = 0; i < np; ++i) {
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
    ocrDbCreate(&haloArray[i]->buffer.guid,
    		(void**) &haloArray[i]->buffer.ptr, sizeof(double)*nx,
    		DB_PROP_NONE, NULL_GUID, NO_ALLOC);
  }
  for (i = 0; i < np; ++i) {
    haloArray[i]->left.read = haloArray[(i-1+np)%np]->right.write;
    haloArray[i]->right.read = haloArray[(i+1)%np]->left.write;
    haloArray[i]->rank = i;
  }
  return;
}

*/
