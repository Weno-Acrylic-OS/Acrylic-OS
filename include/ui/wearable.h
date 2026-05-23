#ifndef WEARABLE_H
#define WEARABLE_H

#include "ui/ui_hal.h"

// Function to get the wearable personality
ui_personality_t* get_wearable_personality();
void create_main_ui(lv_obj_t * parent);

#endif // WEARABLE_H
