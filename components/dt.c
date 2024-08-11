#include <stdio.h>
#include <stdlib.h>

#include "db.h"
#include "tb.h"
#include "dt.h"
#include "../grave.h"

void printdt(List *fields, const Data *dt)
{
	int i = 0;
	for (Node *ptr = fields->head; ptr != NULL; ptr = ptr->next, i++)
	{
		switch (((Field *)ptr->data)->type)
		{
			case INT:
				printf("%5d|\t", dt[i].n);
				break;
			case FLOAT:
				printf("%5.2f|\t", dt[i].f);
				break;
		}
	}
}

void writedt(const Db *db, const Tb *tb, const Data *data, unsigned int len)
{
	sprintf(curr_path, "%s/%s/%s/data/1", path, db->name, tb->name);

	FILE *fp;
	if ((fp = fopen(curr_path, "wb")) == NULL)
	{
		fprintf(stderr, "save_data: can't open file %s\n", curr_path);
		exit(-1);
	}

	Node *ptr = tb->fields.head;
	for (int i = 0; i < len; i++)
	{
		if (ptr == NULL) ptr = tb->fields.head;

		switch (((Field *)ptr->data)->type)
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

void readdt(const Db *db, const Tb *tb, Data *data)
{
	sprintf(curr_path, "%s/%s/%s/data/1", path, db->name, tb->name);

	FILE *fp;
	if ((fp = fopen(curr_path, "rb")) == NULL)
	{
		fprintf(stderr, "save_data: can't open file %s\n", curr_path);
		exit(-1);
	}

	Node *ptr = tb->fields.head;
	int pos = 0, len = 0;

	int i = 0;
	for (Node *ptr = tb->fields.head; ptr != NULL; ptr = ptr->next, i++)
	{
		switch (((Field *) ptr->data)->type)
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
}
