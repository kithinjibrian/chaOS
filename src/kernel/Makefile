CC = i686-elf-gcc
CFLAGS = -std=c11 -g -ffreestanding -Wall -Wextra -Werror -pedantic

NASM = nasm
NASM_FLAGS = -f elf32 -g

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
ASM_SRCS = $(shell find $(SRC_DIR) -type f -name '*.asm')
SRCS = $(C_SRCS) $(ASM_SRCS)

C_OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(C_SRCS:.c=.o))
ASM_OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(ASM_SRCS:.asm=.o))
OBJS = $(C_OBJS) $(ASM_OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(@D)
	$(NASM) $(NASM_FLAGS) $< -o $@

all: $(OBJS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean all
