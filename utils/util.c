#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

static struct stat state;

void create_dir(const char *path)
{
	if (stat(path, &state))
	{
		if (mkdir(path, 0777))
		{
			fprintf(stderr, "can't create direcotry '%s'\n", path);
			exit(-1);
		}
	}
}
