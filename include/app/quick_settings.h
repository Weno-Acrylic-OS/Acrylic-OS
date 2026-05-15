// include/app/quick_settings.h
#ifndef QUICK_SETTINGS_H
#define QUICK_SETTINGS_H

#include "lvgl.h"
#include "app/quick_settings_model.h"

// The maximum number of quick settings that can be displayed.
#define MAX_QUICK_SETTINGS 4

void quick_settings_init();
void create_quick_settings(lv_obj_t * parent);

// --- Configuration API ---

// Retrieves the list of all possible quick setting items.
const quick_setting_item_t* get_available_quick_settings(int* count);

// Sets and saves the new configuration for quick settings.
void set_current_quick_settings(const quick_setting_id_t* settings, int count);

#endif // QUICK_SETTINGS_H
