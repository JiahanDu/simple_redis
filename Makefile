CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g
BUILD_DIR = build
SRC_DIR = src

SRC_FILES = $(SRC_DIR)/hashmap.c $(SRC_DIR)/tcp_client.c

OBJ_FILES = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRC_FILES)))

EXEC = $(BUILD_DIR)/tcp_client

all: $(EXEC)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

run: $(EXEC)
	./$(EXEC) 127.0.0.1 6379

clean:
	rm -rf $(BUILD_DIR)/*
