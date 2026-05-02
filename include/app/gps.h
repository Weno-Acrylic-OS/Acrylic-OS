#ifndef GPS_H
#define GPS_H

#include "lvgl.h"

// Enum for turn directions
typedef enum {
    TURN_LEFT,
    TURN_RIGHT,
    STRAIGHT,
    ARRIVED
} navigation_direction_t;

// Data structure for a single navigation instruction
typedef struct {
    navigation_direction_t direction;
    const char * street_name;
    double distance_meters;
} navigation_instruction_t;

// Function to create the GPS navigation app UI
void create_gps_app(lv_obj_t * parent);

#endif // GPS_H
