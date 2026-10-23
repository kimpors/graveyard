#include "row.h"
#include <string.h>

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
  void *page = get_page(tb->pager, index);
  size_t row_offset = row_index % ROWS_PER_PAGE;
  size_t byte_offset = row_offset * ROW_SIZE;
  return page + byte_offset;
}
