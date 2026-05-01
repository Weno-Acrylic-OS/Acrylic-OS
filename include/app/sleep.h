#ifndef SLEEP_H
#define SLEEP_H

#include "lvgl.h"

// Data structure for sleep information
typedef struct {
    int time_in_bed; // minutes
    int time_asleep; // minutes
    // Sleep stages in minutes
    int rem_sleep;
    int deep_sleep;
    int light_sleep;
} sleep_data_t;

// Function to create the sleep app UI
void create_sleep_app(lv_obj_t * parent);

#endif // SLEEP_H
