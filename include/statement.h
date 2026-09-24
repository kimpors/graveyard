#pragma once
#include "row.h"
#include "input-buffer.h"

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_UNRECOGNIZED,
  PREPARE_SYNTAX_ERROR
} PrepareResult;

typedef enum {
  EXECUTE_SUCCESS,
  EXECUTE_TABLE_FULL
} ExecuteResult;

typedef enum {
  STATEMENT_INSERT,
  STATEMENT_SELECT
} StatementType;

typedef struct {
  StatementType type;
  Row row;
} Statement;


PrepareResult prepare_statement(InputBuffer *buf, Statement *st);
ExecuteResult execute_insert(Statement *st, Table *tb);
ExecuteResult execute_select(Statement *st, Table *tb);
ExecuteResult execute_statement(Statement *st, Table *tb);
