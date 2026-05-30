#!/bin/sh

# This script is executed after the main build but before the image is generated.

set -e

# Copy the GRUB config to the binaries directory so genimage can find it.
# The destination path needs to match what's expected by genimage.cfg's `files` list.
mkdir -p "${BINARIES_DIR}/efi/boot"
cp board/weno-arm64/grub.cfg "${BINARIES_DIR}/efi/boot/grub.cfg"
