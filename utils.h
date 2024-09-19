#pragma once

#include <stdio.h>
#define MAXPATH 1024

int check_dir(const char *fmt, ...);
void create_dir(const char *fmt, ...);
FILE *create_file(const char *mode, const char *fmt, ...);
int sdirwalk(char dest[][MAXPATH], const char *fmt, ...);
