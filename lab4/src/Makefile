CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

PROGRAMS = $(BIN_DIR)/process_memory $(BIN_DIR)/parallel_min_max

$(OBJ_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

all: $(OBJ_DIR) $(BIN_DIR) $(PROGRAMS)

# Задание 1: Сборка process_memory
$(BIN_DIR)/process_memory: $(SRC_DIR)/process_memory.c
	$(CC) $(CFLAGS) $< -o $@

# Задание 3: Сборка parallel_min_max
$(BIN_DIR)/parallel_min_max: $(SRC_DIR)/parallel_min_max.c $(SRC_DIR)/find_min_max.o $(SRC_DIR)/utils.o
	$(CC) $(CFLAGS) $^ -o $@

# Компиляция зависимостей
$(SRC_DIR)/find_min_max.o: $(SRC_DIR)/find_min_max.c $(SRC_DIR)/find_min_max.h
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/utils.o: $(SRC_DIR)/utils.c $(SRC_DIR)/utils.h
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(SRC_DIR)/*.o

.PHONY: all clean
