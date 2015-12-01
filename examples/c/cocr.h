#ifndef COCR_H_
#define COCR_H_

typedef struct {
  ocrEdtDep_t read;
  ocrEdtDep_t write;
} Halo_t;

typedef struct HaloArray_t{
  Halo_t left;
  ocrEdtDep_t data;
  Halo_t right;
  u64 rank;
  u64 nx;
  u64 h;
  u64 t;

  ocrGuid_t evenTriggerGuid;
  ocrGuid_t oddTriggerGuid;
  ocrGuid_t evenTriggerEvent;
  ocrGuid_t oddTriggerEvent;

  ocrGuid_t template;
  void *ptr;
  ocrGuid_t guid;
  ocrGuid_t test;
} HaloArray_t;


void createSingleHaloArrays(HaloArray_t **, u64, u64, u64, u64);
void initSingleHaloArrays(HaloArray_t **, u64, u64, u64);

void createHaloArrays(HaloArray_t *, u64, u64, u64, u64);
void initHaloArrays(HaloArray_t *, u64, u64);
ocrGuid_t fortranMain(u32, u64 *, u32, ocrEdtDep_t[]);
void startRec(HaloArray_t haloArray[], u64 np, u64 nx, u64 h, u64 t);
ocrGuid_t recFunc(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[]);

ocrGuid_t testFunc(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[]);
ocrGuid_t testFunc2(u32 paramc, u64 *paramv, u32 depc, ocrEdtDep_t depv[]);
void testHaloArrays(HaloArray_t *, u64);

#endif COCR_H_