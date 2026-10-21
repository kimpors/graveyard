.PHONY = all run

CC = gcc
CFLAGS += -Wall -gdwarf-2 -g3 -Iinclude
BUILD = build
TARGET = all

OBJ = main.o input-buffer.o command.o statement.o row.o table.o

all: $(OBJ)
	$(CC) $(foreach wrd,$(OBJ),$(BUILD)/$(wrd)) -o $(BUILD)/$(TARGET)

run: all
	./$(BUILD)/$(TARGET)

$(OBJ): %.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $(BUILD)/$@

clean:
	rm -f build/*

