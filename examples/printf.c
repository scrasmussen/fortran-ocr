#include "ocr.h"
#include <string.h>

void printf_str(char * str)
{
  PRINTF("%s\n", str);
}

void printf_i(char * str, int i)
{
  PRINTF("%s: %d\n", str, i);
}

void printf_i32(char * str, u32 i)
{
  
  PRINTF("%s: %d\n", str, i);
}

void printf_p(char * str, void *p)
{
  PRINTF("%s: %p\n", str, p);
}

void printf_pi(char * str, void *p)
{
  PRINTF("%s: %p\n", str, p);
}
