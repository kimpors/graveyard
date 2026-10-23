#pragma once
#include "table.h"
#include "input-buffer.h"

typedef enum {
  COMMAND_SUCCESS,
  COMMAND_UNRECOGNIZED
} CommandResult;

CommandResult execute_command(InputBuffer *buf, Table *tb);
