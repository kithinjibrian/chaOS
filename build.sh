#!/bin/bash

# Build boot and kernel
# Burn iso image
# Run iso image

# Usage: build.sh [clean|run]


HOME="."
KERNEL="chaos"
IMAGE="chaos.iso"
DEST="iso"
LINKER="linker"
LINKER_IN="${LINKER}.ld"
LINKER_OUT="${LINKER}_generated.ld"

bmodules()
{
	MODULE="${HOME}/src/modules"
	make -C "$MODULE"

	mv "$MODULE/a" "$HOME/$DEST/boot/modules/a"
}

generate_linker() {
	gcc -E -P -I "$HOME/linker" -x c "$LINKER_IN" -o "$LINKER_OUT"
}

build() {
	echo "Building $KERNEL"
	generate_linker
	make -C "$HOME"

	if [ -f "$HOME/$KERNEL" ]; then
		echo "Build successful"
		mv "$HOME/$KERNEL" "$HOME/$DEST/boot/kernel"
		grub-mkrescue -o "$HOME/$IMAGE" "$HOME/$DEST/"
	else
		echo "Build failed"
		exit 1
	fi
}

clean() {
	make -C "$HOME" clean
}

if [ "$1" == "clean" ]; then
	clean

elif [ "$1" == "run" ]; then
	build
	qemu-system-i386 -m 256M -cdrom "$IMAGE"

elif [ "$1" == "force-run" ]; then
	clean
	build
	qemu-system-i386 -m 256M -cdrom "$IMAGE"

elif [ "$1" == "linker" ]; then
	generate_linker

elif [ "$1" == "modules" ]; then
	bmodules

elif [ "$1" == "bochs" ]; then
	build
	bochs -f bochsrc

elif [ "$1" == "debug" ]; then
    build
    qemu-system-i386 -cdrom "$IMAGE" -s &

	 # Allow qemu to start
    sleep 1
	
    gdb -ex "file $HOME/$DEST/boot/kernel" -ex "target remote localhost:1234"
else
	build
fi
