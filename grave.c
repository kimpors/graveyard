// #include <asm-generic/errno-base.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
// #include <sys/stat.h>
// #include <sys/types.h>
#include <pwd.h>
#include "utils/util.h"
#include "utils/list.h"
#include "components/db.h"
#include "components/tb.h"
#include "components/dt.h"
#include "grave.h"

char path[PATH];
char curr_path[PATH];

static List dbs;

int main(int argc, char *argv[])
{
	strcpy(path, getpwuid(getuid())->pw_dir);
	strcat(path, "/.local/share/graveyard");
	create_dir(path);

	Db *temp = createdb("temp");
	createdb("oculus");
	Tb *hello = createtb(temp, "hello");

	Field f[2] = {
		{ "id", INT },
		{ "num", FLOAT },
	};

	writetb(temp, hello, f, 2);
	printtb(hello);

	Data dt[255];
	dt[0].n = 120;
	dt[1].f = 22.22;

	writedt(temp, hello, dt, 2);

	Data res[255];
	readdt(temp, hello, res);
	printdt(&hello->fields, res);
	return 0;
}
