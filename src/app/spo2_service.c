#include "app/spo2_service.h"
#include "drivers/spo2.h"
#include <stdio.h> // For logging, if you have a logging system

// Private state for the service
static uint8_t latest_spo2_value = 0;
static uint16_t latest_hr_value = 0;

// Callback handler for the driver
static void on_new_spo2_data(uint8_t spo2_value, uint16_t heart_rate) {
    // This function will be called by the spo2 driver when it has new data.
    latest_spo2_value = spo2_value;
    latest_hr_value = heart_rate;

    // TODO: Add your logic here.
    // - Store the value in persistence.
    // - Notify the UI to update.
    // - Perform averaging or other analysis.
}

void spo2_service_init(void) {
    if (spo2_init()) {
        spo2_register_callback(on_new_spo2_data);
        // Depending on your strategy, you might start measurements here,
        // or wait for an app to request it.
        spo2_start_measurement();
    } else {
        // TODO: Handle SpO2 Service Initialization Failure.
    }
}

uint8_t spo2_service_get_latest_value(void) {
    return latest_spo2_value;
}

uint16_t spo2_service_get_latest_hr(void) {
    return latest_hr_value;
}
