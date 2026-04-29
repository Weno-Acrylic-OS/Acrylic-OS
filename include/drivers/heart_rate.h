// include/drivers/heart_rate.h
#ifndef HEART_RATE_H
#define HEART_RATE_H

#include <stdint.h>

void heart_rate_init();
uint8_t heart_rate_get_bpm();

#endif // HEART_RATE_H
