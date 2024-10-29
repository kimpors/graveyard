#pragma once

#include <stdio.h>
#define MAXPATH 1024

int isdir(const char *fmt, ...);
void mkdirv(const char *fmt, ...);
FILE *fopenv(const char *mode, const char *fmt, ...);
int dirwalk(void (*fnc) (const char *name), const char *fmt, ...);
