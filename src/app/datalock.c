// src/app/datalock.c
#include <app/datalock.h>
#include <string.h> // For strncmp, strncpy
#include <stddef.h> // For NULL
#include "navigation_service.h"

// --- Internal State ---

// NOTE: In a real-world application, all state (pin, locked status, and whether a pin is set)
// must be loaded from and persisted to secure, non-volatile memory on boot and on change.
// Static variables are for demonstration purposes only.

static bool device_is_locked = false;
static bool pin_has_been_set = false;
static char current_pin[DATALOCK_PIN_LENGTH + 1] = {0};

// --- Function Implementations ---

void datalock_init() {
    // On boot, we would normally load the state from NVM.
    // For this demo, we reset to a clean slate every time.
    pin_has_been_set = false;
    device_is_locked = false;
    memset(current_pin, 0, sizeof(current_pin));
}

bool datalock_set_pin(const char* new_pin) {
    if (new_pin == NULL || strlen(new_pin) != DATALOCK_PIN_LENGTH) {
        return false;
    }
    strncpy(current_pin, new_pin, DATALOCK_PIN_LENGTH);
    current_pin[DATALOCK_PIN_LENGTH] = '\0'; // Ensure null termination
    
    // A PIN has now been configured.
    pin_has_been_set = true;
    
    // NOTE: At this point, we would persist `current_pin` and `pin_has_been_set` to NVM.
    return true;
}

bool datalock_is_locked() {
    // The device can only be considered locked if a PIN has been set.
    return device_is_locked && pin_has_been_set;
}

bool datalock_check_pin(const char* pin_attempt) {
    if (pin_attempt == NULL || !pin_has_been_set) {
        return false;
    }

    // Compare the attempt with the stored PIN.
    if (strncmp(pin_attempt, current_pin, DATALOCK_PIN_LENGTH) == 0) {
        // Correct PIN, unlock the device.
        device_is_locked = false;
        return true;
    }

    return false;
}

void datalock_lock_device() {
    // Only allow locking if a PIN has been configured.
    if (pin_has_been_set) {
        device_is_locked = true;
        // NOTE: We would persist the locked status here.
    }
}
