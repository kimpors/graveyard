#pragma once

#include <stdio.h>
#define MAXPATH 1024

void create_dir(const char *fmt, ...);
FILE *create_file(const char *mode, const char *fmt, ...);
