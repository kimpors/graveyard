#include <stdio.h>
#include <stdlib.h>

#include "db.h"
#include "tb.h"
#include "../grave.h"
#include "../utils/util.h"

Db *createdb(const char *name)
{
	sprintf(curr_path, "%s/%s", path, name);
	create_dir(curr_path);

	Db *res = malloc(sizeof(Db *));
	res->name = name;
	res->tables.head = NULL;
	return res;
}

void printdb(const Db *db)
{
	printf("name: %s\n", db->name);

	printf("tables:\n");
	for (Node *ptr = db->tables.head; ptr != NULL; ptr = ptr->next)
	{
		printtb(((const Tb *)ptr->data));
	}
}

// Db *getdb(const char *name)
// {
// 	for (Node *ptr = dbs.head; ptr != NULL; ptr = ptr->next)
// 	{
// 		if (dbs.cmp(ptr->data, name) == 0)
// 		{
// 			return (Db *)ptr->data;
// 		}
// 	}
//
// 	return NULL;
// }
