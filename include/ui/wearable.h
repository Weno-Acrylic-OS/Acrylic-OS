#ifndef WEARABLE_H
#define WEARABLE_H

#include "ui/ui_hal.h"

// Gets the personality struct for the wearable UI
ui_personality_t* get_wearable_personality();

// The main UI creation function for the wearable personality
void create_main_ui(lv_obj_t* parent);

#endif // WEARABLE_H
