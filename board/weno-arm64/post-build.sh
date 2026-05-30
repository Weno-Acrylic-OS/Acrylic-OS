#!/bin/sh

set -e

# Create boot directory structure for genimage
mkdir -p "${BINARIES_DIR}/efi/boot"

# Copy kernel to binaries (needed by genimage)
if [ -f "${BINARIES_DIR}/Image" ]; then
    echo "Kernel Image found"
else
    echo "WARNING: Image kernel not found at ${BINARIES_DIR}/Image"
    ls -la "${BINARIES_DIR}" | grep -i image || true
fi

# Copy the GRUB config
GRUB_CFG_SRC="../../board/weno-arm64/grub.cfg"
if [ -f "$GRUB_CFG_SRC" ]; then
    cp "$GRUB_CFG_SRC" "${BINARIES_DIR}/efi/boot/grub.cfg"
    echo "Copied grub.cfg"
else
    echo "ERROR: grub.cfg not found at $GRUB_CFG_SRC"
fi

# Copy the GRUB EFI bootloader
# Try multiple possible names where it might be generated
if [ -f "${BINARIES_DIR}/grub.efi" ]; then
    cp "${BINARIES_DIR}/grub.efi" "${BINARIES_DIR}/efi/boot/bootaa64.efi"
    echo "Copied grub.efi to bootaa64.efi"
elif [ -f "${BINARIES_DIR}/grubaa64.efi" ]; then
    cp "${BINARIES_DIR}/grubaa64.efi" "${BINARIES_DIR}/efi/boot/bootaa64.efi"
    echo "Copied grubaa64.efi to bootaa64.efi"
else
    echo "ERROR: GRUB EFI bootloader not found!"
    echo "Available files in BINARIES_DIR:"
    ls -lah "${BINARIES_DIR}" | head -20
    exit 1
fi

echo "Post-build script completed successfully"