#define N 10   // number of workers (EDTs)
#define M 50  // size of local block
#define H 1  // number of iterations
#define T 10  // number of iterations

typedef struct {
  double halo;
  ocrGuid_t control;
} buffer_t;
