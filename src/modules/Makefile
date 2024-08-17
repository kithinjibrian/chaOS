NASM = nasm
NASM_FLAGS = -f bin

a: a.asm
	$(NASM) $(NASM_FLAGS) a.asm -o a


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)