#ifndef WIDGET_REGISTRY_H
#define WIDGET_REGISTRY_H

#include "lvgl.h"
#include "app/custom_watch_face.h"

typedef struct {
    widget_type_t id;
    const char *name;
    const char *icon;
    lv_obj_t* (*create_func)(lv_obj_t *parent);
} widget_descriptor_t;

// Forward declare the widget creation functions
lv_obj_t* create_steps_widget(lv_obj_t *parent);
lv_obj_t* create_hr_widget(lv_obj_t *parent);

#endif // WIDGET_REGISTRY_H
