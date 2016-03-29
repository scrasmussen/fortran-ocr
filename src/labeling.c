#include "labeling.h"

ocrEdtDep_t GuidRangeCreate(ocrGuid_t * mapGuid, u64 numberGuid, u64 kind)
{
  PRINTF("Entered GuidRangeCreate\n");

  ocrEdtDep_t array;
  ocrGuid_t *guidArray;
  /* u64 *guidArray; */
  ocrDbCreate(&array.guid, (void **) &guidArray, sizeof(ocrGuid_t) * numberGuid, DB_PROP_NONE, NULL_GUID, NO_ALLOC);
  /* ocrDbCreate(&array.guid, (void **) &guidArray, sizeof(ocrGuid_t) * numberGuid, DB_PROP_NONE, NULL_GUID, NO_ALLOC); */

  guidArray[0] = NULL_GUID;
  array.ptr = guidArray;

  return array; 
}


u8 GuidFromIndex(ocrGuid_t *outGuid, ocrGuid_t *mapGuid, u64 index)
{
  PRINTF("from index %p\n", mapGuid[index+1]);

  return NULL_GUID;
}
