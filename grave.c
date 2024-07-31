#include <asm-generic/errno-base.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <stdio.h>
#include <sys/stat.h>
#include "db.h"
#include <sys/types.h>
#include <pwd.h>
#include "util.h"

extern char path[PATH_MAX];

int main(int argc, char *argv[])
{
	strcpy(path, getpwuid(getuid())->pw_dir);
	strcat(path, "/.local/share/graveyard");

	create_dir(path);
	Db *temp = createdb("temp");
	Tb *hello = createtb(temp, "hello");

	Field f[2] = {
		{ "id", INT },
		{ "num", FLOAT },
	};

	writetb(temp, hello, f, 2);

	for (int i = 0; i < hello->len; i++)
	{
		printf("%5s|\t", hello->fields[i].name);
	}

	Data dt[255];
	dt[0].n = 120;
	dt[1].f = 22.22;

	save_data(temp, hello, dt, 2);

	Data res[255];
	int len = get_data(temp, hello, res);

	int pos = 0;
	putchar('\n');
	for (int i = 0; i < len; i++, pos++)
	{
		if (pos >= hello->len) pos = 0;

		switch (hello->fields[pos].type)
		{
			case INT:
				printf("%5d|\t", res[0].n);
				break;
			case FLOAT:
				printf("%5.2f|\t", res[1].f);
				break;
		}
	}

	return 0;
}
