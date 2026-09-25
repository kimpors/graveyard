#include <stdlib.h>
#include <stdio.h>
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

int readline(InputBuffer *buf, FILE *stream)
{
  ssize_t readed;
  readed = getline(&(buf->data), &(buf->cap), stream);

    if (readed <= 0)
    {
      puts("Error reading input");
      free_input_buffer(buf);
      return EXIT_FAILURE;
    }

    buf->len = readed - 1;
    buf->data[readed - 1] = '\0';

  return EXIT_SUCCESS;
}
