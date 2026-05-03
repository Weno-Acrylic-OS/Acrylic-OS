#ifndef TODAY_SERVICE_H
#define TODAY_SERVICE_H

#include "lvgl.h"

// A function pointer type for creating the widget UI
typedef void (*widget_create_func_t)(lv_obj_t * parent);

// Describes a single widget
typedef struct {
    const char * name;
    widget_create_func_t create_func;
} today_widget_descriptor_t;

/**
 * @brief Initializes the today service.
 */
void today_service_init();

/**
 * @brief Registers a new widget with the service.
 * @param descriptor A pointer to the widget descriptor.
 */
void today_service_register_widget(const today_widget_descriptor_t * descriptor);

/**
 * @brief Renders all registered widgets into a parent container.
 * @param parent The parent object to draw the widgets on.
 */
void today_service_render_widgets(lv_obj_t * parent);

#endif // TODAY_SERVICE_H
