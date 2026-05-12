#ifndef ACTIVITY_SERVICE_H
#define ACTIVITY_SERVICE_H

#include <stdint.h>

void activity_service_init(void);

uint16_t activity_service_get_points(void);
uint16_t activity_service_get_goal(void);

#endif // ACTIVITY_SERVICE_H
