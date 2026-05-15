#ifndef FLASHLIGHT_SERVICE_H
#define FLASHLIGHT_SERVICE_H

#include "lvgl.h"

void flashlight_service_toggle();
bool flashlight_service_is_active();
void create_flashlight_screen();

#endif // FLASHLIGHT_SERVICE_H
