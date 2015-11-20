#ifndef COCR_H_
#define COCR_H_


typedef struct {
  ocrEdtDep_t lHaloReadGuid;
  ocrEdtDep_t lHaloWriteGuid;
  ocrEdtDep_t data;
  ocrEdtDep_t rHaloRead; 
  ocrEdtDep_t rHaloWrite;
  int rank;
  int nx;
  int h;
} haloArray_t;

void createHaloArrays(haloArray_t *, u64, u64, u64);
ocrGuid_t fortranMain(u32, u64 *, u32, ocrEdtDep_t[]);


#endif COCR_H_
