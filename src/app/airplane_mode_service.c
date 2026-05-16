#include "app/airplane_mode_service.h"
#include "app/bluetooth.h" // To control bluetooth status

static bool is_active = false;

void airplane_mode_service_toggle() {
    is_active = !is_active;

    // In a real implementation, this would also disable other radios like Wi-Fi.
    if (is_active) {
        // If airplane mode is turning on, and bluetooth is enabled, we should turn it off
        if (bluetooth_is_enabled()) {
            bluetooth_toggle();
        }
    }
}

bool airplane_mode_service_is_active() {
    return is_active;
}
