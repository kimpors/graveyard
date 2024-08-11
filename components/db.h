#pragma once
#include "../utils/list.h"

typedef struct {
	const char *name;
	List tables;
} Db;

Db *createdb(const char *name);
Db *getdb(const char *name);
void printdb(const Db *db);
