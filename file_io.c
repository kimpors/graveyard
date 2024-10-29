#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include "file_io.h"

static struct stat state;
static char path[MAXPATH];
char base[MAXPATH];

int isdir(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vsprintf(path, fmt, ap);
	va_end(ap);

	return stat(path, &state) != -1;
}

void mkdirv(const char *fmt, ...)
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

FILE *fopenv(const char *mode, const char *fmt, ...)
{
	FILE *fp;
	va_list ap;

	va_start(ap, fmt);
	vsprintf(path, fmt, ap);
	va_end(ap);

	if ((fp = fopen(path, mode)) == NULL)
	{
		fprintf(stderr, "fopenv: can't create file %s\n", path);
	}

	return fp;
}

int dirwalk(void (*fnc) (const char *name), const char *fmt, ...)
{
	int i = 0;
	va_list ap;
	DIR *dfd;
	struct dirent *dp;

	va_start(ap, fmt);
	vsprintf(path, fmt, ap);
	va_end(ap);

	if ((dfd = opendir(path)) == NULL)
	{
		fprintf(stderr, "dirwalk: can't open %s\n", path);
		exit(-1);
	}

	while ((dp = readdir(dfd)) != NULL)
	{
		if (strcmp(dp->d_name, ".") == 0 ||
				strcmp(dp->d_name, "..") == 0)
		{
			continue;
		}

		fnc(dp->d_name);
	}

	closedir(dfd);
	return i;
}
