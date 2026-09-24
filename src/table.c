#include "table.h"

Table *init_table()
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
