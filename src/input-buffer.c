#include <stdlib.h>
#include "input-buffer.h"

InputBuffer *init_input_buffer(void)
{
  return calloc(1, sizeof(InputBuffer));
}

void free_input_buffer(InputBuffer *buf)
{
  free(buf);
  buf = NULL;
}
