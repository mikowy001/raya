TARGET = compiled/raya-sim
TARGET_DIR = compiled
OBJ_DIR = obj

CC = gcc
CFLAGS = -Wall -Wextra -I./src
LIBS = -lraylib 

SRCS := $(shell find src -name "*.c")

OBJS := $(patsubst src/%, $(OBJ_DIR)/%, $(SRCS:.c=.o))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	mkdir -p $(TARGET_DIR)
	$(CC) -o $@ $(OBJS) $(LIBS)

$(OBJ_DIR)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET_DIR)
