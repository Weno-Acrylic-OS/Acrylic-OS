#!/bin/bash

# Default to 'phone' if the first argument is not provided.
PERSONALITY=${1:-phone}

# Paths to build artifacts
KERNEL_PATH="x86_os/final/output/images/Image"
IMG_PATH="build_arm/acrylic_os.img"
EFI_FW_PATH="/opt/homebrew/share/qemu/edk2-aarch64-code.fd"

if [ ! -f "$KERNEL_PATH" ]; then
    echo "Error: Kernel image not found at $KERNEL_PATH"
    echo "Please run a successful build first."
    exit 1
fi
if [ ! -f "$IMG_PATH" ]; then
    echo "Error: Disk image not found at $IMG_PATH"
    echo "Please run a successful build or move your artifact first."
    exit 1
fi

echo "--- Booting Acrylic OS with personality: ${PERSONALITY} ---"

qemu-system-aarch64 \
  -M virt \
  -cpu cortex-a57 \
  -m 1024 \
  -kernel "$KERNEL_PATH" \
  -append "root=/dev/vda2 rootwait console=ttyAMA0,115200 acrylic.personality=${PERSONALITY}" \
  -drive file=${EFI_FW_PATH},if=pflash,format=raw,readonly=on \
  -drive file=nvram.img,if=pflash,format=raw \
  -drive file=${IMG_PATH},if=virtio,format=raw \
  -device virtio-gpu-pci \
  -device qemu-xhci \
  -device usb-kbd \
  -device usb-mouse \
  -serial stdio
