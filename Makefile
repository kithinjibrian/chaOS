LD = ld
LDFLAGS = -m elf_i386 -T linker.ld
EXECUTABLE = chaos
IMAGE = chaos.iso

SRC_CPU_DIR = src/cpu
SRC_LIBC_DIR = src/libc
SRC_BOOT_DIR = src/boot
SRC_KERNEL_DIR = src/kernel
SRC_DRIVERS_DIR = src/drivers

BUILD_CPU_DIR = $(SRC_CPU_DIR)/build
BUILD_LIBC_DIR = $(SRC_LIBC_DIR)/build
BUILD_BOOT_DIR = $(SRC_BOOT_DIR)/build
BUILD_KERNEL_DIR = $(SRC_KERNEL_DIR)/build
BUILD_DRIVERS_DIR = $(SRC_DRIVERS_DIR)/build

CPU_OBJS = $(shell find $(BUILD_CPU_DIR) -type f -name '*.o')
LIBC_OBJS = $(shell find $(BUILD_LIBC_DIR) -type f -name '*.o')
BOOT_OBJS = $(shell find $(BUILD_BOOT_DIR) -type f -name '*.o')
KERNEL_OBJS = $(shell find $(BUILD_KERNEL_DIR) -type f -name '*.o')
DRIVERS_OBJS = $(shell find $(BUILD_DRIVERS_DIR) -type f -name '*.o')

all: boot libc drivers cpu kernel $(EXECUTABLE)

boot:
	$(MAKE) -C $(SRC_BOOT_DIR)

libc:
	$(MAKE) -C $(SRC_LIBC_DIR)

drivers:
	$(MAKE) -C $(SRC_DRIVERS_DIR)

cpu:
	$(MAKE) -C $(SRC_CPU_DIR)

kernel:
	$(MAKE) -C $(SRC_KERNEL_DIR)

$(EXECUTABLE): $(BOOT_OBJS) $(KERNEL_OBJS)
	@echo "Linking $(EXECUTABLE) with $(BOOT_OBJS) $(KERNEL_OBJS)"
	$(LD) $(LDFLAGS) -o $(EXECUTABLE) $(BOOT_OBJS) $(LIBC_OBJS) $(KERNEL_OBJS) $(DRIVERS_OBJS) $(CPU_OBJS)

.PHONY: clean
clean:
	$(MAKE) -C $(SRC_CPU_DIR) clean
	$(MAKE) -C $(SRC_BOOT_DIR) clean
	$(MAKE) -C $(SRC_LIBC_DIR) clean
	$(MAKE) -C $(SRC_KERNEL_DIR) clean
	$(MAKE) -C $(SRC_DRIVERS_DIR) clean
	rm -f $(IMAGE)
	rm -f $(EXECUTABLE)
	clear
