CC = gcc
CFLAGS = -g -m32 -fno-stack-protector -fno-builtin

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
LIBC_INCLUDE_DIR = ../libc/include

SRCS = $(shell find $(SRC_DIR) -type f -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SRCS:.c=.o))

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIBC_INCLUDE_DIR) -c $< -o $@

all: $(OBJS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean all
