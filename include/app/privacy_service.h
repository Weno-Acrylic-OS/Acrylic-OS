#ifndef PRIVACY_SERVICE_H
#define PRIVACY_SERVICE_H

#include <stdbool.h>

typedef enum {
    PRIVACY_SENSOR_LOCATION,
    PRIVACY_SENSOR_MICROPHONE,
    // Add other sensors here in the future
    PRIVACY_SENSOR_COUNT
} privacy_sensor_t;

void privacy_service_init();
void privacy_service_set_sensor_active(privacy_sensor_t sensor, bool is_active);
bool privacy_service_is_sensor_active(privacy_sensor_t sensor);

#endif // PRIVACY_SERVICE_H
