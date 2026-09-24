#pragma once
#include <stdlib.h>

#define PAGE_SIZE                       4096
#define TABLE_PAGES_COUNT               100

typedef struct {
  size_t rows;
  void *pages[TABLE_PAGES_COUNT];
} Table;

Table *init_table();
void free_table(Table *tb);
