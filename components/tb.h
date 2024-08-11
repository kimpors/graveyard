#pragma once
#include "db.h"

typedef enum {
	INT, FLOAT, 
} Type;

typedef struct {
	char *name;
	Type type;
} Field;

typedef struct {
	const char *name;
	List fields;
} Tb;

Tb *createtb(Db *db, const char *name);
void printtb(const Tb *tb);
void writetb(const Db *db, Tb *tb, const Field fields[255], const unsigned n);
int readtb(const Db *db, const Tb *tb, Field *dest);
