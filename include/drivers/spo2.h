#ifndef SPO2_H
#define SPO2_H

#include <stdint.h>
#include <stdbool.h>

// Callback function type for new SpO2 data
typedef void (*spo2_data_callback_t)(uint8_t spo2_value, uint16_t heart_rate);

/**
 * @brief Initializes the SpO2 sensor.
 *
 * @return true if initialization is successful, false otherwise.
 */
bool spo2_init(void);

/**
 * @brief Registers a callback function to be called when new data is available.
 *
 * @param callback The function to call.
 */
void spo2_register_callback(spo2_data_callback_t callback);

/**
 * @brief Starts a new SpO2 measurement.
 * This might be an asynchronous operation.
 */
void spo2_start_measurement(void);

/**
 * @brief Stops measurements.
 */
void spo2_stop_measurement(void);

#endif // SPO2_H
