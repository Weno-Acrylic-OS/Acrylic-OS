#ifndef NAVIGATION_SERVICE_H
#define NAVIGATION_SERVICE_H

#include "lvgl.h"

// A function pointer type for screen creation functions
typedef void (*screen_create_func_t)(lv_obj_t * parent);

// Initialize with the two containers
void navigation_service_init(lv_obj_t * app_container, lv_obj_t * tab_container);
void navigation_service_navigate_to(screen_create_func_t create_func);

#endif // NAVIGATION_SERVICE_H
