#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include "grave.h"
#include "utils.h"

void writedt(const char *tbname);
void getdt(const char *tbname);

static u16 dindex;
static Dt data[DATA];

int main(int argc, char *argv[])
{
	strcpy(base, getpwuid(getuid())->pw_dir);
	strcat(base, "/.local/share/graveyard");
	mkdirv(base);
	refresh("temp");

	printf("dbname: %s\n\n", db.name);

	for (int i = 0; i < HASH; i++)
	{
		if (!*tbs[i].name) continue;

		printf("tbname: %s\n", tbs[i].name);

		for (int j = 0; j < LEN; j++)
		{
			if (!fls[i].type[j]) break;
			printf("\t%s: %d\n", fls[i].name[j], fls[i].type[j]);
		}
	}

	return 0;
}

void readdt(const char *tbname)
{
	if (!tbname) exit(-1); 
	if (sizeof(tbname) > NAME) exit(-2);

	if (!isdir("%s/%s/%s/data", base, db.name, tbname)) 
	{
		fprintf(stderr, "readdb: can't open %s/%s/%s/data\n", base, db.name, tbname);
		exit(-1);
	}

	FILE *fp = fopenv("rb", "%s/%s/%s/data/1", base, db.name, tbname);
	fread(data, sizeof(void *), DATA, fp);
	fclose(fp);
}

void writedt(const char *tbname)
{
	if (!tbname) exit(-1);
	if (sizeof(tbname) > NAME) exit(-2);

	mkdirv("%s/%s/%s/data", base , db.name, tbname);

	FILE *fp = fopenv("wb", "%s/%s/%s/data/1", base , db.name, tbname);
	fwrite(data, sizeof(Dt), DATA, fp);
	fclose(fp);
}
