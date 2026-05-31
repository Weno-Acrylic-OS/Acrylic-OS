#!/bin/sh

set -e

BOARD_DIR="$(dirname "$0")"
IMAGES_DIR="$1"

if [ ! -f "${IMAGES_DIR}/efi-part/EFI/BOOT/bootaa64.efi" ]; then
	echo "ERROR: GRUB ARM64 EFI bootloader not found in ${IMAGES_DIR}"
	exit 1
fi

if [ ! -f "${IMAGES_DIR}/Image" ]; then
	echo "ERROR: Kernel Image not found in ${IMAGES_DIR}"
	exit 1
fi

if [ ! -f "${IMAGES_DIR}/rootfs.ext4" ] && [ ! -f "${IMAGES_DIR}/rootfs.ext2" ]; then
	echo "ERROR: rootfs image not found in ${IMAGES_DIR}"
	exit 1
fi

cp "$BOARD_DIR/grub.cfg" "${IMAGES_DIR}/efi-part/EFI/BOOT/grub.cfg"

support/scripts/genimage.sh -c "$BOARD_DIR/genimage.cfg"
