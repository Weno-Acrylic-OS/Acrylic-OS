// src/drivers/heart_rate.c
#include "drivers/heart_rate.h"
#include <stddef.h>

static const heart_rate_driver_t* registered_driver = NULL;

void heart_rate_init(const heart_rate_driver_t* driver) {
    registered_driver = driver;
    if (registered_driver && registered_driver->init) {
        registered_driver->init();
    }
}

uint8_t heart_rate_get_bpm() {
    if (registered_driver && registered_driver->get_bpm) {
        return registered_driver->get_bpm();
    }
    return 0; // Return a default value if no driver is registered
}
