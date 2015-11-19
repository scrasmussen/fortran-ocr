#define N 10 // number of workers (EDTs)
#define M 1  // size of local block
#define T 4  // number of iterations
#define H 1  // size of halo

typedef struct {
  double halo;
  ocrGuid_t control;
} buffer_t;
