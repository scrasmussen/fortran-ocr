#ifndef COCR_H_
#define COCR_H_


typedef struct {
  ocrGuid_t guid;
  ocrGuid_t event;  
} Trigger_t;

typedef struct {
  ocrEdtDep_t read;
  ocrEdtDep_t write;

  ocrGuid_t *oddTriggerGuid;
  ocrGuid_t *evenTriggerGuid;
  ocrGuid_t oddTriggerEvent;
  ocrGuid_t evenTriggerEvent;

} Halo_t;

typedef struct HaloArray_t{
  Halo_t left;
  ocrEdtDep_t data;
  ocrEdtDep_t buffer;
  Halo_t right;
  u64 rank;
  u64 nx;
  u64 h;
  u64 t;

  Trigger_t oddTrigger;
  Trigger_t evenTrigger;

  ocrGuid_t template;
  void *ptr;
  ocrGuid_t guid;
  ocrGuid_t test;
} HaloArray_t;


void createHaloArrays(HaloArray_t **, u64, u64, u64, u64);
void initHaloArrays(HaloArray_t **, u64, u64, u64);
ocrGuid_t fortranMain(u32, u64 *, u32, ocrEdtDep_t[]);
ocrGuid_t recFunc(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[]);


#endif COCR_H_
