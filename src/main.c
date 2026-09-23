#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFLEN 256

typedef enum {
  COMMAND_SUCCESS,
  COMMAND_UNRECOGNIZED
} CommandResult;

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_UNRECOGNIZED
} PrepareResult;

typedef enum {
  STATEMENT_INSERT,
  STATEMENT_SELECT
} StatementType;

typedef struct {
  StatementType type;
} Statement;

CommandResult execute_command(char *buf)
{
  if (!strcmp(buf, ";exit"))
  {
    exit(EXIT_SUCCESS);
  }

  return COMMAND_UNRECOGNIZED;
}

PrepareResult prepare_statement(char *buf, Statement *st)
{

  if (!strncmp(buf, "insert", 6))
  {
    st->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }
  else if (!strncmp(buf, "select", 6))
  {
    st->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED;
}

void execute_statement(Statement *st)
{
  switch (st->type)
  {
    case STATEMENT_INSERT:
      puts("Insert statement executed");
    break;
    case STATEMENT_SELECT:
      puts("Select statement executed");
    break;
  }
}

int main(void) 
{
  printf("### Graveyard ###\n");

  char buf[BUFLEN];

  do 
  {
    printf("> ");
    scanf("%255s", buf);

    if (!strcmp(buf, ";exit"))
    {
      break;
    }
    else 
    {
      printf("Unexpected command: '%s'\n", buf);
    }
  }while (true);

  return 0;
}
