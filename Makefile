# Makefile for Weno Fit OS

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -Iinclude/kernel
LDFLAGS =

BUILD_DIR = build
SRC_DIR = src

KERNEL_SRC = $(SRC_DIR)/kernel/kernel.c 
             $(SRC_DIR)/kernel/scheduler.c    # Make thinks this is a command!
             $(SRC_DIR)/kernel/task.c         # And this too!
			 
APP_SRC = $(SRC_DIR)/app/main.c

KERNEL_OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(KERNEL_SRC))
APP_OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(APP_SRC))

ALL_OBJ = $(KERNEL_OBJ) $(APP_OBJ)

OUT = $(BUILD_DIR)/weno_fit_os.elf

all: $(BUILD_DIR) $(OUT)

$(OUT): $(ALL_OBJ)
	$(CC) $(LDFLAGS) $(ALL_OBJ) -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/kernel
	mkdir -p $(BUILD_DIR)/app

$(BUILD_DIR)/kernel/%.o: $(SRC_DIR)/kernel/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/app/%.o: $(SRC_DIR)/app/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
