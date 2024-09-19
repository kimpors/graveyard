#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include "utils.h"

static struct stat state;
static char path[MAXPATH];

int check_dir(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vsprintf(path, fmt, ap);
	va_end(ap);

	return stat(path, &state) != -1;
}

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

void dirwalk(void (*fcn)(const char *), const char *fmt, ...)
{
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

		(*fcn)(dp->d_name);
	}

	closedir(dfd);
}

int sdirwalk(char dest[][MAXPATH], const char *fmt, ...)
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

		strcpy(dest[i++], dp->d_name);
	}

	closedir(dfd);
	return i;
}
