#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include "util.h"
#include "db.h"

#define MAX_DBS		100

char path[PATH_MAX];
char curr_path[PATH_MAX];

static int len;
static Db dbs[MAX_DBS];

Db *createdb(const char *name)
{
	sprintf(curr_path, "%s/%s", path, name);
	create_dir(curr_path);
	dbs[len].name = name;
	return &dbs[len++];
}

// TODO make lookup list
Db *getdb(const char *name)
{
	int i;
	int state = 0;

	for (i = 0; i < len; i++)
	{
		if (strcmp(dbs[i].name, name) == 0)
		{
			state = 1;
			break;
		}
	}

	if (!state)
	{
		return NULL;
	}

	return &dbs[i];
}

// TODO make lookup list
Tb *gettb(const Db *db, const char *name)
{
	int i;
	int state = 0;

	for (i = 0; i < db->len; i++)
	{
		if (strcmp(db->tables[i]->name, name) == 0)
		{
			state = 1;
			break;
		}
	}

	if (!state)
	{
		return NULL;
	}

	return db->tables[i];
}

Tb *createtb(Db *db, const char *name)
{
	sprintf(curr_path, "%s/%s/%s", path, db->name, name);
	create_dir(curr_path);

	sprintf(curr_path, "%s/%s/%s/data", path, db->name, name);
	create_dir(curr_path);

	db->tables[db->len] = malloc(sizeof(Tb));
	db->tables[db->len]->name = name;

	return db->tables[db->len++];
}

void writetb(const Db *db, Tb *tb, const Field fields[MAX], const unsigned n)
{
	sprintf(curr_path, "%s/%s/%s/%s.grv", path, db->name, tb->name, tb->name);

	FILE *fp;
	if ((fp = fopen(curr_path, "wb")) == NULL)
	{
		fprintf(stderr, "writetb: can't open file %s\n", curr_path);
		exit(-1);
	}

	tb->len = n;

	for (int i = 0; i < n; i++)
	{
		fprintf(fp, "%s %d\n", fields[i].name, fields[i].type);
		tb->fields[i] = fields[i];
	}

	fclose(fp);
}

int readtb(const Db *db, const Tb *tb, Field *dest)
{
	sprintf(curr_path, "%s/%s/%s/%s.grv", path, db->name, tb->name, tb->name);

	FILE *fp;
	if ((fp = fopen(curr_path, "rb")) == NULL)
	{
		fprintf(stderr, "writetb: can't open file %s\n", curr_path);
		exit(-1);
	}

	Field *pdest = dest;

	while (fscanf(fp, "%s %d\n", pdest->name, &pdest->type) != EOF)
	{
		pdest++;
	}

	return pdest - dest;
}

void save_data(const Db *db, const Tb *tb, const Data *data, unsigned int len)
{
	sprintf(curr_path, "%s/%s/%s/data/1", path, db->name, tb->name);

	FILE *fp;
	if ((fp = fopen(curr_path, "wb")) == NULL)
	{
		fprintf(stderr, "save_data: can't open file %s\n", curr_path);
		exit(-1);
	}

	int pos = 0;
	for (int i = 0; i < len; i++)
	{
		if (pos >= tb->len) pos = 0;

		switch (tb->fields[pos].type)
		{
			case INT:
				fwrite(&data[i].n, sizeof(int), 1, fp);
				break;
			case FLOAT:
				fwrite(&data[i].f, sizeof(float), 1, fp);
				break;
		}
	}

	fclose(fp);
}

int get_data(const Db *db, const Tb *tb, Data *data)
{
	sprintf(curr_path, "%s/%s/%s/data/1", path, db->name, tb->name);

	FILE *fp;
	if ((fp = fopen(curr_path, "rb")) == NULL)
	{
		fprintf(stderr, "save_data: can't open file %s\n", curr_path);
		exit(-1);
	}

	int pos = 0, len = 0;
	for (int i = 0; i < tb->len; i++, len++)
	{
		if (pos >= tb->len) pos = 0;

		switch (tb->fields[pos].type)
		{
			case INT:
				fread(&data[i].n, sizeof(int), 1, fp);
				break;
			case FLOAT:
				fread(&data[i].f, sizeof(float), 1, fp);
				break;
		}
	}

	fclose(fp);
	return len;
}

// TODO make it better
void showdbs(void)
{
	for (int i = 0; i < len; i++)
	{
		printf("name - %s\n", dbs[i].name);
		printf("tables:\n");

		for (int j = 0; j < dbs[i].len; j++)
		{
			printf("table: %s\n", dbs[i].tables[j]->name);
		}

		putchar('\n');
	}
}
