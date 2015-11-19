#ifndef COCR_H_
#define COCR_H_


typedef struct {
  float *A;
  float *lHalo;
  float *rHalo;
  int n;
  int h;
} haloArray_t;

void createHaloArrays(haloArray_t *, double *, u64, u64);
ocrGuid_t fortranMain(u32, u64 *, u32, ocrEdtDep_t[]);


#endif COCR_H_

