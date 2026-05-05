// include/drivers/heart_rate.h
#ifndef HEART_RATE_H
#define HEART_RATE_H

#include <stdint.h>

// Generic heart rate driver interface
typedef struct {
    void (*init)(void);
    uint8_t (*get_bpm)(void);
} heart_rate_driver_t;

void heart_rate_init(const heart_rate_driver_t* driver);
uint8_t heart_rate_get_bpm();

#endif // HEART_RATE_H
