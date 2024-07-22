CC = gcc
CFLAGS = -g -m32 -fno-stack-protector -fno-builtin

NASM = nasm
NASM_FLAGS = -f elf32

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
LIBC_INCLUDE_DIR = ../libc/include
VGA_INCLUDE_DIR = ../drivers/include

C_SRCS = $(shell find $(SRC_DIR) -type f -name '*.c')
ASM_SRCS = $(shell find $(SRC_DIR) -type f -name '*.asm')
SRCS = $(C_SRCS) $(ASM_SRCS)

C_OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(C_SRCS:.c=.o))
ASM_OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(ASM_SRCS:.asm=.o))
OBJS = $(C_OBJS) $(ASM_OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIBC_INCLUDE_DIR) -I$(VGA_INCLUDE_DIR) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(@D)
	$(NASM) $(NASM_FLAGS) $< -o $@

all: $(OBJS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean all