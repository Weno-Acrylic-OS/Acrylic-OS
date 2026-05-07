#ifndef BLE_SERVICE_H
#define BLE_SERVICE_H

#include <stdint.h>
#include <stdbool.h>
#include "app/gamification_service.h"

/**
 * @file ble_service.h
 * @brief This module manages the Bluetooth Low Energy (BLE) services for Weno Fit OS.
 *
 * NOTE: This is a skeleton implementation. The web simulator cannot act as a BLE
 * peripheral, so this code serves as a foundation for a real hardware implementation.
 * The functions are stubs and do not have a real effect in the web build.
 */

/**
 * @brief Initializes the BLE service.
 * On a real device, this would initialize the BLE stack, define the GATT services,
 * and start advertising.
 */
void ble_service_init();

/**
 * @brief Updates the heart rate value in the Heart Rate Measurement characteristic.
 * @param heart_rate The new heart rate value.
 */
void ble_service_update_heart_rate(uint8_t heart_rate);

/**
 * @brief Updates the step count in the custom Steps Count characteristic.
 * @param steps The new step count.
 */
void ble_service_update_steps(uint32_t steps);

/**
 * @brief Updates the goals data in the custom Goals characteristic.
 * @param goals A pointer to the array of goals.
 * @param count The number of goals in the array.
 */
void ble_service_update_goals(const goal_t *goals, int count);


#endif // BLE_SERVICE_H
