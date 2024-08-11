#include "db.h"
#include "tb.h"

typedef union {
	int n;
	float f;
	double d;
	char *s;
} Data;

void writedt(const Db *db, const Tb *tb, const Data *data, unsigned int len);
void readdt(const Db *db, const Tb *tb, Data *data);
void printdt(List *fields, const Data *dt);
