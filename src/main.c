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
  char *data;
  size_t len;
  size_t cap;
} InputBuffer;
typedef struct {
  StatementType type;
} Statement;

typedef struct {
  size_t rows;
  void *pages[TABLE_PAGES_COUNT];
} Table;
{
  if (!strcmp(buf, ";exit"))
  {
    exit(EXIT_SUCCESS);
  }

  return COMMAND_UNRECOGNIZED;
}

PrepareResult prepare_statement(InputBuffer *buf, Statement *st)
{

  if (!strncmp(buf->data, "insert", 6))
  {
    st->type = STATEMENT_INSERT;

    int args = sscanf(buf->data, "insert %ld %s %s",
                      &(st->row.id), st->row.name, st->row.email);

    if (args < 3)
    {
      return PREPARE_SYNTAX_ERROR;
    }

    return PREPARE_SUCCESS;
  }
  else if (!strncmp(buf->data, "select", 6))
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
    return execute_insert(st, tb);
    case STATEMENT_SELECT:
    return execute_select(st, tb);
  }
InputBuffer *new_buffer()
{
  InputBuffer *buf = malloc(sizeof(InputBuffer));
  buf->data = NULL;
  buf->len = 0;
  buf->cap = 0;
  return buf;
}

Table *new_table()
{
  Table *tb = malloc(sizeof(Table));
  tb->rows = 0;
  for (size_t i = 0; i < TABLE_PAGES_COUNT; i++)
  {
    tb->pages[i] = NULL;
  }

  return tb;
}

void free_table(Table *tb)
{
  for (size_t i = 0; i < TABLE_PAGES_COUNT; i++)
  {
    free(tb->pages[i]);
    tb->pages[i] = NULL;
  }

  free(tb);
}

int main(void) 
{
  int len;
  InputBuffer *buf = new_buffer();
  Table *tb = new_table();
  printf("### Graveyard ###\n");

  do 
  {
    printf("> ");
    len = getline(&(buf->data), &(buf->cap), stdin);

    if (len <= 0)
    {
      puts("Error reading input");
      exit(EXIT_FAILURE);
    }

    buf->len = len - 1;
    buf->data[len - 1] = '\0';

    if (buf->data[0] == ';')
    {
      switch (execute_command(buf))
      {
        case COMMAND_SUCCESS: break;
        case COMMAND_UNRECOGNIZED:
          printf("[Error]Unrecognized keyword int a command: '%s'\n", buf->data);
        continue;
      }
    }

    Statement st;
    switch(prepare_statement(buf, &st))
    {
      case PREPARE_SUCCESS: break;
      case PREPARE_SYNTAX_ERROR:
        printf("[Error]Could not parse statement: '%s'\n", buf->data);
      continue;
      case PREPARE_UNRECOGNIZED:
        printf("[Error]Unrecognized keyword in a statement: '%s'\n", buf->data);
      continue;
    }

    switch(execute_statement(&st, tb))
    {
      case EXECUTE_SUCCESS:
        puts("(Executed)");
      break;
      case EXECUTE_TABLE_FULL:
        printf("[Error]Table is full");
      break;
    }
  }while (true);

  return 0;
}
