#ifndef ECG_AFE_H
#define ECG_AFE_H

#include <stdint.h>
#include <stdbool.h>

// Callback with a buffer of raw ECG sample points
typedef void (*ecg_data_callback_t)(int16_t* samples, uint16_t count);

bool ecg_afe_init(void);
void ecg_afe_register_callback(ecg_data_callback_t callback);
void ecg_afe_start_reading(void);
void ecg_afe_stop_reading(void);

#endif // ECG_AFE_H
