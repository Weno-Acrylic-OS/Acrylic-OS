#include "app/privacy_service.h"

static bool sensor_active_states[PRIVACY_SENSOR_COUNT];

void privacy_service_init() {
    for (int i = 0; i < PRIVACY_SENSOR_COUNT; i++) {
        sensor_active_states[i] = false;
    }
}

void privacy_service_set_sensor_active(privacy_sensor_t sensor, bool is_active) {
    if (sensor < PRIVACY_SENSOR_COUNT) {
        sensor_active_states[sensor] = is_active;
    }
}

bool privacy_service_is_sensor_active(privacy_sensor_t sensor) {
    if (sensor < PRIVACY_SENSOR_COUNT) {
        return sensor_active_states[sensor];
    }
    return false;
}
