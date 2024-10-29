#include "grave.h"
#include <stdio.h>

void printdb(const Db *db)
{
	printf("db: %s\n", db->name);
}

void printtb(const Tb *tb)
{
	printf("tb: %s\n", tb->name);
}

void printfl(const Fl *fl)
{
	printf("fl: %s|%d\n", fl->name, fl->type);
}
