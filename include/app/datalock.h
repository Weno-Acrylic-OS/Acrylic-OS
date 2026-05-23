// include/app/datalock.h
#ifndef DATALOCK_H
#define DATALOCK_H

#include <stdbool.h>
#include "lvgl.h"

#define DATALOCK_PIN_LENGTH 4

/**
 * @brief Initializes the DataLock system.
 */
void datalock_init();

/**
 * @brief Sets a new PIN.
 * @param new_pin A string representing the new PIN. Must be DATALOCK_PIN_LENGTH digits.
 * @return True if the PIN was set successfully, false otherwise.
 */
bool datalock_set_pin(const char* new_pin);

/**
 * @brief Checks if the device is currently locked.
 * @return True if the device is locked, false otherwise.
 */
bool datalock_is_locked();

/**
 * @brief Attempts to unlock the device with a given PIN.
 * @param pin_attempt The PIN to check.
 * @return True if the PIN is correct, false otherwise.
 */
bool datalock_check_pin(const char* pin_attempt);

/**
 * @brief Manually locks the device.
 */
void datalock_lock_device();


#endif // DATALOCK_H
