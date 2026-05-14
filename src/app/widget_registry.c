#include "app/widget_registry.h"

// --- Widget Implementations ---

lv_obj_t* create_steps_widget(lv_obj_t *parent) {
    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, "Steps: 1234");
    return label;
}

lv_obj_t* create_hr_widget(lv_obj_t *parent) {
    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, "HR: 72 bpm");
    return label;
}

// --- Widget Registry ---

const widget_descriptor_t available_widgets[] = {
    {WIDGET_TYPE_STEPS, "Steps", LV_SYMBOL_DUMMY, create_steps_widget},
    {WIDGET_TYPE_HEART_RATE, "Heart Rate", LV_SYMBOL_DUMMY, create_hr_widget},
};

const int num_available_widgets = sizeof(available_widgets) / sizeof(widget_descriptor_t);
