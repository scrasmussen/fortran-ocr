#ifndef COCR_H_
#define COCR_H_


typedef struct {
  float *A;
  float *lHalo;
  float *rHalo;
  int n;
  int h;
} haloArray_t;

void createHaloArrays(haloArray_t *, u64);

#endif COCR_H_

