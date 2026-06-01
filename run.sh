#!/bin/bash
qemu-system-aarch64 \
  -M virt \
  -cpu cortex-a57 \
  -m 1024 \
  -drive file=/opt/homebrew/share/qemu/edk2-aarch64-code.fd,if=pflash,format=raw,readonly=on \
  -drive file=nvram.img,if=pflash,format=raw \
  -drive file=build_arm/acrylic_os.img,if=virtio,format=raw \
  -device virtio-gpu-pci \
  -device qemu-xhci \
  -device usb-kbd \
  -device usb-mouse \
  -serial stdio
