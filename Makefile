all: grave.c
	gcc main.c grave.c file_io.c

clean:
	rm -f a.out

update:
	rm -f a.out
	gcc main.c grave.c file_io.c
