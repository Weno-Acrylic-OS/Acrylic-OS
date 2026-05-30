# Makefile for the Acrylic OS Kiosk System

# --- Configuration
BUILDROOT_VERSION := 2023.11.1
BUILDROOT_TARBALL := buildroot-$(BUILDROOT_VERSION).tar.gz
BUILDROOT_URL := https://buildroot.org/downloads/$(BUILDROOT_TARBALL)
BUILDROOT_DIR := $(CURDIR)/x86_os/buildroot-$(BUILDROOT_VERSION)
BUILDROOT_DOWNLOAD_DIR := $(CURDIR)/x86_os

REACT_UI_DIR := $(CURDIR)/dashboard/acrylic-ui
REACT_BUILD_DIR := $(REACT_UI_DIR)/build
OVERLAY_UI_DIR := $(CURDIR)/x86_os/overlay/usr/share/acrylic-ui

# --- Output
OUTPUT_DIR := $(BUILDROOT_DIR)/output
IMG_IMAGE := $(OUTPUT_DIR)/images/acrylic_os.img
FINAL_IMG_NAME := acrylic_os.img
FINAL_IMG_PATH := $(CURDIR)/build_arm/$(FINAL_IMG_NAME)

.PHONY: all clean img run-qemu build-ui

all: img

# --- Main build flow
img: $(FINAL_IMG_PATH)

$(FINAL_IMG_PATH): $(IMG_IMAGE)
	@echo "--- Copying final disk image to build_arm/ ---"
	@mkdir -p $(CURDIR)/build_arm
	@cp $(IMG_IMAGE) $(FINAL_IMG_PATH)

# Check if we are on macOS
ifeq ($(shell uname), Darwin)
	IS_MACOS := yes
else
	IS_MACOS := no
endif

# ... (rest of the file until the target)

$(IMG_IMAGE): $(BUILDROOT_DIR)/.config
	@echo "--- Building Buildroot system (this will take a long time)... ---"
ifeq ($(IS_MACOS), yes)
	@echo "--- Applying macOS patch symlink workaround ---"
	@mkdir -p $(TEMPORARY_PATCH_BIN_DIR)
	@ln -sf "$$(brew --prefix gpatch)/bin/gpatch" "$(TEMPORARY_PATCH_BIN_DIR)/patch"
	@PATH="$(TEMPORARY_PATCH_BIN_DIR):$(PATH)" $(MAKE) -C $(BUILDROOT_DIR)
	@rm -rf $(TEMPORARY_PATCH_BIN_DIR)
else
	@$(MAKE) -C $(BUILDROOT_DIR)
endif

$(BUILDROOT_DIR)/.config: $(BUILDROOT_DIR) x86_os/configs/weno_defconfig $(OVERLAY_UI_DIR)/index.html
	@echo "--- Configuring Buildroot... ---"
ifeq ($(IS_MACOS), yes)
	@echo "--- Applying macOS patch symlink workaround ---"
	@mkdir -p $(TEMPORARY_PATCH_BIN_DIR)
	@ln -sf "$$(brew --prefix gpatch)/bin/gpatch" "$(TEMPORARY_PATCH_BIN_DIR)/patch"
	@PATH="$(TEMPORARY_PATCH_BIN_DIR):$(PATH)" $(MAKE) -C $(BUILDROOT_DIR) BR2_EXTERNAL=$(CURDIR)/x86_os weno_defconfig
	@rm -rf $(TEMPORARY_PATCH_BIN_DIR)
else
	@$(MAKE) -C $(BUILDROOT_DIR) BR2_EXTERNAL=$(CURDIR)/x86_os weno_defconfig
endif

# --- UI Build
build-ui: $(OVERLAY_UI_DIR)/index.html

$(OVERLAY_UI_DIR)/index.html: $(REACT_BUILD_DIR)/index.html
	@echo "--- Copying React UI to overlay... ---"
	@mkdir -p $(OVERLAY_UI_DIR)
	@cp -r $(REACT_BUILD_DIR)/* $(OVERLAY_UI_DIR)/

$(REACT_BUILD_DIR)/index.html:
	@echo "--- Building React UI (installing dependencies)... ---"
	@npm install --prefix $(REACT_UI_DIR)
	@echo "--- Building React UI (running build script)... ---"
	@npm run build --prefix $(REACT_UI_DIR)


# --- Buildroot Setup
$(BUILDROOT_DIR): $(BUILDROOT_DOWNLOAD_DIR)/$(BUILDROOT_TARBALL)
	@echo "--- Extracting Buildroot... ---"
	@tar -xf $< -C $(BUILDROOT_DOWNLOAD_DIR)

$(BUILDROOT_DOWNLOAD_DIR)/$(BUILDROOT_TARBALL):
	@echo "--- Downloading Buildroot... ---"
	@mkdir -p $(BUILDROOT_DOWNLOAD_DIR)
	@wget -O $@ $(BUILDROOT_URL)

# --- Utility Targets
clean:
	@echo "--- Cleaning up OS build... ---"
	@if [ -d "$(BUILDROOT_DIR)" ]; then 
		$(MAKE) -C $(BUILDROOT_DIR) distclean; 
	fi
	rm -rf $(CURDIR)/build_arm
	rm -rf $(OVERLAY_UI_DIR)

run-qemu: img
	qemu-system-aarch64 -M virt -cpu cortex-a57 -m 1024 \
		-drive file=/opt/homebrew/share/qemu/edk2-aarch64-code.fd,if=pflash,format=raw,readonly=on \
		-drive file=$(FINAL_IMG_PATH),if=virtio,format=raw \
		-device virtio-gpu-pci -device qemu-xhci -device usb-kbd -device usb-mouse

help:
	@echo "Acrylic OS Build System"
	@echo "Targets:"
	@echo "  all/img    - Build the final bootable disk image for the target architecture."
	@echo "  run-qemu   - Build and run the image in QEMU."
	@echo "  build-ui   - Build the React UI and place it in the overlay."
	@echo "  clean      - Clean all build artifacts."
	@echo "  help       - Show this help message."

