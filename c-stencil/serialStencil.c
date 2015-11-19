#include <ocr.h>
#include <stdio.h>
#include "newstencil1d.h"

void serialCompute(double *A)
{
  int t, i;
  double buf[N*M+2];
  for (i = 0; i<M*N+2; ++i)
      buf[i] = A[i];
  for (t = 1; t<=T; ++t)
    {
      for (i = 1; i<=M*N; ++i)
	A[i] = serialStencil(buf[i-1], A[i], buf[i+1]);
      for (i = 1; i<=M*N; ++i)
	buf[i] = A[i];
    }
  PRINTF("t = %u\n",t);
}

double serialStencil(double iminus, double i, double iplus)
{
  return (0.25*iminus + 0.5*i + 0.25*iplus);
}
