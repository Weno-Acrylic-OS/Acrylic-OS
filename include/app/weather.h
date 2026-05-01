#ifndef WEATHER_H
#define WEATHER_H

#include "lvgl.h"

// Data structure for weather information
typedef struct {
    int temperature;
    const char * condition;
    const char * icon;
} weather_data_t;

// Function to create the weather app UI
void create_weather_app(lv_obj_t * parent);

#endif // WEATHER_H
