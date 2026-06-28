CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g -Iinclude
LDFLAGS = 

SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin

SRC     = $(wildcard $(SRC_DIR)/*.c)
OBJ     = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET  = $(BIN_DIR)/riscv-decoder

.PHONY: all clean valgrind dirs

all: dirs $(TARGET)

dirs:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

valgrind: all
	valgrind --leak-check=full ./$(TARGET) test/programs/mixed.hex

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)