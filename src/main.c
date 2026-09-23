#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define sizeof_attr(STRUCT, ATTRIBUTE)  sizeof(((STRUCT*)0)->ATTRIBUTE)

#define COL_NAME_LEN                    32
#define COL_EMAIL_LEN                   255
#define PAGE_SIZE                       4096
#define TABLE_PAGES_COUNT               100

typedef enum {
  COMMAND_SUCCESS,
  COMMAND_UNRECOGNIZED
} CommandResult;

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_UNRECOGNIZED,
  PREPARE_SYNTAX_ERROR
} PrepareResult;

typedef enum {
  STATEMENT_INSERT,
  STATEMENT_SELECT
} StatementType;

typedef enum {
  EXECUTE_SUCCESS,
  EXECUTE_TABLE_FULL
} ExecuteResult;

typedef struct {
  char *data;
  size_t len;
  size_t cap;
} InputBuffer;

typedef struct {
  size_t id;
  char name[COL_NAME_LEN];
  char email[COL_EMAIL_LEN];
} Row;

typedef struct {
  StatementType type;
  Row row;
} Statement;

typedef struct {
  size_t rows;
  void *pages[TABLE_PAGES_COUNT];
} Table;

const unsigned ID_SIZE          = sizeof_attr(Row, id);
const unsigned NAME_SIZE        = sizeof_attr(Row, name);
const unsigned EMAIL_SIZE       = sizeof_attr(Row, email);

const unsigned ID_OFFSET        = 0;
const unsigned NAME_OFFSET      = ID_OFFSET + ID_SIZE;
const unsigned EMAIL_OFFSET     = NAME_OFFSET + NAME_SIZE;
const unsigned ROW_SIZE         = ID_SIZE + NAME_SIZE + EMAIL_SIZE;

const unsigned ROWS_PER_PAGE    = PAGE_SIZE / ROW_SIZE;
const unsigned TABLE_ROW_COUNT  = ROWS_PER_PAGE / TABLE_PAGES_COUNT;

void serialize_row(Row *src, void *dst)
{
  memcpy(dst + ID_OFFSET, &(src->id), ID_SIZE);
  memcpy(dst + NAME_OFFSET, &(src->name), NAME_SIZE);
  memcpy(dst + EMAIL_OFFSET, &(src->email), EMAIL_OFFSET);
}

void deserialize_row(void *src, Row *dst)
{
  memcpy(&(dst->id), src + ID_OFFSET, ID_SIZE);
  memcpy(&(dst->name), src + NAME_OFFSET, NAME_SIZE);
  memcpy(&(dst->email), src + EMAIL_OFFSET, EMAIL_SIZE);
}

void *row_slot(Table *tb, size_t row_index)
{
  size_t index = row_index / ROWS_PER_PAGE;
  void *page = tb->pages[index];

  if (!page)
  {
    page = tb->pages[index] = malloc(PAGE_SIZE);
  }

  size_t row_offset = row_index % ROWS_PER_PAGE;
  size_t byte_offset = row_offset * ROW_SIZE;
  return page + byte_offset;
}

CommandResult execute_command(InputBuffer *buf)
{
  if (!strcmp(buf->data, ";exit"))
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

ExecuteResult execute_insert(Statement *st, Table *tb)
{
  if (tb->rows >= TABLE_PAGES_COUNT)
  {
    return EXECUTE_TABLE_FULL;
  }

  serialize_row(&(st->row), row_slot(tb, tb->rows));
  tb->rows++;

  return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement *st, Table *tb)
{
  Row row;
  for (size_t i = 0; i < tb->rows; i++)
  {
    deserialize_row(row_slot(tb, i), &row);
    printf("(%ld, %s, %s)\n", row.id, row.name, row.email);
  }

  return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(Statement *st, Table *tb)
{
  switch (st->type)
  {
    case STATEMENT_INSERT:
    return execute_insert(st, tb);
    case STATEMENT_SELECT:
    return execute_select(st, tb);
  }

  return EXECUTE_SUCCESS;
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
