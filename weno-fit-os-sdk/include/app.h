#ifndef APP_H
#define APP_H

#include "lvgl.h"

typedef void (*app_create_func_t)(lv_obj_t * parent);

typedef struct {
    const char * name;
    app_create_func_t create_func;
} app_descriptor_t;

void register_app(const app_descriptor_t * descriptor);

#endif // APP_H
