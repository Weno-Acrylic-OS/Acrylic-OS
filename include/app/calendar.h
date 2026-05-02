#ifndef CALENDAR_H
#define CALENDAR_H

#include "lvgl.h"

// Data structure for a calendar event
typedef struct {
    const char * time;
    const char * title;
} calendar_event_t;

// Function to create the Calendar app UI
void create_calendar_app(lv_obj_t * parent);

#endif // CALENDAR_H
