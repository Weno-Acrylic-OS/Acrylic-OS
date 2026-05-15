#include "app/bluetooth.h"
#include "app/ble_service.h"

static bool is_enabled = true; // Default to on

void bluetooth_toggle() {
    is_enabled = !is_enabled;
    // In a real implementation, this would call functions
    // to enable/disable the BLE stack and advertising.
    if (is_enabled) {
        ble_service_init(); // Re-initialize services
    } else {
        // Code to de-initialize or stop advertising would go here.
    }
}

bool bluetooth_is_enabled() {
    return is_enabled;
}
