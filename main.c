#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include "grave.h"
#include "utils.h"

void setdb(const Db db);
void settb(Db *db, const Tb tb);
void setdt(const Db *db, const Tb *tb, const Data dt[HASH], unsigned len);

Db *getdb(const char *name);
Tb *gettb(Db *db, const char *name);
void getdt(const Db *db, const Tb *tb, const Data dest[HASH], unsigned len);

void refresh(void);

static Db dbs[HASH];
static char base[MAXPATH];

int main(int argc, char *argv[])
{
	strcpy(base, getpwuid(getuid())->pw_dir);
	strcat(base, "/.local/share/graveyard");
	create_dir("%s", base);
	refresh();

	Db *db;
	Tb *tb;
	Fl *fl = tb->fields;

	printdb((db = getdb("temp")));
	printtb((tb = gettb(db, "hello")));

	for (int i = 0; fl[i].name == NULL; i++)
	{
		printfl(&tb->fields[i]);
	}

	return 0;
}

unsigned hash(const char *s)
{
	unsigned val;

	for (val = 0; *s != '\0'; s++)
	{
		val = *s + 31 * val;
	}

	return val % HASH;
}

Db *getdb(const char *name)
{
	return &dbs[hash(name)];
}

Tb *gettb(Db *db, const char *name)
{
	return &db->tables[hash(name)];
}

Fl *getfl(Tb *tb, const char *name)
{
	for (int i = 0; tb->fields[i].name != NULL; i++)
	{
		if (strcmp(tb->name, name) == 0)
		{
			return &tb->fields[i];
		}
	}

	return NULL;
}

void getdt(const Db *db, const Tb *tb, const Data dest[HASH], unsigned len)
{
	create_dir("%s/%s/%s/data", base, db->name, tb->name);

	FILE *fp = create_file("rb", "%s/%s/%s/data/1", base, db->name, tb->name);
	fread(&dest, sizeof(Data), len, fp);
	fclose(fp);
}


void setdb(const Db db)
{
	if (getdb(db.name)->name != NULL) return;

	create_dir("%s/%s", base, db.name);
	dbs[hash(db.name)] = db;
}

void settb(Db *db, const Tb tb)
{
	if (gettb(db, tb.name)->name != NULL) return;

	create_dir("%s/%s/%s", base, db->name, tb.name);

	FILE *fp = create_file("a", "%s/%s/%s/%s.grv", base, db->name, tb.name, tb.name);

	for (int i = 0; tb.fields[i].name != NULL; i++)
	{
		fprintf(fp, "%s %d\n", tb.fields[i].name, tb.fields[i].type);
	}

	fclose(fp);

	db->tables[hash(tb.name)] = tb;
}

void setdt(const Db *db, const Tb *tb, const Data dt[HASH], unsigned len)
{
	create_dir("%s/%s/%s/data", base, db->name, tb->name);

	FILE *fp = create_file("wb", "%s/%s/%s/data/1", base, db->name, tb->name);
	fwrite(dt, sizeof(Data), len, fp);
	fclose(fp);
}

void readdb(const char *name);
void readtb(Db *db, const char *name);

void refresh(void)
{
	int dblen, tblen;
	char dbs[HASH][MAXPATH];
	char tbs[HASH][MAXPATH];

	dblen = sdirwalk(dbs, base);

	for (int i = 0; i < dblen; i++)
	{
		printf("dbs[i] = %s\n", dbs[i]);
		readdb(dbs[i]);

		tblen = sdirwalk(tbs, "%s/%s", base, dbs[i]);

		for (int j = 0; j < tblen; j++)
		{
			printf("tbs[j] = %s\n", tbs[j]);
			readtb(getdb(dbs[i]), tbs[i]);
		}
	}
}

void readdb(const char *name)
{
	if (!check_dir("%s/%s", base, name)) 
	{
		fprintf(stderr, "readdb: can't open %s/%s\n", base, name);
		exit(-1);
	}

	dbs[hash(name)] = (Db){ name };
}

void readtb(Db *db, const char *name)
{
	if (!check_dir("%s/%s/%s", base, db->name, name)) 
	{
		fprintf(stderr, "readtb: can't open %s/%s\n", base, name);
		exit(-1);
	}

	FILE *fp = create_file("r", "%s/%s/%s/%s.grv", base, db->name, name, name);

	Tb tb = (Tb){ name };
	int i = -1;

	do
	{
		i++;
		tb.fields[i].name = malloc(sizeof(char[HASH]));
	} while (fscanf(fp, "%253s %d\n", (char *)tb.fields[i].name, &tb.fields[i].type) > 0);

	fclose(fp);

	db->tables[hash(tb.name)] = tb;
}
