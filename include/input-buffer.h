#pragma once
#include <stdio.h>
#include <stdint.h>

typedef struct {
  char *data;
  uint64_t len;
  uint64_t cap;
} InputBuffer;

InputBuffer *init_input_buffer(void);
void free_input_buffer(InputBuffer *buf);
int readline(InputBuffer *buf, FILE *stream);
