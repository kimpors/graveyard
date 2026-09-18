.PHONY = all

CC = gcc
CFLAGS += -Wall -g
OBJ = main.o

all: build/$(OBJ)
	$(CC) $^ -o build/all

run: all
	./build/all

build/$(OBJ): build/%.o: src/%.c
	$(CC) -c $^ -o $@

clean:
	rm -f *.o all

