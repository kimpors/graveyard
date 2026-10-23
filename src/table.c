#include "table.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "row.h"

Table *db_open(const char *filename)
{
	Pager *pg = pager_open(filename);

  	Table *tb = malloc(sizeof(Table));
	tb->pager = pg;
	tb->rows = pg->len / ROW_SIZE;
	return tb;
}

void db_close(Table *tb)
{
	Pager *pg = tb->pager;
	size_t full_pages = tb->rows / ROWS_PER_PAGE;

	for (size_t i = 0; i < full_pages; i++)
	{
		if (!pg->pages[i]) continue;
		pager_flush(pg, i, PAGE_SIZE);
		free(pg->pages[i]);
		pg->pages[i] = NULL;
	}

	size_t rows = tb->rows % ROWS_PER_PAGE;
	if (rows > 0)
	{
		size_t index = full_pages;
		if (pg->pages[index])
		{
			pager_flush(pg, index, rows * ROW_SIZE);
			free(pg->pages[index]);
			pg->pages[index] = NULL;
		}
	}

	int res = close(pg->fd);
	if (res == -1)
	{
		printf("[Error]Something went wrong while closing file\n");
		exit(EXIT_FAILURE);
	}

	for (size_t i = 0; i < TABLE_PAGES_COUNT; i++)
	{
		void *page = pg->pages[i];
		if (page)
		{
			free(page);
			pg->pages[i] = NULL;
		}
	}

	free(pg);
	free(tb);
}

void pager_flush(Pager *pager, size_t index, size_t size)
{
	if (!pager->pages[index])
	{
		printf("[Error]Tried to flush null page\n");
		exit(EXIT_FAILURE);
	}

	off_t offset = lseek(pager->fd, index * PAGE_SIZE, SEEK_SET);
	if (offset == -1)
	{
		printf("[Error]Something went wrong while seeking: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	ssize_t written = write(pager->fd, pager->pages[index], size);
	if (written == -1)
	{
		printf("[Error]Something went wrong while writing: %d\n", errno);
		exit(EXIT_FAILURE);
	}
}

Pager *pager_open(const char *filename)
{
	int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);

	if (fd == -1)
	{
		printf("[Error]Unable to open file\n");
		exit(EXIT_FAILURE);
	}

	Pager *pg = calloc(1, sizeof(Pager));
	pg->fd = fd;
	pg->len = lseek(fd, 0, SEEK_END);
	return pg;
}

void *get_page(Pager *pager, size_t index)
{
	if (index > TABLE_PAGES_COUNT)
	{
		printf("[Error]Page index out of bounds. %ld > %d\n",
				index, TABLE_PAGES_COUNT);
		exit(EXIT_FAILURE);
	}

	if (pager->pages[index])
	{
 		return pager->pages[index];
	}

	void *page = malloc(PAGE_SIZE);
	size_t pages = pager->len / PAGE_SIZE;

	if (pager->len % PAGE_SIZE) pages++;

	if (index <= pages)
	{
		lseek(pager->fd, index * PAGE_SIZE, SEEK_SET);
		ssize_t readed = read(pager->fd, page, PAGE_SIZE);

		if (readed == -1)
		{
			printf("[Error]Something went wrong while reading file: %d\n", errno);
			exit(EXIT_FAILURE);
		}
	}

	return pager->pages[index] = page;
}
