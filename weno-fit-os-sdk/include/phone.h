#ifndef WENO_PHONE_H
#define WENO_PHONE_H

#include <stdbool.h>

/**
 * @brief Checks if a phone is connected to the device.
 * @return True if a phone is connected, false otherwise.
 */
bool weno_phone_is_connected();

/**
 * @brief Shows a standardized "Phone Required" screen.
 * This should be called when a feature requires a phone connection
 * but no phone is connected.
 */
void weno_phone_show_required_screen();

#endif // WENO_PHONE_H
