#include <string.h>
#include <stdlib.h>
#include "command.h"

CommandResult execute_command(InputBuffer *buf, Table *tb)
{
  if (!strcmp(buf->data, ";exit"))
  {
		db_close(tb);
    	exit(EXIT_SUCCESS);
  }

  return COMMAND_UNRECOGNIZED;
}
