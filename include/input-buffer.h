#pragma once
#include <stdint.h>

typedef struct {
  char *data;
  uint64_t len;
  uint64_t cap;
} InputBuffer;

InputBuffer *init_input_buffer(void);
void free_input_buffer(InputBuffer *buf);
