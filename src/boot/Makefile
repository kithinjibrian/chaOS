NASM = nasm
NASM_FLAGS = -f elf32

SRC_DIR = src
BUILD_DIR = build

$(BUILD_DIR)/boot.o: $(SRC_DIR)/boot.asm
	@mkdir -p $(@D)
	$(NASM) $(NASM_FLAGS) $(SRC_DIR)/boot.asm -o $(BUILD_DIR)/boot.o


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)