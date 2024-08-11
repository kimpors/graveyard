all: grave.c
	gcc grave.c components/*.c utils/*.c -o all

clean:
	rm -f all

update:
	rm -f all
	gcc grave.c components/*.c utils/*.c -o all
