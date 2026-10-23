#include <stdio.h>
#include <string.h>
#include "statement.h"

PrepareResult prepare_statement(InputBuffer *buf, Statement *st)
{
  if (!strncmp(buf->data, "insert", 6))
  {
    st->type = STATEMENT_INSERT;

    int args = sscanf(buf->data, "insert %ld %32s %255s",
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
