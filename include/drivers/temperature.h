#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <stdint.h>
#include <stdbool.h>

// Callback function type for new temperature data
typedef void (*temperature_data_callback_t)(float temperature);

/**
 * @brief Initializes the temperature sensor.
 *
 * @return true if initialization is successful, false otherwise.
 */
bool temperature_init(void);

/**
 * @brief Registers a callback function for new data.
 */
void temperature_register_callback(temperature_data_callback_t callback);

/**
 * @brief Starts a new temperature measurement.
 */
void temperature_start_measurement(void);

/**
 * @brief Stops measurements.
 */
void temperature_stop_measurement(void);

#endif // TEMPERATURE_H
