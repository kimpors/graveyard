#include <stdio.h>
#include <stdlib.h>

#include "tb.h"
#include "db.h"
#include "../grave.h"
#include "../utils/util.h"

Tb *createtb(Db *db, const char *name)
{
	sprintf(curr_path, "%s/%s/%s", path, db->name, name);
	create_dir(curr_path);

	sprintf(curr_path, "%s/%s/%s/data", path, db->name, name);
	create_dir(curr_path);

	Tb *res = malloc(sizeof(Tb *));
	res->name = name;
	res->fields.head = NULL;
	push(&db->tables, res);
	return res; 
}

void printtb(const Tb *tb)
{
	printf("name = %s\n", tb->name);
	printf("fields:\n");

	for (Node *ptr = tb->fields.head; ptr != NULL; ptr = ptr->next)
	{
		printf("name = %s\n", ((Field *)ptr->data)->name);
		printf("type = %d\n", ((Field *)ptr->data)->type);
	}
}

// Tb *gettb(const Db *db, const char *name)
// {
// 	for (Node *ptr = db->tables.head; ptr != NULL; ptr = ptr->next)
// 	{
// 		if (db->tables.cmp(ptr->data, name) == 0)
// 		{
// 			return (Tb *) ptr->data;
// 		}
// 	}
//
// 	return NULL;
// }

void writetb(const Db *db, Tb *tb, const Field fields[255], const unsigned n)
{
	sprintf(curr_path, "%s/%s/%s/%s.grv", path, db->name, tb->name, tb->name);

	FILE *fp;
	if ((fp = fopen(curr_path, "wb")) == NULL)
	{
		fprintf(stderr, "writetb: can't open file %s\n", curr_path);
		exit(-1);
	}

	for (int i = 0; i < n; i++)
	{
		fprintf(fp, "%s %d\n", fields[i].name, fields[i].type);
		push(&tb->fields, &fields[i]);
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
