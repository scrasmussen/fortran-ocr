#include "ocr.h"
#include <string.h>

void wprintf_str(char * str)
{
  PRINTF("%s\n", str);
}

void wprintf_i64(char * str, int64_t i)
{
  PRINTF("%s: %ld\n", str, i);
}

void wprintf_f(char * str, float i)
{
  PRINTF("%s: %f\n", str, i);
}

void wprintf_i32(char * str, int i)
{
  PRINTF("%s: %d\n", str, i);
}

void wprintf_p(char * str, void *p)
{
  PRINTF("%s: %p\n", str, p);
}
