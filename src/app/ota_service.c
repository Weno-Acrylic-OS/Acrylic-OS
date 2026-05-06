#include "app/ota_service.h"
#include <stdio.h>
#include <string.h>

// --- Forward Declarations ---
static void ota_ui_update_cb(const char* status);

// --- State ---
static void (*ui_update_callback)(const char* status) = NULL;

// --- OTA Logic ---

static void apply_update(const char* firmware_content) {
    ota_ui_update_cb("Applying update...");
    // In a real scenario, this would involve a bootloader
    // and writing the new firmware to flash memory.
    printf("Applying update with content: %s\n", firmware_content);
    ota_ui_update_cb("Update complete!");
}

static void verify_firmware(const char* firmware_content) {
    ota_ui_update_cb("Verifying firmware...");
    // In a real scenario, this would check a cryptographic signature.
    printf("Verifying firmware: %s\n", firmware_content);
    apply_update(firmware_content);
}

void ota_service_start(void (*callback)(const char*)) {
    ui_update_callback = callback;
    ota_ui_update_cb("Downloading firmware...");

    // This is where you would use a tool like web_fetch to get the firmware.
    // For this simulation, we'll just use a hardcoded string.
    const char* firmware_content = "This is a dummy firmware file for Weno Fit OS v2.";
    verify_firmware(firmware_content);
}

// --- UI Callback ---
static void ota_ui_update_cb(const char* status) {
    if (ui_update_callback) {
        ui_update_callback(status);
    }
    printf("OTA Status: %s\n", status);
}

