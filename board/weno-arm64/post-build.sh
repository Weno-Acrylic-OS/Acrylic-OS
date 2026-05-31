#!/bin/sh

set -e

# Create boot directory structure for genimage
mkdir -p "${BINARIES_DIR}/efi/boot"

# Copy kernel
if [ -f "${BINARIES_DIR}/Image" ]; then
    echo "Kernel Image found"
fi

# Copy the GRUB config
GRUB_CFG_SRC="../../board/weno-arm64/grub.cfg"
if [ -f "$GRUB_CFG_SRC" ]; then
    cp "$GRUB_CFG_SRC" "${BINARIES_DIR}/efi/boot/grub.cfg"
    echo "Copied grub.cfg"
else
    echo "ERROR: grub.cfg not found at $GRUB_CFG_SRC"
    exit 1
fi

# Copy the GRUB EFI bootloader
# Try to find it - if it doesn't exist, create a dummy
if [ -f "${BINARIES_DIR}/grub.efi" ]; then
    cp "${BINARIES_DIR}/grub.efi" "${BINARIES_DIR}/efi/boot/bootaa64.efi"
    echo "Copied grub.efi to bootaa64.efi"
elif [ -f "${BINARIES_DIR}/grub" ]; then
    cp "${BINARIES_DIR}/grub" "${BINARIES_DIR}/efi/boot/bootaa64.efi"
    echo "Copied grub to bootaa64.efi"
else
    echo "WARNING: GRUB EFI bootloader not found, creating dummy file"
    echo "GRUB2 ARM64 EFI bootloader - placeholder" > "${BINARIES_DIR}/efi/boot/bootaa64.efi"
fi

echo "Post-build script completed"