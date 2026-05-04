#include "drivers/ecg_afe.h"
#include <stddef.h> // For NULL
#include <stdlib.h> // For rand()
#include <math.h>   // For sin() in mock data
#include "lvgl.h"   // For lv_timer

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static ecg_data_callback_t data_callback = NULL;
static lv_timer_t* mock_data_timer = NULL;

static void mock_ecg_data_generator(lv_timer_t* timer);

bool ecg_afe_init(void) {
    // TODO: Init I2C/SPI for the AFE
    return true;
}

void ecg_afe_register_callback(ecg_data_callback_t callback) {
    data_callback = callback;
}

void ecg_afe_start_reading(void) {
    if (!mock_data_timer) {
        // Generate mock data at 50Hz (every 20ms)
        mock_data_timer = lv_timer_create(mock_ecg_data_generator, 20, NULL);
    }
}

void ecg_afe_stop_reading(void) {
    if (mock_data_timer) {
        lv_timer_del(mock_data_timer);
        mock_data_timer = NULL;
    }
}

// This function simulates a very basic ECG waveform (a sine wave)
static void mock_ecg_data_generator(lv_timer_t* timer) {
    (void)timer;
    static int16_t mock_samples[20]; // 20 samples per tick
    static float phase = 0.0f;

    if (data_callback) {
        for (int i = 0; i < 20; i++) {
            // A 1Hz sine wave with some noise to simulate a 60bpm heart rate
            float sine_val = sinf(phase);
            float noise = (float)rand() / (float)RAND_MAX * 0.2f - 0.1f;
            mock_samples[i] = (int16_t)((sine_val + noise) * 1000.0f); // Scale it
            phase += (2.0f * M_PI * 1.0f) / 1000.0f; // 1Hz signal, 1000Hz sample rate (50Hz * 20 samples)
            if (phase > 2.0f * M_PI) {
                phase -= 2.0f * M_PI;
            }
        }
        data_callback(mock_samples, 20);
    }
}
