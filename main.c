#include "string.h"
#include <ctype.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int createdb(const char *name);
int createtb(const char *db, const char *name);

int main(int argc, char *argv[])
{
	if (createdb("temp") != 0)
	{
		return -1;
	}

	if (createtb("temp", "hello") != 0)
	{
		return -1;
	}

	if (createtb("temp1", "hello") != 0)
	{
		return -1;
	}

	return 0;
}

static char path[PATH_MAX];
static struct stat state;

int set_path()
{
	const char *home;

	if ((home = getenv("HOME")) == NULL)
	{
		fprintf(stderr, "environmental variable HOME is not set\n");
		return -1;
	}

	sprintf(path, "%s%s", home, "/.local/share/graveyard");

	if (stat(path, &state) != 0)
	{
		if (mkdir(path, 0777) != 0)
		{
			fprintf(stderr, "can't create direcotry 'graveyard'\n");
			return -2;
		}
	}

	return 0;
}


int createdb(const char *name)
{
	if (!isalnum(path[0])) set_path();

	char p[PATH_MAX];
	sprintf(p, "%s%c%s", path, '/', name);

	if (stat(p, &state) != 0)
	{
		if (mkdir(p, 0777) != 0)
		{
			fprintf(stderr, "can't create direcotry '%s'\n", name);
			return -3;
		}
	}

	if (stat(p, &state) != 0)
	{
		fprintf(stderr, "can't access direcotry\n");
		return -4;
	}

	if (S_ISDIR(state.st_mode))
	{
		printf("directory exists\n");
	}

	return 0;
}

int createtb(const char *db, const char *name)
{
	if (!isalnum(path[0])) set_path();

	char p[PATH_MAX];
	sprintf(p, "%s%c%s", path, '/', db);

	if (stat(p, &state) != 0)
	{
		fprintf(stderr, "database don't exist '%s'\n", db);
		return -3;
	}

	strcat(p, "/");
	strcat(p, name);

	if (stat(p, &state) != 0)
	{
		if (mkdir(p, 0777) != 0)
		{
			fprintf(stderr, "can't create direcotry '%s'\n", name);
			return -4;
		}
	}

	if (stat(p, &state) != 0)
	{
		fprintf(stderr, "can't access direcotry\n");
		return -5;
	}

	if (S_ISDIR(state.st_mode))
	{
		printf("directory exists\n");
	}

	return 0;
}
