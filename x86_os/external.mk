#
# external.mk for Weno Fit OS x86
#

name: WENO_X86
desc: Weno Fit OS x86 specific configurations

# Path to the overlay directory
WENO_X86_OVERLAY := $(BR2_EXTERNAL_WENO_PATH)/overlay

# Add the overlay to the Buildroot build
BR2_ROOTFS_OVERLAY += $(WENO_X86_OVERLAY)
