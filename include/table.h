#pragma once
#include <stdlib.h>

#define PAGE_SIZE                       4096
#define TABLE_PAGES_COUNT               100

typedef struct {
	int fd;
	off_t len;
	void *pages[TABLE_PAGES_COUNT];
} Pager;

typedef struct {
  size_t rows;
  Pager *pager;
} Table;

Table *db_open(const char *filename);
void db_close(Table *tb);
Pager *pager_open(const char *filename);
void *get_page(Pager *pager, size_t index);
void pager_flush(Pager *pager, size_t index, size_t size);
