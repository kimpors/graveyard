#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define LEN			16
#define NAME 		32
#define HASH 		255
#define DATA		8096

typedef int8_t		i8;
typedef int16_t		i16;
typedef int32_t		i32;
typedef int64_t		i64;

typedef uint8_t		u8;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t 	u64;

typedef float		f32;
typedef double		f64;

typedef enum {
	NONE, BYTE, SHORT, INT,
	LONG, UBYTE, USHORT,
	UINT, ULONG,FLOAT,
	DOUBLE, CHAR
} Type;

typedef union {
	i8 byte;
	i16 i16;
	i32 i32;
	i64 i64;

	u8 u8;
	u16 u16;
	u32 u32;
	u64 u64;

	f32 f32;
	f64 f64;

	char ch;
} Dt;

typedef struct {
	char name[NAME];
} Db;

typedef struct {
	char name[NAME];
} Tb;

typedef struct {
	char name[LEN][NAME];
	Type type[LEN];
} Fl;

static Db db;
static Tb tbs[HASH];
static Fl fls[HASH];
static char base[MAXPATH];

void setdb(const char *name);
void settb(const char *name);
void setfl(const char *tbname, const Fl fields);

Tb *gettb(const char *name);
Fl *getfl(const char *tbname);
void getdt(const char *tbname);

void refresh(const char *name);
