#ifndef LABELED_H
#define LABELED_H

#include <ocr.h>
#define GUID_USER_EVENT_STICKY 19

ocrEdtDep_t GuidRangeCreate(ocrGuid_t * mapGuid, u64 numberGuid, u64 kind);
u8 GuidFromIndex(ocrGuid_t *outGuid, ocrGuid_t *mapGuid, u64 index);

#endif
