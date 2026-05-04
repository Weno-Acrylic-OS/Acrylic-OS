#ifndef SPO2_SERVICE_H
#define SPO2_SERVICE_H

#include <stdint.h>

/**
 * @brief Initializes the SpO2 service.
 * This should be called once at startup.
 */
void spo2_service_init(void);

/**
 * @brief Gets the latest SpO2 reading.
 *
 * @return The last measured SpO2 value (in percent), or 0 if no reading is available.
 */
uint8_t spo2_service_get_latest_value(void);


/**
 * @brief Gets the latest heart rate reading from the SpO2 sensor.
 *
 * @return The last measured heart rate (in bpm), or 0 if no reading is available.
 */
uint16_t spo2_service_get_latest_hr(void);

#endif // SPO2_SERVICE_H
