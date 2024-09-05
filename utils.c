#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>
#include "utils.h"

static struct stat state;
static char path[MAXPATH];

void create_dir(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vsprintf(path, fmt, ap);
	va_end(ap);

	if (stat(path, &state))
	{
		if (mkdir(path, 0777))
		{
			fprintf(stderr, "can't create direcotry '%s'\n", path);
			exit(-1);
		}
	}
}

FILE *create_file(const char *mode, const char *fmt, ...)
{
	FILE *fp;
	va_list ap;

	va_start(ap, fmt);
	vsprintf(path, fmt, ap);
	va_end(ap);

	if ((fp = fopen(path, mode)) == NULL)
	{
		fprintf(stderr, "create_file: can't create file %s\n", path);
		exit(-1);
	}

	return fp;
}
