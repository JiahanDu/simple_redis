CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g
BUILD_DIR = build
SRC_DIR = src
TEST_DIR = tests

SRC_FILES = $(SRC_DIR)/hashmap.c
TEST_FILES = $(TEST_DIR)/test_hashmap.c

OBJ_FILES = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRC_FILES)))
TEST_OBJ_FILES = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(TEST_FILES)))

EXEC = $(BUILD_DIR)/test_hashmap

all: $(EXEC)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJ_FILES) $(TEST_OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

run: $(EXEC)
	./$(EXEC)

clean:
	rm -rf $(BUILD_DIR)/*
