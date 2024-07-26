#!/bin/bash

# Build boot and kernel
# Burn iso image
# Run iso image

# Usage: build.sh [clean|run]


HOME="."
KERNEL="chaos"
IMAGE="chaos.iso"
DEST="iso"

build() {
	echo "Building $KERNEL"
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
	qemu-system-i386 -m 256M -hda "$IMAGE"

elif [ "$1" == "bochs" ]; then
	build
	bochs -f bochsrc

elif [ "$1" == "debug" ]; then
    build
    qemu-system-i386 -hda "$IMAGE" -s &

	 # Allow qemu to start
    sleep 1
	
    gdb -ex "file $HOME/$DEST/boot/kernel" -ex "target remote localhost:1234"
else
	build
fi
