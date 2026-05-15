#ifndef QUICK_SETTINGS_MODEL_H
#define QUICK_SETTINGS_MODEL_H

#include "lvgl.h"

// Enum to identify each possible quick setting
typedef enum {
    QUICK_SETTING_NONE,
    QUICK_SETTING_DND,
    QUICK_SETTING_FIND_PHONE,
    QUICK_SETTING_DATALOCK,
    QUICK_SETTING_SETTINGS,
    QUICK_SETTING_FLASHLIGHT,
    QUICK_SETTING_BLUETOOTH,
    // Add more potential settings here in the future
    // e.g., QUICK_SETTING_WIFI, QUICK_SETTING_BLUETOOTH
    QUICK_SETTING_MAX // Sentinel value
} quick_setting_id_t;

// Struct to define a quick setting item's properties
typedef struct {
    quick_setting_id_t id;
    const char* symbol;
    const char* text;
    lv_event_cb_t event_handler;
} quick_setting_item_t;

#endif // QUICK_SETTINGS_MODEL_H
