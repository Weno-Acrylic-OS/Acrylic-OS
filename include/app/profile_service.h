#ifndef PROFILE_SERVICE_H
#define PROFILE_SERVICE_H

#include <stdint.h>

void profile_service_init(void);

void profile_service_set_age(uint8_t age);
uint8_t profile_service_get_age(void);


#endif // PROFILE_SERVICE_H
