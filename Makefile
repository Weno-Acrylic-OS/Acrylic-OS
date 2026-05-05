# Makefile for Weno Fit OS

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LDFLAGS =

BUILD_DIR = build
SRC_DIR = src

KERNEL_SRC = $(SRC_DIR)/kernel/kernel.c \
             $(SRC_DIR)/kernel/scheduler.c \
             $(SRC_DIR)/kernel/task.c

APP_SRC = $(SRC_DIR)/app/main.c \
          $(SRC_DIR)/app/ota_service.c \
          $(SRC_DIR)/app/calculator.c \
          $(SRC_DIR)/app/spo2_service.c \
          $(SRC_DIR)/app/temperature_service.c \
          $(SRC_DIR)/app/ecg_app.c \
          $(SRC_DIR)/app/ui_tracker.c \
          $(SRC_DIR)/app/ui_simulator_controls.c

DRIVER_SRC = $(SRC_DIR)/drivers/spo2.c \
             $(SRC_DIR)/drivers/temperature.c \
             $(SRC_DIR)/drivers/ecg_afe.c \
             $(SRC_DIR)/drivers/sim_heart_rate.c

KERNEL_OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(KERNEL_SRC))
APP_OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(APP_SRC))
DRIVER_OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(DRIVER_SRC))

ALL_OBJ = $(KERNEL_OBJ) $(APP_OBJ) $(DRIVER_OBJ)

OUT = $(BUILD_DIR)/weno_fit_os.elf

all: $(BUILD_DIR) $(OUT)

$(OUT): $(ALL_OBJ)
	$(CC) $(LDFLAGS) $(ALL_OBJ) -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/kernel
	mkdir -p $(BUILD_DIR)/app
	mkdir -p $(BUILD_DIR)/drivers

$(BUILD_DIR)/kernel/%.o: $(SRC_DIR)/kernel/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/app/%.o: $(SRC_DIR)/app/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/drivers/%.o: $(SRC_DIR)/drivers/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
