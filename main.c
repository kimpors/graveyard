#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include "grave.h"
#include "utils.h"

void pushdb(Db db);
void pushtb(Db *db, const Tb tb);
void pushdt(const Db *db, const Tb *tb, const Data dt[HASH], unsigned len);

Db *getdb(const char *name);
Tb *gettb(Db *db, const char *name);
void getdt(const Db *db, const Tb *tb, const Data dest[HASH], unsigned len);

static Db dbs[HASH];
static char base[MAXPATH];

int main(int argc, char *argv[])
{
	strcpy(base, getpwuid(getuid())->pw_dir);
	strcat(base, "/.local/share/graveyard");
	create_dir("%s", base);

	Db db = { "temp" };
	Tb tb = { "hello", { { "userid", INT }, { "useroffset", FLOAT } }};

	pushdb(db);
	pushtb(&db, tb);

	printdb(getdb("temp"));
	printtb(gettb(&db, "hello"));

	Data dt[HASH];
	dt[0].n = 12;
	dt[1].f = 23.4;

	pushdt(&db, &tb, dt, 2);

	for (int i = 0; tb.fields[i].name != NULL; i++)
	{
		printfl(&tb.fields[i]);
	}

	getdt(&db, &tb, dt, 2);

	for (int i = 0; i < 2; i++)
	{
		printdt(tb.fields[i].type, dt[i]);
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

void pushdb(Db db)
{
	if (getdb(db.name)->name != NULL) return;

	create_dir("%s/%s", base, db.name);
	dbs[hash(db.name)] = db;
}

void pushtb(Db *db, const Tb tb)
{
	if (gettb(db, tb.name)->name != NULL) return;

	create_dir("%s/%s/%s", base, db->name, tb.name);

	FILE *fp = create_file("w", "%s/%s/%s/%s.grv", base, db->name, tb.name, tb.name);

	for (int i = 0; tb.fields[i].name != NULL; i++)
	{
		fprintf(fp, "%s %d\n", tb.fields[i].name, tb.fields[i].type);
	}

	fclose(fp);

	db->tables[hash(tb.name)] = tb;
}

void pushdt(const Db *db, const Tb *tb, const Data dt[HASH], unsigned len)
{
	create_dir("%s/%s/%s/data", base, db->name, tb->name);

	FILE *fp = create_file("wb", "%s/%s/%s/data/1", base, db->name, tb->name);
	fwrite(dt, sizeof(Data), len, fp);
	fclose(fp);
}
