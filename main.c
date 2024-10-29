#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include "grave.h"
#include "utils.h"

void setdb(const char *name);
void settb(const char *name);
void setfl(const char *tbname, const Fl fields);
void writedt(const char *tbname);

Tb *gettb(const char *name);
Fl *getfl(const char *tbname);
void getdt(const char *tbname);

void readdt(const char *tbname);
void refresh(const char *name);

static Db db;
static Tb tbs[HASH];
static Fl fls[HASH];

static u16 dindex;
static Dt data[DATA];
static char base[MAXPATH];

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

u32 hash(const char *s)
{
	u32 val;

	for (val = 0; *s != '\0'; s++)
	{
		val = *s + 31 * val;
	}

	return val % HASH;
}

Tb *gettb(const char *name)
{
	if (!name) return NULL;
	if (sizeof(name) > NAME) return NULL;

	return &tbs[hash(name)];
}

Fl *getfl(const char *tbname)
{
	if (!tbname) return NULL;
	if (sizeof(tbname) > NAME) return NULL;

	return &fls[hash(tbname)];
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

void setdb(const char *name)
{
	if (!name) exit(-1);
	if (sizeof(name) > NAME) exit(-2);

	mkdirv("%s/%s", base, name);
	strcpy(db.name, name);
}

void settb(const char *name)
{
	if (!name) exit(-1);
	if (sizeof(name) > NAME) exit(-2);

	mkdirv("%s/%s/%s", base, db.name, name);
	strcpy(tbs[hash(name)].name, name);
}

void setfl(const char *tbname, const Fl fields)
{
	if (!tbname) exit(-1);
	if (sizeof(tbname) > NAME) exit(-2);

	u16 index = hash(tbname);

	fls[index] = fields;

	FILE *fp = fopenv("w", "%s/%s/%s/%s.grv", base, db.name, tbname, tbname);

	for (int i = 0; i < LEN; i++)
	{
		if (fls[index].type[i] == 0) break;
		fprintf(fp, "%s %d\n", fls[index].name[i], fls[index].type[i]);
	}

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

void readdb(const char name[NAME]);
void readtb(const char name[NAME]);
void readfl(const char tbname[NAME]);

void refresh(const char *dbname)
{
	if (!dbname) exit(-1);
	if (sizeof(dbname) > NAME) exit(-2);

	readdb(dbname);
	dirwalk(readtb, "%s/%s", base, dbname);
	dirwalk(readfl, "%s/%s", base, dbname);
}

void readdb(const char name[NAME])
{
	if (!isdir("%s/%s", base, name)) 
	{
		fprintf(stderr, "readdb: can't open %s/%s\n", base, name);
		exit(-1);
	}

	strcpy(db.name, name);
}

void readtb(const char name[NAME])
{
	if (!isdir("%s/%s/%s", base, db.name, name)) 
	{
		fprintf(stderr, "readtb: can't open %s/%s/%s\n", base, db.name, name);
		exit(-1);
	}
	
	strcpy(tbs[hash(name)].name, name);
}

void readfl(const char tbname[NAME])
{
	FILE *fp = fopenv("r", "%s/%s/%s/%s.grv", base, db.name, tbname, tbname);

	if (!fp)
	{
		fprintf(stderr, "readfl: can't open %s/%s/%s/%s\n", base, db.name, tbname, tbname);
		exit(-1);
	}

	int t;
	char n[NAME];
	u16 index = hash(tbname);

	for (int i = 0; i < LEN; i++)
	{
		if (fscanf(fp, "%s %d\n", n, &t) > 0)
		{
			strcpy(fls[index].name[i], n);
			fls[index].type[i] = t;
			continue;
		}

		break;
	}

	fclose(fp);
}
