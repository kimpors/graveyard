CC := gcc
CFLAGS := -Wall -gdwarf-2 -g3 -Iinclude -Iextern/Unity/src

TARGET := all
BUILD := build
OBJ := $(BUILD)/obj
BIN := $(BUILD)/bin

SRC := $(wildcard src/*.c)
TEST_SRC := $(wildcard test/*.c)

SRC_OBJ := $(SRC:src/%.c=$(OBJ)/%.o)
TEST_BIN := $(TEST_SRC:test/%.c=$(BIN)/test/%)
UNITY_OBJ := $(OBJ)/unity/unity.o

$(shell mkdir -p $(OBJ) $(OBJ)/unity $(BIN) $(BIN)/test)

.PHONY: all run clean test

all: $(SRC_OBJ)
	$(CC) $(SRC_OBJ) -o $(BIN)/$(TARGET)

run: all
	./$(BIN)/$(TARGET)

test: $(TEST_BIN)
	@for t in $(TEST_BIN); do \
		echo "== $$t"; \
		./$$t || exit 1; \
	done

$(TEST_BIN): $(BIN)/test/%: test/%.c $(filter-out $(OBJ)/main.o,$(SRC_OBJ)) $(UNITY_OBJ)
	$(CC) $^ $(CFLAGS) -o $@

$(SRC_OBJ): $(OBJ)/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(UNITY_OBJ): extern/Unity/src/unity.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf build
