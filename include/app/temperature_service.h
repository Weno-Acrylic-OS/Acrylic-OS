#ifndef TEMPERATURE_SERVICE_H
#define TEMPERATURE_SERVICE_H

#include <stdint.h>

/**
 * @brief Initializes the Temperature service.
 */
void temperature_service_init(void);

/**
 * @brief Gets the latest temperature reading in degrees Celsius.
 *
 * @return The last measured temperature, or 0.0f if no reading is available.
 */
float temperature_service_get_latest_value(void);

#endif // TEMPERATURE_SERVICE_H
