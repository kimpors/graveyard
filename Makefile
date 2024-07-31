grave.c: db.c util.c
	gcc grave.c db.c util.c -o a.out

db.c:
	gcc db.c

util.c:
	gcc util.c

clean:
	rm *.so
	rm *.out
