CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g
BUILD_DIR = build

SRC = src/hashmap.c
TEST = tests/test_hashmap.c

OBJ = $(BUILD_DIR)/hashmap.o $(BUILD_DIR)/test_hashmap.o
TARGET = test_hashmap

.PHONY: all clean run

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/hashmap.o: src/hashmap.c include/hashmap.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test_hashmap.o: $(TEST)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
