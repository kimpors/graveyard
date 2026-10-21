#include <string.h>
#include <stdlib.h>
#include "command.h"

CommandResult execute_command(InputBuffer *buf)
{
  if (!strcmp(buf->data, ";exit"))
  {
    exit(EXIT_SUCCESS);
  }

  return COMMAND_UNRECOGNIZED;
}
