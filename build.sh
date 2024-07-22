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
	build && qemu-system-i386 -hda "$IMAGE"

else
	build
fi
