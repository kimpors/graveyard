#define MAX				255
#define MAX_CONTENT 	8096

typedef enum {
	INT, FLOAT, 
} Type;

typedef struct {
	char name[MAX];
	Type type;
} Field;

typedef union {
	int n;
	float f;
	double d;
	char *s;
} Data;

typedef struct {
	const char *name;
	unsigned len;
	Field fields[MAX];
} Tb;

typedef struct {
	const char *name;
	unsigned len;
	Tb *tables[MAX];
} Db;

void refresh(void);
Db *createdb(const char *name);
Tb *createtb(Db *db, const char *name);
void showdbs(void);
Db *getdb(const char *name);
Tb *gettb(const Db *db, const char *name);
void writetb(const Db *db, Tb *tb, const Field fields[MAX], const unsigned n);
int readtb(const Db *db, const Tb *tb, Field *dest);
void save_data(const Db *db, const Tb *tb, const Data *data, unsigned len);
int get_data(const Db *db, const Tb *tb, Data* data);
