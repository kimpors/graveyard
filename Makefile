all: grave.c
	gcc main.c grave.c utils.c

clean:
	rm -f a.out

update:
	rm -f a.out
	gcc main.c grave.c utils.c
