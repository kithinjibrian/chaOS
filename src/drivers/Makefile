CC = i686-elf-gcc
CFLAGS = -std=c11 -g -ffreestanding -Wall -Wextra -Werror -pedantic

SRC_DIR = src
BUILD_DIR = build

INCLUDE_DIRS = \
    include 	\
    ../cpu/include \
    ../libc/include \

INCLUDES = $(patsubst %,-I%,$(INCLUDE_DIRS))

SRCS = $(shell find $(SRC_DIR) -type f -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SRCS:.c=.o))

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

all: $(OBJS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean all
