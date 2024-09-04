CC = i686-elf-gcc
CFLAGS = -std=c11 -g -ffreestanding -Wall -Wextra -Werror

SRC_DIR = src
BUILD_DIR = build

INCLUDE_DIRS = \
    include 	\
    ../fs/include \
    ../cpu/include \
    ../libc/include \
    ../drivers/include

INCLUDES = $(patsubst %,-I%,$(INCLUDE_DIRS))

C_SRCS = $(shell find $(SRC_DIR) -type f -name '*.c')
ASM_SRCS = $(shell find $(SRC_DIR) -type f -name '*.S')

C_OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(C_SRCS:.c=.o))
ASM_OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(ASM_SRCS:.S=.o))

OBJS = $(C_OBJS) $(ASM_OBJS) $(RUST_OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

all: $(OBJS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean all
