#include "app/temperature_service.h"
#include "drivers/temperature.h"
#include <stddef.h> // for NULL

// Private state for the service
static float latest_temperature_value = 0.0f;

// Callback handler for the driver
static void on_new_temperature_data(float temperature) {
    latest_temperature_value = temperature;
    // TODO: Add your logic here (persistence, UI notification, etc.)
}

void temperature_service_init(void) {
    if (temperature_init()) {
        temperature_register_callback(on_new_temperature_data);
        temperature_start_measurement();
    } else {
        // TODO: Handle Temperature Service Initialization Failure.
    }
}

float temperature_service_get_latest_value(void) {
    return latest_temperature_value;
}
