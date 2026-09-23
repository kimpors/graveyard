.PHONY = all

CC = gcc
CFLAGS += -Wall -gdwarf-2 -g3
OBJ = main.o

all: build/$(OBJ)
	$(CC) $^ -o build/all

run: all
	./build/all

build/$(OBJ): build/%.o: src/%.c
	$(CC) -c $^ $(CFLAGS) -o $@

clean:
	rm -f build/*

