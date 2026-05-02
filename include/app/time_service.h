#ifndef TIME_SERVICE_H
#define TIME_SERVICE_H

#include <stdint.h>

void time_service_init();
uint8_t time_service_get_hours();
uint8_t time_service_get_minutes();
uint8_t time_service_get_seconds();

#endif // TIME_SERVICE_H
