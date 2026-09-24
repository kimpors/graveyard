#pragma once
#include <stdlib.h>
#include "table.h"

#define COL_NAME_LEN                    32
#define COL_EMAIL_LEN                   255

#define sizeof_attr(STRUCT, ATTRIBUTE)  sizeof(((STRUCT*)0)->ATTRIBUTE)

typedef struct {
  size_t id;
  char name[COL_NAME_LEN];
  char email[COL_EMAIL_LEN];
} Row;

void serialize_row(Row *src, void *dst);
void deserialize_row(void *src, Row *dst);
void *row_slot(Table *tb, size_t row_index);

const static unsigned ID_SIZE          = sizeof_attr(Row, id);
const static unsigned NAME_SIZE        = sizeof_attr(Row, name);
const static unsigned EMAIL_SIZE       = sizeof_attr(Row, email);

const static unsigned ID_OFFSET        = 0;
const static unsigned NAME_OFFSET      = ID_OFFSET + ID_SIZE;
const static unsigned EMAIL_OFFSET     = NAME_OFFSET + NAME_SIZE;
const static unsigned ROW_SIZE         = ID_SIZE + NAME_SIZE + EMAIL_SIZE;

const static unsigned ROWS_PER_PAGE    = PAGE_SIZE / ROW_SIZE;
const static unsigned TABLE_ROW_COUNT  = ROWS_PER_PAGE / TABLE_PAGES_COUNT;
