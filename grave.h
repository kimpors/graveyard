#pragma once

#include "utils.h"

#define HASH 255
#define FILESIZE 8096

typedef union {
	int n;
	float f;
	double d;
	char *s;
} Data;

typedef enum {
	INT, FLOAT, 
} Type;

typedef struct {
	const char *name;
	Type type;
} Fl;

typedef struct {
	const char *name;
	Fl fields[HASH];
} Tb;

typedef struct {
	const char *name;
	Tb tables[HASH];
} Db;

void printdb(const Db *db);
void printtb(const Tb *tb);
void printfl(const Fl *fl);
void printdt(Type type, Data dt);
